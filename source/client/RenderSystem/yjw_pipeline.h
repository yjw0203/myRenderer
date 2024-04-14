#pragma once

namespace yjw
{
    class Pipeline
    {
    public:
        virtual void initializeResource() = 0;
        virtual void render() = 0;
    };
}