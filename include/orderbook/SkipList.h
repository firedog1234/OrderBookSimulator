//
// Created by Avi Patel on 10/29/25.
//

#pragma once
#include <vector>
#include <iostream>

constexpr int maxNumberOfLevel = 5; // Maximum Level of the skip list

class Node
{
public:

    int data;
    std::vector<Node*> next; // To maintain the levels of the skip list
    Node(int data, int Level) : data(data), next(Level + 1, nullptr) {} // declaring the data and the level of the node
};

class skipList
{
private:
    Node* head;
    int Level;

public:
    skipList();

    void insert(int data);  // To insert the value
    void remove(int data);  // To delete the value
    bool search(int data) const;  // To search for a value
    void display() const;         // Function to display a skip List

};
