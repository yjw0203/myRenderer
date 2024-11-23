#include "Utils.h"
#include <fstream>
#include <iostream>

void CRToString(const CXString& str, std::string& output)
{
    auto cstr = clang_getCString(str);

    output = cstr;

    clang_disposeString(str);
}

int CRWriteFile(const char* path, const std::string& out_str) 
{
	std::ofstream f;
	std::string st = "这是一个测试字符串\n";
	f.open(path, std::ios::out);
	if (!f.is_open()) {
		std::cout << "无法打开文件！" << std::endl;
		return 0;
	}
	f << out_str << std::endl;
	f.close();
	return 1;
}