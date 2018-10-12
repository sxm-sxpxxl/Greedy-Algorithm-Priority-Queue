#include <iostream>
#include <vector>
#include <cstddef>
#include <cmath>
#include <unordered_map>
#include <algorithm>

/*
 * Задача на программирование: очередь с приоритетами
 *
 * Первая строка входа содержит число операций 1 ≤ n ≤ 10^5.
 * Каждая из последующих n строк задают операцию одного из следующих двух типов:
    Insert x, где 0 ≤ x ≤ 10^9 — целое число;
    ExtractMax.
 * Первая операция добавляет число x в очередь с приоритетами,
 * вторая — извлекает максимальное число и выводит его.
 */

template <class T>
class NodeHeap {
private:
    int m_priority;
    T m_data;

public:
    NodeHeap(int priority, T data) {
        m_priority = priority;
        m_data = data;
    }

    int GetPriority() const { return m_priority; }
    T GetData() const { return m_data; }
};

template <class T>
class PriorityMaxQueue {
private:
    typedef NodeHeap<T>* pNode;
    std::vector<pNode> vec_heap;

    inline int GetIndexFromIt(typename std::vector<pNode>::iterator & it) {
        return std::distance(vec_heap.begin(), it);
    }

    inline typename std::vector<pNode>::iterator GetItFromIndex(int index) {
        return vec_heap.begin() + index;
    }

    void InitializeParentNode(int & targetIndex, typename std::vector<pNode>::iterator & targetIt,
                              int & parentIndex, typename std::vector<pNode>::iterator & parentIt) {
        targetIndex = GetIndexFromIt(targetIt);
        parentIndex = static_cast<int>(trunc((targetIndex + 1.0) / 2.0));
        if (parentIndex != 0) parentIndex--;
        parentIt = GetItFromIndex(parentIndex);
    }

    void SiftUp(typename std::vector<pNode>::iterator & targetIt) {
        int targetIndex, parentIndex;
        typename std::vector<pNode>::iterator parentIt;

        InitializeParentNode(targetIndex, targetIt, parentIndex, parentIt);
        if (targetIndex == parentIndex) return;

        while ((*targetIt)->GetPriority() > (*parentIt)->GetPriority()) {
            std::swap_ranges(targetIt, targetIt + 1, parentIt);
            targetIt = parentIt;

            InitializeParentNode(targetIndex, targetIt, parentIndex, parentIt);
        }
    }

    void InitializeChildNodes(int & targetIndex, typename std::vector<pNode>::iterator & targetIt,
                              int & childLeftIndex, typename std::vector<pNode>::iterator & childLeftIt,
                              int & childRightIndex, typename std::vector<pNode>::iterator & childRightIt,
                              bool & isValidChildLeftItMoreThanTarget, bool & isValidChildRightItMoreThanTarget,
                              int & targetPriority, int & childLeftPriority, int & childRightPriority) {
        targetIndex = GetIndexFromIt(targetIt);
        childLeftIndex = (2 * (targetIndex + 1)) - 1;
        childRightIndex = (2 * (targetIndex + 1) + 1) - 1;

        childLeftIt = vec_heap.end();
        childRightIt = vec_heap.end();

        isValidChildLeftItMoreThanTarget = false;
        isValidChildRightItMoreThanTarget = false;

        targetPriority = (*targetIt)->GetPriority();
        if (childLeftIndex < vec_heap.size()) {
            childLeftIt = GetItFromIndex(childLeftIndex);
            childLeftPriority = (*childLeftIt)->GetPriority();
            isValidChildLeftItMoreThanTarget = childLeftPriority > targetPriority;
        }

        if (childRightIndex < vec_heap.size()) {
            childRightIt = GetItFromIndex(childRightIndex);
            childRightPriority = (*childRightIt)->GetPriority();
            isValidChildRightItMoreThanTarget = childRightPriority > targetPriority;
        }
    }

