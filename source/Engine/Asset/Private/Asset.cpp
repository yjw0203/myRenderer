#include "Engine/Asset/Public/Asset.h"
#include <fstream>
#include <iostream>
#include <filesystem>

namespace yjw
{
    const AssetInfo* AssetManagerImplement::GetAssetInfo(AssetID id)
    {
        return GetAssetInfoModifiable(id);
    }

    AssetInfo* AssetManagerImplement::GetAssetInfoModifiable(AssetID id)
    {
        if (id.m_id >= 0 && id.m_id < m_assets.size())
        {
            if (m_assets[id.m_id].m_salt == id.m_salt)
            {
                return &m_assets[id.m_id];
            }
        }
        return nullptr;
    }

    void AssetManagerImplement::process()
    {
        for (int index = 0; index < m_assets.size(); index++)
        {
            assert(m_assets[index].m_ref_count.load() >= 0);
            if (m_assets[index].m_ref_count.load() == 0)
            {
                m_assets[index].m_destory_func(m_assets[index].m_payload);
                m_asset_ids.erase(std::string(m_assets[index].m_url));
                DeallocateAssetID(AssetID{ index,m_assets[index].m_salt });
                m_assets[index].m_url = "";
                m_assets[index].m_salt = 0;
                m_assets[index].m_ref_count.store(0);
                m_assets[index].m_payload = nullptr;
                m_assets[index].m_destory_func = AssetDestoryFunc{};
            }
        }

        for (int index = 0; index < m_pending_load_assets.size(); index++)
        {
            LoadInfo& load_info = m_pending_load_assets[index];
            m_asset_ids[load_info.m_url] = load_info.m_id;
            while (m_assets.size() <= load_info.m_id.m_id)
            {
                m_assets.push_back(AssetInfo{});
                AssetInfo& asset_info = m_assets.back();
                asset_info.m_header.m_type = load_info.m_type;
                asset_info.m_salt = load_info.m_id.m_salt;
                asset_info.m_url = load_info.m_url;
                asset_info.m_ref_count.store(load_info.m_ref_count);
                asset_info.m_destory_func = load_info.m_destory_func;
                asset_info.m_serialize_func = load_info.m_serialize_func;
                json j = json::object();
                LoadAssetFromFile(load_info.m_url.c_str(), asset_info.m_header, j);
                asset_info.m_payload = load_info.m_create_func(&j);
            }
        }
        m_pending_load_assets.clear();

    }

    AssetID AssetManagerImplement::LoadAsset(const char* url, const char* type, AssetCreateAndDeserializeFunc create_func, AssetDestoryFunc destory_func, AssetSerializeFunc serialize_func)
    {
        std::lock_guard guard(m_load_asset_mutex);
        for (int i = 0; i < m_pending_load_assets.size(); i++)
        {
            if (m_pending_load_assets[i].m_url == url)
            {
                m_pending_load_assets[i].m_ref_count.fetch_add(1);
                return m_pending_load_assets[i].m_id;
            }
        }
        AssetID id = AllocateAssetID();
        LoadInfo info{};
        info.m_id = id;
        info.m_type = type;
        info.m_url = url;
        info.m_create_func = create_func;
        info.m_destory_func = destory_func;
        info.m_serialize_func = serialize_func;
        info.m_ref_count.store(1);
        m_pending_load_assets.push_back(info);
        process();//load imidiately now
        return id;
    }

    AssetID AssetManagerImplement::AllocateAssetID()
    {
        std::lock_guard guard(m_allocate_id_mutex);

        int id = 0;
        if (m_reuse_ids.size() > 0)
        {
            id = m_reuse_ids.back();
            m_reuse_ids.pop_back();
        }
        else
        {
            id = m_current_allocate_id++;
        }
        int salt = m_salt_allocator++;
        return AssetID{ id, salt };
    }

    void AssetManagerImplement::DeallocateAssetID(AssetID id)
    {
        std::lock_guard guard(m_allocate_id_mutex);
        m_reuse_ids.push_back(id.m_id);
    }

    void AssetManagerImplement::LoadAssetFromFile(const char* url, AssetHeader& header, json& obj)
    {
        std::string filename = std::string("E:/workspace/myRenderer/resource/") + url;
        std::fstream file;
        file.open(filename, std::ios::in);

        if (!file.is_open()) {
            return ;
        }

        try
        {
            json h;
            file >> h >> obj;
            from_json(h, header);
        }
        catch (...)
        {

        }

        file.close();
    }

    void AssetManagerImplement::SaveAssetToFile(const char* url, const AssetHeader& header, const json& obj)
    {
        std::string filename = std::string("E:/workspace/myRenderer/resource/") + url;
        std::filesystem::path path(filename);
        std::filesystem::create_directories(path.parent_path());
        std::ofstream file(filename);
        file.open(filename, std::ios::out | std::ios::in);

        if (!file.is_open()) {
            return;
        }
        file.clear();
        json h;
        void to_json(json & j, const AssetHeader & obj);
        to_json(h, header);
        file << h << obj << std::endl;
        file.flush();
        file.close();
    }
}