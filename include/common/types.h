#pragma once
#include <cstdint>

enum struct Side : uint8_t {BID, ASK};

enum struct OrderType : uint8_t {ADD, CANCEL, MODIFY};

struct alignas(32) Order {
    int64_t timeStamp;
    uint32_t id;
    double price;
    int32_t quantity;
    Side side;
    OrderType type;
};

/*
Note that here that the enum values BID, ASK, ADD, ... etc etc will not be initialized to 1,2,3... im p sure
that you have to reinterperet cast it to an int if you want to use it as an int of you want
I also used uint8_t b/c other wise the enum would be 4 bytes not 1 and thats j a waste of memory
and not cache friendly i think for an orderbook.
alignas(32) just ensures that the padding will be 32 which i think the compiler does anyways. j good practice
*/

