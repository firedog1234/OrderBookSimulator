//
// Created by Avi Patel on 11/4/25.
//

#include "../../include/orderbook/OrderBookRB.h"

OrderBookRB::OrderBookRB() : bids(), asks() {}

void OrderBookRB::cancelOrder(Order &order) {
  if (order.side == Side::BID)
    bids.findAndDelete(order);
  else
    asks.findAndDelete(order);
}

void OrderBookRB::addOrder(Order &order) {
  // OYE THIS ALSO MAthce because i had a sperate mathc functions but i just
  // like this wyae jnmreo
  if (order.side == Side::BID) {
    while (true) { // temporary condition? fix later -> nah we good temp fix is
                   // a perm fix
      RBNode *bestAsk = asks.getMin();
      if (!bestAsk || order.price < bestAsk->level.priceLevel) {
        break;
      }

      auto &askOrders = bestAsk->level.orders;
      Order *topAsk = askOrders.front();

      if (order.quantity >= topAsk->quantity) {
        order.quantity -= topAsk->quantity;
        asks.findAndDelete(*topAsk);
        if (askOrders.empty()) {
          asks.remove(bestAsk);
        }
        if (order.quantity == 0) {
          return;
        }
      } else {
        topAsk->quantity -= order.quantity;
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

      auto &bidOrders = bestBid->level.orders;

      Order *topBid = bidOrders.front();

      if (order.quantity >= topBid->quantity) {
        order.quantity -= topBid->quantity;
        bids.findAndDelete(*topBid);
        if (bidOrders.empty())
          bids.remove(bestBid);
        if (order.quantity == 0)
          return;
      } else {
        topBid->quantity -= order.quantity;
        return;
      }
    }

    asks.insert(order);
  }
}

RBNode *OrderBookRB::getBestBid() { return bids.getMax(); }

RBNode *OrderBookRB::getBestAsk() { return asks.getMin(); }
