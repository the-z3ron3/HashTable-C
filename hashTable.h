#ifndef HASHTABLE
#define HASHTABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ? Macros
#define TABLE_SIZE  6

#define FNV_OFFSET  14695981039346656037UL      // 64 bit FNV-1A
#define FNV_PRIME   1099511628211UL

typedef unsigned long long int ulli_t;

// ? Structure declarations
// Hash item
typedef struct htItem
{
    void* key;              // key = id
    void* value;
} htItem;

// Hash Table
typedef struct hTable
{
    size_t htSize;              // size of hash table
    size_t htCount;             // number of entries in hash table
    htItem* htItemsArray;       // pointer to array of htItem(s)
} hTable;

// ? Function declarations
ulli_t hashFunction(const char* string, size_t tableSize);

hTable* createHT(size_t tableSize);
htItem* insertIntoHT(hTable* hTable, char* key, char* value);
htItem* getHTItem(hTable* hTable, char* key);
void deleteHT(hTable* hTable);
void printHTable(hTable* hTable);

ulli_t calc_items2_length(htItem* item2);

#endif