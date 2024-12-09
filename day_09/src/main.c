#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

typedef struct {
  size_t capacity;
  size_t length;
  long int *data;
} IntArray;

long int remove_element(IntArray *array, size_t index) {
  assert(index < array->length);
  long int retval = array->data[index];

  if (index == array->length-1) {
    array->length--;
    return retval;
  }

  for (size_t i=index; i<array->length-1; i++) {
    array->data[index] = array->data[index+1];
  }
  array->length--;

  return retval;
}

void print_intarray(IntArray array) {
  for (size_t i=0; i<array.length; i++) {
    if (array.data[i] == -1) printf(".");
    else printf("%ld", array.data[i]);
  }
  printf("\n");
}

bool is_dense(IntArray layout) {
  for (size_t i=0; i<layout.length; i++)
    if (layout.data[i] == -1) return false;
  return true;
}

size_t count_spaces(IntArray layout) {
  size_t retval = 0;

  for (size_t i=0; i<layout.length; i++)
    if (layout.data[i] == -1) retval++;

  return retval;
}

size_t first_blank_space(IntArray array) {
  for (size_t i=0; i<array.length; i++)
    if (array.data[i] == -1) return i;
  fprintf(stderr, "You have reached the unreachable. Well done.\n");
  exit(1);
}

int main(void) {
  // char *input_file = "./test.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);
  sdm_string_view input = sdm_cstr_as_sv(file_contents);

  IntArray current_layout = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(current_layout, 1024);
  long int block_id = 0;
  while (input.length > 0) {
    char b_char = sdm_sv_pop_one_char(&input);
    char s_char = sdm_sv_pop_one_char(&input);

    long int blocks = b_char - '0';
    long int space = s_char - '0';
    while (blocks > 0) {
      SDM_ARRAY_PUSH(current_layout, block_id);
      blocks--;
    }
    while (space > 0) {
      SDM_ARRAY_PUSH(current_layout, -1);
      space--;
    }

    block_id++;
  }

  print_intarray(current_layout);

  size_t num_spaces = count_spaces(current_layout);

  for (long int i=current_layout.length-1; i>=0; i--) {
    while (current_layout.data[current_layout.length-1] == -1) {
      current_layout.length--;
      num_spaces--;
    }

    if (num_spaces == 0) break;
    
    size_t blank = first_blank_space(current_layout);
    current_layout.data[blank] = current_layout.data[current_layout.length-1];
    current_layout.length--;
    num_spaces--;
  }

  print_intarray(current_layout);

  long int part1_ans = 0;
  for (size_t i=0; i<current_layout.length; i++) {
    part1_ans += current_layout.data[i] * i;
  }

  printf("part1_ans = %ld (1851882345 is too low)\n", part1_ans);

  free(file_contents);

  return 0;
}

