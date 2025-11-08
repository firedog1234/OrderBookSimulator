//
// Created by Avi Patel on 11/4/25.
//

#ifndef ORDERBOOKRB_H
#define ORDERBOOKRB_H
#include "RBTree.h"


class OrderBookRB {
private:
    RBTree bids;
    RBTree asks;
public:
    OrderBookRB();

    void addOrder(Order& order);
    void cancelOrder(Order& order);


    RBNode* getBestBid();
    RBNode* getBestAsk();

    //clearn up helper stuff for debug i guess for code points

};



#endif //ORDERBOOKRB_H
