#pragma once
#include <string>
#include <vector>
#include "MetaCommon.h"

namespace meta
{
    class Method
    {
    public:
        using List = std::vector<Method>;
        std::string m_name;
        std::string m_type;
        AccessType m_access;
    };

}
