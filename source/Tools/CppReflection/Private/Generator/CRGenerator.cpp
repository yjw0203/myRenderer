#include "Tools/CppReflection/Private/Generator/CRGenerator.h"
#include <fstream>
#include "mustache.hpp"

namespace Mustache = kainjow::mustache;
void GenerateMustanceData(const meta::Field& field, Mustache::data& data)
{
    data.set("field_name", field.m_name);
    data.set("field_type", field.m_type);
}

void GenerateMustanceData(const meta::Field::List& fields, Mustache::data& data)
{
    for (const meta::Field& field : fields)
    {
        Mustache::data field_data;
        GenerateMustanceData(field, field_data);
        data.push_back(field_data);
    }
}

void GenerateMustanceData(const meta::Class& class_, Mustache::data& data)
{
    data.set("class_name", class_.m_name);
    Mustache::data field_list_data(Mustache::data::type::list);
    GenerateMustanceData(class_.m_fields, field_list_data);
    data.set("field_list", field_list_data);
}

void GenerateMustanceData(const meta::Class::List& classes, Mustache::data& data)
{
    for (const meta::Class& class_ : classes)
    {
        Mustache::data class_data;
        GenerateMustanceData(class_, class_data);
        data.push_back(class_data);
    }
}

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
    std::ofstream file(file_name);
    if (file.is_open()) {
        file << str;
        file.close();
    }
    else {
    }
}

void CRGenerator::Generate(const std::vector<std::string>& include_files, const meta::Class::List& classes, const char* template_file, const char* out_file)
{
    Mustache::data data;
    Mustache::data class_list_data(Mustache::data::type::list);
    GenerateMustanceData(classes, class_list_data);

    Mustache::data header_list_data(Mustache::data::type::list);
    for (const std::string& header : include_files)
    {
        Mustache::data header_data;
        header_data.set("header_file_name", header);
        header_list_data.push_back(header_data);
    }
    data.set("include_header_files", header_list_data);
    data.set("class_list", class_list_data);

    std::string template_str = readFile(template_file);
    std::string out = MustacheRenderTemplate(template_str, data);
    writeFile(out_file, out);
}