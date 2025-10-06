#include "Engine/Utils/Public/Meta/Meta.h"

namespace yjw
{
    int MetaClass::GetSize() const
    {
        return m_size;
    }

    const char* MetaClass::GetName() const
    {
        return m_name.c_str();
    }

    void MetaClass::SetSize(int size)
    {
        m_size = size;
    }

    void MetaClass::SetName(const char* name)
    {
        m_name = name;
    }

    void MetaClass::AddFunction(const char* name, const Function& func)
    {

    }

    void MetaClass::AddField(const char* name, const Field& field)
    {

    }

    void* MetaClass::GetDefaultObject()
    {
        return m_default_object;
    }

    void MetaClass::SetDefaultObject(void* object)
    {
        m_default_object = object;
    }
}