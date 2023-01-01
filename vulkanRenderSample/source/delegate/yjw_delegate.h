#pragma once

#include<vector>
#include<functional>

namespace yjw
{
    typedef std::function<void(void)> VoidFunc;

    class SimpleMulticastDelegate
    {
    public:
        SimpleMulticastDelegate(){ }
        void Broadcast()
        {
            for (auto func : m_funcs)
            {
                func();
            }
        }
        void Register(VoidFunc func)
        {
            m_funcs.push_back(func);
        }
    private:
        std::vector<VoidFunc> m_funcs;
    };
}