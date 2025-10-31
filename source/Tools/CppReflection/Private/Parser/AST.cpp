#include "AST.h"
#include "Unit.h"
#include <map>

CRFunction::~CRFunction()
{
    for (auto ptr : m_parms) { delete ptr; }
}

CRMethod::~CRMethod()
{
    for (auto ptr : m_parms) { delete ptr; }
}

CRClass::~CRClass()
{
    for (auto ptr : m_enums) { delete ptr; }
    for (auto ptr : m_classes) { delete ptr; }
    for (auto ptr : m_structs) { delete ptr; }
    for (auto ptr : m_vars) { delete ptr; }
    for (auto ptr : m_functions) { delete ptr; }
    for (auto ptr : m_methods) { delete ptr; }
    for (auto ptr : m_fields) { delete ptr; }
}

CRStruct::~CRStruct()
{
    for (auto ptr : m_enums){delete ptr;}
    for (auto ptr : m_classes){delete ptr;}
    for (auto ptr : m_structs){delete ptr;}
    for (auto ptr : m_vars){delete ptr;}
    for (auto ptr : m_functions){delete ptr;}
    for (auto ptr : m_methods){delete ptr;}
    for (auto ptr : m_fields){delete ptr;}
}

CRASTree::~CRASTree()
{
    for (auto ptr : m_enums) { delete ptr; }
    for (auto ptr : m_classes) { delete ptr; }
    for (auto ptr : m_structs) { delete ptr; }
    for (auto ptr : m_vars) { delete ptr; }
    for (auto ptr : m_functions) { delete ptr; }
}

void CRASTree::build(const char* filename)
{
    CRTranslationUnit unit = CRCreateTranslationUnitFromSourceFile(filename);
    CRNamespace currentNamespace;
    buildSubTree(unit.GetRootCursor(), currentNamespace, CRCursor::None());
}

bool CRASTree::isEmpty()
{
    return m_enums.empty() && m_classes.empty() && m_structs.empty() && m_functions.empty() && m_vars.empty();
}

