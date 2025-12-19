#pragma once
#include "Cursor.h"
#include <vector>
#include <map>

typedef std::map<std::string, std::string> CRArguments;

class CREnum
{
public:
    std::string m_namespace;
    std::string m_name;
    CRArguments m_arguments;
    std::vector<std::string> m_items;
};

class CRVar
{
public:
    std::string m_namespace;
    std::string m_name;
    std::string m_type;
    bool m_is_fundamental_type;
    CRArguments m_arguments;
};

class CRParm
{
public:
    std::string m_name;
    std::string m_type;
    bool m_is_fundamental_type;
};

class CRFunction
{
public:
    ~CRFunction();
    std::string m_namespace;
    std::string m_name;
    CRArguments m_arguments;
    std::vector<CRParm*> m_parms;
};

class CRMethod
{
public:
    ~CRMethod();
    std::string m_name;
    CRAccessable m_access{ access_unknown };
    CRArguments m_arguments;
    std::vector<CRParm*> m_parms;
};

class CRField
{
public:
    std::string m_name;
    std::string m_type;
    bool m_is_fundamental_type;
    CRAccessable m_access{ access_unknown };
    CRArguments m_arguments;
};

class CRStruct;
class CRClass
{
public:
    ~CRClass();
    std::string m_namespace;
    std::string m_name;
    CRArguments m_arguments;
    std::vector<CREnum*> m_enums;
    std::vector<CRClass*> m_classes;
    std::vector<CRStruct*> m_structs;
    std::vector<CRVar*> m_vars;
    std::vector<CRFunction*> m_functions;
    std::vector<CRMethod*> m_methods;
    std::vector<CRField*> m_fields;
    std::vector<std::string> m_inherits;
};

class CRStruct
{
public:
    ~CRStruct();
    std::string m_namespace;
    std::string m_name;
    CRArguments m_arguments;
    std::vector<CREnum*> m_enums;
    std::vector<CRClass*> m_classes;
    std::vector<CRStruct*> m_structs;
    std::vector<CRVar*> m_vars;
    std::vector<CRFunction*> m_functions;
    std::vector<CRMethod*> m_methods;
    std::vector<CRField*> m_fields;
    std::vector<std::string> m_inherits;
};

class CRASTree
{
public:
    CRASTree() {};
    ~CRASTree();
    void build(const char* filename);
    bool isEmpty();
    void merge(CRASTree* tree);
    std::vector<CREnum*> m_enums;
    std::vector<CRVar*> m_vars;
    std::vector<CRFunction*> m_functions;
    std::vector<CRClass*> m_classes;
    std::vector<CRStruct*> m_structs;

private:
    void buildSubTree(CRCursor cursor, CRNamespace& currentNamespace, CRCursor parent);
    CREnum* buildEnum(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments);
    CRVar* buildVar(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments);
    CRFunction* buildFunction(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments);
    CRClass* buildClass(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments);
    CRStruct* buildStruct(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments);
    CRField* buildField(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments);
    CRMethod* buildMethod(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments);
    CRParm* buildParm(CRCursor cursor);

    void addEnum(CREnum* cr_enum) { m_enums.push_back(cr_enum); }
    void addVar(CRVar* cr_var) { m_vars.push_back(cr_var); }
    void addFunction(CRFunction* cr_function) { m_functions.push_back(cr_function); }
    void addClass(CRClass* cr_class) { m_classes.push_back(cr_class); }
    void addStruct(CRStruct* cr_struct) { m_structs.push_back(cr_struct); }

};