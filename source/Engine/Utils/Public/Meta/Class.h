#pragma once
#include <map>
#include <string>
#include <vector>
#include "Engine/Utils/Public/Meta/Function.h"

namespace yjw
{
    class Field
    {
    public:
        Field(const char* name, int offset, int size)
            : m_name(name), m_offset(offset), m_size(size) {};
        int m_offset = 0;
        int m_size = 0;
        std::string m_name = "";
    };

    class MetaClass
    {
    public:
        int GetSize() const;
        const char* GetName() const;
        void SetSize(int size);
        void SetName(const char* name);
        void AddFunction(const char* name, const Function& func);
        void AddField(const char* name, const Field& field);
        void* GetDefaultObject() const;
        void SetDefaultObject(void* object);
        void* NewObject() const;
    private:
        void* m_default_object = nullptr;
        int m_size = 0;
        std::string m_name;
        std::vector<std::pair<std::string, Function>> m_functions;
        std::map<std::string, Field> m_field;
    public:
        static void RegisterClass(const char* name, MetaClass* class_ptr);
        static const MetaClass* GetClassRuntime(const char* name);
        static void* CreateObjectRuntime(const char* name);
    private:
        static std::map<std::string, MetaClass*> m_classes_map;
    };
}