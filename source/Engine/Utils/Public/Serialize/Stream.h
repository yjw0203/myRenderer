#pragma once
#include "Engine/Utils/Public/DataStructure/Binary.h"
#include <iostream>

namespace yjw
{
    class Serializable
    {
    public:
        virtual void Serialize(Binary& binary)
        {
            binary.resize(sizeof(this));
            memcpy(binary.data(), this, std::min(binary.size(), sizeof(this)));
        }

        virtual void Unserialize(const Binary& binary, int& cur)
        {
            size_t size = sizeof(this);
            memcpy(this, binary.data() + cur, std::min(binary.size(), size));
            cur += size;
        }
    };

    class Stream
    {
    public:
        void LoadFromFile(char* url);
        void SaveToFile(char* url);

        template<typename T>
        void operator << (T& other);
        template<typename T>
        void operator >> (T& other);
    private:
        Binary m_data;
        int m_cur{ 0 };
    };

    template<typename T>
    void Stream::operator << (T& other)
    {
        Binary bin;
        other.Serialize(bin);
        m_data += bin;
    }

    template<typename T>
    void Stream::operator >> (T& other)
    {
        other.Unserialize(m_data, m_cur);
    }
}