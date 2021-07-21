# Hash Map

[![Build status](https://github.com/jacob-pro/hash-map-c/actions/workflows/cmake.yml/badge.svg)](https://github.com/jacob-pro/hash-map-c/actions)

Bristol COMS10008_2018 Imperative Programming (Yr 1), Coursework: Week 10, Hash

## Usage

This hash map / table / dictionary implementation allows for the use of keys of any type of data.

The key is supplied to the HashMap as a pointer, and you can provide your own `HashFunc` and `IsEqualFunc` to work with the type of key.

If the equality function is true for pointer keys X and Y, then X and Y should also have the same hash.

Two initialisers functions with associated hash/equal functions have been provided for string and pointer keys.

Lack of generics in C means that the get/set functions use `(void*)` for the keys and values, and will need to be casted appropriately.

To remove a key-value pair from the table use `hashmap_set_value` to set the value for that key to `NULL`.

If a key does not exist in the table then then `hashmap_get_value` will return `NULL`.

## Implentation details

`hash_t` has been defined as a 32bit unsigned integer. And therefore the hash functions need to return a 32bit result.

The table length is always a power of 2. And will double when the load factor has been exceeded.

The initial size does not need to be a power of 2, if not it will just round up to the nearest.

There are currently no checks that the memory allocation has been successful or that the table length has not overflowed.
