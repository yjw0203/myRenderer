#include "UnitVisitorPolicy.h"
#include "UnitVisitor.h"
#include "Utils.h"
#include <iostream>

int CRCondition_IsField::Condition(CRCursor cursor, const CRNamespace& Namespace)
{
    return cursor.GetKind() == CXCursor_FieldDecl;
}

void CRAction_GatherField::DoAction(CRCursor cursor, const CRNamespace& Namespace, int flag, CRCursor parent)
{
    meta::Field field;
    field.m_name = cursor.GetDisplayName();
    m_result.push_back(field);
}

int CRCondition_IsMethod::Condition(CRCursor cursor, const CRNamespace& Namespace)
{
    return cursor.GetKind() == CXCursor_CXXMethod;
}
void CRAction_GatherMethod::DoAction(CRCursor cursor, const CRNamespace& Namespace, int flag, CRCursor parent)
{
    meta::Method method;
    method.m_name = cursor.GetDisplayName();
    m_result.push_back(method);
}

int CRCondition_IsMetaClass::Condition(CRCursor cursor, const CRNamespace& Namespace)
{
    if (cursor.GetKind() == CXCursor_AnnotateAttr)
    {
        return cursor.GetDisplayName() == "type=class";
    }
    return 0;
}
void CRAction_GatherClass::DoAction(CRCursor cursor, CRNamespace& Namespace, int flag, CRCursor parent)
{
    CRVisitor<CRCondition_IsField, CRAction_GatherField> GatherFieldVisitor;
    GatherFieldVisitor.VisitSubTree(parent, Namespace);

    CRVisitor<CRCondition_IsMethod, CRAction_GatherMethod> GatherMethodVisitor;
    GatherMethodVisitor.VisitSubTree(parent, Namespace);

    meta::Class data;
    data.m_namespace = CRNamespaceToString(Namespace);
    data.m_name = parent.GetDisplayName();
    data.m_fields = GatherFieldVisitor.m_result;
    data.m_methods = GatherMethodVisitor.m_result;
    m_result.push_back(data);
}