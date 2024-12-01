#include "Engine/RHI/Public/rpi/yjw_rpi_state.h"

namespace rpi
{
    struct RPIStates
    {
        RPIDepthStencilState depthStencilState[depth_stencil_state_type_count];
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

    void RPIInitState()
    {
        RPIInitDepthStencilState();
    }

    RPIDepthStencilState RPIGetDepthStencilState(RPIDepthStencilStateType type)
    {
        return RPIDepthStencilState(type);
    }

    RPIRenderPipelineDescriptor RPIGetDefaultRenderPipeline()
    {
        RPIRenderPipelineDescriptor desc{};
        desc.depth_stencil_state = RPIGetDepthStencilState(RPIDepthStencilStateType::default_no_depth_no_stencil);
        return desc;
    }
}