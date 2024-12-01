#pragma once
#include <vector>

namespace yjw
{
    class Binary
    {
    public:
        void operator += (Binary& other);
        void resize(int sizeByte) { m_data.resize(sizeByte); }
        void clear() { m_data.clear(); }
        char* data() { return m_data.data(); }
        const char* data() const { return m_data.data(); }
        size_t size() const { return m_data.size(); }
    private:
        std::vector<char> m_data;
    };

    void Binary::operator += (Binary& other)
    {
        m_data.insert(m_data.end(), other.m_data.begin(), other.m_data.end());
    }
}
