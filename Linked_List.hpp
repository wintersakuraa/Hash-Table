#ifndef LINKED_LIST_H
#define LINKED_LIST_H


template <typename T>
struct Node
{
    T object;
    Node *next;
};


template <typename T>
struct LinkedList
{
    Node<T> *head;
    
    LinkedList();
    void push_front(T element);
};


#endif /* LINKED_LIST_H */
