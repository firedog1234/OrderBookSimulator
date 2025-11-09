#pragma once
#include "../../include/common/types.h" // for PriceLevel, Side, OrderType, Order
#include <vector>
#include <deque>
#include <random>
#include <chrono>

class SkipList {
public:
    struct Node {
        double key; // price
        std::deque<Order> orders; //deque of orders for this price level
        int64_t totalQuantity;
        std::vector<Node*> forward;
        Node(int level, double price, const std::deque<Order>& ordersDeque, int64_t totalQty);
    };
    SkipList(int maxLevel = 16, double p = 0.5);
    ~SkipList();

    bool insert(const PriceLevel& pl);
    bool remove(double price, int quantity);
    PriceLevel* find(double price) const;
    std::vector<PriceLevel> topN(size_t n) const;
    size_t size() const noexcept { return nodeCount; }
    size_t getMemoryUsage() const;
private:
    int maxLevel;
    double p;
    int level;
    size_t nodeCount;
    Node* head;
    mutable std::mt19937 rng;

    int randomLevel() const;
    void freeList();
};

class OrderBookManager {
public:
    OrderBookManager() {
        metrics.name = DataStructureName::SKIP_LIST;
    }
    const SkipList& getBids() const { return bids_; }
    const SkipList& getAsks() const { return asks_; }

    void processOrder(const Order &o);

    std::vector<PriceLevel> getTopBids(size_t n) const { return bids_.topN(n); }
    std::vector<PriceLevel> getTopAsks(size_t n) const { return asks_.topN(n); }

    struct Metrics2 {
        std::vector<double> insertLatencies;
        std::vector<double> deleteLatencies;
        std::vector<double> lookupLatencies;
    };

    Metrics2 metrics2;
    Metrics metrics;
    Metrics getMetrics();

private:
    SkipList bids_;
    SkipList asks_;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
    void computeStats();
};