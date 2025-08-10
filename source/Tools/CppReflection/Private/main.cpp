#include <iostream>
#include <vector>
#include <string>
#include "Parser/Unit.h"
#include "Tools/CppReflection/Private/Generator/CRGenerator.h"
#include "projectInfo.h"
#include <filesystem>
#include <map>
#include <set>

std::string replacePrefix(const std::string& path, const std::string& oldPrefix, const std::string& newPrefix) {
    if (path.substr(0, oldPrefix.size()) == oldPrefix) {
        return newPrefix + path.substr(oldPrefix.size());
    }
    return path;
}

void Print(const std::vector<meta::Class>& classes)
{
    for (const meta::Class& class_ : classes)
    {
        std::cout << "Class: " << class_.m_name << std::endl;
        for (const meta::Field& field : class_.m_fields)
        {
            std::cout << "Field: " << field.m_name << " " << field.m_type << std::endl;
        }
        for (const meta::Method& method : class_.m_methods)
        {
            std::cout << "Method: " << method.m_name << std::endl;
        }
    }
}

int main()
{
    std::vector<std::string> headers;
    std::map<std::string, std::map<std::string, meta::Class>> file_class_map;
    std::set<std::string> class_set;
    std::string floder = SOURCE_PATH;
    std::string generate_h_floder = floder + "/Generate/Public";
    std::string generate_cpp_floder = floder + "/Generate/Private";
    for (const auto& entry : std::filesystem::recursive_directory_iterator(floder)) {
        if (entry.is_regular_file()) {
            std::filesystem::path file_path = entry.path();
            if (file_path.extension() == ".h") {
                std::cout << "Found header file: " << file_path << std::endl;
                CRTranslationUnit unit = CRCreateTranslationUnitFromSourceFile(file_path.string().c_str());
                CRVisitor<CRCondition_IsMetaClass, CRAction_GatherClass> visitor;
                visitor.Visit(unit);
                if (!visitor.m_result.empty())
                {
                    std::string tmp = file_path.replace_extension().string();
                    std::string generate_path = replacePrefix(tmp, floder, "");
                    file_class_map[generate_path] = std::map<std::string, meta::Class>();
                    headers.push_back(file_path.string());
                    for (meta::Class& class_ : visitor.m_result)
                    {
                        std::string class_id = class_.m_namespace + "::" + class_.m_name;
                        if (!class_set.count(class_id))
                        {
                            file_class_map[generate_path][class_.m_namespace + "::" + class_.m_name] = class_;
                            class_set.insert(class_id);
                        }
                    }
                }
            }
        }
    }

    for (auto& itr : file_class_map)
    {
        auto& file_name = itr.first;
        std::string ori_h_path = floder + file_name + ".h";
        std::string generate_h_path = generate_h_floder + file_name + ".generate.h";
        std::string generate_cpp_path = generate_cpp_floder + file_name + ".generate.cpp";
        meta::Class::List class_list;
        for (auto& class_ : itr.second)
        {
            class_list.push_back(class_.second);
        }

        std::vector<std::string> header;
        header.push_back(ori_h_path);
        header.push_back(generate_h_path);

        CRGenerator generator;
        generator.Generate(header, class_list, CR_TEMPLATE_FILE(generate.h.mustache), generate_h_path.c_str());

        generator.Generate(header, class_list, CR_TEMPLATE_FILE(generate.cpp.mustache), generate_cpp_path.c_str());
    }
    return 0;
}
