#include "Engine/Utils/Public/Meta/Meta.h"

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

void* MetaClass::GetDefaultObject() const
{
    return m_default_object;
}

void MetaClass::SetDefaultObject(void* object)
{
    m_default_object = object;
}

void* MetaClass::NewObject() const
{
    void* ptr = malloc(m_size);
    memcpy(ptr, m_default_object, m_size);// to copy virtual ptr.
    return ptr;
}

std::map<std::string, MetaClass*> MetaClass::m_classes_map;
void MetaClass::RegisterClass(const char* name, MetaClass* class_ptr)
{
    m_classes_map[name] = class_ptr;
}

const MetaClass* MetaClass::GetClassRuntime(const char* name)
{
    return m_classes_map[name];
}

void* MetaClass::CreateObjectRuntime(const char* name)
{
    const MetaClass* cl = GetClassRuntime(name);
    if (cl)
    {
        return GetClassRuntime(name)->NewObject();
    }
    else
    {
        return nullptr;
    }
}
