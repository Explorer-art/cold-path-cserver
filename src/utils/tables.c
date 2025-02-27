#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/tables.h>

unsigned long get_hash_djb2(const char *str) {
	unsigned long hash = 5381;
	int c;

	while ((c = *str++)) {
		hash = ((hash << 5) + hash) + c;
	}

	return hash % TABLE_SIZE;
}

HashTable* create_table() {
	HashTable *table = (HashTable*) malloc(sizeof(HashTable));

	if (table == NULL) {
		perror("[ERROR] Allocate memory for new table failed\n");
		return NULL;
	}

	for (int i = 0; i < TABLE_SIZE; i++) {
		table->table[i] = NULL;
	}

	return table;
}

void insert_entry (HashTable* table, const char *key, const char *value) {
	unsigned long hash = get_hash_djb2(key);

	if (table->table[hash] != NULL) {
		free(table->table[hash]->key);
		free(table->table[hash]->value);
		free(table->table[hash]);
	}

	Entry* new_entry = (Entry*) malloc(sizeof(Entry));

	if (new_entry == NULL) {
		perror("[ERROR] Allocate memory for new entry failed\n");
		return;
	}

	new_entry->key = strdup(key);
	new_entry->value = strdup(value);
	table->table[hash] = new_entry;
}

Entry* search(HashTable *table, const char *key) {
	unsigned long hash = get_hash_djb2(key);
	return table->table[hash];
}

void delete_entry(HashTable *table, const char *key) {
	unsigned long hash = get_hash_djb2(key);

	if (table->table[hash] != NULL) {
		free(table->table[hash]->key);
		free(table->table[hash]->value);
		free(table->table[hash]);
	}
}

void delete_table(HashTable *table) {
	for (int i = 0; i < TABLE_SIZE; i++) {
		if (table->table[i] != NULL) {
			free(table->table[i]->key);
			free(table->table[i]->value);
			free(table->table[i]);
		}
	}

	free(table);
}
