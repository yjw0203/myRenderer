#include "UnitVisitorPolicy.h"
#include "UnitVisitor.h"
#include "Utils.h"
#include <iostream>
int CRDontVisitSubTree = -165131;

int CRCondition_IsField::Condition(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace)
{
    if (cursor.GetKind() == CXCursor_ClassDecl || cursor.GetKind() == CXCursor_StructDecl)
    {
        return CRDontVisitSubTree;
    }
    return cursor.GetKind() == CXCursor_FieldDecl;
}

void CRAction_GatherField::DoAction(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace, int flag, CRCursor parent)
{
    meta::Field field;
    field.m_name = cursor.GetDisplayName();
    m_result.push_back(field);
}

int CRCondition_IsMethod::Condition(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace)
{
    return cursor.GetKind() == CXCursor_CXXMethod;
}
void CRAction_GatherMethod::DoAction(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace, int flag, CRCursor parent)
{
    meta::Method method;
    method.m_name = cursor.GetDisplayName();
    m_result.push_back(method);
}

int CRCondition_IsMetaClass::Condition(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace)
{
    if (cursor.GetKind() == CXCursor_AnnotateAttr)
    {
        return cursor.GetDisplayName() == "type=class";
    }
    return 0;
}
void CRAction_GatherClass::DoAction(CRCursor cursor, CRNamespace& Namespace, CRClassNamespace& ClassNamespace, int flag, CRCursor parent)
{
    CRVisitor<CRCondition_IsField, CRAction_GatherField> GatherFieldVisitor;
    GatherFieldVisitor.VisitSubTree(parent, Namespace, ClassNamespace);

    CRVisitor<CRCondition_IsMethod, CRAction_GatherMethod> GatherMethodVisitor;
    GatherMethodVisitor.VisitSubTree(parent, Namespace, ClassNamespace);

    meta::Class data;
    data.m_namespace = CRNamespaceToString(Namespace);
    CRClassNamespace temp_namespace = ClassNamespace;
    temp_namespace.pop_back();
    std::string class_pre = CRClassNamespaceToString(temp_namespace);
    if (class_pre != "") { class_pre += "::"; }
    data.m_name = class_pre + parent.GetDisplayName();
    data.m_fields = GatherFieldVisitor.m_result;
    data.m_methods = GatherMethodVisitor.m_result;
    m_result.push_back(data);
}