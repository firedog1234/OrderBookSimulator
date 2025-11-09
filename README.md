# **OrderBookSimulator**

A high-performance order book simulator implemented in C++ to process and match buy/sell orders in real-time. This project compares Red-Black Trees and Skip Lists as data structures for managing pre-sorted order queues, providing metrics on throughput, latency, and memory usage. A frontend built with Crow and JavaScript visualizes the live order book and key performance metrics.

## **Meet the Team:**

Sai Jayaraman – Skip List implementation

Pranav Bhargava – Data generation, frontend

Avighna Patel – Red-Black Tree implementation

## **Problem Statement**

Order books in trading systems must process tens of thousands of orders per second while maintaining correct buy/sell queues and fast access to best bid and ask prices. Choosing the right underlying data structure is critical for efficiency. This project simulates large-scale order processing and evaluates Red-Black Trees vs Skip Lists in terms of performance and scalability.

## **Features**

	•	Process at least 100,000 limit orders (ADD/CANCEL/MODIFY)

	•	Match buy and sell orders correctly
	
	•	Retrieve best bid/ask in O(log n) time
	
	•	Measure and report performance metrics:
	
	•	Average insert/delete/lookup time
	
	•	Memory usage
	
	•	Throughput
	
	•	Latency distribution (median, 95th/99th percentile)
	
	•	Visualize order book depth and recent activity in the frontend

## **Data**

	•	Orders are randomly generated via a C++ script in the /data folder.
	
	•	The focus is on processing efficiency, not actual market data.

## **Technologies & Libraries**

	•	Programming Language: C++17
	
	•	Frontend: Crow (C++) + JavaScript
	
	•	Algorithms Implemented: Red-Black Tree, Skip List
	
	•	Utilities: STL (std::deque, std::vector, <chrono> for timing)

## **Project Structure**

  	OrderBookSimulator/
	
  	├── README.md
	
  	├── CMakeLists.txt
  
	├── data/             # Random data generation scripts  
  
	├── include/          # Headers for common, orderbook, utils  
  
	├── src/              # Source: main.cpp, red-black tree and skip list implementations
  
	└── frontend/         # Crow + JS frontend  

## **Metrics**

The simulator collects:

	•	Total orders processed
	
	•	Orders per second (throughput)
	
	•	Average insert/delete/lookup times (µs)
	
	•	Memory usage (bytes)
	
	•	Median, 95th, and 99th percentile latencies
	
	•	Total ADD/CANCEL/MODIFY counts

These metrics help evaluate the performance of Red-Black Trees vs Skip Lists.

## **References**

https://youtu.be/mUvB89DMtww 
https://youtu.be/ol-FaNLXlR0?si=VNdUuBFyl-1nYqGR https://en.cppreference.com/w/cpp/chrono.html
https://en.cppreference.com/w/cpp/chrono/time_point/time_since_epoch.html
https://www.youtube.com/watch?v=TlfQOdeFy0Y&t=655s
https://www.youtube.com/watch?v=iw8N1_keEWA&t=171s
https://github.com/msambol/dsa/blob/master/trees/red_black_tree.py#L229
https://www.geeksforgeeks.org/cpp/chrono-in-c/
https://www.youtube.com/watch?v=UOB7-B2MfwA
