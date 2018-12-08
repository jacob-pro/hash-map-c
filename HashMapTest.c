//
//  HashMapTest.c
//  coms10008_week10
//  Created by Jacob Halsey on 08/12/2018.
//
#include "HashMap.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

//These are two strings known to collide in the Java hashcode
static char *collide1 = "Siblings";
static char *collide2 = "Teheran";

void test_string_keys() {
	char *value = "Expected Result";
	HashMap *h = hashmap_init_for_string_key(HM_DEFAULT_SIZE, HM_DEFAULT_FACTOR);

	//Two identical strings
	//C doesn't do string interning so these literals should stored seperately
	char *key1 = "key";
	char *key2 = "key";
	assert(key1 != key2);
	assert(strcmp(key1, key2) == 0);

	//Set the value using key1
	hashmap_set_value(h, key1, value);

	//Both keys are the same so they should both return the same value
	char *test1 = hashmap_get_value(h, key1);
	char *test2 = hashmap_get_value(h, key2);
	assert(test1 == value);
	assert(test2 == value);

	hashmap_destroy(h);
}

void test_deletion() {
	char *value = "value";
	char *key = "key";
	
	HashMap *h = hashmap_init_for_string_key(HM_DEFAULT_SIZE, HM_DEFAULT_FACTOR);
	assert(hashmap_get_size(h) == 0);

	//Set the value
	hashmap_set_value(h, key, value);
	assert(hashmap_get_size(h) == 1);
	char *result = hashmap_get_value(h, key);
	assert(result == value);

	//Delete the value
	hashmap_set_value(h, key, NULL);
	assert(hashmap_get_size(h) == 0);
	char *deleted = hashmap_get_value(h, key);
	assert(deleted == NULL);

	hashmap_destroy(h);
}

void test_collision() {
	char *val1 = "val1";
	char *val2 = "val2";
	char *result;
	HashMap *h = hashmap_init_for_string_key(HM_DEFAULT_SIZE, HM_DEFAULT_FACTOR);

	//Confirm that the values do indeed collide
	hash_t one = java_string_hash(collide1);
	hash_t two = java_string_hash(collide2);
	assert(one == two);

	//Test that both values can be sucessfully stored and retrieved
	hashmap_set_value(h, collide1, val1);
	hashmap_set_value(h, collide2, val2);

	result = hashmap_get_value(h, collide1);
	assert(result == val1);

	result = hashmap_get_value(h, collide2);
	assert(result == val2);

	//Try removing the first value
	hashmap_set_value(h, collide1, NULL);

	result = hashmap_get_value(h, collide1);
	assert(result == NULL);

	result = hashmap_get_value(h, collide2);
	assert(result == val2);

	hashmap_destroy(h);
}

void test_growth() {

	// Create a table with initial size 4. 
	// The load factor of 0.5 means it will need to grow to size 8 on the 3rd entry
	// And then again to size 16 on the 5th entry
	HashMap *h = hashmap_init_for_string_key(4, 0.5);
	assert(hashmap_get_capacity(h) == 4);

	//Use the collision values at the start to check that they do not get broken after the expansion
	char *keys[8] = { collide1, collide2, "three", "four", "five", "six", "seven", "eight" };
	char *values[8] = { "a", "b", "c", "d", "e", "f", "g", "h" };

	for (size_t i = 0; i < 8; i++) {
		hashmap_set_value(h, keys[i], values[i]);
		if (i == 2) assert(hashmap_get_capacity(h) == 8);
		if (i == 4) assert(hashmap_get_capacity(h) == 16);
	}

	for (size_t i = 0; i < 8; i++) {
		char *result = hashmap_get_value(h, keys[i]);
		assert(result == values[i]);
	}

	hashmap_destroy(h);
}

int main() {

	test_string_keys();
	test_deletion();
	test_collision();
	test_growth();

	return EXIT_SUCCESS;
}
