#pragma once

#include<vector>
#include<functional>

namespace yjw
{
#define _STR_CAT(x,y,z) (x##y##z)
#define STR_CAT(x,y,z) _STR_CAT(x,y,z)

#define EXTERN_DELEGATE(NAME) \
    class NAME##Delegate                                        \
    {                                                           \
    public:                                                     \
        NAME##Delegate() { }                                    \
        void Broadcast()                                        \
        {                                                       \
            for (auto func : m_funcs)                           \
            {                                                   \
                func();                                         \
            }                                                   \
        }                                                       \
        void Register(std::function<void(void)> func)           \
        {                                                       \
            m_funcs.push_back(func);                            \
        }                                                       \
        static NAME##Delegate& get()                            \
        {                                                       \
            static NAME##Delegate instance;                     \
            return instance;                                    \
        }                                                       \
    private:                                                    \
        std::vector<std::function<void(void)> > m_funcs;        \
    };                                                          \

#define REGISTER_DELEGATE(NAME,Func) static int STR_CAT(NAME,__LINE__,DelegateRegister) = (NAME##Delegate::get().Register(Func), 0);
#define BROADCAST_DELEGATE(NAME) NAME##Delegate::get().Broadcast();
}

