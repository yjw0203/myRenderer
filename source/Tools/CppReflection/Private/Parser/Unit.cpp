#include "Unit.h"
#include <vector>
#include <string>
#include "projectInfo.h"

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
        "-D__REFLECTION_PARSER__",
        "-DNDEBUG",
        "-D__clang__",
        //"-w",
        //"-MG",
        //"-M",
        //"-ferror-limit=0",
        "-o clangLog.txt"
    };

    std::vector<std::string> GetIncludeCommand()
    {
        std::vector<std::string> out_vec;
        std::string include_paths = INCLUDE_PATH;
        std::string header = "";
        for (int i = 0; i < include_paths.size(); i++)
        {
            char c = include_paths[i];
            if (c != ';')
            {
                header += c;
            }
            else
            {
                out_vec.push_back("-I" + header);
                header = "";
            }
        }
        if (header != "")
        {
            out_vec.push_back("-I" + header);
        }
        return out_vec;
    }
    std::vector<std::string> include_path_command = GetIncludeCommand();
}

CRTranslationUnit CRCreateTranslationUnitFromSourceFile(const char* source_filename)
{
    std::vector<const char*> arguments;
    for (std::string& argument : CRGlobalConfig::g_arguments)
    {
        arguments.push_back(argument.c_str());
    }
    for (std::string& argument : CRGlobalConfig::include_path_command)
    {
        arguments.push_back(argument.c_str());
    }

    CXIndex m_index = clang_createIndex(true, CRGlobalConfig::g_displayDiagnostics);
    CXTranslationUnit unit = clang_createTranslationUnitFromSourceFile(m_index, source_filename, arguments.size(), arguments.data(), 0, nullptr);
    return unit;
}