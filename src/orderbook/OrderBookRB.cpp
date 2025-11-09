//
// Created by Avi Patel on 11/4/25.
//

#include "../../include/orderbook/OrderBookRB.h"

OrderBookRB::OrderBookRB() : bids(), asks() {
  metrics.name = DataStructureName::RB_TREE;
  metrics.avgInsertTime = 0.0;
  metrics.avgDeleteTime = 0.0;
  metrics.avgLookupTime = 0.0;
  metrics.minLatency =
      std::numeric_limits<double>::max(); // saw rhis somewherewrq
  metrics.maxLatency = 0.0;
  metrics.medianLatency = 0.0;
  metrics.percentile95 = 0.0;
  metrics.percentile99 = 0.0;
  metrics.ordersPerSecond = 0.0;
  metrics.totalOrders = 0;
  metrics.memoryUsageBytes = 0;
  metrics.addCount = 0;
  metrics.cancelCount = 0;
  metrics.modifyCount = 0;
  metrics.latencies.clear();
}

void OrderBookRB::cancelOrder(Order &order) {
  auto beg = std::chrono::high_resolution_clock::now();
  if (order.side == Side::BID) {
    bids.findAndDelete(order);
  } else {
    asks.findAndDelete(order);
  }

  orderPrices.erase(order.id);

  auto end = std::chrono::high_resolution_clock::now();
  double duration =
      std::chrono::duration<double, std::micro>(end - beg).count();
  recordDeleteTime(duration);
}

void OrderBookRB::addOrder(Order &order) {
  auto beg = std::chrono::high_resolution_clock::now();
  // OYE THIS ALSO MAthce because i had a sperate mathc functions but i just
  // like this wyae jnmreo

  if (order.side == Side::BID) {
    while (true) {
      RBNode *bestAsk = asks.getMin();

      if (!bestAsk || order.price < bestAsk->level.priceLevel) {
        break;
      }

      Order topAsk = bestAsk->level.orders.front();

      if (order.quantity >= topAsk.quantity) {
        order.quantity -= topAsk.quantity;
        asks.findAndDelete(topAsk);

        if (order.quantity == 0) {
          auto end = std::chrono::high_resolution_clock::now();
          double duration =
              std::chrono::duration<double, std::micro>(end - beg).count();
          recordInsertTime(duration);
          return;
        }
      } else {

        RBNode *curBestAsk = asks.getMin();
        if (curBestAsk) {
          curBestAsk->level.orders.front().quantity -= order.quantity;
        }
        auto end = std::chrono::high_resolution_clock::now();
        double duration =
            std::chrono::duration<double, std::micro>(end - beg).count();
        recordInsertTime(duration);
        return;
      }
    }

    bids.insert(order);
  }

  else if (order.side == Side::ASK) {
    while (true) {
      RBNode *bestBid = bids.getMax();
      if (!bestBid || order.price > bestBid->level.priceLevel) {
        break;
      }

      Order topBid = bestBid->level.orders.front();

      if (order.quantity >= topBid.quantity) {
        order.quantity -= topBid.quantity;
        bids.findAndDelete(topBid);

        if (order.quantity == 0) {
          auto end = std::chrono::high_resolution_clock::now();
          double duration =
              std::chrono::duration<double, std::micro>(end - beg).count();
          recordInsertTime(duration);
          return;
        }
      } else {
        RBNode *curBestBid = bids.getMax();
        if (curBestBid) {
          curBestBid->level.orders.front().quantity -= order.quantity;
        }
        auto end = std::chrono::high_resolution_clock::now();
        double duration =
            std::chrono::duration<double, std::micro>(end - beg).count();
        recordInsertTime(duration);
        return;
      }
    }

    asks.insert(order);
  }
  orderPrices[order.id] = order.price;
  auto end = std::chrono::high_resolution_clock::now();
  double duration =
      std::chrono::duration<double, std::micro>(end - beg).count();
  recordInsertTime(duration);
}

void OrderBookRB::recordInsertTime(double duration) {
  metrics.totalOrders++;
  metrics.addCount++;
  metrics.latencies.push_back(duration);
  metrics.minLatency = metrics.latencies.size() == 1
                           ? duration
                           : std::min(metrics.minLatency, duration);
  metrics.maxLatency = metrics.latencies.size() == 1
                           ? duration
                           : std::max(metrics.maxLatency, duration);
  metrics.avgInsertTime +=
      (duration - metrics.avgInsertTime) / metrics.addCount;
}

void OrderBookRB::recordDeleteTime(double duration) {
  metrics.totalOrders++;
  metrics.cancelCount++;
  metrics.latencies.push_back(duration);
  metrics.minLatency = metrics.latencies.size() == 1
                           ? duration
                           : std::min(metrics.minLatency, duration);
  metrics.maxLatency = metrics.latencies.size() == 1
                           ? duration
                           : std::max(metrics.maxLatency, duration);
  metrics.avgDeleteTime +=
      (duration - metrics.avgDeleteTime) / metrics.cancelCount;
}

