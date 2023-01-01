#pragma once

#include <stdio.h>
#include "yjw_pre_register.h"
namespace yjw
{
    template<typename T>
    class StartupHelper
    {
    public:
        StartupHelper()
        {
            PreRegisterHelper::get().addRegister([] {T::get().startup(); });
        }
    };
#define HelpStartup(T) yjw::StartupHelper<T> T##StarterHelper;
}
