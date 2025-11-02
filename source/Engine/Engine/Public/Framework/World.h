#pragma once

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
    Level* GetLevel();
    RdScene* GetScene();

    void AttachLevel(Level* level);
    void DettachLevel();

    void LoadLevel(const char* url);
    void SaveLevel(const char* url);

private:
    Level* m_level = nullptr;
};
