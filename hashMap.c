/*
 * CS 261 Data Structures
 * Assignment 5
 * Name: Cooper Smith, Anthony Minniti, Gabe Schafman
 * Date: 05/08/19
 */

#include "hashMap.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

int hashFunction1(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += key[i];
    }
    return r;
}

int hashFunction2(const char* key)
{
    int r = 0;
    for (int i = 0; key[i] != '\0'; i++)
    {
        r += (i + 1) * key[i];
    }
    return r;
}

/**
 * Creates a new hash table link with a copy of the key string.
 * @param key Key string to copy in the link.
 * @param value Value to set in the link.
 * @param next Pointer to set as the link's next.
 * @return Hash table link allocated on the heap.
 */
HashLink* hashLinkNew(const char* key, int value, struct HashLink* next)
{
    struct HashLink* link = malloc(sizeof(struct HashLink));   // Dynamic memory allocation for new link.
    link->key = malloc(sizeof(char) * (strlen(key) + 1)); //Dynamic memory allocation for the key.
    strcpy(link->key, key); //Copy the key into the dynamic slot.
    link->value = value; // Add the value to the link.
    link->next = next; // Put the next link provided pointer into new link.
    return link; //Return the link to call function.
}

/**
 * Free the allocated memory for a hash table link created with hashLinkNew.
 * @param link
 */
static void hashLinkDelete(struct HashLink* link)
{
    free(link->key); //Free dynamic allocation key variable
    free(link); //Free dynamic allocation link variable
}

/**
 * Initializes a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param map
 * @param capacity The number of table buckets.
 */
void hashMapInit(struct HashMap* map, int capacity)
{
    map->capacity = capacity;  //Set this (provided) hashmap to desired capacity.
    map->size = 0;  //Set the size to 0.
    map->table = malloc(sizeof(struct HashLink) * capacity); //Dynamically allocate space for sentinels or links IAW capacity.
    for (int i = 0; i < capacity; i++)  //Set all table pointers to NULL.
    {
        map->table[i] = NULL;
    }
}

/**
 * Removes all links in the map and frees all allocated memory. You can use
 * hashLinkDelete to free the links.
 * @param map
 */
void hashMapCleanUp(struct HashMap* map)
{
    struct HashLink* cur;
    struct HashLink* temp; //Create link pointers current and temporary.
    for(int i = 0; i < map->capacity; i++){ //For all map slots 0 -> capacity
        cur = map->table[i]; //Skip the sentinel and go into bucket.
        while(cur != NULL){  //While the current link is not NULL>
            temp = cur->next;  //See where the next link is.
            hashLinkDelete(cur); //Free the memory from this link.
            cur = temp;  //Assign the next link as current for iteration.
        }
    }
    // FIXME: implement  CHECKED BY: TONY
}

/**
 * Creates a hash table map, allocating memory for a link pointer table with
 * the given number of buckets.
 * @param capacity The number of buckets.
 * @return The allocated map.
 */
HashMap* hashMapNew(int capacity)
{
    struct HashMap* map = malloc(sizeof(struct HashMap)); //Allocate memory for the hashMap.
    hashMapInit(map, capacity); //Initialize hashMap, which also builds it. NO SENTINELS!
    return map;  //Return map to call function.
}

/**
 * Removes all links in the map and frees all allocated memory, including the
 * map itself.
 * @param map
 */
void hashMapDelete(struct HashMap* map)
{
    hashMapCleanUp(map); //Clean up all the map, except the pointers directly in the table.
    free(map);  //Free the memory in the
}

/**
 * Returns a pointer to the value of the link with the given key  and skip traversing as well. Returns NULL
 * if no link with that key is in the table.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return Link value or NULL if no matching link.
 */
