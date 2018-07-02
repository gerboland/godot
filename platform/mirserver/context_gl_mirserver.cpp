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

// Mir
#include "mir/geometry/size.h"
#include "mir/graphics/buffer.h"
#include "mir/graphics/display.h"
#include "mir/graphics/display_buffer.h"
#include "mir/graphics/display_configuration.h"
#include "mir/renderer/gl/render_target.h"


void ContextGL_MirServer::release_current() {
    renderTarget->release_current();
}

void ContextGL_MirServer::make_current() {
    renderTarget->make_current();
}

void ContextGL_MirServer::swap_buffers() {
    renderTarget->swap_buffers();
    displayGroup->post();
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
	return width;
}

int ContextGL_MirServer::get_window_height() {
	return height;
}


ContextGL_MirServer::ContextGL_MirServer(const std::shared_ptr<mir::graphics::Display> &display) {

    auto displayConfig = display->configuration();

    displayConfig->for_each_output(
    [this](const mir::graphics::DisplayConfigurationOutput &output) {
        if (output.used && output.connected) {
        	// There should be only 1 screen connected

        	mir::graphics::DisplayConfigurationMode mode = output.modes.at(output.current_mode_index);
    		width = mode.size.width.as_int();
   			height = mode.size.height.as_int();
        }
    });

    display->for_each_display_sync_group([&](mir::graphics::DisplaySyncGroup &group) {
    	group.for_each_display_buffer([&](mir::graphics::DisplayBuffer &displayBuffer) {
    		renderTarget = dynamic_cast<mir::renderer::gl::RenderTarget*>(
            					displayBuffer.native_display_buffer());
    		displayGroup = &group;
    	});
    });
}


ContextGL_MirServer::~ContextGL_MirServer() {

}


#endif
#endif
