#include "hashTable.h"

ulli_t hashFunction(const char* string, size_t tableSize)
{
    ulli_t hash = FNV_OFFSET;
    
    for (int i = 0; string[i] != '\0'; i++)
    {
        hash ^= (ulli_t) (string[i]);
        hash *= FNV_PRIME;
    }
    
    return hash % tableSize;
}

hTable* createHT(size_t tableSize)
{
    hTable* pHTable = (hTable*) calloc(1, sizeof(hTable));
    if (pHTable == NULL)
    {
        puts("[!] - Error allocating memory for Hash Table");
        exit(-1);
    }

    pHTable->htItemsArray = (htItem*) calloc(tableSize, sizeof(htItem));
    if (pHTable->htItemsArray == NULL)
    {
        free(pHTable);    // free table before exiting
        puts("[!] - Error allocating memory for Hash Table");
        exit(-1);
    }

    pHTable->htSize = tableSize;
    pHTable->htCount = 0;

    return pHTable;
}

void deleteHT(hTable* hTable)
{
    htItem* htItemsArray = hTable->htItemsArray;   // pointer to first struct in array of structs (+ == next struct)

    if (hTable->htCount > 0)
    {
        for (ulli_t index = 0; index < hTable->htSize; index++) // for finding collision array index
        {
            // ? For freeing single entries
            if ((htItemsArray[index].key != NULL) && (htItemsArray[index].value != NULL))
            {   
                free(htItemsArray[index].value);
                htItemsArray[index].value = NULL;
                
                free(htItemsArray[index].key);
                htItemsArray[index].key = NULL;
            }
            
            // ? For freeing collision entries
            if ((htItemsArray[index].key != NULL) && (htItemsArray[index].value == NULL))
            {
                htItem* items2 = htItemsArray[index].key;
                
                ulli_t items2_size = calc_items2_length(items2);

                // free collision array in reverse order (good practise)
                for (long long int items2_index = (items2_size-1); items2_index >= 0; items2_index--)
                {   
                    free(items2[items2_index].value);
                    items2[items2_index].value = NULL;
                    
                    free(items2[items2_index].key);
                    items2[items2_index].key = NULL;
                }

                /*WORKING FOR DOUBLE COLLISION*/
                // for (ulli_t index = 0; items2[index].key != NULL && items2[index].value != NULL; index++)
                // {
                //     // printf("index: %llu\n", index);
                //     // printf("items2[%llu]: {%s : %s} - {%p : %p}\n", 
                //     index, items2[index].key, items2[index].value, items2[index].key, items2[index].value);

                //     // printf("free(items2[%d].value) - %s\n\n", index, items2[index].value);
                //     free(items2[index].key);
                //     free(items2[index].value);
                // }

                // // for (ulli_t coll_index = 0; coll_index < calc_items2_length(items2); coll_index++)
                // for (int coll_index = 0; items2[coll_index].key != NULL; coll_index++)
                // {
                //     // free(items2[coll_index].key);
                //     // free(items2[coll_index].value);
                //     printf("\ndeallocated at {%p : %p} - {%s : %s}\n", items2[coll_index].key, items2[coll_index].value, items2[coll_index].key, items2[coll_index].value);
                // }

                free(items2);
            }
        }
    }

    free(htItemsArray);
    free(hTable);
}

void printHTable(hTable* hTable)
{
    if (hTable == NULL)
    {
        printf("[!] - Hash table is empty\n");
        return;
    }

    htItem* htItemsArray = hTable->htItemsArray;   // pointer to first struct in array of structs (+ == next struct)

    if (hTable->htCount > 0)
    {
        for (ulli_t index = 0; index < hTable->htSize; index++)
        {      
            if (htItemsArray[index].key != NULL)  // entry exists
            {
                if (htItemsArray[index].value != NULL)    // single entry
                {
                    printf("[i] - {%s : %s} at index %llu\n", (char*) htItemsArray[index].key, (char*) htItemsArray[index].value, index);
                }
                else    // collision entries
                {
                    htItem* coll_arr = htItemsArray[index].key;
                    for (ulli_t coll_arr_index = 0; coll_arr[coll_arr_index].key != NULL; coll_arr_index++)
                    {
                        printf("[i] - {%s : %s} at index %llu\n", (char*) coll_arr[coll_arr_index].key, (char*) coll_arr[coll_arr_index].value, index);
                    }
                }
            }
        }
    }
    else
    {
        printf("[!] - Hash table is empty\n");
    }
}

