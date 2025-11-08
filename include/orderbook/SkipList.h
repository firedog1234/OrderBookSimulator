#pragma once
#include "../../include/common/types.h" // for PriceLevel, Side, OrderType, Order
#include <vector>
#include <deque>
#include <random>

class SkipList {
public:
    struct Node {
        double key; // price
        std::deque<double> Price; //deque of quantities for this price level
        std::vector<Node*> forward;

        Node(int level, double k, const std::deque<double>& price_deque);
    };

    SkipList(int maxLevel = 16, double p = 0.5);
    ~SkipList();

    bool insert(const PriceLevel& pl);
    bool remove(double price, int quantity);
    PriceLevel* find(double price) const;
    std::vector<PriceLevel> topN(size_t n) const;
    size_t size() const noexcept { return node_count_; }

private:
    int maxLevel_;
    double p_;
    int level_;
    size_t node_count_;
    Node* head_;
    mutable std::mt19937 rng_;

    int randomLevel() const;
    void freeList();
};

class OrderBookManager {
public:
    OrderBookManager() = default;

    void processOrder(const Order &o);

    std::vector<PriceLevel> getTopBids(size_t n) const { return bids_.topN(n); }
    std::vector<PriceLevel> getTopAsks(size_t n) const { return asks_.topN(n); }

private:
    SkipList bids_;
    SkipList asks_;
};