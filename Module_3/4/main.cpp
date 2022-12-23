#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include "unistd.h"

const uint8_t TABLE_SIZE = 4;
const uint8_t CELLS_COUNT = TABLE_SIZE * TABLE_SIZE;
const uint8_t EMPTY_POS_TARGET = CELLS_COUNT - 1;
const std::array<uint8_t , CELLS_COUNT> FINAL_STATE = { 1, 2, 3, 4,
                                                        5, 6, 7, 8,
                                                        9, 10, 11, 12,
                                                        13, 14, 15, 0};

class GameState {
public:
    GameState();
    GameState(const std::array<uint8_t , CELLS_COUNT> &field);
    bool operator==(const GameState&) const;
    bool operator!=(const GameState&) const;
    uint8_t findEvristic() const;
    bool checkUp() const;
    bool checkDown() const;
    bool checkLeft() const;
    bool checkRight() const;
    GameState moveUp() const;
    GameState moveDown() const;
    GameState moveLeft() const;
    GameState moveRight() const;
    bool checkState() const;
    bool canSolve() const;
    size_t invCount() const;
private:
    int8_t zero_pos_;
    std::array<uint8_t, CELLS_COUNT> state_;
    friend struct StateHasher;
    friend std::ostream& operator<<(std::ostream &out, const GameState &state);
};

struct StateHasher{
    size_t operator()(const GameState &game_state) const {
        size_t hash = 0;
        size_t mul = 1;
        for (int i = 0; i < CELLS_COUNT; i++) {
            hash += game_state.state_[i] * mul;
            mul *= CELLS_COUNT;
        }
        return hash;
    }
};

GameState::GameState(): zero_pos_(0), state_{}{}

GameState::GameState(const std::array<uint8_t , CELLS_COUNT> &state) : state_(state){
    zero_pos_ = -1;
    for (int8_t i = 0; i < CELLS_COUNT; i++) {
        if (state_[i] == 0)
            zero_pos_ = i;
    }
    assert(zero_pos_ != -1);
}

bool GameState::operator==(const GameState &game_state) const {
    if (zero_pos_ != game_state.zero_pos_) {
        return false;
    }
    for (size_t i = 0; i < CELLS_COUNT; i++) {
        if (state_[i] != game_state.state_[i]) {
            return false;
        }
    }
    return true;
}

bool GameState::operator!=(const GameState &game_state) const {
    return !(*this == game_state);
}

bool GameState::checkDown() const {
    return zero_pos_ > TABLE_SIZE - 1;
}

bool GameState::checkUp() const {
    return zero_pos_ < TABLE_SIZE * (TABLE_SIZE - 1);
}

bool GameState::checkRight() const {
    return zero_pos_ % TABLE_SIZE != 0;
}

bool GameState::checkLeft() const {
    return zero_pos_ % TABLE_SIZE != TABLE_SIZE - 1;
}

size_t GameState::invCount() const {
    size_t count = 0;
    for (int i = 0; i < CELLS_COUNT - 1; i++) {
        for (int j = i + 1; j < CELLS_COUNT; j++) {
            if (state_[i] && state_[j] && (state_[i] > state_[j]))
                count++;
        }
    }
    return count;
}

GameState GameState::moveDown() const {
    GameState new_state(*this);
    new_state.zero_pos_ -= TABLE_SIZE;
    std::swap(new_state.state_[zero_pos_], new_state.state_[new_state.zero_pos_]);
    return new_state;
}

GameState GameState::moveUp() const {
    GameState new_state(*this);
    new_state.zero_pos_ += TABLE_SIZE;
    std::swap(new_state.state_[zero_pos_], new_state.state_[new_state.zero_pos_]);
    return new_state;
}

GameState GameState::moveRight() const {
    GameState new_state(*this);
    new_state.zero_pos_--;
    std::swap(new_state.state_[zero_pos_], new_state.state_[new_state.zero_pos_]);
    return new_state;
}

GameState GameState::moveLeft() const {
    GameState new_state(*this);
    new_state.zero_pos_++;
    std::swap(new_state.state_[zero_pos_], new_state.state_[new_state.zero_pos_]);
    return new_state;
}

bool GameState::checkState() const {
    if (zero_pos_ != EMPTY_POS_TARGET) {
        return false;
    }
    for (size_t i = 0; i < EMPTY_POS_TARGET; i++) {
        if (state_[i] != i + 1) {
            return false;
        }
    }
    return true;
}

bool GameState::canSolve() const {
    // критерий разрешимости
    return !((invCount() + zero_pos_ / TABLE_SIZE + 1) % 2);
}

uint8_t GameState::findEvristic() const {
    // эвристика ищется как кол-во элементов стоящих не на своих местах
    uint8_t evristic = 0;
    for (int i = 0; i < CELLS_COUNT; ++i) {
        if (state_[i] != FINAL_STATE[i]) {
            ++evristic;
        }
    }
    return evristic;
}

bool operator<(const GameState& l, const GameState& r) {
    return l.findEvristic() < r.findEvristic();
}

std::ostream& operator<<(std::ostream &out, const GameState &game_state){
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            out << static_cast<int>(game_state.state_[i * TABLE_SIZE + j]) << " ";
        }
        out << std::endl;
    }
    return out;
}

std::string GetSolution(const std::array<uint8_t , CELLS_COUNT> &start) {
    GameState startState(start);

    if (!startState.canSolve())
        return "-1";

    std::unordered_map<GameState, char, StateHasher> visited;
    std::set<std::pair<int, GameState>> set;
    set.emplace(startState.findEvristic(), startState);

    visited[startState] = 'S';

    while (true) {
        std::pair<int, GameState> currState = *set.begin();
        set.erase(set.begin());

        if (currState.second.checkState())
            break;

        if (currState.second.checkLeft()) {
            GameState newState = currState.second.moveLeft();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'L';
                set.emplace(newState.findEvristic() + currState.first + 1, newState);
            }
        }
        if (currState.second.checkRight()) {
            GameState newState = currState.second.moveRight();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'R';
                set.emplace(newState.findEvristic() + currState.first + 1, newState);
            }
        }
        if (currState.second.checkUp()) {
            GameState newState = currState.second.moveUp();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'U';
                set.emplace(newState.findEvristic() + currState.first + 1, newState);
            }
        }
        if (currState.second.checkDown()) {
            GameState newState = currState.second.moveDown();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'D';
                set.emplace(newState.findEvristic() + currState.first + 1, newState);
            }
        }
    }

    std::string path;
    GameState state(FINAL_STATE);

    while (visited[state] != 'S') {
        char move = visited[state];
        switch (move) {
            case 'L': {
                state = state.moveRight();
                path += 'L';
                break;
            }
            case 'R': {
                state = state.moveLeft();
                path += 'R';
                break;
            }
            case 'U': {
                state = state.moveDown();
                path += 'U';
                break;
            }
            case 'D': {
                state = state.moveUp();
                path += 'D';
                break;
            }
            default:
                break;
        }
    }

    std::reverse(path.begin(), path.end());
    return path;
}

void run(std::istream &in, std::ostream &out) {
    std::array<uint8_t , CELLS_COUNT> start{};
    for (int i = 0, val; i < CELLS_COUNT; ++i) {
        in >> val;
        start[i] = val;
    }

    std::string history = GetSolution(start);
    if (history == "-1")
        out << -1;
    else
        out << history.size() << std::endl << history << std::endl;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}
