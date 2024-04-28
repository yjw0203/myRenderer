#pragma once

namespace yjw
{
    class RenderSystem
    {
    public:
        
        static RenderSystem& get()
        {
            static RenderSystem instance;
            return instance;
        }

        virtual void startup()
        {

        }

        void initialize();
        void tick();
        void shutdown();

        float deltaTime = 0.f;

        class RenderCamera* activeCamera = nullptr;

        class Scene* scene = nullptr;
    };

}