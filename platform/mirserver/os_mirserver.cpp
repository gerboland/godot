/*************************************************************************/
/*  OS_MirServer.cpp                                                     */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2016 Gerry Boland                                       */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#include "servers/visual/visual_server_raster.h"
#include "drivers/gles2/rasterizer_gles2.h"
#include "os_mirserver.h"
#include "compositor.h"
#include "key_mapping_mirserver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include "print_string.h"
#include "servers/physics/physics_server_sw.h"
#include "errno.h"

#include "main/main.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//stupid linux.h
#ifdef KEY_TAB
#undef KEY_TAB
#endif

#undef CursorShape // something above defines this, and confuses compiler


int OS_MirServer::get_video_driver_count() const {
	return 1;
}

const char * OS_MirServer::get_video_driver_name(int p_driver) const {
	return "GLES2";
}

OS::VideoMode OS_MirServer::get_default_video_mode() const {
	return OS::VideoMode(1024, 800, false); // FIXME: called before we've asked Mir
}

int OS_MirServer::get_audio_driver_count() const {
    return AudioDriverManagerSW::get_driver_count();
}

const char *OS_MirServer::get_audio_driver_name(int p_driver) const {

    AudioDriverSW* driver = AudioDriverManagerSW::get_driver(p_driver);
    ERR_FAIL_COND_V( !driver, "" );
    return AudioDriverManagerSW::get_driver(p_driver)->get_name();
}

void OS_MirServer::initialize(MirServer *server,int p_audio_driver) {

	args=OS::get_singleton()->get_cmdline_args();
	mir_server = server;
	main_loop=NULL;
	
#if defined(OPENGL_ENABLED) || defined(LEGACYGL_ENABLED)

	context_gl = memnew( ContextGL_MirServer(mir_server->the_display()) );
	context_gl->initialize();

	rasterizer = memnew( RasterizerGLES2 );

#endif
	visual_server = memnew( VisualServerRaster(rasterizer) );

	if (get_render_thread_mode()!=RENDER_THREAD_UNSAFE) {

		visual_server =memnew(VisualServerWrapMT(visual_server,get_render_thread_mode()==RENDER_SEPARATE_THREAD));
	}



	AudioDriverManagerSW::get_driver(p_audio_driver)->set_singleton();

	audio_driver_index=p_audio_driver;
	if (AudioDriverManagerSW::get_driver(p_audio_driver)->init()!=OK) {

		bool success=false;
		audio_driver_index=-1;
		for(int i=0;i<AudioDriverManagerSW::get_driver_count();i++) {
			if (i==p_audio_driver)
				continue;
			AudioDriverManagerSW::get_driver(i)->set_singleton();
			if (AudioDriverManagerSW::get_driver(i)->init()==OK) {
				success=true;
				print_line("Audio Driver Failed: "+String(AudioDriverManagerSW::get_driver(p_audio_driver)->get_name()));
				print_line("Using alternate audio driver: "+String(AudioDriverManagerSW::get_driver(i)->get_name()));
				audio_driver_index=i;
				break;
			}
		}
		if (!success) {
			ERR_PRINT("Initializing audio failed.");
		}

	}

	sample_manager = memnew( SampleManagerMallocSW );
	audio_server = memnew( AudioServerSW(sample_manager) );
	audio_server->init();
	spatial_sound_server = memnew( SpatialSoundServerSW );
	spatial_sound_server->init();
	spatial_sound_2d_server = memnew( SpatialSound2DServerSW );
	spatial_sound_2d_server->init();


	ERR_FAIL_COND(!visual_server);


	visual_server->init();
	//
	physics_server = memnew( PhysicsServerSW );
	physics_server->init();
	//physics_2d_server = memnew( Physics2DServerSW );
	physics_2d_server = Physics2DServerWrapMT::init_server<Physics2DServerSW>();
	physics_2d_server->init();

	input = memnew( InputDefault );
#ifdef JOYDEV_ENABLED
	joystick = memnew( joystick_linux(input));
#endif
	_ensure_data_dir();
}

void OS_MirServer::finalize() {

	if(main_loop)
		memdelete(main_loop);
	main_loop=NULL;

	spatial_sound_server->finish();
	memdelete(spatial_sound_server);
	spatial_sound_2d_server->finish();
	memdelete(spatial_sound_2d_server);

	//if (debugger_connection_console) {
//		memdelete(debugger_connection_console);
//}

#ifdef JOYDEV_ENABLED
	memdelete(joystick);
#endif
	memdelete(input);

	memdelete(sample_manager);

	audio_server->finish();
	memdelete(audio_server);

	visual_server->finish();
	memdelete(visual_server);
	memdelete(rasterizer);

	physics_server->finish();
	memdelete(physics_server);

	physics_2d_server->finish();
	memdelete(physics_2d_server);

#if defined(OPENGL_ENABLED) || defined(LEGACYGL_ENABLED)
	memdelete(context_gl);
#endif

	args.clear();
}

