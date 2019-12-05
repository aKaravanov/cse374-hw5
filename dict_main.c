#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"

int main(int argc, char *argv[argc+1]) {
  struct dict_t *dict = dictionary_new(argv[2], 5e5);
  if (!strcmp(argv[1], "gen")) {
    if (dictionary_generate(dict, argv[3]) < 0) {
      printf("Error generating dictionary\n");
      return EXIT_FAILURE;
    }
  } else if (!strcmp(argv[1], "load")) {
    if(!dictionary_load(dict)) {
      printf("Error loading dictionary\n");
      return EXIT_FAILURE;
    }
  }
  char *addr = dictionary_exists(dict, "cat");
  if (addr) {
    printf("i haz cat, it is at: %p\n", (void*)addr);
  }
  printf("%s\n", "Two");
  printf("%d\n", dictionary_smaller_than(dict, 2));
  printf("%d\n", dictionary_equal_to(dict, 2));
  printf("%d\n\n", dictionary_larger_than(dict, 2));

  printf("%s\n", "Three");
  printf("%d\n", dictionary_smaller_than(dict, 3));
  printf("%d\n", dictionary_equal_to(dict, 3));
  printf("%d\n\n", dictionary_larger_than(dict, 3));

  printf("%s\n", "Four");
  printf("%d\n", dictionary_smaller_than(dict, 4));
  printf("%d\n", dictionary_equal_to(dict, 4));
  printf("%d\n\n", dictionary_larger_than(dict, 4));

  //printf("%s\n", "Five");
  //printf("%d\n", dictionary_smaller_than(dict, 5));
  //printf("%d\n", dictionary_equal_to(dict, 5));
  //printf("%d\n\n", dictionary_larger_than(dict, 5));
  //dictionary_close(dict);

  printf("%d\n", dictionary_equal_to(dict, 10));
  dictionary_close(dict);
  return EXIT_SUCCESS;
}
