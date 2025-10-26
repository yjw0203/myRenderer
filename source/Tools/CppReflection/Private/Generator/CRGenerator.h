#pragma once
#include "Tools/CppReflection/Private/Parser/AST.h"

class CRGenerator
{
public:
    void Generate(const std::vector<std::string>& include_files, CRASTree* ast,const char* template_file, const char* out_file);
};