void OS_MirServer::set_video_mode(const VideoMode& p_video_mode,int p_screen) {
}

OS::VideoMode OS_MirServer::get_video_mode(int p_screen) const {
	return current_videomode;
}

void OS_MirServer::get_fullscreen_mode_list(List<VideoMode> *p_list,int p_screen) const {
}

Point2 OS_MirServer::get_mouse_pos() const
{
	return Point2(0,0);
}

int OS_MirServer::get_mouse_button_state() const
{
	return 0;
}

void OS_MirServer::set_window_title(const String&)
{
}

void OS_MirServer::set_cursor_shape(CursorShape p)
{
}

Size2 OS_MirServer::get_window_size() const {
	return Size2(context_gl->get_window_width(),context_gl->get_window_height());
}

MainLoop *OS_MirServer::get_main_loop() const {

	return main_loop;
}

void OS_MirServer::delete_main_loop() {

	if (main_loop)
		memdelete(main_loop);
	main_loop=NULL;
}

void OS_MirServer::set_main_loop( MainLoop * p_main_loop ) {

	main_loop=p_main_loop;
	input->set_main_loop(p_main_loop);
}

bool OS_MirServer::can_draw() const {

	return std::static_pointer_cast<Compositor>(mir_server->the_compositor())->running();
};


String OS_MirServer::get_name() {

	return "MirServer";
}

Error OS_MirServer::shell_open(String p_uri) {

	Error ok;
	List<String> args;
	args.push_back(p_uri);
	ok = execute("/usr/bin/xdg-open",args,false);
	if (ok==OK)
		return OK;
	ok = execute("gnome-open",args,false);
	if (ok==OK)
		return OK;
	ok = execute("kde-open",args,false);
	return ok;
}

String OS_MirServer::get_system_dir(SystemDir p_dir) const {


	String xdgparam;

	switch(p_dir) {
		case SYSTEM_DIR_DESKTOP: {

			xdgparam="DESKTOP";
		} break;
		case SYSTEM_DIR_DCIM: {

			xdgparam="PICTURES";

		} break;
		case SYSTEM_DIR_DOCUMENTS: {

			xdgparam="DOCUMENTS";

		} break;
		case SYSTEM_DIR_DOWNLOADS: {

			xdgparam="DOWNLOAD";

		} break;
		case SYSTEM_DIR_MOVIES: {

			xdgparam="VIDEOS";

		} break;
		case SYSTEM_DIR_MUSIC: {

			xdgparam="MUSIC";

		} break;
		case SYSTEM_DIR_PICTURES: {

			xdgparam="PICTURES";

		} break;
		case SYSTEM_DIR_RINGTONES: {

			xdgparam="MUSIC";

		} break;
	}

	String pipe;
	List<String> arg;
	arg.push_back(xdgparam);
	Error err = const_cast<OS_MirServer*>(this)->execute("/usr/bin/xdg-user-dir",arg,true,NULL,&pipe);
	if (err!=OK)
		return ".";
	return pipe.strip_edges();
}


void OS_MirServer::release_rendering_thread() {

	context_gl->release_current();
}

void OS_MirServer::make_rendering_thread() {

	context_gl->make_current();
}

void OS_MirServer::swap_buffers() {

	context_gl->swap_buffers();
}

void OS_MirServer::alert(const String& p_alert,const String& p_title) {

	print("Alert: %s, %s", p_title, p_alert);
}

void OS_MirServer::run() {

	force_quit = false;

	if (!main_loop)
		return;

	main_loop->init();

	while (!force_quit) {

		//process_xevents(); // get rid of pending events
#ifdef JOYDEV_ENABLED
		//event_id = joystick->process_joysticks(event_id);
#endif
		if (Main::iteration()==true)
			break;
	};

	main_loop->finish();
}

bool OS_MirServer::is_joy_known(int p_device) {
	return input->is_joy_mapped(p_device);
}

String OS_MirServer::get_joy_guid(int p_device) const {
	return input->get_joy_guid_remapped(p_device);
}

void OS_MirServer::set_context(int p_context) {

}

OS_MirServer::OS_MirServer() {

#ifdef RTAUDIO_ENABLED
	AudioDriverManagerSW::add_driver(&driver_rtaudio);
#endif

#ifdef PULSEAUDIO_ENABLED
	AudioDriverManagerSW::add_driver(&driver_pulseaudio);
#endif

#ifdef ALSA_ENABLED
	AudioDriverManagerSW::add_driver(&driver_alsa);
#endif

};
