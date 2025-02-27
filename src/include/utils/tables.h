#ifndef HASH_TABLES_H
#define HASH_TABLES_H

#define TABLE_SIZE 100

typedef struct {
	char *key;
	char *value;
} Entry;

typedef struct {
	Entry* table[TABLE_SIZE];
} HashTable;

#endif