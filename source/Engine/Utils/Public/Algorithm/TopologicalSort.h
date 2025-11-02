#pragma once
#include <vector>

class TopologicalSort
{
public:
    TopologicalSort(int n);
    ~TopologicalSort();
    bool AddEdge(int from, int to);
    bool Sort();// return false if there is not a topological order. 
    std::vector<int>& GetResult();
    std::vector<int>& GetRResult();
private:
    class TopologicalSortImplement* m_implement;
};