int* hashMapGet(struct HashMap* map, const char* key)
{
    int* temp;  //Create a temporary integer variable.
    struct HashLink* ptr;  //Create a HashLink pointer.

    for(int i = 0; i < map->capacity; i++){                                     //iterates through every bucket
        ptr = map->table[i];                                              //starts iterating at the first link after the bucket sentinel.
        while(ptr != NULL){                                                     //if not at the end of each bucket linked list
            if(strcmp(key, ptr->key) != 0){                      //converts both keys to ints and compares the values, if they're not equal moves pointer to next link
                ptr = ptr->next;
            }
            else if(strcmp(key, ptr->key) == 0){                                                              //if the key values are equal, copies link value to temp pointer  //THIS COULD LEAD TO AN ERROR. The else if () is a safer bet.
                temp = &(ptr->value);                                               //Assign the temp int as the value found.
                return temp;                                                        //returns temp pointer to value in desired link
            }
        }
    }
    // FIXME: implement
    return NULL;                //if key doesn't match any in table, returns NULL
}

/**
 * Resizes the hash table to have a number of buckets equal to the given
 * capacity (double of the old capacity). After allocating the new table,
 * all of the links need to rehashed into it because the capacity has changed.
 *
 * Remember to free the old table and any old links if you use hashMapPut to
 * rehash them.
 *
 * @param map
 * @param capacity The new number of buckets.
 */
void resizeTable(struct HashMap* map, int capacity)
{
    /*
    struct HashMap* newMap = hashMapNew(capacity);             //create new map with new capacity
    struct HashLink* ptr;                                      //Create hashLink pointer for transfer of data.
    for(int i = 0; i < map->capacity; i++){             //iterates through old map buckets
        ptr = map->table[i];                      //Skip past sentinels and enter buckets.
        while(ptr != NULL){                             //iterates through the linked list in each bucket
            hashMapPut(newMap, ptr->key, ptr->value);   //puts every link node in the new map
        }
    }
    struct HashMap* temp = map;                                //Track memory address of old map with temporary variable
    map = newMap;                                       //Assign the resized Map to the map pointer.
    hashMapDelete(temp);                                 //delete old map.
    // FIXME: implement
    */
    struct HashLink **oldTable = map->table;
    int oldTableSize = hashMapCapacity(map);
    int i;
    HashLink *curLink;
    HashLink *prevLink;
    hashMapInit(map, capacity);

    for(i = 0; i < oldTableSize; i++) 
	{
            curLink = oldTable[i];
            while(curLink != 0) 
			{
                hashMapPut(map, curLink->key, curLink->value);
                prevLink = curLink;
                curLink = curLink->next;
                free(prevLink);
            }
    }

    free(oldTable);
}

/**
 * Updates the given key-value pair in the hash table. If a link with the given
 * key already exists, this will just update the value and skip traversing. Otherwise, it will
 * create a new link with the given key and value and add it to the table
 * bucket's linked list. You can use hashLinkNew to create the link.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket.
 *
 * @param map
 * @param key
 * @param value
 */
void hashMapPut(struct HashMap* map, const char* key, int value)    //THIS FUNCTION MAY NEED A REDISIGN.
{
    struct HashLink* ptr;                                                              //Create Hashlink pointer for iteration.
    int idx = (HASH_FUNCTION(key) % (map->capacity));                      //Determine the starting index of the addition.
    if(idx < 0){
        idx += map->capacity;
    }
    ptr = map->table[idx];                                                      //Assign pointer to the inital value of the index, skipping sentinel.
    while(ptr != NULL){                                                         //Causes iteration through bucket.
        if(strcmp(key, ptr->key) != 0){                      //Converts both keys to ints and compares the values, execute if not equal.
            ptr = ptr->next;                                                        //Keep iterating through the bucket.
        }
        else if(strcmp(key, ptr->key) == 0){                 //If the key is equal to the key of this entry.
            ptr->value = value;                                                     //Update the key<->value pair.
        }
    }
    if(ptr == NULL){                                                          //If the bucket is empty or your iteration reached the end of a list without doing anything.
        ptr = map->table[idx];                                                        //Assign the address of a NULL POINTER!
        struct HashLink* new = hashLinkNew(key, value, ptr);                    //Create a new link, with the next link NULL.
        map->table[idx] = new;                                                  //Assign the new link as the next value of the bucket.
    }
    float temp = hashMapTableLoad(map);                                         //Set that float equal to the table load.
    if(temp >= MAX_TABLE_LOAD){                                                 //If table load is too high, like the rent,...
        resizeTable(map, 2 * map->capacity);                                        //Double the table size.
    }
    map->size++;
    // FIXME: implement
}