    void SiftDown(typename std::vector<pNode>::iterator & targetIt) {
        int targetIndex, childLeftIndex, childRightIndex;
        typename std::vector<pNode>::iterator childLeftIt;
        typename std::vector<pNode>::iterator childRightIt;

        bool isValidChildLeftItMoreThanTarget;
        bool isValidChildRightItMoreThanTarget;
        int childLeftPriority, childRightPriority, targetPriority;

        InitializeChildNodes(targetIndex, targetIt,
                             childLeftIndex, childLeftIt,
                             childRightIndex, childRightIt,
                             isValidChildLeftItMoreThanTarget, isValidChildRightItMoreThanTarget,
                             targetPriority, childLeftPriority, childRightPriority);

        while (isValidChildLeftItMoreThanTarget || isValidChildRightItMoreThanTarget) {
            if (isValidChildLeftItMoreThanTarget & isValidChildRightItMoreThanTarget) {
                if ((*childLeftIt)->GetPriority() > (*childRightIt)->GetPriority()) {
                    std::swap_ranges(targetIt, targetIt + 1, childLeftIt);
                    targetIt = childLeftIt;
                }
                else {
                    std::swap_ranges(targetIt, targetIt + 1, childRightIt);
                    targetIt = childRightIt;
                }
            } else if (isValidChildLeftItMoreThanTarget) {
                std::swap_ranges(targetIt, targetIt + 1, childLeftIt);
                targetIt = childLeftIt;
            } else if (isValidChildRightItMoreThanTarget) {
                std::swap_ranges(targetIt, targetIt + 1, childRightIt);
                targetIt = childRightIt;
            }

            InitializeChildNodes(targetIndex, targetIt,
                                 childLeftIndex, childLeftIt,
                                 childRightIndex, childRightIt,
                                 isValidChildLeftItMoreThanTarget, isValidChildRightItMoreThanTarget,
                                 targetPriority, childLeftPriority, childRightPriority);
        }
    }

public:
    PriorityMaxQueue() = default;
    ~PriorityMaxQueue() {
        for (size_t i = 0; i < vec_heap.size(); ++i) delete vec_heap[i];
    }

    void Insert(int priority, T data) {
        vec_heap.push_back(
                new NodeHeap<T>(priority, data)
        );
        typename std::vector<pNode>::iterator it = vec_heap.end() - 1;
        SiftUp(it);
    }

    pNode ExtractMax() {
        typename std::vector<pNode>::iterator beginIt = vec_heap.begin();
        pNode resultedNode = *beginIt;

        std::swap_ranges(beginIt, beginIt + 1, vec_heap.end() - 1);
        vec_heap.pop_back();
        SiftDown(beginIt);
        return resultedNode;
    }

    bool Empty() const {
        return vec_heap.empty();
    }
};

struct Operation {
    std::string operation;
    int priority;

    Operation() : operation(""), priority(-1) {}
};

int main() {
    int countOfOperations;
    std::cin >> countOfOperations;
    std::cin.ignore();

    std::vector<Operation> vec_operations(static_cast<unsigned int>(countOfOperations));
    for (auto & operation : vec_operations) {
        std::string str;
        std::getline(std::cin, str);
        std::transform(str.begin(), str.end(), str.begin(), tolower);

        int spacePos = str.find(' ', 0);
        if (spacePos == std::string::npos) {
            operation.operation = str;
        }
        else {
            operation.operation = str.substr(0, static_cast<unsigned int>(spacePos));
            operation.priority = atoi(str.substr(static_cast<unsigned int>(spacePos + 1), str.size()).c_str());
        }
    }

    PriorityMaxQueue<std::string> queue;

    for (auto & operation : vec_operations) {
        std::string operationStr = operation.operation;

        if (operationStr == "insert") {
            queue.Insert(operation.priority, "");
        }
        else if (operationStr == "extractmax") {
            auto node = queue.ExtractMax();
            std::cout << node->GetPriority() << std::endl;
        }
    }
    return 0;
}