#include "Engine/Engine/Public/Framework/Components/SkeletonComponent.h"

namespace yjw
{
    SkeletonComponent::SkeletonComponent()
    {
    }

    SkeletonComponent::~SkeletonComponent()
    {
    }

    void SkeletonComponent::OnLoaded()
    {
        LoadSkeleton(m_url.c_str());
    }

    void SkeletonComponent::LoadSkeleton(const char* url)
    {
        if (std::string("") == url)
        {
            return;
        }
        if (m_skeleton)
        {
            delete m_skeleton;
        }
        m_url = url;
        m_skeleton = new Skeleton();
        m_skeleton->BuildSkeleton(url);
    }

    Skeleton* SkeletonComponent::GetSkeleton()
    {
        return m_skeleton;
    }


}