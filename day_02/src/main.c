#include <assert.h>
#include <limits.h>
#include <stdlib.h>
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

typedef enum {
  GOING_UP,
  GOING_DOWN,
} Direction;

int main(void) {
  // char *input_file = "./test.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);
  sdm_string_view contents_view = sdm_cstr_as_sv(file_contents);

  DynDynIntArray array_of_arrays = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(array_of_arrays, 1024);
  size_t line_num = 0;
  while (contents_view.length) {
    SDM_ENSURE_ARRAY_MIN_CAP(array_of_arrays.data[line_num], 1024);
    array_of_arrays.length = line_num + 1;
    while (*contents_view.data != '\n') {
      SDM_ARRAY_PUSH(array_of_arrays.data[line_num], sdm_pop_integer(&contents_view));
      if (*contents_view.data != '\n') sdm_sv_trim(&contents_view);
    }
    sdm_sv_trim(&contents_view);
    line_num++;
  }

  printf("Num reports = %zu\n", array_of_arrays.length);

  size_t num_safe = 0;
  for (size_t report_num=0; report_num<array_of_arrays.length; report_num++) {
    bool passed = true;
    DynIntArray report = array_of_arrays.data[report_num];
    if ((report.data[1] == report.data[0]) || (abs(report.data[1] - report.data[0]) > 3)) {
      passed = false;
      continue;
    }
    Direction direction = report.data[1] - report.data[0] > 0 ? GOING_UP : GOING_DOWN;
    size_t i = 1;
    while (i < (report.length - 1)) {
      i++;
      switch (direction) {
        case GOING_UP: {
          if (report.data[i] - report.data[i-1] < 0) {
						passed = false;
			    }
          break;
        }
        case GOING_DOWN: {
          if (report.data[i] - report.data[i-1] > 0) {
						passed = false;
			    }
          break;
        }
      }
      if (abs(report.data[i] - report.data[i-1]) > 3) {
				passed = false;
			}
      if (abs(report.data[i] - report.data[i-1]) == 0) {
				passed = false;
			}
      if (!passed) break;
    }
    if (passed) num_safe++;
  }

  printf("Part 1 = %zu\n", num_safe);



  free(file_contents);

  return 0;
}

int sdm_pop_integer(sdm_string_view *SV) {
  char *new_pos;
  int retval = strtol(SV->data, &new_pos, 0);
  size_t diff = new_pos - SV->data;
  SV->data = new_pos;
  SV->length -= diff;
  return retval;
}

