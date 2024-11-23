 #pragma once
#include "Cursor.h"
#include "Meta/MetaClass.h"

struct CRCondition_IsField
{
    int Condition(CRCursor cursor, const CRNamespace& Namespace);
};

struct CRAction_GatherField
{
public:
    void DoAction(CRCursor cursor, const CRNamespace& Namespace, int flag, CRCursor parent);
    std::vector<meta::Field> m_result;
};

struct CRCondition_IsMethod
{
    int Condition(CRCursor cursor, const CRNamespace& Namespace);
};

struct CRAction_GatherMethod
{
public:
    void DoAction(CRCursor cursor, const CRNamespace& Namespace, int flag, CRCursor parent);
    meta::Method::List m_result;
};

struct CRCondition_IsMetaClass
{
    int Condition(CRCursor cursor, const CRNamespace& Namespace);
};

struct CRAction_GatherClass
{
public:
    void DoAction(CRCursor cursor, CRNamespace& Namespace, int flag, CRCursor parent);
    std::vector<meta::Class> m_result;
};