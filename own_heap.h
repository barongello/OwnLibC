#ifndef OH_HEADER_
#define OH_HEADER_

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef OH_CAPACITY
#define OH_CAPACITY (1024 * 1024)
#endif

#if OH_CAPACITY < 8 || OH_CAPACITY % 8 != 0
#error "OH_CAPACITY is either less than 8 or not divisible by 8"
#endif

#ifndef OH_DUMP_STEP
#define OH_DUMP_STEP 16
#endif

#if OH_DUMP_STEP < 1 || OH_DUMP_STEP > 32
#error "OH_DUMP_STEP is either less than 1 or greater than 32"
#endif

#ifndef OH_CHUNK_CAPACITY
#define OH_CHUNK_CAPACITY (1024 * 1024)
#endif

typedef struct {
  uint64_t start;
  uint64_t end;
  uint64_t size;
} OHChunkEntry;

typedef struct {
  OHChunkEntry items[OH_CHUNK_CAPACITY];
  uint64_t     size;
} OHChunkList;

char OH_HEAP[OH_CAPACITY] = { 0 };
OHChunkList OH_CHUNKS_FREE = {
  .items = { { .start = 0, .end = (uint64_t)OH_CAPACITY, .size = (uint64_t)OH_CAPACITY } },
  .size = 1
};
OHChunkList OH_CHUNKS_USED = { 0 };

void      oh_cl_dump               (OHChunkList* cl, char* name);
long long oh_cl_find_index_by_start(OHChunkList* cl, uint64_t start);
long long oh_cl_find_index_by_end  (OHChunkList* cl, uint64_t end);
void      oh_cl_add                (OHChunkList* cl, uint64_t start, uint64_t size, bool merge);
void      oh_cl_remove             (OHChunkList* cl, uint64_t start);

void  oh_dump (void);
void* oh_alloc(uint64_t size);
void  oh_free (void* ptr);

#define OH_IMPLEMENTATION
#ifdef OH_IMPLEMENTATION
void oh_cl_dump(OHChunkList* cl, char* name)
{
  printf("%s chunks (%llu)\n", name, cl->size);

  if (cl->size == 0)
  {
    printf("  `- Empty\n");

    return;
  }

  for (uint64_t i = 0; i < cl->size; ++i)
  {
    const OHChunkEntry* entry = &(cl->items[i]);

    const bool last = (i == cl->size - 1);
    const char char_main = (last ? '`' : '|');
    const char char_sub = (last ? ' ' : '|');

    printf("  %c- Chunk %llu (%llu bytes)\n", char_main, i, entry->size);
    printf("  %c    |- Index\n", char_sub);
    printf("  %c    |    |- Start: %llu\n", char_sub, entry->start);
    printf("  %c    |    `-   End: %llu\n", char_sub, entry->end);
    printf("  %c    `- Pointer\n", char_sub);
    printf("  %c         |- Start: %016llX\n", char_sub, (uint64_t)(&OH_HEAP) + entry->start);
    printf("  %c         `-   End: %016llX\n", char_sub, (uint64_t)(&OH_HEAP) + entry->end);
  }

  printf("\n");
}

long long oh_cl_find_index_by_start(OHChunkList* cl, uint64_t start)
{
  for (uint64_t i = 0; i < cl->size; ++i)
  {
    if (cl->items[i].start != start)
    {
      continue;
    }

    return i;
  }

  return -1;
}

long long oh_cl_find_index_by_end(OHChunkList* cl, uint64_t end)
{
  for (uint64_t i = 0; i < cl->size; ++i)
  {
    if (cl->items[i].end != end)
    {
      continue;
    }

    return i;
  }

  return -1;
}

void oh_cl_add(OHChunkList* cl, uint64_t start, uint64_t size, bool merge)
{
  if (merge)
  {
    uint64_t end = start + size;

    long long index_start = oh_cl_find_index_by_start(cl, end);

    if (index_start > -1)
    {
      OHChunkEntry* entry = &(cl->items[index_start]);

      entry->start -= size;
      entry->size += size;

      return;
    }

    long long index_end = oh_cl_find_index_by_end(cl, start);

    if (index_end > -1)
    {
      OHChunkEntry* entry = &(cl->items[index_end]);

      entry->end += size;
      entry->size += size;

      return;
    }
  }

  assert(cl->size < OH_CHUNK_CAPACITY && "The chunk list is full");

  uint64_t index = 0;

  while (index < cl->size && cl->items[index].start < start)
  {
    ++index;
  }

  for (uint64_t i = cl->size; i > index; --i)
  {
    OHChunkEntry* entry_current = &(cl->items[i]);
    OHChunkEntry* entry_previous = &(cl->items[i - 1]);

    entry_current->start = entry_previous->start;
    entry_current->end = entry_previous->end;
    entry_current->size = entry_previous->size;
  }

  OHChunkEntry* entry = &(cl->items[index]);

  entry->start = start;
  entry->end = start + size;
  entry->size = size;

  ++(cl->size);
}

