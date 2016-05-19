/*************************************************************************/
/*  context_gl_mirserver.h                                               */
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
#ifndef CONTEXT_GL_MIRSERVER_H
#define CONTEXT_GL_MIRSERVER_H

#ifdef MIRSERVER_ENABLED

#if defined(OPENGL_ENABLED) || defined(LEGACYGL_ENABLED)



#include "os/os.h"
#include "drivers/gl_context/context_gl.h"

struct ContextGL_MirServer_Private;

class ContextGL_MirServer : public ContextGL {

	ContextGL_MirServer_Private *p;
	OS::VideoMode default_video_mode;
	bool double_buffer;
	bool direct_render;
	int glx_minor,glx_major;
	bool opengl_3_context;
public:

	virtual void release_current();
	virtual void make_current();
	virtual void swap_buffers();
	virtual int get_window_width();
	virtual int get_window_height();

	virtual Error initialize();

	ContextGL_MirServer(const OS::VideoMode& p_default_video_mode,bool p_opengl_3_context);
	~ContextGL_MirServer();

};

#endif

#endif
#endif
