/*
 Vlasov MM WEB-13

 Задача № 4.1

 Солдаты. В одной военной части решили построить в одну шеренгу по росту. Т.к. часть была далеко
 не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе приходилось
 выгонять из шеренги за плохо начищенные сапоги. Однако солдаты в процессе прихода и ухода
 должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие.
 За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части
 разного роста. Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат,
 а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.

 Требования: скорость выполнения команды - O(log n).

 Формат входных данных.
 Первая строка содержит число N – количество команд (1 ≤ N ≤ 30 000).
 В каждой следующей строке содержится описание команды: число 1 и X если солдат приходит
 в строй (X – рост солдата, натуральное число до 100 000 включительно)
 и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя.
 Солдаты в строе нумеруются с нуля.

 Формат выходных данных.
 На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции,
 на которую должен встать этот солдат (все стоящие за ним двигаются назад).
*/

#include <iostream>

template <class T, class Comparator = std::less<T>>
class AvlTree {
private:
    struct Node {
        explicit Node(const T &key)
                : key_(key),
                  left_(nullptr),
                  right_(nullptr),
                  height_(1),
                  count_(1) {}

        T key_;
        Node *left_;
        Node *right_;
        size_t height_;
        size_t count_;
    };

public:
    AvlTree();
    ~AvlTree();
    void Add(const T &key);
    void DelByPos(size_t pos);
    void Del(const T& key);
    int GetPos(const T &key);
    bool Has(const T& key);

private:
    Node *root_;
    Comparator cmp_ = Comparator();

    void DeleteTree(Node *node);
    Node *AddInternal(Node *node, const T &key);
    Node *BalanceTree(Node *node);
    void FixHeight(Node *node);
    size_t GetHeight(Node *node);
    void FixCount(Node *node);
    size_t GetCount(Node *node);
    int GetBalanceFactor(Node *node);
    Node *RotateLeft(Node *node);
    Node *RotateRight(Node *node);
    Node *DeleteInternal(Node *node, size_t pos);
    Node *FindLeft(Node *node);
    Node *FindRight(Node *node);
    Node *DeleteLeft(Node *node);
    Node *DeleteRight(Node *node);
};

template <class T, class Comparator>
AvlTree<T, Comparator>::AvlTree() : root_(nullptr) {}

template <class T, class Comparator>
AvlTree<T, Comparator>::~AvlTree() {
    DeleteTree(root_);
}

template <class T, class Comparator>
void AvlTree<T, Comparator>::Add(const T &key) {
    if (GetPos(key) == -1)
        root_ = AddInternal(root_, key);
}
template <class T, class Comparator>
void AvlTree<T, Comparator>::DelByPos(size_t pos) {
    root_ = DeleteInternal(root_, pos);
}

template <class T, class Comparator>
void AvlTree<T, Comparator>::Del(const T& key){
    int pos = GetPos(key);
    if (pos == -1) {
        return;
    }
    DelByPos(pos);
}
template <class T, class Comparator>
bool AvlTree<T, Comparator>::Has(const T &key) {
    Node *tmp = root_;
    while (tmp) {
        if (tmp->key_ == key) {
            return true;
        } else if (tmp->key_ < key) {
            tmp = tmp->right_;
        } else {
            tmp = tmp->left_;
        }
    }
    return false;
}

template <class T, class Comparator>
int AvlTree<T, Comparator>::GetPos(const T &key) {
    Node *cur_node = root_;

    int pos = 0;
    while (cur_node) {
        // если нашли
        if (cur_node->key_ == key) {
            return pos + GetCount(cur_node->left_);
        }
        // если искомый ключ больше то надо сместить позицию
        // добавить кол-во нод в левом поддереве и смеситить на 1
        if (cmp_(cur_node->key_, key)) {
            pos += GetCount(cur_node->left_) + 1;
            cur_node = cur_node->right_;
        } else {
            cur_node = cur_node->left_;
        }
    }
    return -1 ;
}

// рекурсивная функция которая удаляет поддеревья и саму ноду
// база рекурсии - пустая нода - лист
template <class T, class Comparator>
void AvlTree<T, Comparator>::DeleteTree(Node *node) {
    if (node) {
        DeleteTree(node->left_);
        DeleteTree(node->right_);
        delete node;
    }
}

