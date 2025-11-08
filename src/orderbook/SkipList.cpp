#include "../../include/orderbook/SkipList.h"
#include <chrono>
#include <algorithm>
#include <numeric>
#include <deque>

//node constructor
//key: the price level
//price: deque of quantities at that price
//forward: vector of pointers to nodes at each skip list level.
SkipList::Node::Node(int lvl, double price, const std::deque<double> &qtys) {
    key = price;
    Price = qtys;
    forward.resize(lvl, nullptr);
}

//skipList constructor
//maxLevel: maximum number of skip list levels
//p: probability used to determine the random level for each new node
//level: current highest level in the list
//nodeCount: number of nodes
//rng: random number generator used for randomLevel()
//head: dummy node at the start of the skip list with a very small key
SkipList::SkipList(int maxLevel, double p)
    : maxLevel(maxLevel), p(p), level(0), nodeCount(0),
      rng(std::mt19937(std::random_device{}())) {
    head = new Node(maxLevel, -std::numeric_limits<double>::infinity(), std::deque<double>{});
}

//destructor
SkipList::~SkipList() {
    freeList();
    delete head;
    head = nullptr;
}

//deletes all nodes and reesets the skiplist’s count and level
void SkipList::freeList()
{
    Node* current = head->forward[0];
    while (current) {
        Node* next = current->forward[0];
        delete current;
        current = next;
    }
    nodeCount = 0;
    level = 0;
}

// Memory usage calculation
size_t SkipList::getMemoryUsage() const {
    size_t memory = sizeof(*this);
    Node* current = head->forward[0];
    while (current) {
        memory += sizeof(Node)+current->forward.capacity()*sizeof(Node*)+current->Price.size()*sizeof(double);
    }
    return memory;
}

//random level generator for new nodes
int SkipList::randomLevel() const
{
    int lvl = 1;
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    while (dist(rng) < p && lvl < maxLevel) {
        lvl++;
    }
    return lvl;
}

//insert a PriceLevel
bool SkipList::insert(const PriceLevel &pl)
{
    std::vector<Node*> update(maxLevel, nullptr); //keeps track of the nodes at each level that will point to the new node
    Node* current = head;

    for (int i = level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < pl.priceLevel)
            current = current->forward[i];
        update[i] = current;
    }

    current = current->forward[0];

    if (current && current->key == pl.priceLevel) {
        // Append quantities to the deque
        for (double qty : pl.Price) {
            current->Price.push_back(qty);
        }
        return true;
    } else {
        int newLevel = randomLevel();
        if (newLevel - 1 > level) {
            for (int i = level + 1; i < newLevel; i++)
                update[i] = head;
            level = newLevel - 1;
        }

        Node* newNode = new Node(newLevel, pl.priceLevel, pl.Price);
        for (int i = 0; i < newLevel; i++) {
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
        nodeCount++;
        return true;
    }
}

// remove quantity or node
bool SkipList::remove(double price, int quantity)
{
    std::vector<Node*> update(maxLevel, nullptr);
    Node* current = head;

    for (int i = level; i >= 0; i--) { //iterates thru the skip list
        while (current->forward[i] && current->forward[i]->key < price)
            current = current->forward[i];
        update[i] = current;
    }
    current = current->forward[0];

    if (current && current->key == price) { //removes from deque
        int qtyToRemove = quantity;
        while (qtyToRemove > 0 && !current->Price.empty()) {
            double frontQty = current->Price.front();
            if (frontQty <= qtyToRemove) {
                qtyToRemove -= static_cast<int>(frontQty);
                current->Price.pop_front();
            } else {
                current->Price.front() = frontQty - qtyToRemove;
                qtyToRemove = 0;
            }
        }

        if (current->Price.empty()) { //removes node if deque is empty
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current) break;
                update[i]->forward[i] = current->forward[i];
            }
            delete current;
            while (level > 0 && head->forward[level] == nullptr)
                level--;
            nodeCount--;
        }
        return true;
    }
    return false;
}

//find a price level
PriceLevel* SkipList::find(double price) const{
    Node* current = head;
    for (int i = level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < price)
            current = current->forward[i];
    }
    current = current->forward[0];
    if (current && current->key == price) {
        return new PriceLevel{current->key, current->Price};
    }
    return nullptr;
}

// returns top bids or asks
std::vector<PriceLevel> SkipList::topN(size_t n) const{
    std::vector<PriceLevel> result;
    Node* current = head->forward[0];
    while (current && result.size() < n) {
        result.emplace_back(current->key, current->Price);
        current = current->forward[0];
    }
    return result;
}

