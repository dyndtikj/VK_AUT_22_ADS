#include "MatrixGraph.hpp"

#include <algorithm>
#include <stdexcept>

MatrixGraph::MatrixGraph(std::size_t count): vertices_count_(count), adj_matrix_(count, std::vector<int>(count)){}

MatrixGraph::MatrixGraph(const IGraph & other) :
        vertices_count_(other.VerticesCount()),
        adj_matrix_(vertices_count_, std::vector<int>(vertices_count_)) {
    for (size_t i = 0; i < vertices_count_; ++i) {
        for (const int j: other.GetNextVertices(i))
            ++adj_matrix_[i][j];
    }
}

void MatrixGraph::AddEdge(const int from, const int to) {
    checkVertex(from);
    checkVertex(to);
    ++adj_matrix_[from][to];
}

std::vector<int> MatrixGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> next;
    for (int i = 0; i < vertices_count_; ++i) {
        if (adj_matrix_[vertex][i] != 0)
            next.emplace_back(i);
    }
    return next;
}

std::vector<int> MatrixGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<int> prev;
    for (int i = 0; i < adj_matrix_.size(); ++i) {
        if (adj_matrix_[i][vertex] != 0)
            prev.emplace_back(i);
    }
    return prev;
}

bool MatrixGraph::hasEdge(const int from, const int to) const {
    checkVertex(from);
    checkVertex(to);
    return adj_matrix_[from][to] != 0;
}

void MatrixGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices_count_) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}

int MatrixGraph::VerticesCount() const{
    return vertices_count_;
}