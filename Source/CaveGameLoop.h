/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once

#include <Engine/GameLoop.h>

namespace CaveGame
{

class CaveGameLoop : public GameLoop
{
public:
    virtual bool on_game_start() override;
    virtual void on_game_update(float delta_time) override;
    virtual void on_game_end() override;
};

} // namespace CaveGame
