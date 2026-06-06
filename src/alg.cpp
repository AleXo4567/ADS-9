// Copyright 2022 NNTU-CS
#include <algorithm>
#include <vector>
#include "tree.h"

static PMTree::Node* buildSubtree(const std::vector<char>& leftover) {
    if (leftover.empty()) return new PMTree::Node(0);
    PMTree::Node* current = new PMTree::Node(0);
    std::vector<char> sorted = leftover;
    std::sort(sorted.begin(), sorted.end());
    for (size_t i = 0; i < sorted.size(); ++i) {
        char symbol = sorted[i];
        std::vector<char> remaining;
        for (size_t j = 0; j < sorted.size(); ++j) {
            if (j != i) remaining.push_back(sorted[j]);
        }
        PMTree::Node* child = buildSubtree(remaining);
        child->sym = symbol;
        current->links.push_back(child);
    }
    return current;
}

PMTree::PMTree(const std::vector<char>& source) : base(source) {
    top = new Node(0);
    std::vector<char> sortedSource = source;
    std::sort(sortedSource.begin(), sortedSource.end());
    for (size_t idx = 0; idx < sortedSource.size(); ++idx) {
        char curSym = sortedSource[idx];
        std::vector<char> others;
        for (size_t j = 0; j < sortedSource.size(); ++j) {
            if (j != idx) others.push_back(sortedSource[j]);
        }
        Node* childNode = buildSubtree(others);
        childNode->sym = curSym;
        top->links.push_back(childNode);
    }
}

PMTree::~PMTree() {
    destroy(top);
}

PMTree::Node* PMTree::generate(const std::vector<char>& remainder) {
    return buildSubtree(remainder);
}

void PMTree::destroy(Node* ptr) {
    if (!ptr) return;
    for (size_t i = 0; i < ptr->links.size(); ++i) {
        destroy(ptr->links[i]);
    }
    delete ptr;
}

static void walk(PMTree::Node* node, std::vector<char>& buffer,
                 std::vector<std::vector<char>>& result, int depth, int maxDepth) {
    if (depth == maxDepth) {
        result.push_back(buffer);
        return;
    }
    for (size_t i = 0; i < node->links.size(); ++i) {
        PMTree::Node* next = node->links[i];
        buffer.push_back(next->sym);
        walk(next, buffer, result, depth + 1, maxDepth);
        buffer.pop_back();
    }
}

std::vector<std::vector<char>> getAllPerms(PMTree& obj) {
    std::vector<std::vector<char>> result;
    std::vector<char> temp;
    int total = static_cast<int>(obj.base.size());
    for (size_t i = 0; i < obj.top->links.size(); ++i) {
        PMTree::Node* first = obj.top->links[i];
        temp.push_back(first->sym);
        walk(first, temp, result, 1, total);
        temp.pop_back();
    }
    return result;
}

std::vector<char> getPerm1(PMTree& obj, int position) {
    std::vector<std::vector<char>> all = getAllPerms(obj);
    if (position <= 0 || position > static_cast<int>(all.size())) return {};
    return all[position - 1];
}

size_t fact(int n) {
    size_t res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

std::vector<char> getPerm2(PMTree& obj, int position) {
    int total = static_cast<int>(obj.base.size());
    if (position <= 0 || static_cast<size_t>(position) > fact(total)) return {};

    std::vector<char> answer;
    int remaining = position - 1;
    PMTree::Node* cur = obj.top;

    for (int step = 0; step < total; ++step) {
        size_t blockSize = fact(total - step - 1);
        int idx = static_cast<int>(remaining / blockSize);
        remaining %= static_cast<int>(blockSize);
        if (idx >= static_cast<int>(cur->links.size())) return {};
        cur = cur->links[idx];
        answer.push_back(cur->sym);
    }
    return answer;
}
