#include <stdio.h>

#define OH_CAPACITY 32
#define OH_IMPLEMENTATION
#include "own_heap.h"

typedef struct {
  char name[15];
  unsigned char age;
} Person;

int main(void)
{
  printf("Starting\n");

  oh_cl_dump(&OH_CHUNKS_FREE, "Free");
  oh_cl_dump(&OH_CHUNKS_USED, "Used");
  oh_dump();

  printf("\n");
  printf("Allocing first person\n");

  Person* p1 = (Person*)oh_alloc(sizeof(Person));

  p1->name[0] = 'A';
  p1->name[1] = 'l';
  p1->name[2] = 'i';
  p1->name[3] = 'c';
  p1->name[4] = 'e';
  p1->age = 21;

  oh_cl_dump(&OH_CHUNKS_FREE, "Free");
  oh_cl_dump(&OH_CHUNKS_USED, "Used");
  oh_dump();

  printf("\n");
  printf("Allocing second person\n");

  Person* p2 = (Person*)oh_alloc(sizeof(Person));

  p2->name[0] = 'B';
  p2->name[1] = 'o';
  p2->name[2] = 'b';
  p2->age = 21;

  oh_cl_dump(&OH_CHUNKS_FREE, "Free");
  oh_cl_dump(&OH_CHUNKS_USED, "Used");
  oh_dump();

  printf("\n");
  printf("Freeing second person\n");

  oh_free(p2);

  oh_cl_dump(&OH_CHUNKS_FREE, "Free");
  oh_cl_dump(&OH_CHUNKS_USED, "Used");
  oh_dump();

  printf("\n");
  printf("Freeing first person\n");

  oh_free(p1);

  oh_cl_dump(&OH_CHUNKS_FREE, "Free");
  oh_cl_dump(&OH_CHUNKS_USED, "Used");
  oh_dump();

  return 0;
}
