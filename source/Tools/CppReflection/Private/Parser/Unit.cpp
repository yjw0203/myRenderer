#include "Unit.h"
#include <vector>
#include <string>

CRTranslationUnit::CRTranslationUnit(const CXTranslationUnit& handle)
    : m_handle(handle) { }

CRTranslationUnit::operator CXTranslationUnit() { return m_handle; }

CRCursor CRTranslationUnit::GetRootCursor()
{
    CXCursor cursor = clang_getTranslationUnitCursor(m_handle);
    return cursor;
}

namespace CRGlobalConfig
{
    bool g_displayDiagnostics = false;
    std::vector<std::string> g_arguments =
    {
        "-x",
        "c++",
        "-std=c++17",
        "-D__REFLECTION_PARSER__"
    };
}

CRTranslationUnit CRCreateTranslationUnitFromSourceFile(const char* source_filename)
{
    std::vector<const char*> arguments;
    for (std::string& argument : CRGlobalConfig::g_arguments)
    {
        arguments.push_back(argument.c_str());
    }

    CXIndex m_index = clang_createIndex(true, CRGlobalConfig::g_displayDiagnostics);
    CXTranslationUnit unit = clang_createTranslationUnitFromSourceFile(m_index, source_filename, arguments.size(), arguments.data(), 0, nullptr);
    return unit;
}