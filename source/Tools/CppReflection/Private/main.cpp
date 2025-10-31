#include <iostream>
#include <vector>
#include <string>
#include "Parser/Unit.h"
#include "Tools/CppReflection/Private/Generator/CRGenerator.h"
#include "projectInfo.h"
#include <filesystem>
#include <map>
#include <set>
#include "Tools/CppReflection/Private/Parser/AST.h"

std::string replacePrefix(const std::string& path, const std::string& oldPrefix, const std::string& newPrefix) {
    if (path.substr(0, oldPrefix.size()) == oldPrefix) {
        return newPrefix + path.substr(oldPrefix.size());
    }
    return path;
}

int main()
{
    CRASTree astree;
    astree.build("E:/workspace/myRenderer/source/Engine/Engine/Public/Framework/Components/StaticMeshComponent.h");

    std::vector<std::string> headers;
    std::map<std::string, CRASTree*> file_ast_map;
    std::set<std::string> class_set;
    std::string floder = SOURCE_PATH;
    std::string generate_h_floder = floder + "/Generate/Public";
    std::string generate_cpp_floder = floder + "/Generate/Private";
    for (const auto& entry : std::filesystem::recursive_directory_iterator(floder)) {
        if (entry.is_regular_file()) {
            std::filesystem::path file_path = entry.path();
            if (file_path.extension() == ".h"/* || file_path.extension() == ".cpp"*/) {
                std::cout << "Found header file: " << file_path << std::endl;
                CRASTree* ast = new CRASTree();
                ast->build(file_path.string().c_str());
                if (!ast->isEmpty())
                {
                    std::string tmp = file_path.replace_extension().string();
                    std::string generate_path = replacePrefix(tmp, floder, "");
                    file_ast_map[generate_path] = ast;
                    headers.push_back(file_path.string());
                }
                else
                {
                    delete ast;
                }
            }
        }
    }

    for (auto& itr : file_ast_map)
    {
        auto& file_name = itr.first;
        std::string ori_h_path = floder + file_name + ".h";
        std::string generate_cpp_path = generate_cpp_floder + file_name + ".generate.cpp";

        std::vector<std::string> header;
        header.push_back(ori_h_path);

        CRGenerator generator;
        generator.Generate(header, itr.second, CR_TEMPLATE_FILE(generate.cpp.mustache), generate_cpp_path.c_str());
    }
    return 0;
}
