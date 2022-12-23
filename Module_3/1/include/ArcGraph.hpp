#pragma once

#include "IGraph.hpp"

class ArcGraph : public IGraph {
public:
    ArcGraph() = default;
    explicit ArcGraph(const std::size_t count);
    ArcGraph(const IGraph & other);
    ~ArcGraph() = default;
    void AddEdge(int from, int to) override;
    int VerticesCount() const override;
    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;
private:
    bool hasEdge(const int from, const int to) const;
    void checkVertex(const int vertex) const;
    std::size_t vertices_count_ = 0;
    std::vector<std::pair<int, int>> pairs_;
};