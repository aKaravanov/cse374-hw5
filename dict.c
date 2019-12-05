#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LENGTH 100
#define DICT_ITEM(MAX_LENGTH)                                                  \
  struct {                                                                     \
    char word[MAX_LENGTH];                                                     \
    size_t len;                                                                \
  }

struct dict_item {
  char word[MAX_LENGTH];
  size_t len;
};

struct dict_t {
  char *path;
  int fd;
  size_t num_items;
  struct dict_item *base;
};

struct dict_t *dictionary_new(char *data_file, size_t num_items) {
  struct dict_t *new = malloc(1 * sizeof(struct dict_t));
  new->path = data_file;
  new->num_items = num_items;
  return new;
}

size_t dictionary_len(struct dict_t *dict) {
  return dict->num_items * sizeof(struct dict_item);
}

// struct dict_t* dictionary_generate(char *input, char *output) {
//}

// This is a private helper function. It should:
// Open the underlying path (dict->path), ftruncate it to the appropriate length
// (dictionary_len), then mmap it.
int dictionary_open_map(struct dict_t *dict) {
  int fd = open(dict->path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  size_t len = dictionary_len(dict);
  if (fd == -1) {
    perror("open");
    return 0;
  }

  if (ftruncate(fd, len) == -1) {
    perror("truncate");
    return 0;
  }

  struct dict_item *base =
      mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (base == MAP_FAILED) {
    perror("mmap");
    return 0;
  }

  dict->fd = fd;
  dict->base = base;
  return 1;
}

int dictionary_generate(struct dict_t *dict, char *input) {
  dictionary_open_map(dict);

  FILE *file = fopen(input, "r");
  char string[MAX_LENGTH];

  if (file == NULL) {
    perror("dict error");
    return 0;
  }

  size_t i = 0;

  while (fgets(string, MAX_LENGTH, file)) {
    string[strcspn(string, "\n")] = 0;
    sprintf(dict->base[i].word, string);
    dict->base[i].len = length_of_string(string);
    i += 1;
  }

  fclose(file);
  return 1;
}

char *dictionary_exists(struct dict_t *dict, char *word) {
  size_t i;
  for (i = 0; dict->base[i].len; i++) {
    if (strcmp(word, dict->base[i].word) == 0) {
      return dict->base[i].word;
    }
  }
  return NULL;
}

int length_of_string(char *word) {
  int i;
  for (i = 0; word[i] != '\0'; ++i)
    ;
  return i;
}

//// Count of words with len > n
int dictionary_larger_than(struct dict_t *dict, size_t n) {
  int count = 0;
  for (size_t i = 0; dict->base[i].len; i++) {
    if (dict->base[i].len > n) {
      count += 1;
    }
  }
  return count;
}

//// Count of words with len < n
int dictionary_smaller_than(struct dict_t *dict, size_t n) {
  int count = 0;
  for (size_t i = 0; dict->base[i].len; i++) {
    if (dict->base[i].len < n) {
      count += 1;
    }
  }
  return count;
}

//// Count of words with len == n
int dictionary_equal_to(struct dict_t *dict, size_t n) {
  int count = 0;
  for (size_t i = 0; dict->base[i].len; i++) {
    if (dict->base[i].len == n) {
      count += 1;
    }
  }
  return count;
}

// Unmaps the given dictionary.
// Free/destroy the underlying dict. Does NOT delete the database file.
void dictionary_close(struct dict_t *dict) {
  munmap(dict->base, dictionary_len(dict));
  free(dict);
}

int dictionary_load(struct dict_t *dict) {
  size_t len = dictionary_len(dict);

  int fd = open(dict->path, O_RDONLY);
  if (fd == -1) {
    perror("open");
    return 0;
  }

  struct dict_item *base = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
  if (base == MAP_FAILED) {
    perror("mmap");
    return 0;
  }

  dict->fd = fd;
  dict->base = base;
  return 1;
}
