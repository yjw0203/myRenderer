#pragma once

extern int CRDontVisitSubTree;

class CRTranslationUnit;
template<class ConditionPolicy, class ActionPolicy>
class CRVisitor : public ActionPolicy
{
public:
    using ActionPolicy::ActionPolicy;

    void VisitSubTree(CRCursor cursor, CRNamespace& Namespace, CRClassNamespace& ClassNamespace)
    {
        dfs(cursor, Namespace, ClassNamespace, CRCursor::None(), true);
    }

    void Visit(CRTranslationUnit unit)
    {
        CRNamespace tempNamespace;
        CRClassNamespace tempClassNamespace;
        VisitSubTree(unit.GetRootCursor(), tempNamespace, tempClassNamespace);
    }

private:
    void dfs(CRCursor cursor, CRNamespace& currentNamespace, CRClassNamespace& currentClassNamespace, CRCursor parent, bool is_root)
    {
        int shouldVisitFlag = ShouldVisit(cursor, currentNamespace, currentClassNamespace);
        if (shouldVisitFlag > 0)DoActionInterface(cursor, currentNamespace, currentClassNamespace, shouldVisitFlag, parent);

        bool push_ns = false;
        if (cursor.GetKind() == CXCursor_Namespace || cursor.GetKind() == CXCursor_FieldDecl)
        {
            auto displayName = cursor.GetDisplayName();
            if (!displayName.empty())
            {
                currentNamespace.push_back(displayName);
                push_ns = true;
            }
        }
        bool push_cns = false;
        if (cursor.GetKind() == CXCursor_ClassDecl || cursor.GetKind() == CXCursor_StructDecl)
        {
            auto displayName = cursor.GetDisplayName();
            if (!displayName.empty())
            {
                currentClassNamespace.push_back(displayName);
                push_cns = true;
            }
        }

        if (shouldVisitFlag != CRDontVisitSubTree || is_root)
        {
            CRCursor::List children = cursor.GetChildren();
            for (auto child : children)
            {
                dfs(child, currentNamespace, currentClassNamespace, cursor, false);
            }
        }

        if (push_ns)
        {
            currentNamespace.pop_back();
        }
        if (push_cns)
        {
            currentClassNamespace.pop_back();
        }
    }
    int ShouldVisit(CRCursor cursor, const CRNamespace& Namespace, CRClassNamespace& ClassNamespace) { return m_condition.Condition(cursor, Namespace, ClassNamespace); }
    void DoActionInterface(CRCursor cursor, CRNamespace& Namespace, CRClassNamespace& ClassNamespace,int flag, CRCursor parent) { this->DoAction(cursor, Namespace, ClassNamespace, flag, parent); }
private:
    ConditionPolicy m_condition;
};