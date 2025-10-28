#include "include/ConcreteIterator.h"
#include "include/Order.h"
#include "include/ConcreteOrder.h"
#include "include/Leaf.h"

ConcreteIterator::ConcreteIterator(Order* root) : currentIndex(0) {
    if (root != nullptr) {
        collectLeaves(root);
    }
}

ConcreteIterator::~ConcreteIterator() {
}

void ConcreteIterator::collectLeaves(Order* order) {
    if (order == nullptr) {
        return;
    }
    
    Leaf* leaf = dynamic_cast<Leaf*>(order);
    if (leaf != nullptr) {
        items.push_back(leaf);
        return;
    }
    
    ConcreteOrder* composite = dynamic_cast<ConcreteOrder*>(order);
    if (composite != nullptr) {
        std::vector<Order*> children = composite->getChildren();
        for (Order* child : children) {
            collectLeaves(child);
        }
    }
}

void ConcreteIterator::first() {
    currentIndex = 0;
}

void ConcreteIterator::next() {
    if (currentIndex < static_cast<int>(items.size())) {
        currentIndex++;
    }
}

bool ConcreteIterator::isDone() const {
    return currentIndex >= static_cast<int>(items.size());
}

Order* ConcreteIterator::currentItem() const {
    if (currentIndex >= 0 && currentIndex < static_cast<int>(items.size())) {
        return items[currentIndex];
    }
    return nullptr;
}