#pragma once
#include <string>

namespace rhi
{
    typedef std::string RHIName;

#define RHI_NAME(name) RHIName(#name)
}