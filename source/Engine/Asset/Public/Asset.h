#pragma once

#include <string>
#include <unordered_map>
#include <queue>
#include <functional>
#include "Engine/Utils/Public/DesignPatterns/Singleton.h"
#include "json.hpp"
#include "Engine/Utils/Public/Define.h"
#include "Generate/Public/generate.h"

namespace yjw
{
    using json = nlohmann::json;
    using AssetCreateAndDeserializeFunc = std::function<void* (const json*)>;
    using AssetDestoryFunc = std::function<void(void*)>;
    using AssetSerializeFunc = std::function<void(void*, json*)>;

    Class(AssetHeader)
    {
    public:
        std::string m_type;
    };
    
    struct AssetInfo
    {
        AssetHeader m_header;
        std::string m_url{};
        void* m_payload{nullptr};
        int m_salt;
        std::atomic_int m_ref_count;
        AssetDestoryFunc m_destory_func;
        AssetSerializeFunc m_serialize_func;
        AssetInfo() {}

        AssetInfo(const AssetInfo& other) noexcept {
            m_header = other.m_header;
            m_url = other.m_url;
            m_payload = other.m_payload;
            m_salt = other.m_salt;
            m_ref_count.store(other.m_ref_count);
            m_destory_func = other.m_destory_func;
            m_serialize_func = other.m_serialize_func;
        }
    };

    struct AssetID
    {
        int m_id{ 0 };
        int m_salt{ 0 };
    };
    
    class AssetManagerImplement
    {
    public:
        template<typename T>
        AssetID RegisterAsset(const char* url)
        {
            if (m_asset_ids.find(std::string(url)) != m_asset_ids.end())
            {
                AssetID id = m_asset_ids[std::string(url)];
                m_assets[id.m_id].m_ref_count.fetch_add(1);
                return id;
            }
            return LoadAsset(url, GetClassName<T>(),
            [](const json* j) {
                T* obj = new T();
                void from_json(const json & j, T& obj);
                from_json(*j, *obj);
                return (void*)obj;
            },
            [](void* obj) {
                T* t = (T*)obj;
                delete t;
            },
            [](void* obj, json* j) {
                void to_json(json & j, const T & obj);
                T* t = (T*)obj;
                to_json(*j, *t);
            });
        }

        void UnregisterAsset(AssetID id)
        {
            if (id.m_id >= 0 && id.m_id < m_assets.size())
            {
                if (m_assets[id.m_id].m_salt == id.m_salt)
                {
                    m_assets[id.m_id].m_ref_count.fetch_sub(1);
                    return;
                }
            }
        }

        void SaveAsset(AssetID id)
        {
            if (id.m_id >= 0 && id.m_id < m_assets.size())
            {
                if (m_assets[id.m_id].m_salt == id.m_salt)
                {
                    json j;
                    m_assets[id.m_id].m_serialize_func(m_assets[id.m_id].m_payload, &j);
                    SaveAssetToFile(m_assets[id.m_id].m_url.c_str(), m_assets[id.m_id].m_header, j);
                    return;
                }
            }
        }

        const AssetInfo* GetAssetInfo(AssetID id);

        AssetInfo* GetAssetInfoModifiable(AssetID id);

        // ensure on exclusive thread
        void process();

    private:
        AssetID LoadAsset(const char* url, const char* type, AssetCreateAndDeserializeFunc create_func, AssetDestoryFunc destory_func, AssetSerializeFunc serialize_func);
        AssetID AllocateAssetID();
        void DeallocateAssetID(AssetID id);
        void LoadAssetFromFile(const char* url, AssetHeader& header, json& obj);
        void SaveAssetToFile(const char* url, const AssetHeader& header, const json& obj);
    private:
        // ensure thread-safe when use, only modify on exclusive thread, except ref count.
        std::vector<AssetInfo> m_assets;
        std::unordered_map<std::string, AssetID> m_asset_ids;

        std::mutex m_load_asset_mutex;
        struct LoadInfo
        {
            AssetID m_id;
            std::string m_type;
            std::string m_url;
            AssetCreateAndDeserializeFunc m_create_func;
            AssetDestoryFunc m_destory_func;
            AssetSerializeFunc m_serialize_func;
            std::atomic_int m_ref_count;
            LoadInfo() {}

            LoadInfo(const LoadInfo& other) noexcept {
                m_id = other.m_id;
                m_type = other.m_type;
                m_url = other.m_url;
                m_create_func = other.m_create_func;
                m_destory_func = other.m_destory_func;
                m_serialize_func = other.m_serialize_func;
                m_ref_count.store(other.m_ref_count);
            }
        };
        std::vector<LoadInfo> m_pending_load_assets;

        std::mutex m_allocate_id_mutex;
        int m_current_allocate_id = 0;
        std::vector<int> m_reuse_ids;
        int m_salt_allocator = 1;

    };
    typedef Singleton<AssetManagerImplement> AssetManager;

    template<typename T>
    class Asset
    {
    public:
        Asset() : m_id{ -1, -1 } {}
        Asset(const char* url)
        {
            m_id = AssetManager::Get()->RegisterAsset<T>(url);
        }
        ~Asset()
        {
            AssetManager::Get()->UnregisterAsset(m_id);
        }
        Asset(Asset&& other) noexcept {
            m_id = other.m_id;
        }
        void SetURL(const char* url)
        {
            AssetManager::Get()->UnregisterAsset(m_id);
            m_id = AssetManager::Get()->RegisterAsset<T>(url);
        }
        const AssetInfo* GetInfo()
        {
            return AssetManager::Get()->GetAssetInfo(m_id);
        }
        const T* GetConstData()
        {
            if (GetInfo())
            {
                return (const T*)GetInfo()->m_payload;
            }
            return nullptr;
        }
        T* GetData()//only edit asset
        {
            if (AssetManager::Get()->GetAssetInfoModifiable(m_id))
            {
                return (T*)AssetManager::Get()->GetAssetInfoModifiable(m_id)->m_payload;
            }
            return nullptr;
        }
        void Save()
        {
            AssetManager::Get()->SaveAsset(m_id);
        }

    private:
        AssetID m_id{};
    };

    Class(AssetReferece)
    {
    public:
        std::string m_url;


        // todo: add bool m_is_loaded after reflection support not reflection filed.
        template<typename T>
        Asset<T> GetAsset()
        {
            return Asset<T>(m_url.c_str());
        }

        template<typename T>
        void LoadAsset()
        {
            AssetManager::Get()->RegisterAsset<T>(m_url.c_str());
        }

        void UnloadAsset()
        {
            //AssetManager::Get()->UnregisterAsset(m_id);
        }
    };
}