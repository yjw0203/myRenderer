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

    CRToString(clang_getCursorDisplayName(m_handle), displayName);

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
