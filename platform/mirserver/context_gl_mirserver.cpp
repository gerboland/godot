/*************************************************************************/
/*  context_gl_mireserver.cpp                                            */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2016 - Gerry Boland                                     */
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
#include "context_gl_mirserver.h"

#ifdef MIRSERVER_ENABLED
#if defined(OPENGL_ENABLED) || defined(LEGACYGL_ENABLED)
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct ContextGL_MirServer_Private {

};

void ContextGL_MirServer::release_current() {

}

void ContextGL_MirServer::make_current() {

}

void ContextGL_MirServer::swap_buffers() {

}

/*
static GLWrapperFuncPtr wrapper_get_proc_address(const char* p_function) {

	//print_line(String()+"getting proc of: "+p_function);
	GLWrapperFuncPtr func=(GLWrapperFuncPtr)glXGetProcAddress( (const GLubyte*) p_function);
	if (!func) {
		print_line("Couldn't find function: "+String(p_function));
	}

	return func;

}*/

Error ContextGL_MirServer::initialize() {


	return OK;
}

int ContextGL_MirServer::get_window_width() {
	return 800;
}

int ContextGL_MirServer::get_window_height() {
	return 600;
}


ContextGL_MirServer::ContextGL_MirServer(const OS::VideoMode& p_default_video_mode,bool p_opengl_3_context) {

	default_video_mode=p_default_video_mode;

	opengl_3_context=p_opengl_3_context;

	double_buffer=false;
	direct_render=false;
	glx_minor=glx_major=0;
	p = memnew( ContextGL_MirServer_Private );
//	p->glx_context=0;
}


ContextGL_MirServer::~ContextGL_MirServer() {

	memdelete( p );
}


#endif
#endif
