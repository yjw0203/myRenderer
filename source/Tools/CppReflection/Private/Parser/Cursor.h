#pragma once
#include <clang-c/Index.h>
#include <vector>
#include <string>
typedef std::vector<std::string> CRNamespace;
std::string CRNamespaceToString(const CRNamespace& Namespace);

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

private:
    CXCursor m_handle;
}; 