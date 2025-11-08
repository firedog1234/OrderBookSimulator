//
// Created by Avi Patel on 10/29/25.
//

#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "../common/types.h"

struct RBNode {
  RBNode *parent;
  RBNode *left;
  RBNode *right;

  PriceLevel level;

  bool isRed;

  explicit RBNode(Order &order);
};

class RBTree {
private:
  RBNode *root = nullptr;
  static constexpr uint32_t SIZE = 100'000;

  std::vector<std::unique_ptr<RBNode>> node_pool;
  std::unordered_map<uint32_t, Order *> lookUp;

  RBNode *CreateNode(Order &order);

  void rotateLeft(RBNode *node);
  void rotateRight(RBNode *node);
  void fixInsert(RBNode *node);
  static RBNode *getUncle(RBNode *node);
  static RBNode *getGrandParent(RBNode *node);
  RBNode *getInorderSuccessor(RBNode *node);
  void transplant(RBNode *u, RBNode *v);
  void fixDelete(RBNode* x, RBNode* parent);

public:
  RBTree();

  void insert(Order &order);
  void remove(RBNode *toRemove);
  void removeNode();
  void findAndDelete(const Order &order);

  RBNode* getMin();
  RBNode* getMax();

  RBNode *find(const Order &order);

  [[nodiscard]] Order get_best_bid() const;
  [[nodiscard]] Order get_best_ask() const;
};

/* i saw something about preallocating nodes to make it run faster so i think we
should do that since it is an orderbook
https://www.youtube.com/watch?v=UOB7-B2MfwA
The above link is for smart pointer like unique pointer all it does it basically
it auto deletes you pointers.
*/
