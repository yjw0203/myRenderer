#pragma once
#include "Engine/RHI/Public/rpi/yjw_rpi_define.h"

namespace rpi
{
    enum RPIDepthStencilStateType
    {
        default_no_depth_no_stencil,
        default_depth_read,
        default_depth_read_and_write,
        depth_stencil_state_type_count,
    };

    enum RPIColorBlendStateType
    {
        no_blend,
        default_alpha_blend,
        color_blend_state_type_count
    };

    RPIDepthStencilState RPIGetDepthStencilState(RPIDepthStencilStateType type);
    RPIColorBlendState RPIGetColorBlendState(RPIColorBlendStateType type);
    RPIRenderPipelineDescriptor RPIGetDefaultRenderPipeline();
}