//
// Created by Avi Patel on 10/29/25.
//

#pragma once
#include "../../include/common/types.h"   // for PriceLevel
#include <vector>
#include <iostream>
#include <random>
#include <memory>
#include <limits>

class SkipList {
public:
    explicit SkipList(int maxLevel = 16, double p = 0.5);
    ~SkipList();

    // Insert a price level. If a level at a price already exists, replace its Price deque.
    // Returns true if inserted (or replaced).
    bool insert(const PriceLevel &pl) ;

    // Remove price level by price. Returns true if removed.
    bool remove(double price);

    // Return pointer to PriceLevel (owned by skip list) or nullptr if not found.
    // Caller must not delete a returned pointer.
    PriceLevel* find(double price) const;

    // Return top N levels (sorted highest to lowest for asks/bids usage can filter)
    std::vector<PriceLevel> topN(size_t n) const;

    // Number of elements currently stored
    size_t size() const noexcept { return node_count_; }

private:
    struct Node {
        double key;
        PriceLevel value;
        std::vector<Node*> forward;
        Node(int level, double k, const PriceLevel &v);
    };

    int randomLevel() const;
    void freeList();

    Node *head_;
    int maxLevel_;
    double p_;                 // probability for level increase (0 < p < 1)
    int level_;                // current highest level (0-based index)
    size_t node_count_;
    mutable std::mt19937 rng_; // rng for randomLevel
};