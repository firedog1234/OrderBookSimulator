//
// Created by Avi Patel on 10/29/25.
//

#pragma once
#include <memory>
#include <vector>
#include "../common/types.h"


struct RBNode {
    Order order;
    RBNode *parent;
    RBNode *left;
    RBNode *right;
    PriceLevel level;

    explicit RBNode(const Order &order) : order(order), parent(nullptr), left(nullptr), right(nullptr), level() {}
};
class RBTree {
private:
    RBNode *root = nullptr;
    std::vector<std::unique_ptr<RBNode>> node_pool;
    void rotate_left(RBNode* node);
    void rotate_right(RBNode* node);
    constexpr uint32_t SIZE = 100'000;

public:
    RBTree();
    void insert(const Order& order);
    void remove(int order_id);
    [[nodiscard]] Order get_best_bid() const;
    [[nodiscard]] Order get_best_ask() const;
};

/* i saw something about preallocating nodes to make it run faster so i think we should do that
since it is an orderbook
https://www.youtube.com/watch?v=UOB7-B2MfwA
The above link is for smart pointer like unique pointer all it does it basically it auto deletes you pointers.
*/