RBNode *OrderBookRB::getBestBid() { return bids.getMax(); }

RBNode *OrderBookRB::getBestAsk() { return asks.getMin(); }

void OrderBookRB::modifyOrderQuantity(uint32_t orderId, Side side,
                                      int32_t newQuantity) {
  auto beg = std::chrono::high_resolution_clock::now();

  RBTree &tree = (side == Side::BID) ? bids : asks;
  if (orderPrices.find(orderId) == orderPrices.end()) {
    return;
  }

  Order dummyOrder;
  dummyOrder.id = orderId;
  dummyOrder.price = orderPrices[orderId];
  RBNode *node = tree.find(dummyOrder);
  if (!node) {
    return;
  }

  for (Order &o : node->level.orders) {
    if (o.id == orderId) {
      o.quantity = newQuantity;
      break;
    }
  } // prolly shoudve dont something more effiencet in rbtree to avound multiple
    // O(n) traversal
  // we couldve used a hashmap to store an id and its order buttt ik if thats
  // allowed here

  auto end = std::chrono::high_resolution_clock::now();
  double duration =
      std::chrono::duration<double, std::micro>(end - beg).count();

  metrics.modifyCount++;
  metrics.latencies.push_back(duration);
  metrics.avgLookupTime +=
      (duration - metrics.avgLookupTime) / metrics.modifyCount;
  metrics.minLatency = metrics.latencies.size() == 1
                           ? duration
                           : std::min(metrics.minLatency, duration);
  metrics.maxLatency = metrics.latencies.size() == 1
                           ? duration
                           : std::max(metrics.maxLatency, duration);
}

void OrderBookRB::modifyOrderPrice(uint32_t orderId, Side side,
                                   double newPrice) {
  auto beg = std::chrono::high_resolution_clock::now();

  RBTree &tree = (side == Side::BID) ? bids : asks;

  if (orderPrices.find(orderId) == orderPrices.end()) {
    return;
  }
  Order dummyOrder;
  dummyOrder.id = orderId;
  dummyOrder.price = orderPrices[orderId];

  RBNode *node = tree.find(dummyOrder);
  if (!node) {
    return;
  }

  std::optional<Order> orderPtr;

  for (Order &o : node->level.orders) {
    if (o.id == orderId) {
      orderPtr = o;
      break;
    }
  }

  if (!orderPtr.has_value()) {
    return;
  }

  if (orderPtr->price != newPrice) {
    tree.findAndDelete(*orderPtr);
    orderPtr->price = newPrice;
    tree.insert(*orderPtr);
  }

  if (orderPtr.has_value()) {
    orderPrices[orderId] = orderPtr->price;
  }

  auto end = std::chrono::high_resolution_clock::now();
  double duration =
      std::chrono::duration<double, std::micro>(end - beg).count();

  metrics.modifyCount++;
  metrics.latencies.push_back(duration);
  metrics.avgLookupTime +=
      (duration - metrics.avgLookupTime) / metrics.modifyCount;
  metrics.minLatency = metrics.latencies.size() == 1
                           ? duration
                           : std::min(metrics.minLatency, duration);
  metrics.maxLatency = metrics.latencies.size() == 1
                           ? duration
                           : std::max(metrics.maxLatency, duration);
}

double OrderBookRB::ordersPerSecond() {
  if (metrics.latencies.empty()) {
    return 0.0;
  }

  double totalTimeMicro =
      std::accumulate(metrics.latencies.begin(), metrics.latencies.end(),
                      0.0); // sai told me bout this
  double totalTimeSec = totalTimeMicro / 1e6;
  return totalTimeSec > 0 ? metrics.totalOrders / totalTimeSec : 0;
}

Metrics OrderBookRB::getMetrics() {
  Metrics result = metrics;

  result.ordersPerSecond = ordersPerSecond();
  result.memoryUsageBytes = bids.getMemory() + asks.getMemory();

  if (!result.latencies.empty()) {
    std::vector<double> sortedLatencies = result.latencies;
    std::sort(sortedLatencies.begin(), sortedLatencies.end());
    int n = sortedLatencies.size();

    result.medianLatency =
        (n % 2 == 0)
            ? (sortedLatencies[n / 2 - 1] + sortedLatencies[n / 2]) / 2.0
            : sortedLatencies[n / 2];

    result.percentile95 = sortedLatencies[int(std::ceil(0.95 * n)) - 1];
    result.percentile99 = sortedLatencies[int(std::ceil(0.99 * n)) - 1];

  }

  else {
    result.medianLatency = 0;
    result.percentile95 = 0;
    result.percentile99 = 0;
  }

  result.totalOrders = metrics.addCount + metrics.cancelCount + metrics.modifyCount;

  return result;
}
