#include "yjw_global_delegate.h"

namespace yjw
{
    SimpleMulticastDelegate CoreDelegate::OnApplicationInitialize;
    SimpleMulticastDelegate CoreDelegate::OnApplicationLoop;
    SimpleMulticastDelegate CoreDelegate::OnApplicationShutdown;
}
