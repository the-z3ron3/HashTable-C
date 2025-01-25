# Hash Table Data Structure in C

This project implements a Hash Table data structure in C, with a unique approach to collision resolution using dynamic arrays instead of linked lists. This design choice enhances CPU access speeds and aims to mimic the functionality of Python dictionaries in C.

## Features

- **Dynamic Array for Collision Resolution**: Faster than linked lists, improving lookup performance.
- **64-bit FNV-1A Hashing**: Ensures a good distribution of keys to minimize collisions.
- **Core Operations**:
  - `hTable* createHT(size_t tableSize)`: Creates a new hash table with a specified size.
  - `htItem* insertIntoHT(hTable* hTable, char* key, char* value)`: Inserts a key-value pair into the hash table.
  - `htItem* getHTItem(hTable* hTable, char* key)`: Retrieves the value associated with a key.
  - `void deleteHT(hTable* hTable)`: Deletes the hash table and frees all associated memory.
  - `void printHTable(hTable* hTable)`: Prints the current state of the hash table.

## Planned Features

- Additional operations such as updating existing values, resizing the hash table dynamically, and more.

## Motivation

The project was inspired by the need to bring Python-like dictionary functionality into C while optimizing performance. The use of dynamic arrays for collision handling offers a novel and efficient alternative to traditional linked list implementations.

## Usage

1. Clone the repository.
2. Compile the code using any C compiler (e.g., `gcc`,).
3. Include the header file in your project and call the provided functions to utilize the hash table.

## Example

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"

int main(void)
{
    hTable* hTable = createHT(TABLE_SIZE);

    htItem* hItem = NULL;

    // ? Insert data into Hash table in key : value format
    insertIntoHT(hTable, "xyb", "4");
    insertIntoHT(hTable, "xyc", "5");
    insertIntoHT(hTable, "xyd", "6");
    insertIntoHT(hTable, "xyh", "7");

    // ? Print all data from Hash table
    printHTable(hTable);

    // ? Retrieve value from given key
    hItem = getHTItem(hTable, "xyc");
    if (hItem != NULL)
    {
        printf("key: %s, value: %s\n", (char *)hItem->key, (char *)hItem->value);
    }

    hItem = getHTItem(hTable, "xyd");
    if (hItem != NULL)
    {
        printf("key: %s, value: %s\n", (char *)hItem->key, (char *)hItem->value);
    }
    
    // ? Delete Hash table
    deleteHT(hTable);
    
    return (0);
}

```

## Requirements

- C compiler supporting C99 or later.

---

Thank you for checking out this project! Your feedback is greatly appreciated.
