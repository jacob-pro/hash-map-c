# coms10008_week10

## Usage

This hash map / table / dictionary implementation allows for the use of keys of any type of data.
The key is supplied to the HashMap as a pointer, and you can provide your own `hash_func` and `is_equal_func` to work with the type of key.
If the equality function is true for pointer keys X and Y, then X and Y should also have the same hash.
Two initialisers functions with associated hash/equal functions have been provided for string and pointer keys.
Sadly the lack of generics in C means that the get/set functions just use `(void*)` for the keys and values, and will need to be casted appropriately.
To remove a key-value pair from the table use `hashmap_set_value` to set the value for that key to `NULL`.
If a key does not exist in the table then then `hashmap_get_value` will return `NULL`.

## Implentation details

`hash_t` has been defined as a 32bit unsigned integer. And therefore the hash functions need to return a 32bit result.
The table length is always a power of 2. And will double when the load factor has been exceeded.
The initial size does not need to be a power of 2, if not it will just round up to the nearest.
There are currently no checks that the memory allocation has been successful or that the table length has not overflowed.
However if the table grows to that sort of size then you are probably not using it correctly.
