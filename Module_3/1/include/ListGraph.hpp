#pragma once

#include "IGraph.hpp"

class ListGraph : public IGraph {
public:
    ListGraph() = default;
    explicit ListGraph(const std::size_t count) : vertices_count_(count), adj_lists_(count) {}
    ListGraph(const IGraph & other);
    ~ListGraph() = default;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    bool hasEdge(const int from, const int to) const;
    void checkVertex(const int vertex) const;
    std::size_t vertices_count_ = 0;
    std::vector<std::vector<int>> adj_lists_;
};