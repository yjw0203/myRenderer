#pragma once
#include <clang-c/Index.h>
#include <string>

void CRToString(const CXString& str, std::string& output);

int CRWriteFile(const char* path, const std::string& out_str);