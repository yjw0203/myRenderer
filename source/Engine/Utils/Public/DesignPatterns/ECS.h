#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <functional>

namespace yjw
{
    class IComponent {
    public:
        virtual ~IComponent() = default;
    };

    class EntityManager;
    class Entity {
    public:
        Entity() {}
        Entity(int id, EntityManager* entity_manager) : m_id(id), m_entity_manager(entity_manager) {}
        operator int() { return m_id; }
        int GetId() const { return m_id; }
        bool operator==(const Entity& rhs) const {
            return GetId() == rhs.GetId();
        }
        template<typename T>
        T* AddComponent() {
            if (m_entity_manager)
            {
                return m_entity_manager->GetComponentManager().AddComponent<T>(*this);
            }
            return nullptr;
        }
        template<typename T>
        T* GetComponent() {
            if (m_entity_manager)
            {
                return m_entity_manager->GetComponentManager().GetComponent<T>(*this);
            }
            return nullptr;
        }
        template<typename T>
        void RemoveComponent() {
            if (m_entity_manager)
            {
                return m_entity_manager->GetComponentManager().RemoveComponent<T>(*this);
            }
        }

        void SetParent(void* data) { m_parent = data; }
        void* GetParent() { return m_parent; }

    private:
        void* m_parent = nullptr;
        int m_id;
        EntityManager* m_entity_manager = nullptr;
    };
    
}

namespace std {
    template<>
    struct hash<yjw::Entity> {
        size_t operator()(const yjw::Entity& obj) const {
            return hash<int>()(obj.GetId());
        }
    };
}

namespace yjw
{
    class ComponentManager {
    public:
        template<typename T>
        T* AddComponent(Entity& entity) {
            return (T*)(components[entity][typeid(T)] = std::make_unique<T>()).get();
        }

        template<typename T>
        T* GetComponent(Entity& entity) {
            auto it = components.find(entity);
            if (it != components.end()) {
                auto typeIt = it->second.find(typeid(T));
                if (typeIt != it->second.end()) {
                    return static_cast<T*>(typeIt->second.get());
                }
            }
            return nullptr;
        }

        template<typename T>
        void RemoveComponent(Entity& entity) {
            auto it = components.find(entity);
            if (it != components.end()) {
                it->second.erase(typeid(T));
            }
        }

    private:
        using EntityComponentMap = std::unordered_map<Entity, std::unordered_map<std::type_index, std::unique_ptr<IComponent>>>;
        EntityComponentMap components;
    };

    class EntityManager {
    public:
        EntityManager() : nextId(0) {}

        Entity& CreateEntity() {
            Entity entity(nextId++, this);
            entities.push_back(entity);
            return entities.back();
        }

        std::vector<Entity>& GetEntities() { return entities; }

        ComponentManager& GetComponentManager() { return componentManager; }

    private:
        std::vector<Entity> entities;
        ComponentManager componentManager;
        int nextId;
    };

    class SystemBase {
    public:
        virtual ~SystemBase() = default;
        virtual void Update(float deltaTime) = 0;
        void SetEntityManager(class EntityManager* entity_manager) { m_entity_manager = entity_manager; }
        class EntityManager* GetEntityManager() { return m_entity_manager; }
        std::vector<Entity>& GetEntities() { return GetEntityManager()->GetEntities(); }
    protected:
        class EntityManager* m_entity_manager = nullptr;
    };

    class SystemManager {
    public:
        SystemManager(EntityManager& entityManager) : m_entity_manager{ entityManager } {}
        template<typename T>
        T* RegisterSystem() {
            std::unique_ptr<T> system = std::make_unique<T>();
            system->SetEntityManager(&m_entity_manager);
            systems.push_back(move(system));
            return system.get();
        }

        void Update(float deltaTime) {
            for (auto& system : systems) {
                system->Update(deltaTime);
            }
        }

    private:
        EntityManager& m_entity_manager;
        std::vector<std::unique_ptr<SystemBase>> systems;
    };

    class ECSManager{
    public:
        EntityManager& GetEntityManager() { return m_entity_manager; }
        SystemManager& GetSystemManager() { return m_system_manager; }

    private:
        EntityManager m_entity_manager;
        SystemManager m_system_manager{ m_entity_manager };
    };

}
