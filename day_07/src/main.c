#include <stdbool.h>
#include <assert.h>
#include <math.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

typedef struct {
  size_t capacity;
  size_t length;
  sdm_string_view *data;
} Lines;

typedef struct {
  size_t capacity;
  size_t length;
  unsigned long int *data;
} IntArray;

typedef struct {
  size_t capacity;
  size_t length;
  IntArray *data;
} IntArrayArray;

typedef unsigned long int (*binop)(unsigned long int, unsigned long int);

unsigned long int add(unsigned long int a, unsigned long int b) {
  return a + b;
}

unsigned long int mul(unsigned long int a, unsigned long int b) {
  return a * b;
}

unsigned long int cat(unsigned long int a, unsigned long int b) {
  unsigned long int b_copy = b;

  while (b > 0) {
    a *= 10;
    b /= 10;
  }

  return a + b_copy;
}

binop ops[3] = {&add, &mul, &cat};

int main(void) {
  // char *input_file = "./test.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);

  sdm_string_view contents_view = sdm_cstr_as_sv(file_contents);

  Lines lines = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(lines, 1024);
  while (contents_view.length) {
    SDM_ARRAY_PUSH(lines, sdm_sv_pop_by_delim(&contents_view, '\n'));
  }

  IntArray results = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(results, 1024);
  IntArrayArray arglist = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(arglist, 1024);

  for (size_t i=0; i<lines.length; i++) {
    sdm_string_view line = lines.data[i];
    SDM_ARRAY_PUSH(results, sdm_sv_pop_integer(&line));
    assert(line.data[0] == ':');
    sdm_sv_pop_one_char(&line);
    sdm_sv_trim(&line);

    SDM_ARRAY_PUSH(arglist, (IntArray){0});
    SDM_ENSURE_ARRAY_MIN_CAP(arglist.data[arglist.length-1], 1024);
    while (line.length) {
      SDM_ARRAY_PUSH(arglist.data[arglist.length-1], sdm_sv_pop_integer(&line));
      sdm_sv_trim(&line);
    }
  }

  unsigned long int part1_ans = 0;
  for (size_t i=0; i<results.length; i++) {
    IntArray args = arglist.data[i];
    unsigned long int result = results.data[i];

    for (int mask=0; mask<pow(2, (int)args.length); mask++) {
      unsigned long int total = args.data[0];
      int mask_copy = mask;
      for (size_t i=1; i<args.length; i++) {
        size_t ind = mask_copy % 2;
        mask_copy /= 2;
        total = ops[ind](total, args.data[i]);
        if (total > result) {
          break;
        }
      }
      if (total == result) {
        part1_ans += result;
        break;
      }
    }
  }

  printf("Part 1 = %ld (5091289 is too low, 5357428 is too low)\n", part1_ans);

  unsigned long int part2_ans = 0;
  for (size_t i=0; i<results.length; i++) {
    IntArray args = arglist.data[i];
    unsigned long int result = results.data[i];

    for (int mask=0; mask<pow(3, (int)args.length); mask++) {
      unsigned long int total = args.data[0];
      int mask_copy = mask;
      for (size_t i=1; i<args.length; i++) {
        size_t ind = mask_copy % 3;
        mask_copy /= 3;
        total = ops[ind](total, args.data[i]);
        if (total > result) {
          break;
        }
      }
      if (total == result) {
        part2_ans += result;
        break;
      }
    }
  }

  printf("Part 2 = %ld (7068204125831 is too low)\n", part2_ans);

  free(file_contents);

  return 0;
}

