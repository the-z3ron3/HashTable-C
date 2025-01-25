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
