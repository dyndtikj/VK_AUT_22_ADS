/*
 Vlasov MM WEB-13

 Задача № 1.1

 Реализуйте структуру данных типа “множество строк” на основе динамической
 хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из
 строчных латинских букв.

 Хеш-функция строки должна быть реализована с помощью вычисления значения
 многочлена методом Горнера.

 Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте
 при добавлении элементов в случае, когда коэффициент заполнения таблицы
 достигает 3/4.

 Структура данных должна поддерживать операции добавления строки в множество,
 удаления строки из множества и проверки принадлежности данной строки множеству.

 1_1. Для разрешения коллизий используйте квадратичное пробирование. i-ая проба
 g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
 */

#include <iostream>
#include <string>
#include <vector>

const size_t begin_size = 8;
const double percent = 0.75;
const size_t default_prime = 31;

enum class Status { use, clear, del };

class StrHash {
public:
    explicit StrHash(size_t prime = default_prime) : prime_(prime) {}

    ~StrHash() = default;
    size_t operator()(const std::string& str) const {
        size_t hash = 0;
        for (auto el : str) {
            hash = hash * prime_ + el;
        }
        return hash;
    }

private:
    size_t prime_;
};

template <class T, class Hasher = StrHash>
class UnorderedMap {
public:
    explicit UnorderedMap(size_t size = begin_size);
    ~UnorderedMap();

    bool Add(const T& key);
    bool Del(const T& key);
    bool Has(const T& key) const;

private:
    struct TableNode {
        TableNode() : status_(Status::clear) {}

        explicit TableNode(const T& key) : status_(Status::use), key_(key) {}

        ~TableNode() = default;

        Status status_;
        T key_;
    };

    void grow();
    size_t next(const size_t hash, const size_t idx) const;
    Hasher hasher_;
    std::vector<TableNode> map_;
    size_t count_;
};

template <class T, class Hasher>
UnorderedMap<T, Hasher>::UnorderedMap(size_t size)
        : map_(size, TableNode()), count_(0) {}

template <class T, class Hasher>
UnorderedMap<T, Hasher>::~UnorderedMap() {}

template <class T, class Hasher>
bool UnorderedMap<T, Hasher>::Add(const T& key) {
    if (static_cast<double>(count_) >=
        percent * static_cast<double>(map_.size())) {
        grow();
    }
    size_t hash = hasher_(key) % map_.size();

    int pos_del = -1;
    for (size_t i = 0; i < map_.size(); ++i) {
        if (map_[hash].status_ == Status::clear) {
            break;
        }

        if ((map_[hash].key_ == key) && map_[hash].status_ != Status::del) {
            return false;
        }

        if ((map_[hash].status_ == Status::del) && (pos_del == -1)) {
            pos_del = hash;
        }

        hash = next(hash, i) % map_.size();
    }

    hash = ((pos_del == -1) ? hash : pos_del);
    map_[hash] = TableNode(key);
    ++count_;
    return true;
}

template <class T, class Hasher>
bool UnorderedMap<T, Hasher>::Has(const T& key) const {
    size_t hash = hasher_(key) % map_.size();
    for (size_t i = 0; i < map_.size(); ++i) {
        if ((map_[hash].status_ == Status::clear)) {
            break;
        }

        if (map_[hash].key_ == key && map_[hash].status_ != Status::del) {
            return true;
        }

        hash = next(hash, i) % map_.size();
    }
    return false;
}

template <class T, class Hasher>
bool UnorderedMap<T, Hasher>::Del(const T& key) {
    size_t hash = hasher_(key) % map_.size();
    for (size_t i = 0; i < map_.size(); ++i) {
        if (map_[hash].status_ == Status::clear) {
            break;
        }

        if (map_[hash].key_ == key && map_[hash].status_ != Status::del) {
            map_[hash].status_ = Status::del;
            --count_;
            return true;
        }

        hash = next(hash, i) % map_.size();
    }
    return false;
}

template <class T, class Hasher>
void UnorderedMap<T, Hasher>::grow() {
    std::vector<TableNode> new_map_(map_.size() * 2, TableNode());
    for (size_t i = 0; i < map_.size(); ++i) {
        if (map_[i].status_ == Status::use) {
            size_t hash = hasher_(map_[i].key_) % new_map_.size();
            size_t counter = 0;
            while (new_map_[hash].status_ != Status::clear) {
                hash = next(hash, counter++) % new_map_.size();
            }
            new_map_[hash].key_ = map_[i].key_;
            new_map_[hash].status_ = Status::use;
        }
    }
    map_ = std::move(new_map_);
    return;
}

template <class T, class H>
size_t UnorderedMap<T, H>::next(const size_t hash, const size_t idx) const {
    return hash + idx * (idx + 1) / 2;
}

void run(std::istream& is, std::ostream& os) {
    UnorderedMap<std::string, StrHash> table;
    char command;
    std::string key;
    while (is >> command >> key) {
        switch (command) {
            case '+':
                os << (table.Add(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                os << (table.Has(key) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                os << (table.Del(key) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
    return;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}
