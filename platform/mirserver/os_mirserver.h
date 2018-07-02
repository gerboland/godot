/*************************************************************************/
/*  OS_MirServer.h                                                       */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2016 Juan Linietsky, Ariel Manzur.                 */
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
#ifndef OS_MIRSERVER_H
#define OS_MIRSERVER_H


#include "os/input.h"
#include "drivers/unix/os_unix.h"
#include "context_gl_mirserver.h"
#include "servers/visual_server.h"
#include "servers/visual/visual_server_wrap_mt.h"
#include "servers/visual/rasterizer.h"
#include "servers/physics_server.h"
#include "servers/audio/audio_server_sw.h"
#include "servers/audio/sample_manager_sw.h"
#include "servers/spatial_sound/spatial_sound_server_sw.h"
#include "servers/spatial_sound_2d/spatial_sound_2d_server_sw.h"
#include "drivers/rtaudio/audio_driver_rtaudio.h"
#include "drivers/alsa/audio_driver_alsa.h"
#include "drivers/pulseaudio/audio_driver_pulseaudio.h"
#include "servers/physics_2d/physics_2d_server_sw.h"
#include "servers/physics_2d/physics_2d_server_wrap_mt.h"
#include "main/input_default.h"
#include "mirserver.h"
#include "joystick_linux.h"

#undef CursorShape

/**
	@author Gerry Boland <gerboland@gmail.com>
*/

class OS_MirServer : public OS_Unix {

    MirServer *mir_server;
#if defined(OPENGL_ENABLED) || defined(LEGACYGL_ENABLED)
	ContextGL_MirServer *context_gl;
#endif
	Rasterizer *rasterizer;
	VisualServer *visual_server;
	List<String> args;
	MainLoop *main_loop;

	PhysicsServer *physics_server;
	Physics2DServer *physics_2d_server;

	Point2i center;

	//void handle_key_event(XKeyEvent *p_event,bool p_echo=false);
	//void process_xevents();
	virtual void delete_main_loop();
	IP_Unix *ip_unix;

	AudioServerSW *audio_server;
	SampleManagerMallocSW *sample_manager;
	SpatialSoundServerSW *spatial_sound_server;
	SpatialSound2DServerSW *spatial_sound_2d_server;

	bool force_quit;

	InputDefault *input;

#ifdef JOYDEV_ENABLED
	joystick_linux *joystick;
#endif

#ifdef RTAUDIO_ENABLED
	AudioDriverRtAudio driver_rtaudio;
#endif

#ifdef ALSA_ENABLED
	AudioDriverALSA driver_alsa;
#endif

#ifdef PULSEAUDIO_ENABLED
	AudioDriverPulseAudio driver_pulseaudio;
#endif

	int audio_driver_index;
	unsigned int capture_idle;

protected:

	virtual int get_video_driver_count() const;
	virtual const char * get_video_driver_name(int p_driver) const;
	virtual VideoMode get_default_video_mode() const;

	virtual int get_audio_driver_count() const;
	virtual const char * get_audio_driver_name(int p_driver) const;

	virtual void initialize(const VideoMode& p_desired,int p_video_driver,int p_audio_driver);
	virtual void finalize();

	virtual void set_main_loop( MainLoop * p_main_loop );


public:
	virtual Point2 get_mouse_pos() const;
	virtual int get_mouse_button_state() const;

	virtual void set_window_title(const String&);
	virtual void set_cursor_shape(CursorShape p_shape);

	virtual String get_name();

	virtual MainLoop *get_main_loop() const;

	virtual bool can_draw() const;

	virtual void release_rendering_thread();
	virtual void make_rendering_thread();
	virtual void swap_buffers();

	virtual String get_system_dir(SystemDir p_dir) const;

	virtual Error shell_open(String p_uri);

	virtual void set_video_mode(const VideoMode& p_video_mode,int p_screen=0);
	virtual VideoMode get_video_mode(int p_screen=0) const;
	virtual void get_fullscreen_mode_list(List<VideoMode> *p_list,int p_screen=0) const;

	virtual Size2 get_window_size() const;

	virtual void alert(const String& p_alert,const String& p_title="ALERT!");

	virtual bool is_joy_known(int p_device);
	virtual String get_joy_guid(int p_device) const;

	virtual void set_context(int p_context);

	void run();

	OS_MirServer();
};

#endif
