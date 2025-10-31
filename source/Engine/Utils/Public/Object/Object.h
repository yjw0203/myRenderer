#pragma once
#include "Engine/Utils/Public/Meta/Meta.h"
#include "Engine/Utils/Public/Define.h"
#include "Generate/Public/Generate.h"
#include "Engine/Utils/Public/Serialize/Archive.h"
namespace yjw
{
    class MObject
    {
    public:
        MObject() {}
        virtual void OnLoaded() {}
        virtual void Serialize(Archive& Ar);
        virtual const MetaClass* GetClass() = 0;
        static MObject* CreateNewObject(const char* name);
    };
}
