#ifndef OHT_HEADER_
#define OHT_HEADER_

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef OHT_KEY_TYPE
#define OHT_KEY_TYPE void*
#endif

#ifndef OHT_KEY_DEFAULT
#define OHT_KEY_DEFAULT (void*)0
#endif

#ifndef OHT_KEY_PRINTF_FORMAT
#define OHT_KEY_PRINTF_FORMAT "%llu"
#endif

#ifndef OHT_KEY_PRINTF_CAST
#define OHT_KEY_PRINTF_CAST uint64_t
#endif

#ifndef OHT_VALUE_TYPE
#define OHT_VALUE_TYPE uint64_t
#endif

#ifndef OHT_VALUE_DEFAULT
#define OHT_VALUE_DEFAULT 0
#endif

#ifndef OHT_VALUE_PRINTF_FORMAT
#define OHT_VALUE_PRINTF_FORMAT "%llu"
#endif

#ifndef OHT_VALUE_PRINTF_CAST
#define OHT_VALUE_PRINTF_CAST uint64_t
#endif

#ifndef OHT_INDEX_TYPE
#define OHT_INDEX_TYPE uint64_t
#endif

#ifndef OHT_INDEX_PRINTF_FORMAT
#define OHT_INDEX_PRINTF_FORMAT "%llu"
#endif

#ifndef OHT_INDEX_PRINTF_CAST
#define OHT_INDEX_PRINTF_CAST uint64_t
#endif

typedef OHT_KEY_TYPE   OHTKey;
typedef OHT_VALUE_TYPE OHTValue;
typedef OHT_INDEX_TYPE OHTIndex;

#ifndef OHT_CAPACITY
#define OHT_CAPACITY (OHTIndex)4096
#endif

typedef struct
{
  OHTKey   key;
  OHTValue value;
  bool     occupied;
} OHTEntry;

typedef struct
{
  OHTEntry items[OHT_CAPACITY];
  OHTIndex size;
} OHT;

void oht_init(OHT* ht);
void oht_dump(const OHT* ht);
OHTIndex oht_find_index(const OHT* ht, OHTKey key);
OHTValue oht_get(const OHT* ht, OHTKey key);
OHTIndex oht_set(OHT* ht, OHTKey key, OHTValue value);
OHTIndex oht_rem(OHT* ht, OHTKey key);
OHTIndex oht_size(const OHT* ht);

#ifdef OHT_IMPLEMENTATION
void oht_init(OHT* ht)
{
  for (OHTIndex i = 0; i < OHT_CAPACITY; ++i)
  {
    ht->items[i] = (OHTEntry) {
      .key = OHT_KEY_DEFAULT,
      .value = OHT_VALUE_DEFAULT,
      .occupied = false
    };
  }
}

void oht_dump(const OHT* ht)
{
  printf("Hash table size: %llu\n", ht->size);

  for (OHTIndex i = 0; i < OHT_CAPACITY; ++i)
  {
    const OHTEntry* entry = &(ht->items[i]);

    printf(
      "Index "OHT_INDEX_PRINTF_FORMAT": { key = "OHT_KEY_PRINTF_FORMAT", value = "OHT_VALUE_PRINTF_FORMAT", occupied = %s }\n",
      (OHT_INDEX_PRINTF_CAST)i,
      (OHT_KEY_PRINTF_CAST)entry->key,
      (OHT_VALUE_PRINTF_CAST)entry->value,
      (entry->occupied ? "true" : "false")
    );
  }
}

OHTIndex oht_find_index(const OHT* ht, OHTKey key)
{
  srand((OHTIndex)key);

  OHTIndex index = rand() % OHT_CAPACITY;

  uint64_t tries = 0;

  while (ht->items[index].occupied && ht->items[index].key != key)
  {
    index = (index + 1) % OHT_CAPACITY;

    ++tries;

    assert(tries < OHT_CAPACITY);
  }

  return index;
}

OHTValue oht_get(const OHT* ht, OHTKey key)
{
  OHTIndex index = oht_find_index(ht, key);

  return ht->items[index].value;
}

OHTIndex oht_set(OHT* ht, OHTKey key, OHTValue value)
{
  OHTIndex index = oht_find_index(ht, key);

  if (!ht->items[index].occupied)
  {
    ht->items[index] = (OHTEntry) {
      .key = key,
      .occupied = true
    };

    ++(ht->size);
  }

  ht->items[index].value = value;

  return index;
}

OHTIndex oht_rem(OHT* ht, OHTKey key)
{
  OHTIndex index = oht_find_index(ht, key);

  if (ht->items[index].occupied) {
    ht->items[index] = (OHTEntry) {
      .key = OHT_KEY_DEFAULT,
      .value = OHT_VALUE_DEFAULT,
      .occupied = false
    };

    --(ht->size);
  }

  return index;
}

OHTIndex oht_size(const OHT* ht)
{
  return ht->size;
}
#endif
#endif
