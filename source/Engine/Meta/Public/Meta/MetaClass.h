#pragma once
#include <string>
#include "MetaField.h"
#include "MetaMethod.h"

namespace meta
{
    class Class
    {
    public:
        using List = std::vector<Class>;
        std::string m_namespace;
        std::string m_name;
        std::vector<std::string> m_inherit_classes;
        Field::List m_fields;
        Method::List m_methods;
    };
}