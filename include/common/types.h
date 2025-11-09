#pragma once

#include <crow.h>
#include <deque>
#include <stdexcept>

enum struct Side : uint8_t { BID, ASK};

enum struct OrderType : uint8_t { ADD, CANCEL, MODIFY };

struct Order {
  int64_t timeStamp;
  uint32_t id;
  double price;
  int32_t quantity;
  Side side;
  OrderType type;
};

struct PriceLevel {
  double priceLevel;
  std::deque<Order> orders;
  int64_t totalQuantity = 0;
};

enum struct DataStructureName : uint8_t { RB_TREE, SKIP_LIST };

inline std::string dataStructureToString(DataStructureName name) {
  return name == DataStructureName::RB_TREE ? "Red-Black Tree" : "Skip List";
}

struct Metrics {
  // data structure type
  DataStructureName name;

  // timing metrics (will be expressed in microseconds)
  double avgInsertTime;
  double avgDeleteTime;
  double avgLookupTime;
  double minLatency;
  double maxLatency;
  double medianLatency;
  double percentile95;
  double percentile99;

  // throughput
  double ordersPerSecond;
  uint64_t totalOrders;

  // memory usage
  size_t memoryUsageBytes;

  // order counts
  uint32_t addCount;
  uint32_t cancelCount;
  uint32_t modifyCount;

  // latency distribution
  std::vector<double> latencies;
};

inline crow::json::wvalue to_json(const Metrics &metrics) {
  crow::json::wvalue json;

  json["name"] = dataStructureToString(metrics.name);

  // timing (µs)
  json["avgInsertTime"] = metrics.avgInsertTime;
  json["avgDeleteTime"] = metrics.avgDeleteTime;
  json["avgLookupTime"] = metrics.avgLookupTime;
  json["minLatency"] = metrics.minLatency;
  json["maxLatency"] = metrics.maxLatency;
  json["medianLatency"] = metrics.medianLatency;
  json["percentile95"] = metrics.percentile95;
  json["percentile99"] = metrics.percentile99;

  // throughput
  json["ordersPerSecond"] = metrics.ordersPerSecond;
  json["totalOrders"] = static_cast<int64_t>(metrics.totalOrders);

  // memory
  json["memoryUsageBytes"] = static_cast<int64_t>(metrics.memoryUsageBytes);

  // counts
  json["addCount"] = static_cast<int64_t>(metrics.addCount);
  json["cancelCount"] = static_cast<int64_t>(metrics.cancelCount);
  json["modifyCount"] = static_cast<int64_t>(metrics.modifyCount);

  // latency distribution
  json["latencies"] = crow::json::wvalue::list(metrics.latencies.size());
  for (size_t i = 0; i < metrics.latencies.size(); ++i)
    json["latencies"][i] = metrics.latencies[i];

  return json;
}

struct OrderBookAmountInput {
  int32_t amount;
  OrderBookAmountInput() {};
  OrderBookAmountInput(int32_t amount) : amount(amount) {}
};

class OrderBookNumberTooLargeException : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

inline OrderBookAmountInput to_input(const crow::json::rvalue &json) {
  if (json.has("amount")) {
    if (std::isnan(static_cast<int>(json["amount"]))) {
      throw std::invalid_argument("Amount must be an integer");
    }
    if (static_cast<int>(json["amount"]) > 100000) {
      throw OrderBookNumberTooLargeException("Number of orders in the order book must be less than 100000");
    }
    OrderBookAmountInput input = OrderBookAmountInput(static_cast<int>(json["amount"]));
    return input;
  }
  throw std::runtime_error("No amount field was sent.");
}
/*
Note that here that the enum values BID, ASK, ADD, ... etc etc will not be
initialized to 1,2,3... im p sure that you have to reinterperet cast it to an
int if you want to use it as an int of you want I also used uint8_t b/c other
wise the enum would be 4 bytes not 1 and thats j a waste of memory and not cache
friendly i think for an orderbook.
*/
