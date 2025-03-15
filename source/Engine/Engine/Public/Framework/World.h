#pragma once

namespace yjw
{
    class Level;
    class Scene;
    class World
    {
    public:
        World() { InitWorld(); }
        ~World() { DestroyWorld(); }
        void InitWorld();
        void DestroyWorld();
        Level* GetLevel() { return m_level; }
        Scene* GetScene() { return m_scene; }

    private:
        Level* m_level = nullptr;
        Scene* m_scene = nullptr;
    };
}