bool parserAnnotateAttr(const std::string& text, CRArguments& arguments)
{
    bool parser_result = false;
    if (text.size() >= 2 && text[0] == 'C' && text[1] == 'R')
    {
        parser_result = true;
    }
    else
    {
        return false;
    }

    std::string key = "";
    std::string value = "";
    int flag = 0;
    for (int i = 2; i < text.size(); i++)
    {
        char c = text[i];
        if (c == ',')
        {
            if (key != "")
            {
                arguments[key] = value;
                key = value = "";
            }
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
        {
            if (flag == 0)
            {
                key += c;
            }
            else
            {
                value += c;
            }
        }
        else if (c == '=')
        {
            if (flag == 0)
            {
                flag = 1;
            }
        }

        if (i + 1 == text.size())
        {
            if (key != "")
            {
                arguments[key] = value;
                key = value = "";
            }
        }
    }
    return parser_result;
}

void CRASTree::buildSubTree(CRCursor cursor, CRNamespace& currentNamespace, CRCursor parent)
{
    CXCursorKind kind = cursor.GetKind();
    if (kind == CXCursor_TranslationUnit)
    {
        CRCursor::List children = cursor.GetChildren();
        for (auto child : children)
        {
            buildSubTree(child, currentNamespace, cursor);
        }
        return;
    }

    if (kind == CXCursor_Namespace)
    {
        auto displayName = cursor.GetDisplayName();
        if (!displayName.empty())
        {
            currentNamespace.push_back(displayName);
            CRCursor::List children = cursor.GetChildren();
            for (auto child : children)
            {
                buildSubTree(child, currentNamespace, cursor);
            }
            currentNamespace.pop_back();
        }
        return;
    }

    std::string annotateAttr = cursor.GetAnnotateAttr();
    bool isInMainfile = cursor.IsLocationMainFile();
    CRArguments arguments;
    if (isInMainfile && parserAnnotateAttr(annotateAttr, arguments))
    {
        if (kind == CXCursor_VarDecl)
        {
            addVar(buildVar(cursor, currentNamespace, arguments));
        }

        if (kind == CXCursor_FunctionDecl)
        {
            addFunction(buildFunction(cursor, currentNamespace, arguments));
        }

        if (kind == CXCursor_EnumDecl)
        {
            addEnum(buildEnum(cursor, currentNamespace, arguments));
        }

        if (kind == CXCursor_ClassDecl)
        {
            addClass(buildClass(cursor, currentNamespace, arguments));
        }

        if (kind == CXCursor_StructDecl)
        {
            addStruct(buildStruct(cursor, currentNamespace, arguments));
        }
    }
}

CREnum* CRASTree::buildEnum(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments)
{
    CREnum* obj = new CREnum();
    obj->m_namespace = CRNamespaceToString(currentNamespace);
    obj->m_arguments = arguments;
    obj->m_name = cursor.GetDisplayName();
    CRCursor::List children = cursor.GetChildren();
    for (auto child : children)
    {
        auto kind = child.GetKind();
        if (kind == CXCursor_EnumConstantDecl)
        {
            obj->m_items.push_back(child.GetDisplayName());
        }
    }
    return obj;
}

CRVar* CRASTree::buildVar(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments)
{
    CRVar* obj = new CRVar();
    obj->m_namespace = CRNamespaceToString(currentNamespace);
    obj->m_arguments = arguments;
    obj->m_name = cursor.GetDisplayName();
    obj->m_type = cursor.GetTypeName();
    obj->m_is_fundamental_type = cursor.IsFundamentalType();
    return obj;
}

CRFunction* CRASTree::buildFunction(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments)
{
    CRFunction* obj = new CRFunction();
    obj->m_namespace = CRNamespaceToString(currentNamespace);
    obj->m_arguments = arguments;
    obj->m_name = cursor.GetDisplayName();
    CRCursor::List children = cursor.GetChildren();
    for (auto child : children)
    {
        auto kind = child.GetKind();
        if (kind == CXCursor_ParmDecl)
        {
            obj->m_parms.push_back(buildParm(child));
        }
    }
    return obj;
}

CRClass* CRASTree::buildClass(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments)
{
    CRClass* obj = new CRClass();
    obj->m_namespace = CRNamespaceToString(currentNamespace);
    obj->m_arguments = arguments;
    obj->m_name = cursor.GetDisplayName();
    CRCursor::List children = cursor.GetChildren();
    currentNamespace.push_back(obj->m_name);
    bool is_public = false;
    for (auto child : children)
    {
        auto name = child.GetDisplayName();
        auto kind = child.GetKind();
        if (kind == CXCursor_CXXBaseSpecifier)
        {
            obj->m_inherits.push_back(child.GetDisplayName());
            continue;
        }
        if (kind == CXCursor_CXXAccessSpecifier)
        {
            if (child.GetAccessable() == CRAccessable::access_public)
            {
                is_public = true;
            }
            else
            {
                is_public = false;
            }
        }

        std::string annotateAttr = child.GetAnnotateAttr();
        CRArguments arguments;
        if (parserAnnotateAttr(annotateAttr, arguments))
        {
            if (kind == CXCursor_VarDecl)
            {
                obj->m_vars.push_back(buildVar(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_FunctionDecl)
            {
                obj->m_functions.push_back(buildFunction(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_EnumDecl)
            {
                obj->m_enums.push_back(buildEnum(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_ClassDecl)
            {
                obj->m_classes.push_back(buildClass(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_StructDecl)
            {
                obj->m_structs.push_back(buildStruct(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_FieldDecl)
            {
                auto field = buildField(child, currentNamespace, arguments);
                field->m_access = access_public;
                obj->m_fields.push_back(field);
            }

            if (kind == CXCursor_CXXMethod)
            {
                auto method = buildMethod(child, currentNamespace, arguments);
                method->m_access = access_public;
                obj->m_methods.push_back(method);
            }
        }
    }
    currentNamespace.pop_back();
    return obj;
}

CRStruct* CRASTree::buildStruct(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments)
{
    CRStruct* obj = new CRStruct();
    obj->m_namespace = CRNamespaceToString(currentNamespace);
    obj->m_arguments = arguments;
    obj->m_name = cursor.GetDisplayName();
    CRCursor::List children = cursor.GetChildren();
    for (auto child : children)
    {
        auto kind = child.GetKind();
        if (kind == CXCursor_CXXBaseSpecifier)
        {
            obj->m_inherits.push_back(child.GetDisplayName());
            continue;
        }
        std::string annotateAttr = child.GetAnnotateAttr();
        CRArguments arguments;
        if (parserAnnotateAttr(annotateAttr, arguments))
        {
            if (kind == CXCursor_VarDecl)
            {
                obj->m_vars.push_back(buildVar(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_FunctionDecl)
            {
                obj->m_functions.push_back(buildFunction(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_EnumDecl)
            {
                obj->m_enums.push_back(buildEnum(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_ClassDecl)
            {
                obj->m_classes.push_back(buildClass(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_StructDecl)
            {
                obj->m_structs.push_back(buildStruct(child, currentNamespace, arguments));
            }

            if (kind == CXCursor_FieldDecl)
            {
                auto field = buildField(child, currentNamespace, arguments);
                field->m_access = access_public;
                obj->m_fields.push_back(field);
            }

            if (kind == CXCursor_CXXMethod)
            {
                auto method = buildMethod(child, currentNamespace, arguments);
                method->m_access = access_public;
                obj->m_methods.push_back(method);
            }
        }
    }
    return obj;
}

CRField* CRASTree::buildField(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments)
{
    CRField* obj = new CRField();
    obj->m_arguments = arguments;
    obj->m_type = cursor.GetTypeName();
    obj->m_name = cursor.GetDisplayName();
    obj->m_is_fundamental_type = cursor.IsFundamentalType();
    return obj;
}

CRMethod* CRASTree::buildMethod(CRCursor cursor, CRNamespace& currentNamespace, CRArguments& arguments)
{
    CRMethod* obj = new CRMethod();
    obj->m_arguments = arguments;
    obj->m_name = cursor.GetDisplayName();
    CRCursor::List children = cursor.GetChildren();
    for (auto child : children)
    {
        auto kind = child.GetKind();
        if (kind == CXCursor_ParmDecl)
        {
            obj->m_parms.push_back(buildParm(child));
        }
    }
    return obj;
}

CRParm* CRASTree::buildParm(CRCursor cursor)
{
    CRParm* obj = new CRParm();
    obj->m_type = cursor.GetTypeName();
    obj->m_name = cursor.GetDisplayName();
    obj->m_is_fundamental_type = cursor.IsFundamentalType();
    return obj;
}
