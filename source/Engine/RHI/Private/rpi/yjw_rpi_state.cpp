#include "Engine/RHI/Public/rpi/yjw_rpi_state.h"

namespace rpi
{
    struct RPIStates
    {
        RPIDepthStencilState depthStencilState[depth_stencil_state_type_count];
        RPIColorBlendState colorBlendState[color_blend_state_type_count];
    };
    RPIStates g_rpi_states{};
#define RPIDepthStencilState(type) g_rpi_states.depthStencilState[(int)type]

    void RPIInitDepthStencilState()
    {
        RPIDepthStencilState state{};
        RPIDepthStencilState(default_no_depth_no_stencil) = state;

        state.depthTestEnable = true;
        state.depthCompareOp = RHICompareOp::compare_op_less;
        RPIDepthStencilState(default_depth_read) = state;

        state.depthTestEnable = true;
        state.depthCompareOp = RHICompareOp::compare_op_less;
        state.depthWriteEnable = true;
        RPIDepthStencilState(default_depth_read_and_write) = state;
    }

    void RPIInitColorBlendState()
    {
        RPIColorBlendState state{};
        state.blendEnable = false;
        state.colorWriteMask = color_component_r | color_component_g | color_component_b | color_component_a;
        g_rpi_states.colorBlendState[(int)no_blend] = state;

        state.blendEnable = true;
        state.srcColorBlendFactor = RHIBlendFactor::blend_factor_src_alpha;
        state.dstColorBlendFactor = RHIBlendFactor::blend_factor_one_minus_src_alpha;
        state.colorBlendOp = RHIBlendOp::blend_op_add;
        state.srcAlphaBlendFactor = RHIBlendFactor::blend_factor_zero;
        state.dstAlphaBlendFactor = RHIBlendFactor::blend_factor_one;
        state.alphaBlendOp = RHIBlendOp::blend_op_add;
        g_rpi_states.colorBlendState[(int)default_alpha_blend] = state;
    }

    void RPIInitState()
    {
        RPIInitDepthStencilState();
        RPIInitColorBlendState();
    }

    RPIDepthStencilState RPIGetDepthStencilState(RPIDepthStencilStateType type)
    {
        return RPIDepthStencilState(type);
    }

    RPIColorBlendState RPIGetColorBlendState(RPIColorBlendStateType type)
    {
        return g_rpi_states.colorBlendState[type];
    }

    RPIRenderPipelineDescriptor RPIGetDefaultRenderPipeline()
    {
        RPIRenderPipelineDescriptor desc{};
        desc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_no_depth_no_stencil);
        for (int i = 0; i < RHI_MAX_RENDER_TARGETS; i++)
        {
            desc.color_blend_state[i] = RPIGetColorBlendState(RPIColorBlendStateType::no_blend);
        }
        return desc;
    }
}