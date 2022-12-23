/*
 Vlasov MM WEB-13

 Постройте B-дерево минимального порядка t и выведите его по слоям.
 В качестве ключа используются числа, лежащие в диапазоне 0..2^32 -1

 Требования:
 B-дерево должно быть реализовано в виде шаблонного класса.
 Решение должно поддерживать передачу функции сравнения снаружи.

 Формат входных данных
 Сначала вводится минимальный порядок дерева t.
 Затем вводятся элементы дерева.
 Формат выходных данных
 Программа должна вывести B-дерево по слоям. Каждый слой на новой строке,
 элементы должны выводится в том порядке, в котором они лежат в узлах.

 3.1
*/

#include <assert.h>
#include <iostream>
#include <queue>
#include <vector>

template <class T>
struct Node {
    Node()
            : keys_(0), children_(0), leaf_(true)
    {}

    Node(const Node& node) = delete;
    Node &operator=(const Node& node) = delete;
    ~Node();

    //  информация о колве ключей - размер вектора
    std::vector<T> keys_;
    std::vector<Node*> children_;
    bool leaf_;
};

template <class T, class Comparator = std::less<T>>
class BTree {
public:
    BTree(const int& min_order, const Comparator& cmp = Comparator())
            : min_order_(min_order), cmp_(cmp), root_(nullptr), count_(0)
    {}
    ~BTree();

    void Add(const T &key);
    void Traverse() const;

private:
    Node<T> *root_;
    size_t min_order_;
    Comparator cmp_;
    size_t count_;

    void Split(Node<T>* node, const size_t& idx);
    void AddAfterSplit(Node<T>* node, const T& key);
};

template <class T>
Node<T>::~Node() {
    for(auto child : children_){
        delete child;
    }
}

template <class T, class Comparator>
BTree<T, Comparator>::~BTree() {
    // удаление корня приводит к удалению всех нод
    if (root_) {
        delete root_;
    }
}

template <class T, class Comparator>
void BTree<T, Comparator>::Split(Node<T>* node, const size_t& idx) {
    Node<T>* children = node->children_[idx];
    // разбиение по медианному ключу
    T mid = children->keys_[min_order_ - 1];

    Node<T>* tmp = new Node<T>();
    tmp->leaf_ = children->leaf_;

    // перемещаем вторую половину объектов в tmp (без медианного ключа) через итератор
    tmp->keys_ = std::vector<T>(children->keys_.begin() + min_order_,children->keys_.end());
    // изменяем размер и исключаем медиану
    children->keys_.resize(min_order_ - 1);
    // если есть потомки то половину переносим
    if(!(children->leaf_)) {
        tmp->children_ = std::vector<Node<T>*>(children->children_.begin() + min_order_, children->children_.end());
        children->children_.resize(min_order_);
    }
    // изменяем размеры
    node->keys_.resize(node->keys_.size() + 1);
    node->children_.resize(node->children_.size() + 1);
    // сдвиг указателей чтоб добавить указатель
    for(size_t i = (node->children_.size() - 1); i > idx + 1; --i) {
        node->children_[i] = node->children_[i - 1];
    }
    node->children_[idx + 1] = tmp;
    // сдвиг ключей для вставки медианы
    for(size_t i = (node->keys_.size() - 1); i > idx; --i) {
        node->keys_[i] = node->keys_[i - 1];
    }
    node->keys_[idx] = mid;
    return;
}

template <class T, class Comparator>
void BTree<T, Comparator>::Add(const T& key) {
    if(root_ == nullptr) {
        root_ = new Node<T>();
    }
    // если достигнут максимум то надо сделать Split
    if(root_->keys_.size() == (2 * min_order_ - 1)) {
        Node<T>* tmp = new Node<T>();
        tmp->leaf_ = false;
        tmp->Node<T>::children_.push_back(root_);
        root_ = tmp;
        Split(root_, 0);
    }
    AddAfterSplit(root_, key);
    return;
}

template <class T, class Comparator>
void BTree<T, Comparator>::AddAfterSplit(Node<T>* node, const T& key) {
    while(!(node->leaf_)) {
        // ищем позицию для вставки справа налево
        size_t i = node->children_.size() - 1;
        while(i > 0 && cmp_(key, node->keys_[i - 1])) {
            i--;
        }
        // доп оптимизация - заблаговременный Split
        if (node->children_[i]->keys_.size() == 2 * min_order_ - 1) {
            Split(node, i);
            // если ключ больше медианного
            if (key > node->keys_[i]) {
                i++;
            }
        }
        node = node->children_[i];
    }
    node->keys_.resize(node->keys_.size() + 1);
    size_t i = node->keys_.size() - 1;

    while(i > 0 && cmp_(key, node->keys_[i - 1])) {
        node->keys_[i] = node->keys_[i - 1];
        i--;
    }
    node->keys_[i] = key;
    return;
}

template <class T, class Comparator>
void BTree<T, Comparator>::Traverse() const {
    std::queue<Node<T>*> children;
    children.push(root_);
    size_t end = children.size();

    while(children.size() > 0) {
        Node<T>* node = children.front();
        children.pop();
        for(size_t i = 0; i < node->keys_.size(); ++i) {
            std::cout << node->keys_[i] << " ";
        }
        for(size_t i = 0; i < node->children_.size(); ++i) {
            children.push(node->children_[i]);
        }
        end--;
        if(end == 0) {
            std::cout << "\n";
            // обновляем новый конец для вывода элементов следующего слоя
            end = children.size();
        }
    }
    return;
}

void task(std::istream & is, std::ostream & os) {
    int t = 0;
    std::cin >> t;

    BTree<int> btree(t);
    int key = 0;
    while (std::cin >> key) {
        btree.Add(key);
    }
    btree.Traverse();
}

int main() {
    task(std::cin, std::cout);
    return 0;
}
