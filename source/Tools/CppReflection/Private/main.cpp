#include <iostream>
#include <vector>
#include <string>
#include "Parser/Unit.h"

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
    std::string cpp_file = "E:/workspace/CppReflection/Source/Example/Private/main.cpp";
    std::string generate_file_h = "E:/workspace/CppReflection/Source/Example/Private/main.generated.h";
    CRTranslationUnit unit = CRCreateTranslationUnitFromSourceFile(cpp_file.c_str());
    CRVisitor<CRCondition_IsMetaClass, CRAction_GatherClass> visitor;
    visitor.Visit(unit);
    Print(visitor.m_result);
    return 0;
}
