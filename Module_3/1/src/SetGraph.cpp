#include "SetGraph.hpp"

#include <algorithm>
#include <stdexcept>

SetGraph::SetGraph(std::size_t count): vertices_count_(count), adj_sets_(count) {}

SetGraph::SetGraph(const IGraph & other) :
        vertices_count_(other.VerticesCount()),
        adj_sets_(vertices_count_)
{
    for (size_t i = 0; i < vertices_count_; ++i) {
        for (const int el: other.GetNextVertices(i))
            adj_sets_[i].emplace(el);
    }
}

void SetGraph::AddEdge(const int from, const int to) {
    checkVertex(from);
    checkVertex(to);
    adj_sets_[from].emplace(to);
}

std::vector<int> SetGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> next;
    for (auto &i: adj_sets_[vertex])
        next.emplace_back(i);
    return next;
}

std::vector<int> SetGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> prev;
    for (int v = 0; v < vertices_count_; ++v)
        if (std::find_if(adj_sets_[v].begin(), adj_sets_[v].end(),
                         [vertex](const auto &i) {
                             return i == vertex;
                         }) != adj_sets_[v].end())
            prev.push_back(v);
    return prev;
}

bool SetGraph::hasEdge(const int from, const int to) const {
    checkVertex(from);
    checkVertex(to);
    return adj_sets_[from].find(to) != adj_sets_[from].cend();
}

void SetGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices_count_) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}

int SetGraph::VerticesCount() const {
    return vertices_count_;
}
