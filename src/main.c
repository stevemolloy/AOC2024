#include <stdio.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

int main(void) {
  char *input_file = "./src/main.c";
  char *file_contents = sdm_read_entire_file(input_file);

  printf("%s\n", file_contents);

  free(file_contents);

  return 0;
}
