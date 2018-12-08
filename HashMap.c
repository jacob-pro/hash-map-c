//
//  HashMap.c
//  coms10008_week10
//  Created by Jacob Halsey on 07/12/2018.
//
#include "HashMap.h"
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	void *key;
	hash_t hash;
	void *value;
	struct Node *next;
} Node;

struct HashMap {
	hash_t size;		// Number of entries
	hash_t capacity;	// Must be power of 2 (the length of table)
	hash_t resize_at;
	float load_factor;
	hash_func hash_func;
	is_equal_func is_equal_func;
	Node **table;
};

// Get the nearest power of two
// https://www.techiedelight.com/round-next-highest-power-2/
hash_t power_of_two(hash_t n) {
	n--;
	while (n & n - 1) {
		n = n & n - 1;
	}
	return n << 1;
}

HashMap *hashmap_init_with_options(hash_t initial_size, float load_factor, hash_func hash, is_equal_func equal) {
	HashMap *h = malloc(sizeof(HashMap));
	h->size = 0;
	h->capacity = (initial_size > 0) ? power_of_two(initial_size) : power_of_two(HM_DEFAULT_SIZE);
	h->load_factor = (load_factor > 0 && load_factor < 1) ? load_factor : HM_DEFAULT_FACTOR;
	h->resize_at = h->capacity * h->load_factor;
	h->is_equal_func = equal;
	h->hash_func = hash;
	h->table = malloc(sizeof(Node*) * h->capacity);
	for (hash_t i = 0; i < h->capacity; i++) h->table[i] = NULL;
	return h;
}

HashMap *hashmap_init_for_pointer_key(hash_t initial_size, float load_factor) {
	return hashmap_init_with_options(initial_size, load_factor, pointer_hash, pointer_is_equal);
}

HashMap *hashmap_init_for_string_key(hash_t initial_size, float load_factor) {
	return hashmap_init_with_options(initial_size, load_factor, java_string_hash, string_is_equal);
}

#define INDEX_FROM_HASH(hash, capacity) \
	(hash & (capacity - 1))		// the capactiy is always a power of 2 hence why this can be done

void *hashmap_get_value(HashMap *h, void *key) {
	if (key == NULL) return NULL;

	hash_t hash = h->hash_func(key);
	hash_t idx = INDEX_FROM_HASH(hash, h->capacity);

	Node *node = h->table[idx];
	while (node != NULL) {
		if (h->is_equal_func(node->key, key)) return node->value;
		node = node->next;
	}

	return NULL;	// Not found
}

void expand_table(HashMap *h) {
	// this should really check for overflow 
	// but that will not occur until 2^31 which is already too large to be practical

	hash_t oldSize = h->capacity;
	hash_t newSize = oldSize << 1;	//Doubles the size
	
	Node **oldTable = h->table;
	Node **newTable = malloc(sizeof(Node*) * newSize);
	
	for (hash_t i = 0; i < newSize; i++) newTable[i] = NULL;

	for (hash_t i = 0; i < oldSize; i++) {		//iterate the old table
		Node *node = oldTable[i];
		while (node != NULL) {	//iterate entries in each old bucket

			Node *originalNext = node->next;		//this value may get changed when it is moved

			hash_t newIdx = INDEX_FROM_HASH(node->hash, newSize);
			Node *newBucket = newTable[newIdx];	//we may already have put something in the new bucket
			newTable[newIdx] = node;	//replace this node at the bucket
			node->next = newBucket;		//then set the next of this node to whatever was/not already there

			node = originalNext;
		}
	}

	h->table = newTable;
	h->capacity = newSize;
	h->resize_at = newSize * h->load_factor;
	free(oldTable);
}

void hashmap_set_value(HashMap *h, void *key, void *value) {
	if (key == NULL) return;

	hash_t hash = h->hash_func(key);
	hash_t idx = INDEX_FROM_HASH(hash, h->capacity);

	//If this key is already in the table
	Node *previous = NULL;
	Node *eval = h->table[idx];
	while (eval != NULL) {
		if (h->is_equal_func(eval->key, key)) {
			
			if (value == NULL) {		//remove the eval node
				
				if (previous != NULL) {
					previous->next = eval->next;
				} else {
					h->table[idx] = eval->next;
				}
				free(eval);
				h->size--;
				

			} else {			//replace the eval value
				eval->value = value;
			}

			return;
		}
		previous = eval;
		eval = eval->next;
	}

	// See if table should be resized
	if (h->size >= h->resize_at) {
		expand_table(h);
		idx = INDEX_FROM_HASH(hash, h->capacity);		//the capacity will have changed
	}

	// Create the node
	Node *new = malloc(sizeof(Node));
	new->value = value;
	new->key = key;
	new->hash = hash;
	new->next = NULL;
	if (previous != NULL) {
		previous->next = new;
	} else {
		h->table[idx] = new;
	}
	h->size++;
}

void hashmap_destroy(HashMap *h) {
	for (hash_t i = 0; i < h->capacity; i++) {
		Node *n = h->table[i];
		while (n != NULL) {
			Node *toFree = n;
			n = n->next;
			free(toFree);
		}
	}
	free(h->table);	 // this was the first memory leak in 10 weeks :(
	free(h);
}

hash_t hashmap_get_size(HashMap *h) {
	return h->size;
}

hash_t hashmap_get_capacity(HashMap *h) {
	return h->capacity;
}

bool pointer_is_equal(const void *key1, const void *key2) {
	return (key1 == key2);
}

bool string_is_equal(const void *key1, const void *key2) {
	return (strcmp(key1, key2) == 0);
}

uint32_t pointer_hash(const void *key) {
	return (uint32_t)key;		//Hashing a pointer is pointless?
}

uint32_t java_string_hash(const void *key) {
	const char *str = key;
	uint32_t h = 0;
	for (size_t i = 0; i < strlen(str); i++) {
		h = 31 * h + str[i];
	}
	return h;
}
