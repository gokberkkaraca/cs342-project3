#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int* dynamicMemoryPointers[100];
int count = 0;

void useStackMemory(int n) {
  if (n == 0){
    printf("End of recursive calls, type \"OK\" to exit\n");
    char input[20];
    scanf("%s", input);
    exit(0);
  }
  else if (n > 0) {
    useStackMemory(n-1);
  }
}

void allocateHeapMemory(int amount) {
  dynamicMemoryPointers[count] = malloc(amount * 4);
  count++;
  printf("Allocated memory from heap\n");
}

void deallocateHeapMemory() {
  count--;
  free(dynamicMemoryPointers[count]);
  printf("Deallocated memory from heap\n");
}

int main() {

  char input[20];
  printf("%s%s%s%s%s",
        "1 - Use stack memory\n",
        "2 - Allocate heap memory\n",
        "3 - Deallocate heap memory\n",
        "4 - Exit\n",
        "Selection: ");
  scanf("%s", input);
  printf("\n");

  while (1) {
    if (strcmp(input, "1") == 0) {
      int numOfRecursions;
      numOfRecursions = 0;
      printf("Enter number of recursions: ");
      scanf("%d", &numOfRecursions);
      useStackMemory(numOfRecursions);
    }
    else if (strcmp(input, "2") == 0) {
      allocateHeapMemory(10000);
    }
    else if (strcmp(input, "3") == 0) {
      deallocateHeapMemory();
    }
    else if (strcmp(input, "4") == 0) {
      exit(0);
    }
    else {
      printf("Invalid input\n");
    }

    printf("\n%s%s%s%s%s",
          "1 - Use stack memory\n",
          "2 - Allocate heap memory\n",
          "3 - Deallocate heap memory\n",
          "4 - Exit\n",
          "Selection: ");
    scanf("%s", input);
    printf("\n");
  }

  exit (0);
}
