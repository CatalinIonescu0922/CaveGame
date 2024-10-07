/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

struct VSInput
{
    float2 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 color : COLOR;
    float4 position : SV_Position;
};

VSOutput cave_vertex_main(VSInput input)
{
    VSOutput output;
    output.color = input.color;
    output.position = float4(input.position, 0, 1);
    return output;
}
