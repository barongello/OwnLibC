CC=cc
CFLAGS=-Wall -Wextra -std=c99 -pedantic

hashtable: own_hash_table_test.c own_hash_table.h
	$(CC) $(CFLAGS) -o own_hash_table_test own_hash_table_test.c

heap: own_heap_test.c own_heap.h
	$(CC) $(CFLAGS) -o own_heap_test own_heap_test.c
