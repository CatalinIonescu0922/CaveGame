/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <Core/CoreDefines.h>
#if CAVE_PLATFORM_WINDOWS

    #include <Core/Assertion.h>
    #include <Core/Platform/Window.h>
    #include <Core/Platform/Windows/WindowsGuardedInclude.h>
    #include <Engine/Engine.h>

namespace CaveGame
{

static LRESULT win32_window_procedure(HWND window_handle, UINT message_id, WPARAM w_param, LPARAM l_param)
{
    switch (message_id)
    {
        case WM_QUIT:
        case WM_CLOSE:
        {
            Window& window = Engine::get_window();
            CAVE_ASSERT(window.get_native_handle() == window_handle);
            window.mark_as_should_close();
            return 0;
        }
    }

    return DefWindowProcA(window_handle, message_id, w_param, l_param);
}

static void win32_register_window_class()
{
    static bool s_class_is_registered = false;
    if (!s_class_is_registered)
    {
        WNDCLASSA window_class = {};
        window_class.hInstance = GetModuleHandleA(nullptr);
        window_class.lpfnWndProc = win32_window_procedure;
        window_class.lpszClassName = "CaveGameWindowClass";

        RegisterClassA(&window_class);
        s_class_is_registered = true;
    }
}

bool Window::initialize()
{
    if (m_native_handle != nullptr)
    {
        // The window has already been initialized.
        return false;
    }

    win32_register_window_class();

    DWORD window_style_flags = WS_OVERLAPPEDWINDOW;
    window_style_flags |= WS_VISIBLE | WS_MAXIMIZE;

    m_native_handle = CreateWindowA(
        "CaveGameWindowClass",
        "CaveGame",
        window_style_flags,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    return m_native_handle != nullptr;
}

void Window::shutdown()
{
    if (m_native_handle == nullptr)
    {
        // The window has already been shut down.
        return;
    }

    DestroyWindow(static_cast<HWND>(m_native_handle));
    m_native_handle = nullptr;
}

void Window::process_event_queue()
{
    CAVE_ASSERT(m_native_handle != nullptr);

    MSG message = {};
    while (PeekMessageA(&message, static_cast<HWND>(m_native_handle), 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
}

u32 Window::get_client_area_width() const
{
    RECT client_rect;
    GetClientRect(static_cast<HWND>(m_native_handle), &client_rect);
    return client_rect.right - client_rect.left;
}

u32 Window::get_client_area_height() const
{
    RECT client_rect;
    GetClientRect(static_cast<HWND>(m_native_handle), &client_rect);
    return client_rect.bottom - client_rect.top;
}

} // namespace CaveGame

#endif // CAVE_PLATFORM_WINDOWS
