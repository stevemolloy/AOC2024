#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

int main(void) {
  char *input_file = __FILE__;
  // char *input_file = "./test.txt";
  // char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);
  sdm_string_view input = sdm_cstr_as_sv(file_contents);

  printf(SDM_SV_F"\n", SDM_SV_Vals(input));

  free(file_contents);

  return 0;
}

