#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SIZE 10
#define LOAD_FACTOR 0.7

typedef struct HashNode {
    int key;
    int value;
    struct HashNode* next;
} HashNode;

typedef struct HashTable {
    int size;
    int count;
    HashNode** nodes;
} HashTable;

unsigned int hash(int key, int size) {
    return key % size;
}

HashNode* create_node(int key, int value) {
    HashNode* new_node = (HashNode*)malloc(sizeof(HashNode));
    new_node->key = key;
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

HashTable* create_table(int size) {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->nodes = (HashNode**)malloc(sizeof(HashNode*) * size);
    for (int i = 0; i < size; i++) {
        table->nodes[i] = NULL;
    }
    return table;
}

void free_node(HashNode* node) {
    if (node != NULL) {
        free_node(node->next);
        free(node);
    }
}

void free_table(HashTable* table) {
    for (int i = 0; i < table->size; i++) {
        free_node(table->nodes[i]);
    }
    free(table->nodes);
    free(table);
}

void insert(HashTable* table, int key, int value);
void rehash(HashTable* table);

void insert(HashTable* table, int key, int value) {
    unsigned int index = hash(key, table->size);
    HashNode* new_node = create_node(key, value);
    if (table->nodes[index] == NULL) {
        table->nodes[index] = new_node;
    } else {
        HashNode* temp = table->nodes[index];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }
    table->count++;

    // Check load factor and rehash if necessary
    if ((float)table->count / table->size > LOAD_FACTOR) {
        rehash(table);
    }
}

void rehash(HashTable* table) {
    int new_size = table->size * 2;
    HashNode** old_nodes = table->nodes;
    int old_size = table->size;

    table->nodes = (HashNode**)malloc(sizeof(HashNode*) * new_size);
    table->size = new_size;
    table->count = 0;

    for (int i = 0; i < new_size; i++) {
        table->nodes[i] = NULL;
    }

    for (int i = 0; i < old_size; i++) {
        HashNode* temp = old_nodes[i];
        while (temp != NULL) {
            insert(table, temp->key, temp->value);
            HashNode* old_node = temp;
            temp = temp->next;
            free(old_node);
        }
    }
    free(old_nodes);
}

int search(HashTable* table, int key) {
    unsigned int index = hash(key, table->size);
    HashNode* temp = table->nodes[index];
    while (temp != NULL) {
        if (temp->key == key) {
            return temp->value;
        }
        temp = temp->next;
    }
    return -1; 
}

void delete(HashTable* table, int key) {
    unsigned int index = hash(key, table->size);
    HashNode* temp = table->nodes[index];
    HashNode* prev = NULL;
    while (temp != NULL) {
        if (temp->key == key) {
            if (prev == NULL) {
                table->nodes[index] = temp->next;
            } else {
                prev->next = temp->next;
            }
            free(temp);
            table->count--;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}

int main() {
    HashTable* table = create_table(INITIAL_SIZE);

    insert(table, 1, 10);
    insert(table, 2, 20);
    insert(table, 3, 30);
    insert(table, 4, 40);
    insert(table, 5, 50);

    printf("Value for key 3: %d\n", search(table, 3));

    delete(table, 3);

    printf("Value for key 3 after deletion: %d\n", search(table, 3));

    free_table(table);

    return 0;
}
