/*************************************************************************/
/*  MirServer.cpp                                                        */
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


#include "mirserver.h"

// local
#include "compositor.h"
#include "windowmanager.h"

// std
#include <memory>

namespace mg = mir::graphics;
namespace mo  = mir::options;
namespace msh = mir::shell;
namespace ms = mir::scene;



MirServer::MirServer(int &argc, char **argv)
{
    // Casting char** to be a const char** safe as Mir won't change it, nor will we
    set_command_line(argc, const_cast<const char **>(argv));

    override_the_compositor([]
        {
            return std::make_shared<Compositor>();
        });

    // override_the_gl_config([]
    //     {
    //         return std::make_shared<MirGLConfig>();
    //     });

    // override_the_server_status_listener([]
    //     {
    //         return std::make_shared<MirServerStatusListener>();
    //     });

    override_the_window_manager_builder([this](msh::FocusController* focus_controller)
        -> std::shared_ptr<msh::WindowManager>
        {
            return std::make_shared<WindowManager>(focus_controller, the_shell_display_layout());
        });


    set_terminator([](int)
        {
            printf("Signal caught by Mir, stopping Mir server..\n");
            // TODO: quit safely!
            exit(0);
        });

    try {
        apply_settings();
    } catch (const std::exception &ex) {
        printf("MirServer failed on setup: %s", ex.what());
        exit(1);
    }
}