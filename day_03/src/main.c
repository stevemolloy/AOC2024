#include <stdio.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

int main(void) {
  // char *input_file = "./test.txt";
  // char *input_file = "./test2.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);
  sdm_string_view contents_view = sdm_cstr_as_sv(file_contents);

  size_t i = 0;
  int val1, val2;
  int part1_ans = 0, part2_ans = 0;
  int enabled = 1;
  while (i<contents_view.length) {
    if (strncmp(contents_view.data + i, "do()", 4) == 0)         enabled = 1;
    else if (strncmp(contents_view.data + i, "don't()", 7) == 0) enabled = 0;
    val1 = 0;
    val2 = 0;
    if (strncmp(contents_view.data + i, "mul(", 4) == 0) {
      i += 4;
      while (isdigit(contents_view.data[i])) {
        val1 = (val1 * 10) + contents_view.data[i] - '0';
        i++;
      }
      if ((val1 > 999) || (contents_view.data[i] != ',')) continue;
      i++;
      while (isdigit(contents_view.data[i])) {
        val2 = (val2 * 10) + contents_view.data[i] - '0';
        i++;
      }
      if ((val2 > 999) || (contents_view.data[i] != ')')) continue;
      part1_ans += val1 * val2;
      part2_ans += enabled * val1 * val2;
    }
    i++;
  }

  printf("Answer to part 1 = %d\n", part1_ans);
  printf("Answer to part 2 = %d\n", part2_ans);

  free(file_contents);

  return 0;
}

