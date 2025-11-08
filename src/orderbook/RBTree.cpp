//
// Created by Avi Patel on 10/29/25.
//

#include "../../include/orderbook/RBTree.h"

RBTree::RBTree() { node_pool.reserve(SIZE); }

RBNode::RBNode(Order &order)
    : parent(nullptr), left(nullptr), right(nullptr), isRed(true) {
  level.priceLevel = order.price;
  level.orders.push_back(order);
  level.totalQuantity += order.quantity;
}

RBNode *RBTree::CreateNode(Order &order) {
  node_pool.emplace_back(std::make_unique<RBNode>(order));
  return node_pool.back().get();
}

// mirrot the rotate left u alr wrote it
void RBTree::rotateRight(RBNode *node) {
  RBNode *leftNode = node->left;
  if (!leftNode)
    return;

  node->left = leftNode->right;

  if (leftNode->right) {
    leftNode->right->parent = node;
  }

  leftNode->parent = node->parent;

  if (!node->parent) {
    root = leftNode;
  } else if (node == node->parent->right) {
    node->parent->right = leftNode;
  } else {
    node->parent->left = leftNode;
  }

  leftNode->right = node;
  node->parent = leftNode;
}

void RBTree::rotateLeft(RBNode *node) {
  RBNode *rightNode = node->right;
  if (!rightNode)
    return;

  node->right = rightNode->left;
  if (rightNode->left) {
    rightNode->left->parent = node;
  }
  // rightNode->left->parent = node;

  rightNode->parent = node->parent;

  if (!node->parent) {
    root = rightNode;
  } else if (node == node->parent->left) {
    node->parent->left = rightNode;
  } else {
    node->parent->right = rightNode;
  }

  rightNode->left = node;
  node->parent = rightNode;
}

void RBTree::insert(Order &order) {
  RBNode *newNode = CreateNode(order);
  RBNode *start = root;
  RBNode *theParent = nullptr;

  if (root == nullptr) {
    root = newNode;
    newNode->isRed = false;

    return;
  }

  while (start != nullptr) {
    theParent = start;
    if (newNode->level.priceLevel < start->level.priceLevel) {
      start = start->left;
    } else if (newNode->level.priceLevel > start->level.priceLevel) {
      start = start->right;
    } else {
      start->level.orders.push_back(order);
      return;
    }
  }

  // It says theParent may be null i dont see how tho

  newNode->parent = theParent;

  if (newNode->level.priceLevel < theParent->level.priceLevel) {
    theParent->left = newNode;
  } else {
    theParent->right = newNode;
  }

  fixInsert(newNode);
}

RBNode *RBTree::getUncle(RBNode *node) {
  RBNode *grand = getGrandParent(node);
  if (grand == nullptr)
    return nullptr;
  return (node->parent == grand->right) ? grand->left : grand->right;
}

RBNode *RBTree::getGrandParent(RBNode *node) {
  return (node == nullptr || node->parent == nullptr) ? nullptr
                                                      : node->parent->parent;
}

void RBTree::fixInsert(RBNode *node) {
  // CASE ONE: node is first
  if (root == nullptr) {
    root = node;
    root->isRed = false;
    return;
  }

  RBNode *parent = node->parent;
  RBNode *grand = getGrandParent(node);
  RBNode *uncle = getUncle(node);

  // CASE TWO: Parent is Red Uncle is Red
  if (parent != nullptr && uncle != nullptr && parent->isRed && uncle->isRed) {
    parent->isRed = false;
    uncle->isRed = false;
    grand->isRed = true;
    fixInsert(grand);
  }

  // CASE THREE: Parent is Red and Uncle is Black
  else if (parent != nullptr && parent->isRed &&
           (uncle == nullptr || !uncle->isRed)) {
    if (grand == nullptr) {
      return;
    }
    if (parent == grand->left && node == parent->left) {
      rotateRight(grand);
      parent->isRed = false;
      grand->isRed = true;
    }

    // CASE B: Parent is left child, Node is right child (Left-Right)
    else if (parent == grand->left && node == parent->right) {
      rotateLeft(parent);
      node = parent;
      rotateRight(grand);
      node->isRed = false;
      grand->isRed = true;
    }

    // CASE C: Parent is right child, Node is right child (Right-Right)
    else if (parent == grand->right && node == parent->right) {
      rotateLeft(grand);
      parent->isRed = false;
      grand->isRed = true;
    }

    // CASE D: Parent is right child, Node is left child (Right-Left)
    else if (parent == grand->right && node == parent->left) {
      rotateRight(parent);
      node = parent;
      rotateLeft(grand);
      node->isRed = false;
      grand->isRed = true;
    }
  }

  root->isRed = false;
}

RBNode *RBTree::getInorderSuccessor(RBNode *node) {
  if (node == nullptr || node->right == nullptr)
    return nullptr;

  RBNode *concerned = node->right;

  while (concerned->left != nullptr) {
    concerned = concerned->left;
  }
  return concerned;
}

void RBTree::findAndDelete(const Order &order) {
  double price = order.price;
  RBNode *node = root;
  RBNode *foundNode = nullptr;

  while (node != nullptr) {
    if (price < node->level.priceLevel) {
      node = node->left;
    } else if (price > node->level.priceLevel) {
      node = node->right;
    } else {
      auto &orders = node->level.orders;
      for (auto it = orders.begin(); it != orders.end(); ++it) {
        if ((*it).id == order.id) {
          orders.erase(
              it); // https://www.geeksforgeeks.org/cpp/dequeclear-dequeerase-c-stl/
          node->level.totalQuantity -= order.quantity;
          break;
        }
      }
      foundNode = node;
      break;
    }
    // YO ADD YOUR DELETE NODE IF YOUR THING THE UM THE ORDER DEQUE IS EMPTY YOU
    // FEEL AFTER YOU IMPLEMNT DELETE NOTE TO SELF
    // IGU
  }
  if (foundNode != nullptr && foundNode->level.orders.empty()) {
    remove(foundNode);
  }
}

