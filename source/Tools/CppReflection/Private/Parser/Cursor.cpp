#include "Cursor.h"
#include <string>
#include "Utils.h"

std::string CRNamespaceToString(const CRNamespace& Namespace)
{
    std::string res = "";
    int first = 1;
    for (auto& space_name : Namespace)(res += first ? "" : "::") += space_name, first = 0;
    return res;
}

std::string CRClassNamespaceToString(const CRClassNamespace& Namespace)
{
    std::string res = "";
    int first = 1;
    for (auto& space_name : Namespace)(res += first ? "" : "::") += space_name, first = 0;
    return res;
}

CRCursor::CRCursor(const CXCursor& handle)
    : m_handle(handle) { }

CRCursor::operator CXCursor() { return m_handle; }

CRCursor CRCursor::None()
{
    return clang_getNullCursor();
}

CRCursor CRCursor::GetParent(void) const
{
    return clang_getCursorLexicalParent(m_handle);
}

CRCursor::List CRCursor::GetChildren(void) const
{
    List children;

    auto visitor = [](CXCursor cursor, CXCursor parent, CXClientData data)
        {
            if (!clang_Location_isFromMainFile(clang_getRangeEnd(clang_getCursorExtent(cursor))))
            {
                return CXChildVisit_Continue;
            }

            auto container = static_cast<List*>(data);

            container->emplace_back(cursor);

            if (cursor.kind == CXCursor_LastPreprocessing)
                return CXChildVisit_Break;

            return CXChildVisit_Continue;
        };

    clang_visitChildren(m_handle, visitor, &children);

    return children;
}

std::string CRCursor::GetDisplayName(void) const
{
    std::string displayName;

    CRToString(clang_getCursorSpelling(m_handle), displayName);

    return displayName;
}

CXCursorKind CRCursor::GetKind(void) const
{
    return m_handle.kind;
}

bool CRCursor::IsDefinition(void) const
{
    return clang_isCursorDefinition(m_handle) ? true : false;
}

std::string CRCursor::GetAnnotateAttr(void) const
{
    CRCursor::List children = GetChildren();
    for (CRCursor& cur : children)
    {
        if (cur.GetKind() == CXCursor_AnnotateAttr)
        {
            return cur.GetDisplayName();
        }
    }
    return "";
}

std::string CRCursor::GetTypeName(void) const
{
    std::string type = "";
    CRToString(clang_getTypeSpelling(clang_getUnqualifiedType(clang_getCursorType(m_handle))), type);
    return type;
}

bool CRCursor::IsFundamentalType(void) const
{
    CXTypeKind kind = clang_getCursorType(m_handle).kind;
    switch (kind) 
    {
        case CXType_Void:
        case CXType_Bool:
        case CXType_Char_U:
        case CXType_UChar:
        case CXType_Char16:
        case CXType_Char32:
        case CXType_Char_S:
        case CXType_SChar:
        case CXType_WChar:
        case CXType_UShort:
        case CXType_UInt:
        case CXType_ULong:
        case CXType_ULongLong:
        case CXType_Short:
        case CXType_Int:
        case CXType_Long:
        case CXType_LongLong:
        case CXType_Float:
        case CXType_Double:
        case CXType_LongDouble:
            return true;
        default:
            return false;
    }
    return false;
}

CRAccessable CRCursor::GetAccessable(void) const
{
    CX_CXXAccessSpecifier access = clang_getCXXAccessSpecifier(m_handle);
    if (access == CX_CXXPublic)
    {
        return CRAccessable::access_public;
    }
    else if(access == CX_CXXPrivate)
    {
        return CRAccessable::access_private;
    }
    else if (access == CX_CXXProtected)
    {
        return CRAccessable::access_protect;
    }
    else
    {
        return CRAccessable::access_unknown;
    }
}

bool CRCursor::IsLocationMainFile(void) const
{
    return clang_Location_isFromMainFile(clang_getRangeEnd(clang_getCursorExtent(m_handle)));
}

std::vector<std::string> CRCursor::GetBaseClassesTypes()
{
    std::vector<std::string> vec;
    CRCursor::List children = GetChildren();
    for (CRCursor& cur : children)
    {
        if (cur.GetKind() == CXCursor_CXXBaseSpecifier)
        {
            vec.push_back(cur.GetDisplayName());
        }
    }
    return vec;
}
