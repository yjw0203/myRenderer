#pragma once
#include <vector>
#include <map>
#include <string>

namespace yjw
{
    class Archive;
    void SerializeImpl(Archive& Ar, void* data, int size);

    template<typename T>
    void Serialize(Archive& Ar, T& obj)
    {
        SerializeImpl(Ar, &obj, sizeof(obj));
    }

    class Archive
    {
    public:
        virtual ~Archive() {};

        template<typename BaseType>
        Archive& operator<<(BaseType& value) {
            Serialize(*this, value);
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

        virtual void SerializeInternal(void* data, int length) = 0;

        virtual void close() {}
    };

    inline void SerializeImpl(Archive& Ar, void* data, int size)
    {
        Ar.SerializeInternal(data, size);
    }
}