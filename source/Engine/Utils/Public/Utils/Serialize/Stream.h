#pragma once
#include "Public/Utils/DataStructure/Binary.h"

namespace yjw
{
    class Serializable
    {
    public:
        virtual void Serialize(Binary& binary) = 0;
        virtual void Unserialize(const Binary& binary, int& cur) = 0;
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