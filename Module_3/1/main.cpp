#include <iostream>
#include <algorithm>
#include "sstream"
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"
#include "ArcGraph.hpp"

void print(const IGraph& graph, std::ostream & out){
    for (int i = 0; i < graph.VerticesCount(); ++i) {
        out << "Vertex: " << i << " NEXT:[";
        const auto next = graph.GetNextVertices(i);
        std::for_each(
                next.cbegin(),
                next.cend(),
                [&out](const int x) { out << x << ' '; });

        out << "] PREV:[";
        const auto prev = graph.GetPrevVertices(i);

        std::for_each(
                prev.cbegin(),
                prev.cend(),
                [&out](const int x) { out << x << ' '; });

        out << ']' << '\n';
    }
}

bool equal_graphs(const IGraph& l_graph,const IGraph& r_graph, std::ostream & out){
    if (l_graph.VerticesCount() != r_graph.VerticesCount()){
        return false;
    }
    for (int i = 0; i < l_graph.VerticesCount(); ++i) {
        auto l_next = l_graph.GetNextVertices(i);
        auto r_next = r_graph.GetNextVertices(i);
        std::set<int> l_set(l_next.begin(), l_next.end());
        std::set<int> r_set(r_next.begin(), r_next.end());

        auto l_prev = l_graph.GetPrevVertices(i);
        auto r_prev = r_graph.GetPrevVertices(i);
        std::set<int> l_set2(l_prev.begin(), l_prev.end());
        std::set<int> r_set2(r_prev.begin(), r_prev.end());

        if (l_set != r_set || l_set2 != r_set2){
            out << "GRAPHS ARE NOT EQUAL" << std::endl;
            return false;

        }
    }
    out << "Graphs are equal!" << std::endl;
    return true;
}
void test(std::istream & in, std::ostream & out) {
    std::cout << "Enter vertices count, commands count" << std::endl;
    size_t vertices, com_num;
    in >> vertices >> com_num;

    ListGraph list_graph(vertices);
    std::cout << "Fill graph:   from   to" << std::endl;

    for (size_t i = 0; i < com_num; ++i) {
        int from, to;
        in >> from >> to;
        list_graph.AddEdge(from, to);
    }
    print(list_graph, out);
    // the first check is the list graph
    MatrixGraph matrix_graph(list_graph);
    out << "MatrixGraph from ListGraph" << std::endl;
    equal_graphs(matrix_graph, list_graph, out);
    out << "Matrix graph:" << std::endl;
    print(matrix_graph, out);

    SetGraph set_graph(list_graph);
    out << "SetGraph from ListGraph" << std::endl;
    equal_graphs(set_graph, list_graph, out);
    out << "Set graph: " << std::endl;
    print(set_graph, out);

    ArcGraph arc_graph(list_graph);
    out << "ArcGraph from SetGraph" << std::endl;
    equal_graphs(arc_graph, set_graph, out);
    out << "Arc graph:" << std::endl;
    print(arc_graph, out);

}

int main(int argc, char* argv[]) {
    std::stringstream is(
        "10 5\n"\
        "1 5\n"\
        "5 1\n"\
        "1 2\n"\
        "3 1\n"\
        "9 1\n"\
    );
    test(is, std::cout);
    return 0;
}