void OrderBookManager::processOrder(const Order &o){
    auto realStart = std::chrono::high_resolution_clock::now();
    if (o.type == OrderType::ADD) {
        PriceLevel pl(o.price, std::deque<double>{static_cast<double>(o.quantity)});
        auto start = std::chrono::high_resolution_clock::now();
        if (o.side == Side::BID) {
            bids_.insert(pl);
        }
        else {
            asks_.insert(pl);
        }
        auto end = std::chrono::high_resolution_clock::now();
        double microsec = std::chrono::duration<double, std::micro>(end - start).count();
        metrics.insertLatencies.push_back(microsec);
        metrics.addCount++;
        metrics.totalOrders++;
    }
    else if (o.type == OrderType::CANCEL) {
        auto start = std::chrono::high_resolution_clock::now();
        if (o.side == Side::BID)
            bids_.remove(o.price, o.quantity);
        else
            asks_.remove(o.price, o.quantity);
        auto end = std::chrono::high_resolution_clock::now();
        double microsec = std::chrono::duration<double, std::micro>(end - start).count();
        metrics.deleteLatencies.push_back(microsec);
        metrics.cancelCount++;
    }
    else if (o.type == OrderType::MODIFY) {
        auto start = std::chrono::high_resolution_clock::now();
        SkipList* sl = (o.side == Side::BID) ? &bids_ : &asks_;
        if (PriceLevel* pl = sl->find(o.price)) {
            pl->Price.clear();
            pl->Price.push_back(static_cast<double>(o.quantity));
        }
        auto end = std::chrono::high_resolution_clock::now();
        double microsec = std::chrono::duration<double, std::micro>(end - start).count();
        metrics.lookupLatencies.push_back(microsec);
        metrics.modifyCount++;
    }
    auto realEnd = std::chrono::high_resolution_clock::now();
    double totalMicrosec = std::chrono::duration<double, std::micro>(realEnd - realStart).count();
    metrics.latencies.push_back(totalMicrosec);

    if (startTime_.time_since_epoch().count() == 0) { //duration since startTime_, checks if it was initialized
        startTime_ = realStart;
    }
}

void OrderBookManager:: computeStats() {
    if (metrics.latencies.empty()) return;

    //sets the memory usage
    metrics.memoryUsageBytes = bids_.getMemoryUsage() + asks_.getMemoryUsage();

    // min/max for all operations
    metrics.minLatency = *std::min_element(metrics.latencies.begin(), metrics.latencies.end());
    metrics.maxLatency = *std::max_element(metrics.latencies.begin(), metrics.latencies.end());

    // averages
    if (!metrics.insertLatencies.empty()) {
        metrics.avgInsertTime = std::accumulate(metrics.insertLatencies.begin(), metrics.insertLatencies.end(), 0.0) /
                                metrics.insertLatencies.size();
    }
    if (!metrics.deleteLatencies.empty()) {
        metrics.avgDeleteTime = std::accumulate(metrics.deleteLatencies.begin(), metrics.deleteLatencies.end(), 0.0) /
                               metrics.deleteLatencies.size();
    }
    if (!metrics.lookupLatencies.empty()) {
        metrics.avgLookupTime = std::accumulate(metrics.lookupLatencies.begin(), metrics.lookupLatencies.end(), 0.0) /
                               metrics.lookupLatencies.size();
    }

    //final stats
    std::vector<double> sorted = metrics.latencies;
    std::sort(sorted.begin(), sorted.end());
    metrics.medianLatency = sorted[sorted.size() / 2];
    int percentile95 = 0.95 * sorted.size();
    int percentile99 = 0.99 * sorted.size();
    metrics.percentile95 = sorted[percentile95];
    metrics.percentile99 = sorted[percentile99];

    //throughput: total orders / total time
    auto endTime_ = std::chrono::high_resolution_clock::now();
    double totalSeconds = std::chrono::duration<double>(endTime_ - startTime_).count();
    if (totalSeconds > 0) {
        metrics.ordersPerSecond = metrics.totalOrders / totalSeconds;
    }
}

OrderBookManager::Metrics OrderBookManager::getMetrics() {
    computeStats();
    return metrics;
}

//chrono derived from https://en.cppreference.com/w/cpp/chrono.html for calculating time
//https://en.cppreference.com/w/cpp/chrono/time_point/time_since_epoch.html used to calculate time difference for latency