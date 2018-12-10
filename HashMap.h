//
//  HashMap.h
//  coms10008_week10
//  Created by Jacob Halsey on 07/12/2018.
//
#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

// Currently setup to work with a 32bit hash
typedef uint32_t hash_t;

#define HM_DEFAULT_FACTOR 0.75f
#define HM_DEFAULT_SIZE 32

typedef struct HashMap HashMap;

typedef hash_t(*hash_func) (const void *key);
typedef bool(*is_equal_func) (const void *key1, const void *key2);

HashMap *hashmap_init_with_options(hash_t initial_size, float load_factor, hash_func hash, is_equal_func equal);

HashMap *hashmap_init_for_pointer_key(hash_t initial_size, float load_factor);
HashMap *hashmap_init_for_string_key(hash_t initial_size, float load_factor);

// Get the value for a given key. Returns NULL if the key does not exist
void *hashmap_get_value(HashMap *h, void *key);

// Set the value for a given key. Use NULL to remove the key-value pair
void hashmap_set_value(HashMap *h, void *key, void *value);

// Free the hashmap
void hashmap_destroy(HashMap *h);

// The number of key-value pairs stored in the hashmap
hash_t hashmap_get_size(HashMap *h);

// The current size of the hashtable
hash_t hashmap_get_capacity(HashMap *h);

/*
 * Some default functions for pointer and string keys
 */

bool pointer_is_equal(const void *key1, const void *key2);
bool string_is_equal(const void *key1, const void *key2);

uint32_t pointer_hash(const void *key);
uint32_t java_string_hash(const void *key);

#endif
