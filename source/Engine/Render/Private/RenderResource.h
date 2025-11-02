#pragma once

class RenderResource
{
public:
    //virtual bool IsLoaded() = 0;
    //virtual void Load(const char* name) = 0;
    //virtual void Unload() = 0;
private:

};

/*
template<typename T>
class RenderResourceProxy
{
public:
    bool operator()
    {
        return m_shared_ptr;
    }

    bool IsLoaded();
    void Load();
private:
    std::shared_ptr<T> m_shared_ptr;
};

    

class RenderResourceManager
{
public:
    template<typename T>
    std::shared_ptr<T> LoadResource(const char* name)
    {
        if (m_resource_maps.find(name) == m_resource_maps.end())
        {
            m_resource_maps[name] = std::make_shared<T>(name);
        }
        return (std::shared_ptr<T>)m_resource_maps[name];
    }
private:
    std::unordered_map<std::string, std::shared_ptr<RenderResource>> m_resource_maps;
};
*/
