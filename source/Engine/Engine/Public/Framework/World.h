#pragma once

namespace yjw
{
    class Level;
    class RdScene;
    class World
    {
    public:
        World() { InitWorld(); }
        ~World() { DestroyWorld(); }
        void InitWorld();
        void Tick(float deltaTime);
        void DestroyWorld();
        Level* GetLevel() { return m_level; }
        RdScene* GetScene() { return m_scene; }

    private:
        Level* m_level = nullptr;
        RdScene* m_scene = nullptr;
    };
}