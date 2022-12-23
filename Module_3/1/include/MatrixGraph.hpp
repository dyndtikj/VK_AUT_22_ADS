#pragma once

#include "IGraph.hpp"

class MatrixGraph : public IGraph {
public:
    MatrixGraph() = default;
    explicit MatrixGraph(const std::size_t count);
    MatrixGraph(const IGraph & other);
    ~MatrixGraph() = default;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    bool hasEdge(const int from, const int to) const;
    void checkVertex(const int vertex) const;
    std::size_t vertices_count_ = 0;
    std::vector<std::vector<int>> adj_matrix_;
};