#pragma once

#include "delegate/yjw_delegate.h"

namespace yjw
{
    class CoreDelegate
    {
    public:
        static SimpleMulticastDelegate OnApplicationInitialize;
        static SimpleMulticastDelegate OnApplicationLoop;
        static SimpleMulticastDelegate OnApplicationShutdown;
    };
}
#define RegisterDelegate(Delegate,Func) DelegateRegister(Delegate,Func);