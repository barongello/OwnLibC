#include <stdio.h>

#define OH_CAPACITY 40
#define OH_DUMP_STEP 16
#define OH_IMPLEMENTATION
#include "own_heap.h"

typedef struct {
  char name[15];
  unsigned char age;
} Person;

int main(void)
{
  oh_dump_full("Starting");

  printf("Allocing first person\n");

  Person* p1 = (Person*)oh_alloc(sizeof(Person));

  p1->name[0] = 'A';
  p1->name[1] = 'l';
  p1->name[2] = 'i';
  p1->name[3] = 'c';
  p1->name[4] = 'e';
  p1->age = 21;

  oh_dump_full("Alloced first person");

  printf("Allocing second person\n");

  Person* p2 = (Person*)oh_alloc(sizeof(Person));

  p2->name[0] = 'B';
  p2->name[1] = 'o';
  p2->name[2] = 'b';
  p2->age = 21;

  oh_dump_full("Alloced second person");

  printf("First person: (name = %s, age = %d)\n", p1->name, p1->age);
  printf("Second person: (name = %s, age = %d)\n", p2->name, p2->age);

  printf("\n");
  printf("Freeing second person\n");

  oh_free(p2);

  oh_dump_full("Freed second person");

  printf("Freeing first person\n");

  oh_free(p1);

  oh_dump_full("Freed first person");

  uint32_t* ptrs[8] = { 0 };

  for (size_t i = 0; i < 8; ++i)
  {
    printf("Allocing uint32_t (%zu)\n", i);

    ptrs[i] = oh_alloc(4);
    *(ptrs[i]) = (uint32_t)(rand() % UINT32_MAX);

    printf("Alloced uint32_t (%zu): (ptr: %016llX, value: %u = %08X)\n", i, (uint64_t)ptrs[i], *(ptrs[i]), *(ptrs[i]));
  }

  oh_dump_full("Alloced 8 uint32_t");

  printf("Deallocing 4 uint32_t (0, 2, 4, 6)\n");

  for (size_t i = 0; i < 8; i += 2)
  {
    printf("Freeing uint32_ptr (%zu)\n", i);

    oh_free(ptrs[i]);

    oh_dump_full("Freed uint32_ptr (%zu)", i);
  }

  oh_dump_full("Freed 4 uint32_t");

  char* ptrs2[4] = { 0 };

  ptrs2[0] = (char*)oh_alloc(2);
  ptrs2[1] = (char*)oh_alloc(3);
  ptrs2[2] = (char*)oh_alloc(2);
  ptrs2[3] = (char*)oh_alloc(3);

  *(ptrs2[0] + 0) = 'A';
  *(ptrs2[0] + 1) = 'B';

  *(ptrs2[1] + 0) = 'C';
  *(ptrs2[1] + 1) = 'D';
  *(ptrs2[1] + 2) = 'E';

  *(ptrs2[2] + 0) = 'F';
  *(ptrs2[2] + 1) = 'G';

  *(ptrs2[3] + 0) = 'H';
  *(ptrs2[3] + 1) = 'I';
  *(ptrs2[3] + 2) = 'J';

  oh_dump_full("Final");

  return 0;
}
