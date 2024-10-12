/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Core/CoreTypes.h>

namespace CaveGame
{

enum class ImageFormat : u32
{
	Unknown = 0,
	B8G8R8A8,
	R8G8B8A8,
};

enum class ImageFilteringMode : u8
{
	Unknown = 0,
	Nearest,
	Linear,
};

enum class ImageAddressMode : u8
{
	Unknown = 0,
	Wrap,
	Mirror,
	Clamp,
};

} // namespace CaveGame
