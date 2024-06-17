#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10
#define DELETED_NODE (struct HashNode*)(0xFFFFFFFFFFFFFFFFUL)

struct HashNode {
    int key;
    int value;
};

struct HashNode* hashTable[TABLE_SIZE];

unsigned int hashFunction(int key) {
    return key % TABLE_SIZE;
}

void initHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

void insert(int key, int value) {
    struct HashNode* temp = (struct HashNode*)malloc(sizeof(struct HashNode));
    temp->key = key;
    temp->value = value;

    int hashIndex = hashFunction(key);

    while (hashTable[hashIndex] != NULL && hashTable[hashIndex] != DELETED_NODE && hashTable[hashIndex]->key != key) {
        hashIndex++;
        hashIndex %= TABLE_SIZE;
    }

    hashTable[hashIndex] = temp;
}

struct HashNode* search(int key) {
    int hashIndex = hashFunction(key);

    while (hashTable[hashIndex] != NULL) {
        if (hashTable[hashIndex]->key == key) {
            return hashTable[hashIndex];
        }
        hashIndex++;
        hashIndex %= TABLE_SIZE;
    }
    return NULL;
}

void delete(int key) {
    int hashIndex = hashFunction(key);

    while (hashTable[hashIndex] != NULL) {
        if (hashTable[hashIndex]->key == key) {
            struct HashNode* temp = hashTable[hashIndex];
            hashTable[hashIndex] = DELETED_NODE;
            free(temp);
            return;
        }
        hashIndex++;
        hashIndex %= TABLE_SIZE;
    }
}

void display() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] != NULL && hashTable[i] != DELETED_NODE) {
            printf("Key: %d, Value: %d\n", hashTable[i]->key, hashTable[i]->value);
        } else {
            printf("~\n");
        }
    }
}

int main() {
    initHashTable();

    insert(1, 10);
    insert(2, 20);
    insert(12, 120);

    display();

    struct HashNode* node = search(12);
    if (node != NULL) {
        printf("Found key 12 with value: %d\n", node->value);
    } else {
        printf("Key 12 not found\n");
    }

    delete(12);
    node = search(12);
    if (node != NULL) {
        printf("Found key 12 with value: %d\n", node->value);
    } else {
        printf("Key 12 not found\n");
    }

    display();

    return 0;
}
