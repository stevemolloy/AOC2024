#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

typedef struct {
  size_t x, y;
} Coord;

typedef struct {
  size_t capacity;
  size_t length;
  Coord *data;
} CoordArray;

bool array_contains_coord(CoordArray array, Coord coord) {
  for (size_t i=0; i<array.length; i++) {
    if ((array.data[i].x == coord.x) && (array.data[i].y == coord.y)) return true;
  }
  return false;
}

void add_coord_if_not_there(CoordArray *array, Coord coord) {
  if (!array_contains_coord(*array, coord)) {
    SDM_ARRAY_PUSH(*array, coord);
  }
}

typedef struct {
  size_t capacity;
  size_t length;
  sdm_string_view *data;
} SVArray;

int count_routes(SVArray array, CoordArray *peaks, size_t row, size_t col, int target) {
  static int retval = 0;

  if ((row>0) && array.data[row-1].data[col]-'0'==target) {
    if (target == 9) {
      add_coord_if_not_there(peaks, (Coord){.x=row-1, .y=col});
      retval++;
    }
    else count_routes(array, peaks, row-1, col, target+1);
  }
  if ((row<array.length-1) && array.data[row+1].data[col]-'0'==target) {
    if (target == 9) {
      add_coord_if_not_there(peaks, (Coord){.x=row+1, .y=col});
      retval++;
    }
    else count_routes(array, peaks, row+1, col, target+1);
  }

  if ((col>0) && array.data[row].data[col-1]-'0'==target) {
    if (target == 9) {
      add_coord_if_not_there(peaks, (Coord){.x=row, .y=col-1});
      retval++;
    }
    else count_routes(array, peaks, row, col-1, target+1);
  }
  if ((col<array.data[0].length-1) && array.data[row].data[col+1]-'0'==target) {
    if (target == 9) {
      add_coord_if_not_there(peaks, (Coord){.x=row, .y=col+1});
      retval++;
    }
    else count_routes(array, peaks, row, col+1, target+1);
  }

  return retval;
}

int main(void) {
  // char *input_file = "./test.txt";
  // char *input_file = "./test_2.txt";
  // char *input_file = "./test_3.txt";
  // char *input_file = "./test_4.txt";
  // char *input_file = "./test_5.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);
  sdm_string_view input = sdm_cstr_as_sv(file_contents);

  printf(SDM_SV_F"\n", SDM_SV_Vals(input));

  SVArray input_rows = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(input_rows, 1024);

  while (input.length > 0) {
    SDM_ARRAY_PUSH(input_rows, sdm_sv_pop_by_delim(&input, '\n'));
  }

  size_t part1_ans = 0;
  size_t part2_ans = 0;
  for (size_t row = 0; row < input_rows.length; row++) {
    for (size_t col = 0; col < input_rows.data[0].length; col++) {
      if (input_rows.data[row].data[col] != '0') continue;
      CoordArray peaks_found = {0};
      SDM_ENSURE_ARRAY_MIN_CAP(peaks_found, 64);
      part2_ans = count_routes(input_rows, &peaks_found, row, col, 1);
      printf("peaks_found.length = %zu\n", peaks_found.length);
      part1_ans += peaks_found.length;
      SDM_ARRAY_FREE(peaks_found);
    }
  }

  printf("Part 1 = %zu\n", part1_ans);
  printf("Part 2 = %zu\n", part2_ans);
  
  SDM_ARRAY_FREE(input_rows);
  free(file_contents);

  return 0;
}

