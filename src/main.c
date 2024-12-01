#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

typedef struct {
  size_t capacity;
  size_t length;
  int *data;
} DynIntArray;

int sv_to_int(sdm_string_view sv) {
  int val = 0;
  for (size_t i=0; i<sv.length; i++) {
    val *= 10;
    val += sv.data[i] - '0';
  }
  return val;
}

int int_compare(const void *a, const void *b) {
  int int_a = *(int*)a;
  int int_b = *(int*)b;

  return int_a - int_b;
}

size_t repetitions(DynIntArray da, int val) {
  size_t count = 0;
  for (size_t i=0; i<da.length; i++) {
    if (da.data[i] == val) {
      count++;
    }
  }
  return count;
}

int main(void) {
  // char *input_file = "./test.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);

  DynIntArray leftlist  = {0};
  DynIntArray rightlist = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(leftlist, 1024);
  SDM_ENSURE_ARRAY_MIN_CAP(rightlist, 1024);

  sdm_string_view contents_view = sdm_cstr_as_sv(file_contents);

  while (contents_view.length) {
    sdm_sv_trim(&contents_view);
    sdm_string_view popped = sdm_sv_pop_by_delim(&contents_view, ' ');
    sdm_sv_trim(&contents_view);

    SDM_ARRAY_PUSH(leftlist, sv_to_int(popped));

    popped = sdm_sv_pop_by_delim(&contents_view, '\n');
    sdm_sv_trim(&contents_view);
    SDM_ARRAY_PUSH(rightlist, sv_to_int(popped));
  }

  qsort(leftlist.data, leftlist.length, sizeof(leftlist.data[0]), &int_compare);
  qsort(rightlist.data, rightlist.length, sizeof(rightlist.data[0]), &int_compare);

  assert(leftlist.length == rightlist.length && "List of inconsistent length");
  int sum = 0;
  for (size_t i=0; i<leftlist.length; i++) {
    int diff = abs(leftlist.data[i] - rightlist.data[i]);
    sum += diff;
  }

  printf("Answer to part 1 = %d\n", sum);

  size_t similarity = 0;
  for (size_t i=0; i<leftlist.length; i++) {
    similarity += leftlist.data[i] * repetitions(rightlist, leftlist.data[i]);
  }

  printf("Answer to part 2 = %zu\n", similarity);

  free(file_contents);

  return 0;
}
