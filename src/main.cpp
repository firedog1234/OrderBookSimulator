#include "../include/common/types.h"
#include <crow.h>
#include <iostream>
#include "../include/orderbook/OrderBookRB.h"
#include "../data/generate_data.cpp"
#include "../include/orderbook/SkipList.h"

int main() {
   crow::SimpleApp app;

  CROW_ROUTE(app, "/").methods("OPTIONS"_method)
  ([]() {
      auto response = crow::response("");
      response.add_header("Access-Control-Allow-Origin", "*");
      response.add_header("Access-Control-Allow-Methods", "GET, OPTIONS");
      response.add_header("Access-Control-Allow-Headers", "Content-Type");
      return response;
  });
  CROW_ROUTE(app, "/").methods("POST"_method)
  ([](const crow::request& req) {
      try {
          OrderBookAmountInput amount = to_input(crow::json::load(req.body));
          OrderGenerator generator = OrderGenerator(5.0, 500.0, 1, 1000);
          std::vector<Order> orders = generator.generateOrders(amount.amount);
          OrderBookRB tree;
          for (auto order : orders) {
              if (order.type == OrderType::ADD) {
                  tree.addOrder(order);
              } else if (order.type == OrderType::CANCEL) {
                  tree.cancelOrder(order);
              } else if (order.type == OrderType::MODIFY) {
                  tree.modifyOrderPrice(order.id, order.side, order.price);
                  tree.modifyOrderQuantity(order.id, order.side, order.quantity);
              }
          }
          OrderBookManager list;
          for (auto order : orders) {
              list.processOrder(order);
          }
          Metrics rbTreeMetrics = tree.getMetrics();
          Metrics listMetrics = list.getMetrics();

          crow::json::wvalue result(crow::json::type::List);
          result[0] = to_json(rbTreeMetrics);
          result[1] = to_json(listMetrics);
          auto response  = crow::response(result);
          return response;
      } catch (OrderBookNumberTooLargeException) {
          CROW_LOG_ERROR << "The amount of orders to process must be less than 100,000";
          return crow::response(400, R"({"message" : "The amount of orders to process must be less than 100,000"})");
      } catch (std::invalid_argument) {
          CROW_LOG_ERROR << "The amount must be an integer";
          return crow::response(400, R"({"message" : "The amount must be an integer"})");
      } catch (std::exception& e) {
          CROW_LOG_ERROR << "An unknown exception occured: " << e.what();
          return crow::response(400, R"({"message" : "An unknown exception occured"})");
      }
  });

  // Start server
  app.port(8080).multithreaded().run();
}