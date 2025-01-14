#include "hashMap.h"
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Allocates a string for the next word in the file and returns it. This string
 * is null terminated. Returns NULL after reaching the end of the file.
 * @param file
 * @return Allocated string or NULL.
 */
char* nextWord(FILE* file)
{
    int maxLength = 16;
    int length = 0;
    char* word = malloc(sizeof(char) * maxLength);
    while (1)
    {
        char c = fgetc(file);
        if ((c >= '0' && c <= '9') ||
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            c == '\'')
        {
            if (length + 1 >= maxLength)
            {
                maxLength *= 2;
                word = realloc(word, maxLength);
            }
            word[length] = c;
            length++;
        }
        else if (length > 0 || c == EOF)
        {
            break;
        }
    }
    if (length == 0)
    {
        free(word);
        return NULL;
    }
    word[length] = '\0';
    return word;
}

/**
 * Loads the contents of the file into the hash map.
 * @param file
 * @param map
 */
void loadDictionary(FILE* file, struct HashMap* map)
{
    char* word = nextWord(file);
    while(word != NULL){
        int idx = abs(HASH_FUNCTION(word)) % map->capacity;
        for(int i = 0; i < map->capacity; i++){
            if(i == idx){
                hashMapPut(map, word, idx);
            }
        }
        word = nextWord(file);
    }
    // FIXME: implement
}

int minMoves(int a, int b, int c){
    if(a < b && a < c){
        return a;
    }
    if(b < a && b < c){
        return b;
    }
    return c;
}

struct HashLink* leDist(const char* input, const char* key){   //returns the pointer to the link that contains the matching word?
    int inLen, keyLen;
    inLen = strlen(input);
    keyLen = strlen(key);

    int mtrx[inLen + 1][keyLen + 1];
    for(int i = 0; i <= inLen; i ++){   //rows
        mtrx[i][0] = i;
    }
    for(int i = 0; i <= keyLen; i ++){  //columns
        mtrx[0][i] = i;
    }
    for(int i = 1; i <= inLen; i++){
        char c1;
        c1 = input[i - 1];
        for(int j = 1; j <= keyLen; j++){
            char c2;
            c2 = key[j - 1];
            if(c1 == c2){
                mtrx[i][j] = mtrx[i - 1][j - 1];
            }
            else{
                int del = mtrx[i - 1][j] + 1;
                int ins = mtrx[i][j - 1] + 1;
                int sub = mtrx[i - 1][j - 1] + 1;
                mtrx[i][j] = minMoves(del, ins, sub);
            }
        }
    }

}


/**
 * Prints the concordance of the given file and performance information. Uses
 * the file input1.txt by default or a file name specified as a command line
 * argument.
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, const char** argv)
{
    // FIXME: implement
    struct HashMap* map = hashMapNew(1000);
    struct HashLink* cur1;
    struct HashLink* cur2;
    int moves, notWord;


    FILE* file = fopen("dictionary.txt", "r");
    clock_t timer = clock();
    loadDictionary(file, map);
    timer = clock() - timer;
    printf("Dictionary loaded in %f seconds\n", (float)timer / (float)CLOCKS_PER_SEC);
    fclose(file);

    char inputBuffer[256];
    int quit = 0;
    while (!quit)
    {
        printf("Enter a word or \"quit\" to quit: ");
        scanf("%s", inputBuffer);
        char* tmp[strlen(inputBuffer)];
        strcpy(tmp, inputBuffer);
        int i = 0;
        for(i = 0; i < strlen(inputBuffer); i++){                                 //puts user inputted word to lower case to aid comparisons
            if(!isalpha(tmp[i])){
                notWord = 1;
            }
            else{
                tmp[i] = tolower(tmp[i]);
                notWord = 0;
            }
        }

        if(notWord == 0){

            int matching;
            for(int i = 0; i < map->capacity; i++){     //iterates through all buckets(sentinels)
                cur1 = map->table[i]->next;
                while(cur1 != NULL){
                    moves = leDist(notWord, cur1->key);  //not sure how to catch the return...maybe a hash map link
                    if(moves == 0){
                        matching = 1;
                        printf("The word \"%s\" is spelled correctly. \n", inputBuffer);
                    }
                    else{
                        matching = 0;
                        cur1->value = moves;
                    }
                    cur1 = cur1->next;
                }
            }

            if(matching == 0){
                int count = 0;
                struct HashLink* minArr[5];        //creates hashlink array to hold the locations of the five closest words
                for(int j = 0; j < map->capacity; j++){
                    cur2 = map->table[j]->next;
                    while(cur2 != NULL){
                        if(count < 5){      //fills the array with the first five words
                            minArr[count] = cur2;
                            count++;
                        }
                        else{
                            for(int k = 0; k < 5; k++){     //compares every word distance with distances in the array
                                if(cur2->value < minArr[k]){
                                    minArr[k] = cur2;       //once a new distance is placed in an array location, jump out of array comparison loop
                                    break;
                                }
                            }
                        }
                    }
                }
                printf("The inputted word:%s, is spelled incorrectly, did you mean; %s, %s, %s, %s, %s?\n", inputBuffer, minArr[0], minArr[1], minArr[2], minArr[3], minArr[4]);
            }
        }

        // Implement the spell checker code here..

        if (strcmp(inputBuffer, "quit") == 0)
        {
            quit = 1;
        }
    }

    hashMapDelete(map);
    return 0;
}