/**
 * Removes and frees the link with the given key from the table. If no such link
 * exists, this does nothing. Remember to search the entire linked list at the
 * bucket. You can use hashLinkDelete to free the link.
 * @param map
 * @param key
 */
void hashMapRemove(struct HashMap* map, const char* key)
{
    struct HashLink* cur;
    struct HashLink* temp;
    struct HashLink* prev;                                                      //Create current, temporary, and previous HashLink pointers.
    int idx = abs((HASH_FUNCTION(key)) % (map->capacity));                      //Determine the starting index of the search.
    cur = map->table[idx];                                                      //Jump into bucket where the key points.
    prev = map->table[idx];                                                     //Track your previous pointer.
    while(cur != NULL){                                                         //While the bucket is not empty. Because if it's empty, we do nothing.
        if(strcmp(key, cur->key) == 0){                          //If the value is found.
            temp = cur;                                                                 //Assign the temp to the current value.
            prev->next = cur->next;                                                     //Assign the previous next, to the current next.
            hashLinkDelete(temp);                                                       //Delete the container that had the value.
        }
        else{
            prev = cur;                                                         //Assign the previous pointer to the current spot.
            cur = prev->next;                                                    //Iterate to the next container.
        }
    }
    // FIXME: implement
}

/**
 * Returns 1 if a link with the given key is in the table and 0 otherwise.
 *
 * Use HASH_FUNCTION(key) and the map's capacity to find the index of the
 * correct linked list bucket. Also make sure to search the entire list.
 *
 * @param map
 * @param key
 * @return 1 if the key is found, 0 otherwise.
 */
int hashMapContainsKey(struct HashMap* map, const char* key)
{
    struct HashLink* ptr;                                                              //Create pointer for link iteration.
    int idx = abs((HASH_FUNCTION(key)) % (map->capacity));                      //Determine the starting index of the search.
    ptr = map->table[idx];                                                      //Start looking for the value in the bucket it would be in.
    while(ptr != NULL){                                                         //While not at the end of each bucket linked list
        if(HASH_FUNCTION(key) != HASH_FUNCTION(ptr->key)){                      //Converts both keys to ints and compares the values, if they're not equal moves pointer to next link
            ptr = ptr->next;
        }
        else {                                                                  //If the key values are equal
            return 1;
        }
    }
    // FIXME: implement
    return 0;                                                                   //Link is not in the table
}

/**
 * Returns the number of links in the table.
 * @param map
 * @return Number of links in the table.
 */
int hashMapSize(struct HashMap* map)
{
    // FIXME: implement
    return (map->size);                                                         //Return map size variable.
}

/**
 * Returns the number of buckets in the table.
 * @param map
 * @return Number of buckets in the table.
 */
int hashMapCapacity(struct HashMap* map)
{
    // FIXME: implement
    return (map->capacity);                                                     //Returns max number of buckets in the table.
}

/**
 * Returns the number of table buckets without any links.
 * @param map
 * @return Number of empty buckets.
 */
int hashMapEmptyBuckets(struct HashMap* map)
{
    int i, j;
    j = 0;
    for(i = 0; i < map->capacity; i++){
        if(map->table[i] == NULL)
            j++;
    }
    // FIXME: implement
    return j;
}

/**
 * Returns the ratio of (number of links) / (number of buckets) in the table.
 * Remember that the buckets are linked lists, so this ratio tells you nothing
 * about the number of empty buckets. Remember also that the load is a floating
 * point number, so don't do integer division.
 * @param map
 * @return Table load.
 */
float hashMapTableLoad(struct HashMap* map)
{
    float load = ((float)map->size) / ((float)map->capacity);
    // FIXME: implement
    return load;
}

/**
 * Prints all the links in each of the buckets in the table.
 * @param map
 */
void hashMapPrint(struct HashMap* map)
{
    struct HashLink* temp;                                                             //Create a temporary pointer for links
    for(int i = 0; i < map->capacity; i++){                                     //Start iterating through buckets
        temp = map->table[i];
        while(temp != NULL){
            printf("%d \t", temp->value);
            temp = temp->next;
        }
        printf("\n");
    }
  // FIXME: implement
}
