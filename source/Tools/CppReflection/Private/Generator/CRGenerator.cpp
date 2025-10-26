#include "Tools/CppReflection/Private/Generator/CRGenerator.h"
#include <fstream>
#include <filesystem>
#include "mustache.hpp"

namespace Mustache = kainjow::mustache;

std::string MustacheRenderTemplate(const std::string& template_str, Mustache::data& data)
{
    Mustache::mustache tmpl(template_str);
    return tmpl.render(data);
}

std::string readFile(const char* file_name)
{
    std::ifstream file(file_name);
    std::string content;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            content += line + "\n";
        }
        file.close();
        return content;
    }
    else {
        return "";
    }
}

void writeFile(const char* file_name, const std::string& str)
{
    std::string abs_path_str = file_name;
    std::filesystem::path abs_path(abs_path_str);
    std::filesystem::create_directories(abs_path.parent_path());

    std::ofstream file(file_name);
    if (file.is_open()) {
        file << str;
        file.close();
    }
    else {
    }
}

void GenerateMustanceData(CREnum* obj, Mustache::data& data)
{
    data.set("name", obj->m_name);
    Mustache::data item_list_data;
    for (const std::string& item : obj->m_items)
    {
        item_list_data.push_back(item);
    }
    data.set("item_list", item_list_data);
}

void GenerateMustanceData(CRParm* obj, Mustache::data& data)
{
    data.set("name", obj->m_name);
    data.set("type", obj->m_type);
    data.set("is_fundamental_type", obj->m_is_fundamental_type);
}

void GenerateMustanceData(CRMethod* obj, Mustache::data& data)
{
    data.set("name", obj->m_name);
    Mustache::data parm_list_data;
    for (CRParm* item : obj->m_parms)
    {
        parm_list_data.push_back(item);
    }
    data.set("parm_list", parm_list_data);
}

void GenerateMustanceData(CRField* obj, Mustache::data& data)
{
    data.set("name", obj->m_name);
    data.set("type", obj->m_type);
    data.set("is_fundamental_type", obj->m_is_fundamental_type);
}

void GenerateMustanceData(CRVar* obj, Mustache::data& data)
{
    data.set("name", obj->m_name);
    data.set("type", obj->m_type);
    data.set("is_fundamental_type", obj->m_is_fundamental_type);
}

void GenerateMustanceData(CRFunction* obj, Mustache::data& data)
{
    data.set("name", obj->m_name);
    Mustache::data parm_list_data;
    for (CRParm* item : obj->m_parms)
    {
        parm_list_data.push_back(item);
    }
    data.set("parm_list", parm_list_data);
}

void GenerateMustanceData(CRStruct* obj, Mustache::data& data);
void GenerateMustanceData(CRClass* obj, Mustache::data& data);

template<typename T>
void SetGenerateMustanceDataList(Mustache::data& data, const char* name, std::vector<T*>& obj)
{
    Mustache::data list_data(Mustache::data::type::list);
    for (T* item : obj)
    {
        Mustache::data item_data;
        GenerateMustanceData(item, item_data);
        list_data.push_back(item_data);
    }
    data.set(name, list_data);
}

void GenerateMustanceData(CRStruct* obj, Mustache::data& data)
{
    data.set("name", obj->m_name);
    SetGenerateMustanceDataList(data, "enum_list", obj->m_enums);
    SetGenerateMustanceDataList(data, "class_list", obj->m_classes);
    SetGenerateMustanceDataList(data, "struct_list", obj->m_structs);
    SetGenerateMustanceDataList(data, "var_list", obj->m_vars);
    SetGenerateMustanceDataList(data, "function_list", obj->m_functions);
    SetGenerateMustanceDataList(data, "metahod_list", obj->m_methods);
    SetGenerateMustanceDataList(data, "field_list", obj->m_fields);
    Mustache::data list_data(Mustache::data::type::list);
    for (const std::string& item : obj->m_inherits)
    {
        Mustache::data data;
        data.set("name", item);
        list_data.push_back(data);
    }
    data.set("inherit_list", list_data);
}

void GenerateMustanceData(CRClass* obj, Mustache::data& data)
{
    data.set("name", obj->m_name);
    SetGenerateMustanceDataList(data, "enum_list", obj->m_enums);
    SetGenerateMustanceDataList(data, "class_list", obj->m_classes);
    SetGenerateMustanceDataList(data, "struct_list", obj->m_structs);
    SetGenerateMustanceDataList(data, "var_list", obj->m_vars);
    SetGenerateMustanceDataList(data, "function_list", obj->m_functions);
    SetGenerateMustanceDataList(data, "metahod_list", obj->m_methods);
    SetGenerateMustanceDataList(data, "field_list", obj->m_fields);
    Mustache::data list_data(Mustache::data::type::list);
    for (const std::string& item: obj->m_inherits)
    {
        Mustache::data data;
        data.set("name", item);
        list_data.push_back(data);
    }
    data.set("inherit_list", list_data);
}

void GenerateMustanceData(CRASTree* ast, Mustache::data& data)
{
    SetGenerateMustanceDataList(data, "enum_list", ast->m_enums);
    SetGenerateMustanceDataList(data, "class_list", ast->m_classes);
    SetGenerateMustanceDataList(data, "struct_list", ast->m_structs);
    SetGenerateMustanceDataList(data, "var_list", ast->m_vars);
    SetGenerateMustanceDataList(data, "function_list", ast->m_functions);
}

void CRGenerator::Generate(const std::vector<std::string>& include_files, CRASTree* ast, const char* template_file, const char* out_file)
{
    Mustache::data data;
    Mustache::data ast_data;
    GenerateMustanceData(ast, ast_data);
    data.set("ast", ast_data);

    Mustache::data header_list_data(Mustache::data::type::list);
    for (const std::string& header : include_files)
    {
        Mustache::data header_data;
        header_data.set("header_file_name", header);
        header_list_data.push_back(header_data);
    }
    data.set("include_header_files", header_list_data);

    std::string template_str = readFile(template_file);
    std::string out = MustacheRenderTemplate(template_str, data);
    writeFile(out_file, out);
}