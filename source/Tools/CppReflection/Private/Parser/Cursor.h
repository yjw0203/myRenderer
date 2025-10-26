#pragma once
#include <clang-c/Index.h>
#include <vector>
#include <string>
typedef std::vector<std::string> CRNamespace;
typedef std::vector<std::string> CRClassNamespace;
std::string CRNamespaceToString(const CRNamespace& Namespace);
std::string CRClassNamespaceToString(const CRClassNamespace& Namespace);

enum CRAccessable
{
    access_unknown,
    access_public,
    access_protect,
    access_private
};

class CRCursor
{
public:
    typedef std::vector<CRCursor> List;
    CRCursor(const CXCursor& handle);
    operator CXCursor();

    static CRCursor None();

    CRCursor GetParent(void) const;
    List GetChildren(void) const;
    std::string GetDisplayName(void) const;
    CXCursorKind CRCursor::GetKind(void) const;
    bool CRCursor::IsDefinition(void) const;
    std::string GetAnnotateAttr(void) const;
    std::string GetTypeName(void) const;
    bool IsFundamentalType(void) const;
    CRAccessable GetAccessable(void) const;
    bool IsLocationMainFile(void) const;
    std::vector<std::string> GetBaseClassesTypes();
private:
    CXCursor m_handle;
}; 