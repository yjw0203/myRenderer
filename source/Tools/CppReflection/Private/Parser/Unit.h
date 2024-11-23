#pragma once
#include <clang-c/Index.h>
#include "Cursor.h"
#include "UnitVisitorPolicy.h"

class CRTranslationUnit
{
public:
    CRTranslationUnit(const CXTranslationUnit& handle);
    operator CXTranslationUnit();
    CRCursor GetRootCursor();

private:
    CXTranslationUnit m_handle;
};

#include "UnitVisitor.h"

CRTranslationUnit CRCreateTranslationUnitFromSourceFile(const char* source_filename);
