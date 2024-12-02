#include <stdio.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

typedef struct {
  size_t capacity;
  size_t length;
  int *data;
} DynIntArray;

typedef struct {
  size_t capacity;
  size_t length;
  DynIntArray *data;
} DynDynIntArray;

int main(void) {
  // char *input_file = "./test.txt";
  // char *input_file = "./input.txt";
  char *input_file = "./src/main.c";
  char *file_contents = sdm_read_entire_file(input_file);
  sdm_string_view contents_view = sdm_cstr_as_sv(file_contents);

  printf(SDM_SV_F, SDM_SV_Vals(contents_view));

  free(file_contents);

  return 0;
}

