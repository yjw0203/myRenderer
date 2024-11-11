#pragma once

namespace yjw
{
    class Module
    {
    public:
        virtual void StartUp() = 0;
        virtual void ShutDown() = 0;
        virtual void ClientTick(float DeltaTime) = 0;
        virtual const char* GetModuleName() = 0;
    private:
    };

    template<class T>
    class ModuleInterface : Module
    {
    public:
        static T* Get()
        {
            static T instance;
            return &instance;
        }
    };
}