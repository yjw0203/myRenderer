#include "Utils.h"
#include <fstream>
#include <iostream>

void CRToString(const CXString& str, std::string& output)
{
    auto cstr = clang_getCString(str);

    output = cstr;

    clang_disposeString(str);
}
