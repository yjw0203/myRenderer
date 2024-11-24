#pragma once
#include "Meta/MetaClass.h"

class CRGenerator
{
public:
    void Generate(const std::vector<std::string>& include_files, const std::vector<meta::Class>& classes,const char* template_file, const char* out_file);
};