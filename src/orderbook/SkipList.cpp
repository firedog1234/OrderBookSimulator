//
// Created by Avi Patel on 10/29/25.
//

#include "../../include/orderbook/SkipList.h"

skipList:: skipList()
{
    head = new Node(0, maxNumberOfLevel);    // Initializing the skip list with the max number of levels

    Level = 0;                               // At start the level is 0

}



void skipList::insert(int data)
{
    int newLevel = 0;


    // Deciding the level of inserting node on the basis of coin toss

    while (newLevel < maxNumberOfLevel && (std::rand() % 2) == 1) // here rand()%2 is doing the coin toss
    {
        newLevel++;
    }
    // Resizing the size of the levels to make place for the inserting value

    if (Level < newLevel)
    {
        head->next.resize(newLevel + 1, nullptr);

        Level = newLevel;
    }


    Node* current = head; // pointer to the head to traverse through the skip list


    std::vector<Node*> Update(Level + 1, nullptr); // To store the update node at eah level

    // Loop over the levels upto which we want the value to be inserted

    for (int i = Level; i >= 0; i--)
    {
        // Finding the place for the inserting value

        while (current->next[i] and current->next[i]->data < data)
        {
            current = current->next[i];
        }
        // Updating the level accordingly

        Update[i] = current;

    }

    current = current->next[0];   // Moves the current to the next node at level 0

    if (current == nullptr || current->data != data) // if the current is null, then it does not exit. we need to insert the value
    {
        Node* newNode = new Node(data, Level);

        for (int i = 0; i <= newLevel; i++)
        {
            newNode->next[i] = Update[i]->next[i];

            Update[i]->next[i] = newNode;  // To insert the value at each level

        }

        std::cout << "Element " << data << " inserted successfully.\n";
    }
    else
    {
        std::cout << "Element " << data << " already exists.\n";  // Incase if value already exists
    }
}


void skipList::remove(int data)
{
    // Function to remove value
    Node* current = head; // start by setting a current pointer to the head node

    std::vector<Node*> Update(Level + 1, nullptr); // Create an update vector to store the updated node at each level, Remember only those nodes will be updated where the value to be deleted is present.



    for (int i = Level; i >= 0; i--)
    {
        while (current->next[i] and current->next[i]->data < data)
        {
            current = current->next[i];
        }

        Update[i] = current;         // Update array is keeping the track, where the changes should be made, after deleting the node.
    }

    current = current->next[0];     // Set the current pointer to the next node at level 0.

    if (current != nullptr && current->data == data) // If the value is present, then delete the value
    {
        for (int i = 0; i <= Level; i++)      // Deleting the value from each level
        {
            // Setting the pointers
            if (Update[i]->next[i] != current)
            {
                break;
            }
            else
            {
                Update[i]->next[i] = current->next[i];
            }
        }

        delete current; // deleting the node

        while (Level > 0 && head->next[Level] == nullptr)  // decrement the level variable incase there is not any value at that level
        {
            Level--;
        }

        std::cout << "Element " << data << " deleted successfully." << std::endl;
    }
    else // Incase the value does not exist
    {
        std::cout << "Element " << data << " not found." << std::endl;
    }
}


bool skipList::search(int data) const
{
    Node* current = head;           // start by setting a current pointer to the head node to traverse through the skip list


    for (int i = Level; i >= 0; i--) // Begin traversing from the top level and iteratively approaching the bottom of the skip list
    {
        while (current->next[i] and current->next[i]->data < data) // keep on moving forward if the value of the next node is less than the searching node otherwise move downward (handled by outer for loop)
        {
            current = current->next[i]; // moving forward

        }
    }

    current = current->next[0]; // Move to the next of the node at level 0

    if (current != nullptr && current->data == data) // if value is found
    {
        std::cout << "Element " << data << " found.\n";
        return true;
    }
    else  // Incase value does not exist
    {
        std::cout << "Element " << data << " not found.\n";
        return false;
    }
}


void skipList::display() const
{

    std::cout << "skip List:" << std::endl;

    for (int i = Level; i >= 0; i--) //
    {
        Node* current = head->next[i]; // Initializes the pointer to the first node of that level

        std::cout << "Level " << i << ": ";

        while (current != nullptr)       // Start displaying all the values present at that level
        {
            std::cout << current->data << " ";
            current = current->next[i]; // Moving to the right of the node
        }
        std::cout << std::endl;
    }
}