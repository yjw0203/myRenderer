#pragma once
#include <string>
#include <vector>
#include "MetaCommon.h"
namespace meta
{
    class Field
    {
    public:
        using List = std::vector<Field>;
        std::string m_name;
        std::string m_type;
        AccessType m_access;
    };
}