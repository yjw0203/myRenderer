#include "yjw_rhi_descriptr_set.h"

namespace rhi
{
    RHIAttachmentsSetCreator RHIAttachmentsSet::New(std::shared_ptr<RHIPipeline> pipeline, int width, int height)
    {
        return RHIAttachmentsSetCreator(pipeline, width, height);
    }

    RHIDescriptorsSetCreator RHIDescriptorsSet::New(std::shared_ptr<RHIPipeline> pipeline)
    {
        return RHIDescriptorsSetCreator(pipeline);
    }
}