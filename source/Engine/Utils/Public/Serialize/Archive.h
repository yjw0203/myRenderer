#pragma once
#include <vector>
#include <map>
#include <string>
#include "Engine/Utils/Public/Object/Object.h"

namespace yjw
{
    class Archive;

    template<typename T, typename = void>
    struct HasSerializeMethod : std::false_type {};

    template<typename T>
    struct HasSerializeMethod<T, std::void_t<decltype(std::declval<T&>().Serialize(std::declval<Archive&>()))>>
        : std::true_type {};


    void SerializeImpl(Archive& Ar, void* data, int size);

    template<typename T>
    void Serialize(Archive& Ar, T& obj)
    {
        SerializeImpl(Ar, &obj, sizeof(obj));
    }

    enum class ArchiveType
    {
        read,
        write
    };

    class Archive
    {
    public:
        virtual ~Archive() {};

        virtual ArchiveType GetArchiveType() = 0;

        template<typename BaseType/*, typename = std::enable_if_t<!HasSerializeMethod<BaseType>::value>*/>
        auto operator<<(BaseType& value) -> std::enable_if_t<!HasSerializeMethod<BaseType>::value, Archive&>
        {
            SerializeInternal(&value, sizeof(value));
            return *this;
        }

        template<typename BaseType/*, typename = std::enable_if_t<HasSerializeMethod<BaseType>::value>*/>
        auto operator<<(BaseType& value) -> std::enable_if_t<HasSerializeMethod<BaseType>::value, Archive&>
        {
            value.Serialize(*this);
            return *this;
        }

        template<typename BaseType>
        Archive& operator<<(BaseType*& value) {
            int not_null;
            std::string class_name;
            if (GetArchiveType() == ArchiveType::read)
            {
                (*this) << not_null;
                if (not_null)
                {
                    (*this) << class_name;
                    value = (BaseType*)MetaClass::CreateObjectRuntime(class_name.c_str());
                    (*this) << *value;
                }
            }
            else if(GetArchiveType() == ArchiveType::write)
            {
                not_null = value != nullptr ? 1 : 0;
                (*this) << not_null;
                if (not_null)
                {
                    class_name = value->GetClass()->GetName();
                    (*this) << class_name;
                    (*this) << *value;
                }
            }
            return *this;
        }
        
        template<>
        Archive& operator<<(std::string& value) {
            std::vector<char> vec(value.begin(), value.end());
            (*this) << vec;
            value = std::string(vec.begin(), vec.end());
            return *this;
        }

        template<typename BaseType1, typename BaseType2>
        Archive& operator<<(std::pair<BaseType1, BaseType2>& value) {
            (*this) << value.first;
            (*this) << value.second;
            return *this;
        }

        template<typename BaseType>
        Archive& operator<<(std::vector<BaseType>& vec) {
            int num = vec.size();
            SerializeInternal(&num, sizeof(int));
            if (vec.size() < num)
            {
                vec.resize(num);
            }
            for (int i = 0; i < num; i++)
            {
                (*this) << vec[i];
            }
            return *this;
        }

        template<typename KeyType, typename ValueType>
        Archive& operator<<(std::map<KeyType, ValueType>& mp) {
            std::vector<std::pair<KeyType, ValueType>> vec(mp.begin(), mp.end());
            (*this) << vec;
            mp.clear();
            for (std::pair<KeyType, ValueType>& elm : vec)
            {
                mp.emplace(elm.first, elm.second);
            }
            return *this;
        }

        void Serialize(void* data, int length)
        {
            SerializeInternal(data, length);
        }

        virtual void SerializeInternal(void* data, int length) = 0;

        virtual void close() {}
    };

    inline void SerializeImpl(Archive& Ar, void* data, int size)
    {
        Ar.SerializeInternal(data, size);
    }
}