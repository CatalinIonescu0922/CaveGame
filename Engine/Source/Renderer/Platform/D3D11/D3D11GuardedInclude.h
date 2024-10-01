/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/CoreDefines.h>
#include <Core/Platform/Windows/WindowsGuardedInclude.h>

#if CAVE_PLATFORM_WINDOWS
    #include <d3d11.h>
    #include <dxgi1_3.h>
#else
    #error Trying to include the D3D11 headers but they are not available!
#endif // CAVE_PLATFORM_WINDOWS

//
// Releases the reference held by a raw pointer.
// All D3D11 objects are referenced, and when a raw pointer is released the reference count must be
// decreased. This macros facilitiates that, by also ensuring the the given pointer is valid.
//
#define CAVE_D3D11_RELEASE(raw_pointer) \
    if (raw_pointer != nullptr)         \
    {                                   \
        raw_pointer->Release();         \
        raw_pointer = nullptr;          \
    }
