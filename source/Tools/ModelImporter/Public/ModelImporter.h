#pragma once
#include <string>

namespace yjw
{
    void ImportModel(const std::string& out_path, const std::string& name, const std::string& in_path);
    void ImportAnimation(const std::string& out_path, const std::string& name, const std::string& in_path);
    void ImportAsset(const std::string& out_path, const std::string& name, const std::string& in_path);
}