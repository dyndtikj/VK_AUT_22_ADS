/*
    Vlasov MM WEB-13

    Задача 2. Количество различных путей (3 балла) *
    Дан невзвешенный неориентированный граф. В графе может быть несколько кратчайших путей между какими-то вершинами.
    Найдите количество различных кратчайших путей между заданными вершинами.
    Требования: сложность O(V+E).

    Формат ввода.
    v: кол-во вершин (макс. 50000),
    n: кол-во ребер (макс. 200000),
    n пар реберных вершин,
    пара вершин u, w для запроса.
    Формат вывода.
    Количество кратчайших путей от u к w.
*/

#pragma once

#include <vector>
#include <queue>
#include <stdexcept>
#include <algorithm>
#include <climits>
#include <iostream>

struct IGraph {
    virtual ~IGraph() {}
    virtual void AddEdge(int from, int to) = 0;
    virtual int VerticesCount() const  = 0;
    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};


// доступ за о(1)
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
    // неориентированный
    adj_lists_[from].emplace_back(to);
    adj_lists_[to].emplace_back(from);
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

int count_paths(const IGraph &graph, const int &from, const int &to) {
    std::vector<int> len(graph.VerticesCount(), INT_MAX);
    std::vector<int> count_len(graph.VerticesCount(), 0);
    
    std::queue<int> vertices_queue;
    vertices_queue.push(from);

    // длина пути до самой вершины - 0
    len[from] = 0;
    count_len[from] = 1;
    while (!vertices_queue.empty()) {
        // достаем вершину и проходим по следующим
        int current = vertices_queue.front();
        vertices_queue.pop();
        for (auto v : graph.GetNextVertices(current)) {
            if (len[v] == len[current] + 1) {
                // если длины совпали то значит надо прибавить кол-во путей до текущей
                count_len[v] += count_len[current];
            } else if (len[v] > len[current] + 1) {
                // если путь до вершины длиннее чем от текущей то надо изменить
                len[v] = len[current] + 1;
                count_len[v] = count_len[current];
                vertices_queue.push(v);
            }
        }
    }
    return count_len[to];
}

int run(std::istream &in = std::cin, std::ostream &out = std::cout) {
    int vertices = 0, edges = 0;
    in >> vertices >> edges;
    int first, second;
    ListGraph graph(vertices);
    for (int i = 0; i < edges; ++i) {
        std::cin >> first >> second;
        graph.AddEdge(first, second);
    }
    in >> first >> second;
    out << count_paths(graph, first, second);
    return 0;
}

int main(){
    run();
}