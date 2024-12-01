#include "Engine/Utils/Public/Algorithm/TopologicalSort.h"
#include <queue>

namespace yjw
{
    class TopologicalSortImplement
    {
    public:
        TopologicalSortImplement(int n);
        ~TopologicalSortImplement();
        bool AddEdge(int from, int to);
        bool Sort();
        std::vector<int>& GetResult();
    private:
        std::vector<std::vector<int>> m_edges;
        std::vector<int> m_result;
        std::vector<int> m_inDegree;
    };

    TopologicalSortImplement::TopologicalSortImplement(int n)
    {
        m_edges.resize(n);
        m_inDegree.resize(n);
    }

    TopologicalSortImplement::~TopologicalSortImplement()
    {}

    bool TopologicalSortImplement::AddEdge(int from, int to)
    {
        if (from >= 0 && from < m_edges.size() && to >= 0 && to < m_edges.size())
        {
            m_inDegree[to]++;
            m_edges[from].push_back(to);
            return true;
        }
        else
        {
            return false;
        }
    }

    bool TopologicalSortImplement::Sort()
    {
        m_result.clear();
        std::vector<int> temp_inDegree = m_inDegree;

        std::queue<int> Q;

        //1. find 0 in-degree ndoe as first node.
        for (int i = 0; i < temp_inDegree.size(); i++)
        {
            if (temp_inDegree[i] == 0)
            {
                Q.push(i);
            }
        }
        
        //2. Topological Sort
        while (!Q.empty())
        {
            int p = Q.front();
            Q.pop();
            m_result.push_back(p);
            for (int to : m_edges[p])
            {
                temp_inDegree[to]--;
                if (temp_inDegree[to] == 0)
                {
                    Q.push(to);
                }
            }
        }

        //3. Check has topological order
        if (m_result.size() == m_inDegree.size())
            return true;
        else
            return false;
    }

    std::vector<int>& TopologicalSortImplement::GetResult()
    {
        return m_result;
    }

    TopologicalSort::TopologicalSort(int n)
    {
        m_implement = new TopologicalSortImplement(n);
    }

    TopologicalSort::~TopologicalSort()
    {
        delete m_implement;
    }

    bool TopologicalSort::AddEdge(int from, int to)
    {
        return m_implement->AddEdge(from, to);
    }

    bool TopologicalSort::Sort()
    {
        return m_implement->Sort();
    }

    std::vector<int>& TopologicalSort::GetResult()
    {
        return m_implement->GetResult();
    }
}