#include <stdio.h>

#define OHT_CAPACITY (1024)
#define OHT_IMPLEMENTATION
#include "own_hash_table.h"

int main(void)
{
  OHT ht;

  oht_init(&ht);

  for (OHTIndex i = 0; i < OHT_CAPACITY; ++i)
  {
    oht_set(&ht, (OHTKey)i, (OHTValue)(i * 2));
  }

  // oht_dump(&ht);

  // for (OwnHashTableIndex i = 0; i < 10; ++i)
  // {
  //   OwnHashTableKey key = (OwnHashTableKey)((OwnHashTableIndex)(rand() % OWN_HASH_TABLE_CAPACITY));
  //   OwnHashTableValue value = own_ht_get(&ht, key);

  //   printf("Value for key %llu: %llu\n", (OwnHashTableIndex)key, (OwnHashTableIndex)value);
  // }

  // OwnHashTableValue value = own_ht_get(&ht, (OwnHashTableKey)1006);

  // printf("Value for key 1006 (before rem): %llu\n", (OwnHashTableIndex)value);

  // printf("Size of hash table (before rem): %llu\n", own_ht_size(&ht));

  // own_ht_rem(&ht, (OwnHashTableKey)1006);

  // value = own_ht_get(&ht, (OwnHashTableKey)1006);

  // printf("Value for key 1006 (after rem): %llu\n", (OwnHashTableIndex)value);

  // printf("Size of hash table (after rem): %llu\n", own_ht_size(&ht));

  return 0;
}
