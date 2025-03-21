 #pragma once
#include "Cursor.h"
#include "Engine/Meta/Public/MetaClass.h"

struct CRCondition_IsField
{
    int Condition(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace);
};

struct CRAction_GatherField
{
public:
    void DoAction(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace, int flag, CRCursor parent);
    std::vector<meta::Field> m_result;
};

struct CRCondition_IsMethod
{
    int Condition(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace);
};

struct CRAction_GatherMethod
{
public:
    void DoAction(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace, int flag, CRCursor parent);
    meta::Method::List m_result;
};

struct CRCondition_IsMetaClass
{
    int Condition(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace);
};

struct CRAction_GatherClass
{
public:
    void DoAction(CRCursor cursor, CRNamespace& Namespace, CRClassNamespace& ClassNamespace, int flag, CRCursor parent);
    std::vector<meta::Class> m_result;
};