RBNode *RBTree::find(const Order &order) {
  double price = order.price;
  RBNode *node = root;
  while (node != nullptr) {
    if (price < node->level.priceLevel) {
      node = node->left;
    } else if (price > node->level.priceLevel) {
      node = node->right;
    } else {
      for (Order o : node->level.orders) {
        if (o.id == order.id) {
          return node;
        }
      }
      return nullptr;
    }
  }
  return nullptr;
}

void RBTree::transplant(RBNode *u, RBNode *v) {
  if (u->parent == nullptr) {
    root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  if (v) {
    v->parent = u->parent;
  }
}

void RBTree::fixDelete(RBNode *x, RBNode *parent) {

  RBNode *cur = x;
  RBNode *curParent = parent;

  while ((cur != root) && (cur == nullptr || cur->isRed == false)) {
    if (curParent == nullptr)
      break;
    ////need

    RBNode *sibling =
        (curParent->left == cur) ? curParent->right : curParent->left;

    // CASE ONE: sibling is red
    if (sibling && sibling->isRed) {
      sibling->isRed = false;
      curParent->isRed = true;
      if (curParent->left == cur)
        rotateLeft(curParent);
      else
        rotateRight(curParent);
      // update sibling after rotation cuz wedk if its cause its prolly not the
      // same
      sibling = (curParent->left == cur) ? curParent->right : curParent->left;
    }

    bool leftBlack =
        (!sibling || !sibling->left || sibling->left->isRed == false);
    bool rightBlack =
        (!sibling || !sibling->right || sibling->right->isRed == false);

    // CASE TWO: sibling is black and both children black
    if (sibling && leftBlack && rightBlack) {
      if (sibling) {
        sibling->isRed = true;
      }
      cur = curParent;
      curParent = curParent->parent;
    } else {
      // CASE THREE: sibling is black and has a red child
      if (curParent->left == cur) {
        if (sibling && (!sibling->right || sibling->right->isRed == false)) {
          if (sibling && sibling->left) {
            sibling->left->isRed = false;
          }
          if (sibling) {
            sibling->isRed = true;
          }
          if (sibling) {
            rotateRight(sibling);
          }
          sibling = curParent->right;
        }

        if (sibling) {
          sibling->isRed = curParent->isRed;
        }
        curParent->isRed = false;
        if (sibling && sibling->right) {
          sibling->right->isRed = false;
        }
        rotateLeft(curParent);
        cur = root;
        break;
      } else {
        // cur is the child on the right
        if (sibling && (!sibling->left || sibling->left->isRed == false)) {
          if (sibling && sibling->right) {
            sibling->right->isRed = false;
          }
          if (sibling) {
            sibling->isRed = true;
          }
          if (sibling) {
            rotateLeft(sibling);
          };
          sibling = curParent->left;
        }

        if (sibling)
          sibling->isRed = curParent->isRed;
        curParent->isRed = false;
        if (sibling && sibling->left)
          sibling->left->isRed = false;
        rotateRight(curParent);
        cur = root;
        break;
      }
    }
  }

  if (cur) {
    cur->isRed = false;
  }
}

void RBTree::remove(RBNode *z) {
  if (!z)
    return;

  RBNode *y = z;
  bool yOriginalIsRed = y->isRed;
  RBNode *x = nullptr;
  RBNode *xParent = nullptr;

  if (z->left == nullptr) {
    x = z->right;
    xParent = z->parent;
    transplant(z, z->right);
  } else if (z->right == nullptr) {
    x = z->left;
    xParent = z->parent;
    transplant(z, z->left);
  } else {
    RBNode *succ = getInorderSuccessor(z);
    if (!succ)
      return; // defensive

    y = succ;
    yOriginalIsRed = y->isRed;
    x = y->right;

    if (y->parent == z) {
      xParent = y;
      if (x)
        x->parent = y;
    } else {
      xParent = y->parent;
      transplant(y, y->right);
      y->right = z->right;
      if (y->right)
        y->right->parent = y;
    }

    transplant(z, y);
    y->left = z->left;
    if (y->left)
      y->left->parent = y;
    y->isRed = z->isRed;
  }

  if (!yOriginalIsRed) {
    fixDelete(x, xParent);
  }
}

RBNode *RBTree::getMin() {
  RBNode *node = root;
  if (!node)
    return nullptr;
  while (node->left)
    node = node->left;
  return node;
}

RBNode *RBTree::getMax() {
  RBNode *node = root;
  if (!node)
    return nullptr;
  while (node->right)
    node = node->right;
  return node;
}

size_t RBTree::calculateMemory(RBNode *node) {
  if (!node)
    return 0;

  size_t memory = sizeof(*node);
  memory += sizeof(Order) * node->level.orders.size();
  memory += sizeof(node->level.totalQuantity);

  memory += calculateMemory(node->left);
  memory += calculateMemory(node->right);

  return memory;
}

size_t RBTree::getMemory() { return calculateMemory(root); }

// my logic come from this dudes video:
// https://www.youtube.com/watch?v=TlfQOdeFy0Y&t=655s
// https://www.youtube.com/watch?v=iw8N1_keEWA&t=171s
// this helped me fix my delete stuff
// https://github.com/msambol/dsa/blob/master/trees/red_black_tree.py#L229
// deleted node was red no rotations required
// https://www.geeksforgeeks.org/cpp/chrono-in-c/    helped for metrics
