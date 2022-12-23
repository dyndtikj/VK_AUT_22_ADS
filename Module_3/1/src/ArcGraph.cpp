#include "ArcGraph.hpp"
#include "iostream"
#include <algorithm>
#include <stdexcept>

ArcGraph::ArcGraph(const std::size_t count) : vertices_count_(count) {}

ArcGraph::ArcGraph(const IGraph & other) :
        vertices_count_(other.VerticesCount())
{
    for (size_t i = 0; i < vertices_count_; ++i) {
        for (const auto & v: other.GetNextVertices(i)) {
            pairs_.emplace_back(i, v);
        }
    }
}

void ArcGraph::AddEdge(const int from, const int to) {
    checkVertex(from);
    checkVertex(to);
    pairs_.emplace_back(from, to);
}

std::vector<int> ArcGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> next;

    std::for_each(
            pairs_.cbegin(), pairs_.cend(),
            [&next, vertex](const auto& pair) {
                if (pair.first == vertex) next.emplace_back(pair.second);
            }
    );
    return next;
}

std::vector<int> ArcGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> prev;

    std::for_each(
            pairs_.cbegin(),
            pairs_.cend(),
            [&prev, vertex](const std::pair<int, int> & duple) {
                if (duple.second == vertex) prev.emplace_back(duple.first);
            }
    );

    return prev;
}

bool ArcGraph::hasEdge(const int from, const int to) const {
    checkVertex(from);
    checkVertex(to);

    return !(std::find_if(pairs_.cbegin(), pairs_.cend(),
                          [from, to](const std::pair<int, int> &duple) {
                              const bool first_match = from == duple.first;
                              const bool second_match = from == duple.second;
                              return first_match and second_match;
                          }) == pairs_.cend());
}

void ArcGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices_count_) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}

int ArcGraph::VerticesCount() const {
    return vertices_count_;
}