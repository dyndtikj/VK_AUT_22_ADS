#pragma once

#include <set>
#include "IGraph.hpp"

class SetGraph : public IGraph {
public:
    SetGraph() = default;
    explicit SetGraph(const std::size_t count);
    SetGraph(const IGraph & other);
    ~SetGraph() = default;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    bool hasEdge(const int from, const int to) const;
    void checkVertex(const int vertex) const;
    std::size_t vertices_count_ = 0;
    std::vector<std::set<int>>  adj_sets_;
};