/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <CaveGameLoop.h>

namespace CaveGame
{

bool CaveGameLoop::on_game_start()
{
    return true;
}

void CaveGameLoop::on_game_end()
{}

void CaveGameLoop::on_game_update(float delta_time)
{}

} // namespace CaveGame
