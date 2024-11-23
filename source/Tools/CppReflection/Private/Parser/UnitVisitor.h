#pragma once

class CRTranslationUnit;
template<class ConditionPolicy, class ActionPolicy>
class CRVisitor : public ActionPolicy
{
public:
    using ActionPolicy::ActionPolicy;

    void VisitSubTree(CRCursor cursor, CRNamespace& Namespace)
    {
        dfs(cursor, Namespace,CRCursor::None());
    }

    void Visit(CRTranslationUnit unit)
    {
        CRNamespace tempNamespace;
        VisitSubTree(unit.GetRootCursor(), tempNamespace);
    }

private:
    void dfs(CRCursor cursor, CRNamespace& currentNamespace, CRCursor parent)
    {
        int shouldVisitFlag = ShouldVisit(cursor, currentNamespace);
        if (shouldVisitFlag)DoActionInterface(cursor, currentNamespace, shouldVisitFlag, parent);

        bool push_ns = false;
        if (cursor.GetKind() == CXCursor_Namespace || cursor.GetKind() == CXCursor_ClassDecl || cursor.GetKind() == CXCursor_StructDecl || cursor.GetKind() == CXCursor_FieldDecl)
        {
            auto displayName = cursor.GetDisplayName();
            if (!displayName.empty())
            {
                currentNamespace.push_back(displayName);
                push_ns = true;
            }
        }

        CRCursor::List children = cursor.GetChildren();
        for (auto child : children)
        {
            dfs(child, currentNamespace, cursor);
        }

        if (push_ns)
        {
            currentNamespace.pop_back();
        }
    }
    int ShouldVisit(CRCursor cursor, const CRNamespace& Namespace) { return m_condition.Condition(cursor, Namespace); }
    void DoActionInterface(CRCursor cursor, CRNamespace& Namespace,int flag, CRCursor parent) { this->DoAction(cursor, Namespace, flag, parent); }
private:
    ConditionPolicy m_condition;
};