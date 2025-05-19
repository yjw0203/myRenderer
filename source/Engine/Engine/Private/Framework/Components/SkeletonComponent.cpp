#include "Engine/Engine/Public/Framework/Components/SkeletonComponent.h"

namespace yjw
{
    SkeletonComponent::SkeletonComponent()
    {
        m_skeleton = new Skeleton();
    }

    SkeletonComponent::~SkeletonComponent()
    {
        delete m_skeleton;
    }

    void SkeletonComponent::LoadSkeleton(const char* url)
    {
        m_skeleton->BuildSkeleton(url);
    }

    Skeleton* SkeletonComponent::GetSkeleton()
    {
        return m_skeleton;
    }


}