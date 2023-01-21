#pragma once

namespace yjw
{
    class IModule
    {
    public:
        virtual void startup() = 0;
    };

    extern std::vector<IModule* > g_modules;

#define IMPLEMENT_MODULE(NAME) g_modules // todo
}