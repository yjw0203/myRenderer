#include "yjw_rhi_pipeline.h"

namespace rhi
{
    RHIGraphicsPipelineCreator RHIPipeline::NewGraphicsPipline()
    {
        return RHIGraphicsPipelineCreator();
    }

    RHIComputePipelineCreator RHIPipeline::NewComputePipline()
    {
        return RHIComputePipelineCreator();
    }

    RHIPipeline::~RHIPipeline() 
    {
        RHIPipelineDestoryer destory = RHIPipelineDestoryer(this); 
    }
}