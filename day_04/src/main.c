#include <stdio.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

typedef struct {
  size_t capacity;
  size_t length;
  sdm_string_view *data;
} DynSVArray;

int main(void) {
  // char *input_file = "./test.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);
  sdm_string_view contents_view = sdm_cstr_as_sv(file_contents);

  // printf(SDM_SV_F"\n\n", SDM_SV_Vals(contents_view));

  DynSVArray lines = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(lines, 1024);
  while (contents_view.length > 0) {
    SDM_ARRAY_PUSH(lines, sdm_sv_pop_by_delim(&contents_view, '\n'));
  }

  size_t count = 0;
  for (size_t linenum=0; linenum<lines.length; linenum++) {
    for (size_t j=0; j<lines.data[linenum].length; j++) {
      if (strncmp(lines.data[linenum].data + j, "XMAS", 4) == 0) {
        count++;
      }
      if (strncmp(lines.data[linenum].data + j, "SAMX", 4) == 0) {
        count++;
      }
    }
  }
  for (size_t linenum=0; linenum<(lines.length - 3); linenum++) {
    for (size_t j=0; j<lines.data[linenum].length; j++) {
      if ((lines.data[linenum].data[j] == 'X') && (lines.data[linenum+1].data[j] == 'M') && (lines.data[linenum+2].data[j] == 'A') && (lines.data[linenum+3].data[j] == 'S')) {
        count++;
      }
      if ((lines.data[linenum].data[j] == 'S') && (lines.data[linenum+1].data[j] == 'A') && (lines.data[linenum+2].data[j] == 'M') && (lines.data[linenum+3].data[j] == 'X')) {
        count++;
      }
    }
  }
  for (size_t linenum=0; linenum<(lines.length - 3); linenum++) {
    for (size_t j=0; j<lines.data[linenum].length-3; j++) {
      if ((lines.data[linenum].data[j] == 'X') && (lines.data[linenum+1].data[j+1] == 'M') && (lines.data[linenum+2].data[j+2] == 'A') && (lines.data[linenum+3].data[j+3] == 'S')) {
        count++;
      }
    }
  }
  for (size_t linenum=0; linenum<(lines.length - 3); linenum++) {
    for (size_t j=3; j<lines.data[linenum].length; j++) {
      if ((lines.data[linenum].data[j] == 'X') && (lines.data[linenum+1].data[j-1] == 'M') && (lines.data[linenum+2].data[j-2] == 'A') && (lines.data[linenum+3].data[j-3] == 'S')) {
        count++;
      }
    }
  }
  for (size_t linenum=3; linenum<(lines.length); linenum++) {
    for (size_t j=0; j<lines.data[linenum].length-3; j++) {
      if ((lines.data[linenum].data[j] == 'X') && (lines.data[linenum-1].data[j+1] == 'M') && (lines.data[linenum-2].data[j+2] == 'A') && (lines.data[linenum-3].data[j+3] == 'S')) {
        count++;
      }
    }
  }
  for (size_t linenum=3; linenum<(lines.length); linenum++) {
    for (size_t j=3; j<lines.data[linenum].length; j++) {
      if ((lines.data[linenum].data[j] == 'X') && (lines.data[linenum-1].data[j-1] == 'M') && (lines.data[linenum-2].data[j-2] == 'A') && (lines.data[linenum-3].data[j-3] == 'S')) {
        count++;
      }
    }
  }

  printf("Part 1 = %zu\n", count);

  SDM_ARRAY_FREE(lines);
  free(file_contents);

  return 0;
}

