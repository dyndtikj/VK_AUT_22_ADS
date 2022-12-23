#include "ListGraph.hpp"

#include <algorithm>
#include <stdexcept>

ListGraph::ListGraph(const IGraph & other) :
        vertices_count_(other.VerticesCount()),
        adj_lists_(vertices_count_)
{
    for (size_t i = 0; i < vertices_count_; ++i) {
        for (const int v: other.GetNextVertices(i))
            adj_lists_[i].emplace_back(v);
    }
}

void ListGraph::AddEdge(const int from, const int to) {
    checkVertex(from);
    checkVertex(to);
    adj_lists_[from].emplace_back(to);
}

std::vector<int> ListGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    return adj_lists_[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> prev;

    for (size_t v = 0; v < adj_lists_.size(); ++v) {
        for (const int adj : adj_lists_[v])
            if (adj == vertex)
                prev.emplace_back(v);
    }

    return prev;
}

bool ListGraph::hasEdge(const int from, const int to) const {
    checkVertex(from);
    checkVertex(to);

    return !(std::find_if(
            adj_lists_[from].cbegin(),adj_lists_[from].cend(),[to](const int a) { return to == a; })
            == adj_lists_[from].cend());
}

void ListGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices_count_) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}

int ListGraph::VerticesCount() const {
    return vertices_count_;
}