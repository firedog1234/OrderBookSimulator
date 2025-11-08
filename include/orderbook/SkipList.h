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
    size_t getMemoryUsage() const;
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
    OrderBookManager() {
    metrics.name = DataStructureName::SKIP_LIST;
    }

    void processOrder(const Order &o);

    std::vector<PriceLevel> getTopBids(size_t n) const { return bids_.topN(n); }
    std::vector<PriceLevel> getTopAsks(size_t n) const { return asks_.topN(n); }

    struct Metrics {
        DataStructureName name;
        // metrics in microseconds
        double avgInsertTime = 0;
        double avgDeleteTime = 0;
        double avgLookupTime = 0;
        double minLatency = std::numeric_limits<double>::max();
        double maxLatency = 0;
        double medianLatency = 0;
        double percentile95 = 0;
        double percentile99 = 0;
        //throughput
        double ordersPerSecond = 0;
        uint64_t totalOrders = 0;
        // memory
        size_t memoryUsageBytes = 0;
        // order count
        uint64_t addCount = 0;
        uint64_t cancelCount = 0;
        uint64_t modifyCount = 0;
        //latency
        std::vector<double> latencies;
        std::vector<double> insertLatencies;
        std::vector<double> deleteLatencies;
        std::vector<double> lookupLatencies;
    };
    Metrics metrics;
    Metrics getMetrics();

private:
    SkipList bids_;
    SkipList asks_;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
    void computeStats();
};