// рекурсивная функция добавления элемента к дереву - корень которого node
template <class T, class Comparator>
typename AvlTree<T, Comparator>::Node *AvlTree<T, Comparator>::AddInternal(
        Node *node, const T &key) {
    // если дошли до нужного места то выделяем новую ноду и балансируем
    if (node == nullptr) {
        return new Node(key);
    }

    if (cmp_(node->key_, key)) {
        node->right_ = AddInternal(node->right_, key);
    } else {
        node->left_ = AddInternal(node->left_, key);
    }

    return BalanceTree(node);
}
template <class T, class Comparator>
typename AvlTree<T, Comparator>::Node *AvlTree<T, Comparator>::BalanceTree(
        Node *node) {
    // устанавливаем численные характеристики ноды
    FixHeight(node);
    FixCount(node);

    // в случае различия если высоты отличаюся на |2| то надо выполнить соответсвующий поворот

    // случай что правое поддерево выше на 2 делаем левый поворот
    if (GetBalanceFactor(node) == 2) {
        // если для правого поддерева - высота левого больше то надо сделать
        // дополнительный поворот вправо для правого поддерева
        if (GetBalanceFactor(node->right_) < 0) {
            node->right_ = RotateRight(node->right_);
        }
        return RotateLeft(node);
    }
    // если левое выше на 2 то правый поворот
    if (GetBalanceFactor(node) == -2) {
        // если для левого поддерева - высота левого меньше то еще
        // левый поворот для левого поддерева
        if (GetBalanceFactor(node->left_) > 0) {
            node->left_ = RotateLeft(node->left_);
        }
        return RotateRight(node);
    }
    return node;

}

template <class T, class Comparator>
void AvlTree<T, Comparator>::FixHeight(Node *node) {
    node->height_ =
            std::max(GetHeight(node->left_), GetHeight(node->right_)) + 1;
}

template <class T, class Comparator>
size_t AvlTree<T, Comparator>::GetHeight(Node *node) {
    return node ? node->height_ : 0;
}

template <class T, class Comparator>
void AvlTree<T, Comparator>::FixCount(Node *node) {
    node->count_ = GetCount(node->left_) + GetCount(node->right_) + 1;
}

template <class T, class Comparator>
size_t AvlTree<T, Comparator>::GetCount(Node *node) {
    return node ? node->count_ : 0;
}

template <class T, class Comparator>
int AvlTree<T, Comparator>::GetBalanceFactor(Node *node) {
    return GetHeight(node->right_) - GetHeight(node->left_);
}

template <class T, class Comparator>
typename AvlTree<T, Comparator>::Node *AvlTree<T, Comparator>::RotateLeft(
        Node *node) {
    Node *tmp = node->right_;
    node->right_ = tmp->left_;
    tmp->left_ = node;

    FixHeight(node);
    FixHeight(tmp);
    FixCount(node);
    FixCount(tmp);

    return tmp;
}

template <class T, class Comparator>
typename AvlTree<T, Comparator>::Node *AvlTree<T, Comparator>::RotateRight(
        Node *node) {
    Node *tmp = node->left_;
    node->left_ = tmp->right_;
    tmp->right_ = node;

    FixHeight(node);
    FixHeight(tmp);
    FixCount(node);
    FixCount(tmp);

    return tmp;
}

template <class T, class Comparator>
typename AvlTree<T, Comparator>::Node *AvlTree<T, Comparator>::DeleteInternal(
        Node *node, size_t pos) {
    if (node == nullptr) {
        return nullptr;
    }

    size_t current_pos = GetCount(node->left_);
    if (current_pos < pos) {
        node->right_ = DeleteInternal(node->right_, pos - current_pos - 1);
    } else if (current_pos > pos) {
        node->left_ = DeleteInternal(node->left_, pos);
    } else {
        Node *left = node->left_;
        Node *right = node->right_;
        delete node;

        if (left == nullptr && right == nullptr) {
            return nullptr;
        }

        if (GetHeight(left) > GetHeight(right)) {
            Node *min = FindRight(left);
            min->left_ = DeleteRight(left);
            min->right_ = right;

            return BalanceTree(min);
        } else {
            Node *min = FindLeft(right);
            min->right_ = DeleteLeft(right);
            min->left_ = left;

            return BalanceTree(min);
        }
    }
    return BalanceTree(node);
}

template <class T, class Comparator>
typename AvlTree<T, Comparator>::Node *AvlTree<T, Comparator>::FindLeft(
        Node *node) {
    while (node->left_ != nullptr) {
        node = node->left_;
    }
    return node;
}

template <class T, class Comparator>
typename AvlTree<T, Comparator>::Node *AvlTree<T, Comparator>::FindRight(
        Node *node) {
    while (node->right_ != nullptr) {
        node = node->right_;
    }
    return node;
}

template <class T, class Comparator>
typename AvlTree<T, Comparator>::Node *AvlTree<T, Comparator>::DeleteLeft(
        Node *node) {
    if (!node->left_) return node->right_;
    node->left_ = DeleteLeft(node->left_);
    return BalanceTree(node);
}
template <class T, class Comparator>
typename AvlTree<T, Comparator>::Node *AvlTree<T, Comparator>::DeleteRight(
        Node *node) {
    if (!node->right_) return node->left_;
    node->right_ = DeleteRight(node->right_);
    return BalanceTree(node);
}

void task(std::istream& is, std::ostream& os){
    AvlTree<int, std::greater<int>> tree;
    int count = 0;
    is >> count;

    int op = 0, key = 0;
    for (int i = 0; i < count; i++) {
        is >> op >> key;
        if (op == 1) {
            tree.Add(key);
            os << tree.GetPos(key) << std::endl;
            continue;
        }
        if (op == 2) {
            tree.DelByPos(key);
            continue;
        }
        return;
    }
}

int main() {
    task(std::cin, std::cout);
    return 0;
}
