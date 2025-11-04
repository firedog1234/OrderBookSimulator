#include "../include/common/types.h"
#include <random>
//
// Created by Avi Patel on 10/29/25.
//

class OrderGenerator {
  std::mt19937 gen;
  std::uniform_real_distribution<double> priceDistribution;
  std::uniform_int_distribution<int32_t> quantityDistribution;
  std::uniform_int_distribution<int> sideDistribution;
  std::uniform_real_distribution<double> typeDistribution;
  uint32_t nextId = 0;

  uint32_t getOrderIdForModifyCancel(std::vector<Order> &orders) {
      std::uniform_int_distribution<size_t> orderDistribution(0, orders.size() - 1);
      return orders[orderDistribution(gen)].id;
  };
public:
  OrderGenerator(double minPrice, double maxPrice, int minQty, int maxQty)
      : priceDistribution(minPrice, maxPrice),
        quantityDistribution(minQty, maxQty), sideDistribution(0, 1),
        typeDistribution(0, 1) {};
  std::vector<Order> generateOrders(int numOrders) {
    std::vector<Order> orders;
    for (int i = 0; i < numOrders; i++) {
      Order order;
      order.price = priceDistribution(gen);
      order.quantity = quantityDistribution(gen);
      order.side = sideDistribution(gen) == 1 ? Side::BID : Side::ASK;
      double typeRandom = typeDistribution(gen);
      if (orders.empty() || typeRandom >= 0.5) {
        order.type = OrderType::ADD;
        order.id = nextId++;
      } else {
        order.type = typeDistribution(gen) > 0.5 ? OrderType::CANCEL : OrderType::MODIFY;
        order.id = getOrderIdForModifyCancel(orders);
      }
      orders.push_back(order);
    }
    return orders;
  }
};


