cmake_minimum_required(VERSION 3.30)
project(Order_Book)

set(CMAKE_CXX_STANDARD 20)

add_executable(Order_Book src/main.cpp
        data/generate_data.cpp
        include/common/macros.h
        include/common/types.h
        src/orderbook/RBTree.cpp
        include/orderbook/RBTree.h
        src/orderbook/SkipList.cpp
        include/orderbook/SkipList.h
        include/utils/benchmark.h)