void oh_cl_remove(OHChunkList* cl, uint64_t start)
{
  int index = oh_cl_find_index_by_start(cl, start);

  if (index == -1)
  {
    return;
  }

  for (uint64_t i = index; i < cl->size - 1; ++i)
  {
    OHChunkEntry* entry_current = &(cl->items[i]);
    OHChunkEntry* entry_next = &(cl->items[i + 1]);

    entry_current->start = entry_next->start;
    entry_current->end = entry_next->end;
    entry_current->size = entry_next->size;
  }

  OHChunkEntry* entry_last = &(cl->items[cl->size - 1]);

  entry_last->start = 0;
  entry_last->end = 0;
  entry_last->size = 0;

  --(cl->size);
}

void oh_dump(void)
{
  printf("Heap (%llu bytes)\n", (uint64_t)OH_CAPACITY);

  printf("Address          | ");

  for (uint8_t i = 0; i < OH_DUMP_STEP; ++i)
  {
    printf("%02X ", i);
  }

  printf("|\n");

  printf("-------------------");
  
  for (uint8_t i = 0; i < OH_DUMP_STEP; ++i)
  {
    printf("---");
  }

  printf("--");

  for (uint8_t i = 0; i < OH_DUMP_STEP; ++i)
  {
    printf("-");
  }

  printf("\n");

  const void* ptr = &OH_HEAP;

  for (uint64_t i = 0; i < OH_CAPACITY; i += OH_DUMP_STEP)
  {
    const uint64_t address = (uint64_t)ptr + i;

    printf("%016llX | ", address);

    for (uint64_t j = 0; j < OH_DUMP_STEP; ++j)
    {
      if (i + j >= OH_CAPACITY)
      {
        printf("   ");

        continue;
      }

      const unsigned char byte = *((unsigned char*)(address + j));

      printf("%02X ", byte);
    }

    printf("| ");

    for (uint64_t j = 0; j < OH_DUMP_STEP; ++j)
    {
      if (i + j >= OH_CAPACITY)
      {
        continue;
      }

      const unsigned char byte = *((unsigned char*)(address + j));

      printf("%c", isprint(byte) ? byte : '.');
    }

    printf("\n");
  }
}

void* oh_alloc(uint64_t size)
{
  assert(size > 0 && "The size to alloc is 0");
  assert(OH_CHUNKS_FREE.size > 0 && "There are no free chunks to alloc");

  uint64_t index;

  for (index = 0; index < OH_CHUNKS_FREE.size; ++index)
  {
    if (OH_CHUNKS_FREE.items[index].size < size)
    {
      continue;
    }

    break;
  }

  OHChunkEntry* entry = &(OH_CHUNKS_FREE.items[index]);

  oh_cl_add(&OH_CHUNKS_USED, entry->start, size, false);

  void* ptr = (void*)((uint64_t)(&OH_HEAP) + entry->start);

  if (entry->size == size)
  {
    oh_cl_remove(&OH_CHUNKS_FREE, entry->start);
  }
  else
  {
    entry->start += size;
    entry->size -= size;
  }

  return ptr;
}

void oh_free(void* ptr)
{
  assert(OH_CHUNKS_USED.size > 0 && "There are no used chunks");

  const uint64_t start = (uint64_t)ptr - (uint64_t)(&OH_HEAP);
  const long long index = oh_cl_find_index_by_start(&OH_CHUNKS_USED, start);

  assert(index > -1 && "This pointer is not alloced");

  OHChunkEntry* entry = &(OH_CHUNKS_USED.items[index]);

  for (uint64_t i = 0; i < entry->size; ++i)
  {
    OH_HEAP[entry->start + i] = 0;
  }

  oh_cl_add(&OH_CHUNKS_FREE, entry->start, entry->size, true);

  oh_cl_remove(&OH_CHUNKS_USED, entry->start);
}
#endif
#endif
