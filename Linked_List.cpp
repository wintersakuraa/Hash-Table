#include "Linked_List.hpp"


template <typename T>
LinkedList<T>::LinkedList()
{
    head = nullptr;
}


template <typename T>
void LinkedList<T>::push_front(T element)
{
    Node<T> *newNode = new Node<T>;
    
    newNode->object = element;
    newNode->next = head;
    
    head = newNode;
}


