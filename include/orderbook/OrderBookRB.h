//
// Created by Avi Patel on 11/4/25.
//

#ifndef ORDERBOOKRB_H
#define ORDERBOOKRB_H
#include "RBTree.h"

#include <algorithm>
#include <chrono>
#include <numeric>
#include <vector>

class OrderBookRB {
private:
  RBTree bids;
  RBTree asks;
  Metrics metrics;

  std::unordered_map<uint32_t, double> orderPrices;

  void recordInsertTime(double duration);
  void recordDeleteTime(double duration);

  double ordersPerSecond();

public:
  OrderBookRB();

  void addOrder(Order &order);
  void cancelOrder(Order &order);

  RBNode *getBestBid();
  RBNode *getBestAsk();

  Metrics getMetrics();

  void modifyOrderQuantity(uint32_t orderId, Side side, int32_t newQuantity);
  void modifyOrderPrice(uint32_t orderId, Side side, double newPrice);

  // clearn up helper stuff for debug i guess for code points
};

#endif // ORDERBOOKRB_H
