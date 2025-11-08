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
        int totalOrders = 0;
        // memory
        size_t memoryUsageBytes = 0;
        // order count
        int addCount = 0;
        int cancelCount = 0;
        int modifyCount = 0;
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