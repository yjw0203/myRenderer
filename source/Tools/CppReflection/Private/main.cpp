#include <iostream>
#include <vector>
#include <string>
#include "Parser/Unit.h"
#include "Tools/CppReflection/Private/Generator/CRGenerator.h"
#include "projectInfo.h"
#include <filesystem>
#include <map>

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
    std::map<std::string, meta::Class> class_map;
    std::string floder = SOURCE_PATH;
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
                    headers.push_back(file_path.string());
                    for (meta::Class& class_ : visitor.m_result)
                    {
                        class_map[class_.m_namespace + "::" + class_.m_name] = class_;
                    }
                }
            }
        }
    }

    meta::Class::List class_list;
    for (auto& class_ : class_map)
    {
        class_list.push_back(class_.second);
    }

    std::string h_file = std::string("") + SOURCE_PATH + "/" + "UnitTest/Test_Utils/Private/main.h";
    std::string generate_file_h = "E:/workspace/CppReflection/Source/Example/Private/main.generated.h";
    CRTranslationUnit unit = CRCreateTranslationUnitFromSourceFile(h_file.c_str());
    CRVisitor<CRCondition_IsMetaClass, CRAction_GatherClass> visitor;
    visitor.Visit(unit);
    CRGenerator generator;
    generator.Generate(headers, class_list, CR_TEMPLATE_FILE(generate.h.mustache), CR_GENERATE_PUBLIC_FILE(generate.h));

    headers.push_back(CR_GENERATE_PUBLIC_FILE(generate.h));
    generator.Generate(headers, class_list, CR_TEMPLATE_FILE(generate.cpp.mustache), CR_GENERATE_PRIVATE_FILE(generate.cpp));
    Print(visitor.m_result);
    return 0;
}
