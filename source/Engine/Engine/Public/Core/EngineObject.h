#pragma once
#include "Engine/Utils/Public/Object/Object.h"

class EngineObjectBase
{
public:
    EngineObjectBase(){}
    virtual ~EngineObjectBase(){}
protected:

};

template<typename T>
class EngineObject : public EngineObjectBase
{
public:
    T* Get() { return m_ptr; }
    bool operator() { return m_ptr != nullptr; }
    T* operator() { return m_ptr; }
private:
    T* m_ptr = nullptr;
};

class EngineObjectDependencyInvoker
{
public:
    virtual void Invoke(EngineObjectBase* object) = 0;
};

class EngineObjectable : public MObject
{
public:
    void EngineObjectInvokeDependency(EngineObjectDependencyInvoker* invoker) {};
};