ulli_t calc_items2_length(htItem* item2)
{
    ulli_t counter = 0;
    
    for (ulli_t i = 0; item2[i].key != NULL && item2[i].value != NULL; i++)
    {
        // printf("inside calc - %s : %s\n", item2[i].key, item2[i].value);
        counter = counter + 1;
    }
    
    return (counter);
}

htItem* insertIntoHT(hTable* pHTable, char* key, char* value)
{
    htItem* htItemsArray = pHTable->htItemsArray;   // pointer to first struct in array of structs (+ == next struct)
    // htItem** htItemsArray = (htItem**) &pHTable->htItemsArray->key;   // pointer to first struct in array of structs (+ == next member)

    ulli_t index = hashFunction(key, pHTable->htSize);

    if (htItemsArray[index].key == NULL)  // if inserting at empty entry slot
    {
		// ~ Allocate memory for key and put its address inside HT Items array
        char* new_key = (char*) calloc(1, strlen(key) + 1);      
        if (new_key == NULL)      // error check
        {
            printf("[!] - Error allocating memory for Hash Table item's (%s) key\n", key);
            deleteHT(pHTable);
            exit(-1);
        }
        
        strcpy(new_key, key);
        htItemsArray[index].key = new_key;

        // printf("htItemsArray[%u].key: %s\n", index,htItemsArray[index].key);

        // printf("addr(htItemsArray[index].key): %p\n", htItemsArray[index].key);
        // printf("addrrr: %s\n", htItemsArray[index].key);
        
        // strcpy(htItemsArray[index].key, key);

		// ~ Allocate memory for value and put its address inside HT Items array
        char* new_value = (char*) calloc(1, strlen(value) + 1);
        if (new_value == NULL)    // error check
        {
            printf("[!] - Error allocating memory for Hash Table item's (%s) value\n", value);
            deleteHT(pHTable);
            exit(-1);
        }

        strcpy(new_value, value);
        htItemsArray[index].value = new_value;

        // printf("htItemsArray[%u].value: %s\n\n", index, htItemsArray[index].value);
        
        // printf("single - allocated at index(%lu) {%p : %p} - {%s : %s}\n\n", index, htItemsArray[index].key, htItemsArray[index].value, htItemsArray[index].key, htItemsArray[index].value);

        /*
        htItemsArray[index].key = key;
        htItemsArray[index].value = value;
        */

	   	// ~ Increment hash table item counter by 1
        pHTable->htCount = pHTable->htCount + 1;
    }
    else    // collision
    {
        // printf("coll: key: %s - value: %s\n", key, value);
        if (htItemsArray[index].value != NULL)    // single collision
        {
            /* Update value for same key */
            // if (strcmp(htItemsArray[index].key, key) == 0)
            // {
            //     htItemsArray[index].value = value;
            //     return (&htItemsArray[index]);
            // }

            // puts("single collision\n");
			
			// ~ Copy address of already existing key and value pair
            char* initial_key = htItemsArray[index].key;
            char* initial_value = htItemsArray[index].value;

            // printf("initial_key: %p\n", initial_key);
            // printf("initial_key: %p\n", initial_value);

            // free(htItemsArray[index].key);
            // free(htItemsArray[index].value);

            htItemsArray[index].key = (htItem*) calloc(3 /*2 htItem + sentinel*/, sizeof(htItem));
            htItemsArray[index].value = NULL;
            
            htItem* htItemsCollisionArray = htItemsArray[index].key;

            // printf("addr: %p\n", htItemsCollisionArray);           // htItem*
            // printf("addr: %x\n", htItemsCollisionArray+1);         // htItem*
            // printf("addr: %x\n", htItemsCollisionArray[0]);        // htItem
            // printf("addr: %llu\n", htItemsCollisionArray[0].key);    // void*

            // htItemsCollisionArray[0].key = initial_key;
            // htItemsCollisionArray[0].value = initial_value;
            
            // htItemsCollisionArray[0].key = (char*) calloc(1, strlen(key) + 1);
            // htItemsCollisionArray[0].value = (char*) calloc(1, strlen(value) + 1);
            // printf("index: %lu\n",index);

			// ~ Assigning values to HT Items Collision array
            htItemsCollisionArray[0].key = initial_key;
            htItemsCollisionArray[0].value = initial_value;

            // printf("double - allocated at {%p : %p} - {%s : %s}\n", htItemsCollisionArray[0].key, htItemsCollisionArray[0].value, htItemsCollisionArray[0].key, htItemsCollisionArray[0].value);
            // exit(0);
            
            // strcpy(htItemsCollisionArray[0].key, initial_key);
            // strcpy(htItemsCollisionArray[0].value, initial_value);

			// ~ Allocating memory and assigning value for the pair which collided
            htItemsCollisionArray[1].key = (char*) calloc(1, strlen(key) + 1);
            htItemsCollisionArray[1].value = (char*) calloc(1, strlen(value) + 1);
            strcpy(htItemsCollisionArray[1].key, key);
            strcpy(htItemsCollisionArray[1].value, value);

            // printf("double - allocated at {%p : %p} - {%s : %s}\n", htItemsCollisionArray[1].key, htItemsCollisionArray[1].value, htItemsCollisionArray[1].key, htItemsCollisionArray[1].value);
            
            // ~ Setting last value as NULL because it is a sentinel entry
			htItemsCollisionArray[2].key = NULL;
            htItemsCollisionArray[2].value = NULL;

            // printf("double - allocated at {%p : %p} - {%s : %s}\n", htItemsCollisionArray[2].key, htItemsCollisionArray[2].value, htItemsCollisionArray[2].key, htItemsCollisionArray[2].value);

            // for (int i = 0; htItemsCollisionArray[i].key != NULL; i++)
            // {
            //     printf("char: %s : %s\n", htItemsCollisionArray[i].key, htItemsCollisionArray[i].value);
            // }

			// ~ Debug Print Collision array
            // for (ulli_t indexxx = 0; htItemsCollisionArray[indexxx].key != NULL && htItemsCollisionArray[indexxx].value != NULL; indexxx++)
            // {
            //     // printf("indexxx: %llu\n", indexxx);
            //     printf("htItemsCollisionArray[%llu]: {%s : %s} - {%p : %p}\n", 
            //     indexxx, htItemsCollisionArray[indexxx].key, htItemsCollisionArray[indexxx].value, htItemsCollisionArray[indexxx].key, htItemsCollisionArray[indexxx].value);

            //     // printf("free(htItemsCollisionArray[%d].value) - %s\n\n", indexxx, htItemsCollisionArray[indexxx].value);
            //     // free(htItemsCollisionArray[indexxx].key);
            //     // free(htItemsCollisionArray[indexxx].value);
            // }

            pHTable->htCount = pHTable->htCount + 1;

            // printf("addr(htItemsCollisionArray): %llu\n", htItemsCollisionArray);
            // printf("addr(htItemsCollisionArray+1): %llu\n", htItemsCollisionArray+1);
            return (&htItemsCollisionArray[1]);
        }
        else    // more than 1 collision
        {
            // puts("more than 1 collision\n");

            htItem* htItemsCollisionArray = htItemsArray[index].key;
            // printf("triple - addr(htItemsCollisionArray): %p\n", htItemsCollisionArray);

            /* Update value for same key */
            // for (int coll_arr_index = 0; htItemsCollisionArray[coll_arr_index].key != NULL; coll_arr_index++)
            // {
            //     // printf("index: %d - char: %s -> %s\n", coll_arr_index, htItemsCollisionArray[coll_arr_index].key, htItemsCollisionArray[coll_arr_index].value);
            //     if (strcmp(htItemsCollisionArray[coll_arr_index].key, key) == 0)
            //     {
            //         htItemsCollisionArray[coll_arr_index].value = value;
            //         return (&htItemsCollisionArray[coll_arr_index]);
            //     }
            // }

			// ! Realloc without traversing whole Collision array
            size_t size = calc_items2_length(htItemsCollisionArray); // 2

            // printf("size htItemsCollisionArray: %llu\n", size);

            htItem* new_items2 = (htItem*) realloc(htItemsCollisionArray, sizeof(htItem) * (size + 2));  // error check

            // printf("addr of htItemsCollisionArray: %p\n", htItemsCollisionArray);
            // printf("addr of new_items2: %p\n", new_items2);

            // getch();
            
            char* new_key = (char*) calloc(1, strlen(key) + 1);  // add error check
            char* new_value = (char*) calloc(1, strlen(value) + 1);

            // printf("initial_keyy: %s\n", htItemsCollisionArray[size].key);
            // printf("initial_keyy: %s\n", htItemsCollisionArray[size].value);
            
            strcpy(new_key, key);
            strcpy(new_value, value);

            new_items2[size].key = new_key;
            new_items2[size].value = new_value;
            
            // printf("triple - allocated at {%p : %p} - {%s : %s}\n", new_items2[size].key, new_items2[size].value, new_items2[size].key, new_items2[size].value);

            new_items2[size + 1].key = NULL;
            new_items2[size + 1].value = NULL;
            
            // printf("initial_keyz: %s\n", htItemsCollisionArray[0].key);
            // printf("initial_keyz: %s\n", htItemsCollisionArray[0].value);
            
            // printf("initial_keyz: %s\n", htItemsCollisionArray[1].key);
            // printf("initial_keyz: %s\n", htItemsCollisionArray[1].value);
            
            // printf("initial_keyz: %s\n", htItemsCollisionArray[2].key);
            // printf("initial_keyz: %s\n", htItemsCollisionArray[2].value);

            // printf("initial_keyz: %s\n", htItemsCollisionArray[3].key);
            // printf("initial_keyz: %s\n", htItemsCollisionArray[3].value);

            
            // strcpy(htItemsCollisionArray[size+1].key, NULL);
            // strcpy(htItemsCollisionArray[size+1].value, NULL);

            // for (int i = 0; i < size+1; i++)
            // {
            //     printf("charr %d: %s : %s\n", i, htItemsCollisionArray[i].key, htItemsCollisionArray[i].value);
            //     // printf("charr %d\n", i);
            // }
            

            
            
            // for (int i = 0; i < size+2; i++)
            // {
            //     printf("charr %d: %s : %s\n", i, htItemsCollisionArray[i].key, htItemsCollisionArray[i].value);
            //     // printf("charr %d\n", i);
            // }
            
            // puts("here");
            // exit(0);

            // for (int i = 0; htItemsCollisionArray[i].key != NULL; i++)
            // {
            //     printf("index: %d - char: %s -> %s\n", i, htItemsCollisionArray[i].key, htItemsCollisionArray[i].value);
            // }
            // puts("here");
            // exit(0);

            // for (int i = 0; new_items2[i].key != NULL; i++)
            // {
            //     printf("char: %s : %s\n", new_items2[i].key, new_items2[i].value);
            // }

            for (ulli_t indexxx = 0; new_items2[indexxx].key != NULL && new_items2[indexxx].value != NULL; indexxx++)
            {
                // printf("indexxx: %llu\n", indexxx);
                // printf("new_items2[%llu]: {%s : %s} - {%p : %p}\n", 
                // indexxx, new_items2[indexxx].key, new_items2[indexxx].value, new_items2[indexxx].key, new_items2[indexxx].value);

                // printf("free(new_items2[%d].value) - %s\n\n", indexxx, new_items2[indexxx].value);
                // free(new_items2[indexxx].key);
                // free(new_items2[indexxx].value);
            }

            htItemsArray[index].key = new_items2; ////////////// biggest bug fix

            // htItem* htItemsArray = htItemsArray[index].key;

            // for (ulli_t indexz = 0; htItemsArray[indexz].key != NULL && htItemsArray[indexz].value != NULL; indexz++)
            // {
            //     // printf("indexz: %llu\n", indexz);
            //     printf("htItemsArray[%llu]: {%s : %s} - {%p : %p}\n", 
            //     indexz, htItemsArray[indexz].key, htItemsArray[indexz].value, htItemsArray[indexz].key, htItemsArray[indexz].value);

            //     // printf("free(htItemsArray[%d].value) - %s\n\n", indexz, htItemsArray[indexz].value);
            //     // free(htItemsArray[indexz].key);
            //     // free(htItemsArray[indexz].value);
            // }

            pHTable->htCount = pHTable->htCount + 1;

            // printf("size in insert: %llu\n", calc_items2_length(htItemsArray[index].key));
            return (&new_items2[size]);
        }
    }
    
    return (&htItemsArray[index]);
}

htItem* getHTItem(hTable* hTable, char* key)
{
    if (hTable->htCount == 0 || hTable->htSize == 0)
    {
        return (NULL);
    }

    ulli_t index = hashFunction(key, hTable->htSize);

    htItem* htItemsArray = hTable->htItemsArray;   // pointer to first struct in array of structs (+ == next struct)

    if (htItemsArray[index].key != NULL)  // entry exists
    {
        if (htItemsArray[index].value != NULL)    // single entry
        {
            return (&htItemsArray[index]);
        }
        else    // collision entries
        {
            htItem* coll_arr = htItemsArray[index].key;
            for (ulli_t coll_arr_index = 0; coll_arr[coll_arr_index].key != NULL; coll_arr_index++)
            {
                // ? Compare given key with hash table collision array
                if (strcmp(coll_arr[coll_arr_index].key, key) == 0)
                {
                    return (&coll_arr[coll_arr_index]);
                }
            }
        }
    }

    // ? Key not found in hash table
    return (NULL);
}