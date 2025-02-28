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

HashTable* create_table();
void insert_entry(HashTable *table, const char *key, const char *value);
void delete_entry(HashTable *table, const char *key);
void delete_table(HashTable *table);

#endif