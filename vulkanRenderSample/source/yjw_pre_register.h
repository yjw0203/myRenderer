#pragma once

#include <functional>
#include <vector>

namespace yjw
{
    class PreRegisterHelper
    {
    public:
        static PreRegisterHelper& get()
        {
            static PreRegisterHelper instance;
            return instance;
        }

        void executePreRegister()
        {
            for (std::function<void(void)> action : m_register_action)
            {
                action();
            }
        }

        void addRegister(std::function<void(void)> register_action)
        {
            m_register_action.push_back(register_action);
        }

    private:
        std::vector<std::function<void(void)> > m_register_action;
    };
}