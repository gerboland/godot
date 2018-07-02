/*************************************************************************/
/*  godot_mirserver.cpp                                                  */
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

#include "os_mirserver.h"
#include "mirserver.h"

#include "main/main.h"

#include <condition_variable>
#include <cstdio>
#include <mutex>
#include <thread>

#include <mir/main_loop.h>
#include <mir/graphics/display.h>
#include <mir/graphics/gl_context.h>

int main(int argc, char* argv[]) {
    MirServer server(argc, argv);

    std::mutex mutex;
    std::condition_variable started_cv;
    bool started{false};
    auto const display = server.the_display();

   	auto const temp_context = display->create_gl_context();
   	Error err = Main::setup(argv[0],argc-1,&argv[1]);
	if (err!=OK) {
	//	server.stop();
   	//	if (server_thread.joinable()) server_thread.join();
		return 255;
	}

    auto const ml = server.the_main_loop();

    auto server_thread = std::thread([&]
    {
        try
        {
            // By enqueuing the notification code in the main loop, we are
            // ensuring that the server has really and fully started before
            // leaving start_mir_server().
            ml->enqueue(
                nullptr,
                [&]
                {
                    std::lock_guard<std::mutex> lock(mutex);
                    started = true;
                    started_cv.notify_one();
                });

            server.run();
        }
        catch (std::exception const& e)
        {
            printf("Mir failed to start: %s\n", e.what());
            return 1;
        }
    });

    std::unique_lock<std::mutex> lock(mutex);
    started_cv.wait_for(lock, std::chrono::seconds{10}, [&]{ return started; });

	// Mir server has started in its own thread, now can start up Godot. Godot needs
	// an active GL context by now.
    OS_MirServer os(&server);

	if (Main::start())
		os.run(); // it is actually the OS that decides how to run
	Main::cleanup();

	// Shutdown Mir
	server.stop();
    if (server_thread.joinable()) server_thread.join();

	return os.get_exit_code();
}
