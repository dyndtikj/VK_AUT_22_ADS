/*
    Vlasov MM WEB-13
    Задача 3. «Города» (4 балла) *
    Требуется отыскать самый выгодный маршрут между городами.
    Требования: время работы O((N+M)logN), где N-количество городов, M-известных дорог между ними.

    Формат входных данных.
    Первая строка содержит число N – количество городов.
    Вторая строка содержит число M - количество дорог.

    Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
    Последняя строка содержит маршрут (откуда и куда нужно доехать).
    Формат выходных данных.
    Вывести длину самого выгодного маршрута.
*/


#pragma once

#include <vector>
#include <queue>
#include <set>
#include <stdexcept>
#include <algorithm>
#include <climits>
#include <iostream>

// переделано на взвешенный
struct IGraph {
    virtual ~IGraph() {}
    virtual void AddEdge(int from, int to, size_t weight) = 0;
    virtual int VerticesCount() const  = 0;
    virtual std::vector<std::pair<int, size_t>> GetNextVertices(int vertex) const = 0;
    virtual std::vector<std::pair<int, size_t>> GetPrevVertices(int vertex) const = 0;
};

// доступ за о(1)
class ListGraph : public IGraph {
public:
    ListGraph() = default;
    explicit ListGraph(const std::size_t count) : vertices_count_(count), adj_lists_(count) {}
    ListGraph(const IGraph & other);
    ~ListGraph() = default;
    void AddEdge(int from, int to, size_t weight) override;
    int VerticesCount() const override;
    std::vector<std::pair<int, size_t>> GetNextVertices(int vertex) const override;
    std::vector<std::pair<int, size_t>> GetPrevVertices(int vertex) const override;
private:
    bool hasEdge(const int from, const int to) const;
    void checkVertex(const int vertex) const;
    std::size_t vertices_count_ = 0;
    std::vector<std::vector<std::pair<int, size_t>>> adj_lists_;
};

ListGraph::ListGraph(const IGraph & other) :
        vertices_count_(other.VerticesCount()),
        adj_lists_(vertices_count_)
{
    for (size_t i = 0; i < vertices_count_; ++i) {
        for (const auto & v: other.GetNextVertices(i))
            adj_lists_[i].emplace_back(v);
    }
}

void ListGraph::AddEdge(const int from, const int to, size_t weight) {
    checkVertex(from);
    checkVertex(to);
    // неориентированный
    adj_lists_[from].emplace_back(to, weight);
    adj_lists_[to].emplace_back(from, weight);
}

std::vector<std::pair<int, size_t>> ListGraph::GetNextVertices(const int vertex) const {
    checkVertex(vertex);
    return adj_lists_[vertex];
}

std::vector<std::pair<int, size_t>> ListGraph::GetPrevVertices(const int vertex) const {
    checkVertex(vertex);
    std::vector<std::pair<int, size_t>> prev;

    for (size_t v = 0; v < adj_lists_.size(); ++v) {
        for (const auto & adj : adj_lists_[v])
            if (adj.first == vertex)
                prev.emplace_back(v, adj.second);
    }

    return prev;
}

bool ListGraph::hasEdge(const int from, const int to) const {
    checkVertex(from);
    checkVertex(to);

    return !(std::find_if(
            adj_lists_[from].cbegin(),adj_lists_[from].cend(),[to](const std::pair<int, size_t> & a)
            { return to == a.first; }) == adj_lists_[from].cend());
}

void ListGraph::checkVertex(const int vertex) const {
    if (vertex < static_cast<int>(vertices_count_) and vertex >= 0) return;
    throw std::runtime_error("Invalid vertex ID!");
}

int ListGraph::VerticesCount() const {
    return vertices_count_;
}

size_t FindShortestPath(const IGraph& graph, const int from, const int to) {
    std::vector<int> path_weights(graph.VerticesCount(), INT_MAX);
    path_weights[from] = 0;

    std::set<std::pair<int, size_t>> vertices;
    vertices.emplace(from, 0);

    while (!vertices.empty()) {
        const int current = vertices.begin()->first;
        vertices.erase(vertices.begin());
        for (const auto & next: graph.GetNextVertices(current)) {
            // если путь до вершины такой же или короче как от текущей то continue
            if (path_weights[next.first] <= path_weights[current] + next.second) continue;
            // изменяем длину пути и пересоздаем пару во множестве вершин
            vertices.erase({next.first, path_weights[next.first]});
            path_weights[next.first] = path_weights[current] + next.second;
            vertices.emplace(next.first, path_weights[next.first]);
        }
    }

    return path_weights[to];
}

void run(std::istream & in = std::cin, std::ostream & out = std::cout) {
    int vertices = 0, edges = 0;

    in >> vertices >> edges;
    int first = 0, second = 0;
    size_t weight = 0;
    ListGraph graph(vertices);
    for (int i = 0; i < edges; ++i) {
        in >> first >> second >> weight;
        graph.AddEdge(first, second, weight);
    }
    in >> first >> second;
    out << FindShortestPath(graph, first, second);
}

int main(int argc, char * argv[]) {
    run();
    return 0;
}