// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 50;

// Number of words in hash table
unsigned int wordCount = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word to obtain a hash value
    unsigned int wordHash = hash(word);

    // Access linked list at that index of the in the hash table
    node *cursor = table[wordHash];

    // Traverse linked list, looking for the word (using strcasecmp)
    while (cursor != NULL)
    {
        // Check if the word matches
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        // Move cursor to next node
        cursor = cursor->next;
    }

    // Word not found in dictionary so return false
    return false;
}

// Hashes word to a number
// djb2 hash function from http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + tolower(c);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // wordBuffer is a buffer variable in which to store words read from file
    char wordBuffer[LENGTH + 1];

    // Scan dict for strings that are not the end of the file
    while (fscanf(dict, "%s", wordBuffer) != EOF)
    {
        // Create a new node for each word
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            return false;
        }

        // Copy word into node using strcopy
        strcpy(new_node->word, wordBuffer);
        new_node->next = NULL;

        // Hash word to obtain a hash value
        unsigned int wordHash = hash(wordBuffer);

        // Insert node into hash table at wordHash location
        new_node->next = table[wordHash];
        table[wordHash] = new_node;

        // Increase word count
        wordCount++;
    }

    // Close dictionary
    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (wordCount == 0)
    {
        unload();
        return 0;
    }
    else
    {
        return wordCount;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through buckets
    for (int i = 0; i < N; i++)
    {
        // Set cursor to this each bucket location
        node *cursor = table[i];

        // If cursor is not NULL, then free
        while (cursor != NULL)
        {
            // Create temp
            node *tmp = cursor;
            // Move cursor to next node
            cursor = cursor->next;
            // Free up temp
            free(tmp);
        }

        // If cursor is NULL on last iteration, then return false
        if (i == N - 1 && cursor == NULL)
        {
            return true;
        }
    }

    // If above check fails, then return false
    return false;
}