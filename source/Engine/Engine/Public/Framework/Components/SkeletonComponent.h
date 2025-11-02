#pragma once
#include "Engine/Engine/Public/Framework/Components/Component.h"
#include "Engine/Utils/Public/DesignPatterns/ECS.h"
#include "Engine/Animation/Public/Skeleton.h"

class Meta() SkeletonComponent : public Component
{
    GENERATED_BODY();
public:
    SkeletonComponent();
    ~SkeletonComponent();
    virtual void OnLoaded();
    void LoadSkeleton(const char* url);

    Skeleton* GetSkeleton();

private:
    Meta()
    std::string m_url;

    Skeleton* m_skeleton = nullptr;
};
