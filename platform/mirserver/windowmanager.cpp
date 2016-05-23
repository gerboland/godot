#include "windowmanager.h"

WindowManager::WindowManager(mir::shell::FocusController* focus_controller,
                             std::shared_ptr<mir::shell::DisplayLayout> const& display_layout)
{

}

void WindowManager::add_session(std::shared_ptr<mir::scene::Session> const& session)
{

}

void WindowManager::remove_session(std::shared_ptr<mir::scene::Session> const& session)
{

}

auto WindowManager::add_surface(
    std::shared_ptr<mir::scene::Session> const& session,
    mir::scene::SurfaceCreationParameters const& params,
    std::function<mir::frontend::SurfaceId(std::shared_ptr<mir::scene::Session> const& session, 
        mir::scene::SurfaceCreationParameters const& params)> const& build)
-> mir::frontend::SurfaceId
{

	return build(session, params);
}

void WindowManager::modify_surface(
    std::shared_ptr<mir::scene::Session> const& session,
    std::shared_ptr<mir::scene::Surface> const& surface,
    mir::shell::SurfaceSpecification const& modifications)
{

}

void WindowManager::remove_surface(
    std::shared_ptr<mir::scene::Session> const& session,
    std::weak_ptr<mir::scene::Surface> const& surface)
{

}

void WindowManager::add_display(mir::geometry::Rectangle const& area)
{

}

void WindowManager::remove_display(mir::geometry::Rectangle const& area)
{

}

bool WindowManager::handle_keyboard_event(MirKeyboardEvent const* event)
{
	return true;
}

bool WindowManager::handle_touch_event(MirTouchEvent const* event)
{
	return true;
}

bool WindowManager::handle_pointer_event(MirPointerEvent const* event)
{
	return true;
}

void WindowManager::handle_raise_surface(
    std::shared_ptr<mir::scene::Session> const& session,
    std::shared_ptr<mir::scene::Surface> const& surface,
    uint64_t timestamp)
{

}

int WindowManager::set_surface_attribute(
    std::shared_ptr<mir::scene::Session> const& session,
    std::shared_ptr<mir::scene::Surface> const& surface,
    MirSurfaceAttrib attrib,
    int value)
{
	return 1;
}