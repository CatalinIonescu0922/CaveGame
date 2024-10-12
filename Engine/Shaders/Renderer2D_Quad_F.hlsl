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
    float2 texture_coordinates : TEXTURE_COORDINATES;
    uint texture_index : TEXTURE_INDEX;
};

struct FSOutput
{
    float4 color : SV_Target0;
};

Texture2D u_textures[16] : register(t0);
SamplerState u_samplers[16] : register(s0);

FSOutput cave_fragment_main(FSInput input)
{
    float4 texture_color = float4(1, 1, 1, 1);
    switch (input.texture_index)
    {
        case 0:  texture_color = u_textures[0].Sample(u_samplers[0], input.texture_coordinates); break;
        case 1:  texture_color = u_textures[1].Sample(u_samplers[1], input.texture_coordinates); break;
        case 2:  texture_color = u_textures[2].Sample(u_samplers[2], input.texture_coordinates); break;
        case 3:  texture_color = u_textures[3].Sample(u_samplers[3], input.texture_coordinates); break;
        case 4:  texture_color = u_textures[4].Sample(u_samplers[4], input.texture_coordinates); break;
        case 5:  texture_color = u_textures[5].Sample(u_samplers[5], input.texture_coordinates); break;
        case 6:  texture_color = u_textures[6].Sample(u_samplers[6], input.texture_coordinates); break;
        case 7:  texture_color = u_textures[7].Sample(u_samplers[7], input.texture_coordinates); break;
        case 8:  texture_color = u_textures[8].Sample(u_samplers[8], input.texture_coordinates); break;
        case 9:  texture_color = u_textures[9].Sample(u_samplers[9], input.texture_coordinates); break;
        case 10: texture_color = u_textures[10].Sample(u_samplers[10], input.texture_coordinates); break;
        case 11: texture_color = u_textures[11].Sample(u_samplers[11], input.texture_coordinates); break;
        case 12: texture_color = u_textures[12].Sample(u_samplers[12], input.texture_coordinates); break;
        case 13: texture_color = u_textures[13].Sample(u_samplers[13], input.texture_coordinates); break;
        case 14: texture_color = u_textures[14].Sample(u_samplers[14], input.texture_coordinates); break;
        case 15: texture_color = u_textures[15].Sample(u_samplers[15], input.texture_coordinates); break;
    }
    
    FSOutput output;
    output.color = texture_color * input.color;
    return output;
}
