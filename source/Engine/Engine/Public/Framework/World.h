#pragma once

namespace yjw
{
    class Level;
    class SceneInterface;
    class World
    {
    public:
        World() { InitWorld(); }
        ~World() { DestroyWorld(); }
        void InitWorld();
        void DestroyWorld();
        Level* GetLevel() { return m_level; }
        SceneInterface* GetScene() { return m_scene; }

    private:
        Level* m_level = nullptr;
        SceneInterface* m_scene = nullptr;
    };
}