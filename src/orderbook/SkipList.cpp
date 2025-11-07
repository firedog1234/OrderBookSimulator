//
// Created by Avi Patel on 10/29/25.
//

#include "../../include/orderbook/SkipList.h"
#include <cmath>
#include <iostream>

// Node constructor
SkipList::Node::Node(int level, double k, const PriceLevel &v)
    : key(k), value(v), forward(level, nullptr) {}

// SkipList constructor
SkipList::SkipList(int maxLevel, double p)
    : head_(nullptr),
      maxLevel_(maxLevel),
      p_(p),
      level_(0),
      node_count_(0),
      rng_(std::random_device{}())
{
    // Create header node with a minimal key
    head_ = new Node(maxLevel_, -std::numeric_limits<double>::infinity(), PriceLevel{});
}

// Destructor - free all nodes
SkipList::~SkipList() {
    freeList();
    delete head_;
    head_ = nullptr;
}

void SkipList::freeList() {
    Node *cur = head_->forward[0];
    while (cur) {
        Node *next = cur->forward[0];
        delete cur;
        cur = next;
    }
    node_count_ = 0;
    level_ = 0;
}

// Random level generator (geometric distribution)
int SkipList::randomLevel() const{
    int lvl = 1;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    while (dist(rng_) < p_ && lvl < maxLevel_) ++lvl;
    return lvl;
}

bool SkipList::insert(const PriceLevel &pl) {
    std::vector<Node*> update(maxLevel_, nullptr);
    Node *x = head_;

    // find position for insert
    for (int i = level_; i >= 0; --i) {
        while (x->forward[i] && x->forward[i]->key < pl.priceLevel) {
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];

    if (x && x->key == pl.priceLevel) {
        // replace existing price level's deque (or merge as needed)
        x->value.Price = pl.Price;
        return true;
    } else {
        int lvl = randomLevel();
        if (lvl - 1 > level_) {
            for (int i = level_ + 1; i < lvl; ++i) {
                update[i] = head_;
            }
            level_ = lvl - 1;
        }

        Node *newNode = new Node(lvl, pl.priceLevel, pl);
        for (int i = 0; i < lvl; ++i) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
        ++node_count_;
        return true;
    }
}

bool SkipList::remove(double price) {
    std::vector<Node*> update(maxLevel_, nullptr);
    Node *x = head_;

    for (int i = level_; i >= 0; --i) {
        while (x->forward[i] && x->forward[i]->key < price) {
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];

    if (x && x->key == price) {
        for (int i = 0; i <= level_; ++i) {
            if (update[i]->forward[i] != x) break;
            update[i]->forward[i] = x->forward[i];
        }
        delete x;
        // decrease level if needed
        while (level_ > 0 && head_->forward[level_] == nullptr) --level_;
        --node_count_;
        return true;
    }
    return false;
}

PriceLevel* SkipList::find(double price) const {
    Node *x = head_;
    for (int i = level_; i >= 0; --i) {
        while (x->forward[i] && x->forward[i]->key < price) {
            x = x->forward[i];
        }
    }
    x = x->forward[0];
    if (x && x->key == price) return &x->value;
    return nullptr;
}

std::vector<PriceLevel> SkipList::topN(size_t n) const {
    std::vector<PriceLevel> res;
    Node *cur = head_->forward[0];
    // collect in increasing price order (lowest to highest)
    while (cur && res.size() < n) {
        res.push_back(cur->value);
        cur = cur->forward[0];
    }
    return res;
}