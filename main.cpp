#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include "Linked_List.hpp"
#include "Linked_List.cpp"


using namespace std;


long long getKey()
{
    long long key = 0;
    long long grade = 1;
    
    for (int i = 0; i < 13; i++)
    {
        if (i == 12)
        {
            int temp = rand() % 9 + 1;
            key += temp * grade;
            grade *= 10;
        }
        else
        {
            int temp = rand() % 10;
            key += temp * grade;
            grade *= 10;
        }
    }
    
    return  key;
}


struct Zombie
{
    string name;
    string skinColor;
    int numberOfBitten;

    
    Zombie()
    {
        string randomName[5] = {"Bob", "Steve", "Jimmy", "Ben", "George"};
        string randomSkinColor[3] = {"blue", "green", "yellow"};

        name = randomName[rand() % 5];
        skinColor = randomSkinColor[rand() % 3];
        numberOfBitten = 0;
    }
    
    
    friend std::ostream& operator<< (std::ostream &out, Zombie &zombie);
};


struct HashNode
{
    long long key;
    Zombie zombie;
};



ostream& operator<< (ostream &out, Zombie &zombie)
{
    out << "Name: " << zombie.name << endl
        << "Skin Color: " << zombie.skinColor << endl
        << "Number Of Bitten: " << zombie.numberOfBitten << endl;

    return out;
}


struct HashTable
{
private:
    
    int M, N, numOfItems;
    double loadFactor;
    LinkedList<HashNode> *bucketsArray;
    
    
    int hash(long long key)
    {
        return key % M;
    }
    
    
    void realloc(int newSize)
    {
        LinkedList<HashNode> *newArray = new LinkedList<HashNode>[newSize];

        for (int i = 0; i < M; i++)
        {
            if (bucketsArray[i].head != nullptr)
            {
                Node<HashNode> *tempNode = bucketsArray[i].head;
                while (tempNode != nullptr)
                {
                    int newIndex = tempNode->object.key % newSize;
                    newArray[newIndex].push_front(tempNode->object);
                    tempNode = tempNode->next;
                }
            }
        }
        
        delete[] bucketsArray;
        bucketsArray = newArray;
        M = newSize;
    }
    
    
public:
    
    HashTable()
    {
        M = 8;
        N = 0;
        numOfItems = 0;
        loadFactor = 0;
        bucketsArray = new LinkedList<HashNode>[M];
    }
    
    
    ~HashTable()
    {
        for (int i = 0; i < M; i++)
        {
            Node<HashNode>* head = bucketsArray[i].head;
            if (head != nullptr)
            {
                Node<HashNode>* eraser = head;
                while(!eraser)
                {
                    eraser = head;
                    head = head->next;
                    delete eraser;
                }
            }
        }
        
        delete [] bucketsArray;
    }
    
    
    Zombie* find(long long key)
    {
        int index = hash(key);

        if (N != 0)
        {
            Node<HashNode> *tempNode = bucketsArray[index].head;
            while (tempNode != nullptr)
            {
                if (tempNode->object.key == key)
                {
                    return &tempNode->object.zombie;
                }
                tempNode = tempNode->next;
            }
        }

        return nullptr;
    }
    
    
    void insert(long long key, Zombie value)
    {
        if (loadFactor >= 0.75)
        {
            realloc(M * 2);
        }
        
        int index = hash(key);
        LinkedList<HashNode>* list = &bucketsArray[index];

        Zombie* pZombie = find(key);

        if (pZombie)
        {
            *pZombie = value;
        }
        else
        {
            numOfItems++;
            N++;
            loadFactor = (double)N / M;
            list->push_front({key, value});
        }
    }
    
    
    void erase(long long key)
    {
        int index = hash(key);
        LinkedList<HashNode>* list = &bucketsArray[index];
        Zombie* pZombie = find(key);
        
        if (pZombie)
        {
            Node<HashNode>* currentNode = list->head;
            Node<HashNode>* prevNode = list->head;
            
            while (currentNode != nullptr)
            {
                if (currentNode->object.key == key)
                {
                    if (currentNode == prevNode)
                    {
                        list->head = list->head->next;
                        delete currentNode;
                        numOfItems--;
                        N--;
                        break;
                    }
                    else
                    {
                        prevNode->next = currentNode->next;
                        numOfItems--;
                        delete currentNode;
                        break;
                    }
                }
                prevNode = currentNode;
                currentNode = currentNode->next;
            }
        }
    }
    
    
    int size()
    {
        return  numOfItems;
    }
};


bool testHashTable()
{
    const int iters = 500000;
    const int keysAmount = iters * 1;
    // generate random keys:
    long long* keys = new long long[keysAmount];
    long long* keysToInsert = new long long[iters];
    long long* keysToErase = new long long[iters];
    long long* keysToFind = new long long[iters];
    for (int i = 0; i < keysAmount; i++)
    {
        keys[i] = getKey();
    }
    for (int i = 0; i < iters; i++)
    {
        keysToInsert[i] = keys[getKey() % keysAmount];
        keysToErase[i] = keys[getKey() % keysAmount];
        keysToFind[i] = keys[getKey() % keysAmount];
    }
    // test my HashTable:
    HashTable hashTable;
    clock_t myStart = clock();
    for (int i = 0; i < iters; i++)
    {
        hashTable.insert(keysToInsert[i], Zombie());
    }
    int myInsertSize = hashTable.size();
    for (int i = 0; i < iters; i++)
    {
        hashTable.erase(keysToErase[i]);
    }
    int myEraseSize = hashTable.size();
    int myFoundAmount = 0;
    for (int i = 0; i < iters; i++)
    {
        if (hashTable.find(keysToFind[i]) != NULL)
        {
            myFoundAmount++;
        }
    }
    clock_t myEnd = clock();
    float myTime = (float(myEnd - myStart)) / CLOCKS_PER_SEC;
    // test STL hash table:
    unordered_map<long long, Zombie> unorderedMap;
    clock_t stlStart = clock();
    for (int i = 0; i < iters; i++)
    {
        unorderedMap.insert({keysToInsert[i], Zombie()});
        
    }
    int stlInsertSize = (int)unorderedMap.size();
    for (int i = 0; i < iters; i++)
    {
        unorderedMap.erase(keysToErase[i]);
    }
    int stlEraseSize = (int)unorderedMap.size();
    int stlFoundAmount = 0;
    for (int i = 0; i < iters; i++)
    {
        if (unorderedMap.find(keysToFind[i]) != unorderedMap.end())
        {
            stlFoundAmount++;
        }
    }
    clock_t stlEnd = clock();
    float stlTime = (float(stlEnd - stlStart)) / CLOCKS_PER_SEC;
    cout << "My HashTable:" << endl;
    cout << "Time: " << myTime << ", size: " << myInsertSize << " - " << myEraseSize << ", found amount: " << myFoundAmount << endl;
    cout << "STL unordered_map:" << endl;
    cout << "Time: " << stlTime << ", size: " << stlInsertSize << " - " << stlEraseSize << ", found amount: " << stlFoundAmount << endl << endl;
    delete[] keys;
    delete[] keysToInsert;
    delete[] keysToErase;
    delete[] keysToFind;
    if (myInsertSize == stlInsertSize && myEraseSize == stlEraseSize && myFoundAmount == stlFoundAmount)
    {
        cout << "The lab is completed" << endl;
        return true;
    }
        cerr << ":(" << endl;
        return false;
    }


int main()
{
    srand((unsigned int)time(NULL));
    testHashTable();
}
