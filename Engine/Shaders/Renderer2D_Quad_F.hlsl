/*
 * Copyright (c) Catalin Ionescu 2024. All rights reserved.
 * Copyright (c) Robert Bengulescu 2024. All rights reserved.
 * Copyright (c) Traian Avram 2024. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0.
 */

struct FSInput
{
    float4 color : COLOR;
};

struct FSOutput
{
    float4 color : SV_Target0;
};

FSOutput cave_fragment_main(FSInput input)
{
    FSOutput output;
    output.color = input.color;
    return output;
}
