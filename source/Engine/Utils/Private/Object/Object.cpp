#include "Engine/Utils/Public/Object/Object.h"

namespace yjw
{
    void MObject::Serialize(Archive& Ar)
    {
    }

    MObject* MObject::CreateNewObject(const char* name)
    {
        return (MObject*)MetaClass::CreateObjectRuntime(name);
    }
}