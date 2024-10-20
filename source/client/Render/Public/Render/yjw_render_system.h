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

        void AttachScene(class Scene* scene);

        float deltaTime = 0.f;

        class RenderCamera* activeCamera = nullptr;

        class Scene* m_scene = nullptr;
    };

}