#include <stdio.h>
#include <string.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

int main(void) {
  // char *input_file = "./test.txt";
  // char *input_file = "./test2.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);

  size_t i = 0;
  int val1, val2;
  int part1_ans = 0, part2_ans = 0;
  int enabled = 1;
  while (i<strlen(file_contents)) {
    if (strncmp(file_contents + i, "do()", 4) == 0)         enabled = 1;
    else if (strncmp(file_contents + i, "don't()", 7) == 0) enabled = 0;
    val1 = 0;
    val2 = 0;
    if (strncmp(file_contents + i, "mul(", 4) == 0) {
      i += 4;
      while (isdigit(file_contents[i])) {
        val1 = (val1 * 10) + file_contents[i] - '0';
        i++;
      }
      if ((val1 > 999) || (file_contents[i] != ',')) continue;
      i++;
      while (isdigit(file_contents[i])) {
        val2 = (val2 * 10) + file_contents[i] - '0';
        i++;
      }
      if ((val2 > 999) || (file_contents[i] != ')')) continue;
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

