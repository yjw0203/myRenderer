#pragma once

namespace yjw
{
    class Parameter
    {
    public:
        std::string m_name = "";
    };

    class Function
    {
    public:
        void Invoke() {};
    private:
        void* m_func_ptr = nullptr;
    };
}