/*
 Vlasov MM WEB-13

 Задача № 2.2
 Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
 Требуется построить бинарное дерево, заданное наивным порядком вставки.

 Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤
 K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.

 Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции
 сравнения снаружи.

 2.2 Выведите элементы в порядке pre-order (сверху вниз).
 */

#include <iostream>
#include "queue"
#include <stack>
#include <vector>
#include <cassert>


template <class T>
struct Node {
    T key_;
    Node<T>* right_;
    Node<T>* left_;

    explicit Node(const T& key) : key_(key), right_(nullptr), left_(nullptr) {}
    explicit Node(T&& key)
            : key_(std::move(key)), right_(nullptr), left_(nullptr) {}

    ~Node() = default;
};

template <class T, class Comparator = std::less<T>>
class BSTree {
public:
    explicit BSTree(Comparator cmp = Comparator())
            : root_(nullptr), cmp_(cmp) {}
    ~BSTree();

    void Add(const T& new_key);
    void Traverse() const;

private:
    Node<T>* root_;
    Comparator cmp_;
    void DelNode(Node<T>*);
};

    template <class T, class Comparator>
    BSTree<T, Comparator>::~BSTree() {
        DelNode(root_);
    }

template <class T, class Comparator>
void BSTree<T, Comparator>::DelNode(Node<T>* del_node) {
    if (!root_) {
        return;
    }
    std::queue<Node<T>*> nodes;
    nodes.push(del_node);
    while (!nodes.empty()) {
        Node<T>* node = nodes.front();
        nodes.pop();
        if (node->left_) {
            nodes.push(node->left_);
        }
        if (node->right_) {
            nodes.push(node->right_);
        }
        delete node;
    }
}

template <class T, class Comparator>
void BSTree<T, Comparator>::Traverse() const {
    std::stack<Node<T>*> nodes;
    Node<T>* cur_root = root_;

    while (!nodes.empty() || cur_root != nullptr) {
        if (cur_root) {
            // firsly visit root then left and right
            std::cout << cur_root->key_ << " ";
            // push right part on stack
            if (cur_root->right_) {
                nodes.push(cur_root->right_);
            }
            // work with left part
            cur_root = cur_root->left_;
        } else {
            cur_root = nodes.top();
            nodes.pop();
        }
    }
    return;
}

template <class T, class Comparator>
void BSTree<T, Comparator>::Add(const T& new_key) {
    Node<T>* new_node = new Node<T>(new_key);
    if (root_ == nullptr) {
        root_ = new_node;
        return;
    }
    Node<T>* next_node = root_;

    while (true) {
        if (cmp_(new_key, next_node->key_)) {
            if (next_node->left_ != nullptr) {
                next_node = next_node->left_;
                continue;
            } else {
                next_node->left_ = new_node;
                return;
            }
        } else {
            if (next_node->right_ != nullptr) {
                next_node = next_node->right_;
                continue;
            } else {
                next_node->right_ = new_node;
                return;
            }
        }
    }
}

void task(std::istream& is, std::ostream& os){
    BSTree<int> tree;
    int N = 0;
    std::cin >> N;
    assert(N > 0 && N < 1000000);
    for (size_t i = 0; i < N; ++i) {
        int key = 0;
        std::cin >> key;
        tree.Add(key);
    }
    tree.Traverse();
}

int main() {
    task(std::cin, std::cout);
    return 0;
}

