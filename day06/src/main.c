#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

typedef enum {
  MOVING_UP,
  MOVING_DOWN,
  MOVING_LEFT,
  MOVING_RIGHT,
} Direction;

typedef struct {
  size_t capacity;
  size_t length;
  sdm_string_view *data;
} SVArray;

void find_guard(SVArray rows, size_t *row, size_t *col) {
  for (*row=0; *row<rows.length; (*row)++) {
    for (*col=0; *col<rows.data[*row].length; (*col)++) {
      if (rows.data[*row].data[*col] == '^') {
        return;
      }
    }
  }
}

int main(void) {
  char *input_file = "./test.txt";
  // char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);

  char *working_data = malloc(strlen(file_contents));
  if (working_data == NULL) {
    fprintf(stderr, "Memory problem :(\n");
    return 1;
  }
  strcpy(working_data, file_contents);
  sdm_string_view contents_view = sdm_cstr_as_sv(working_data);

  printf(SDM_SV_F"\n", SDM_SV_Vals(contents_view));

  SVArray rows = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(rows, 1024);
  while (contents_view.length) {
    SDM_ARRAY_PUSH(rows, sdm_sv_pop_by_delim(&contents_view, '\n'));
  }

  size_t guard_row, guard_col;
  find_guard(rows, &guard_row, &guard_col);
  printf("Found the guard at %zu, %zu\n", guard_row, guard_col);

  Direction dir = MOVING_UP;

  bool finished_moving = false;

  while (!finished_moving) {
    switch (dir) {
      case MOVING_UP: {
        if (guard_row == 0) {
          finished_moving = true;
          rows.data[guard_row].data[guard_col] = 'X';
        } else if (rows.data[guard_row-1].data[guard_col] == '#') {
          dir = MOVING_RIGHT;
        } else {
          printf("Moving up!\n");
          rows.data[guard_row].data[guard_col] = 'X';
          guard_row--;
        }
        break;
      }
      case MOVING_DOWN: {
        if (guard_row == rows.length-1) {
          finished_moving = true;
          rows.data[guard_row].data[guard_col] = 'X';
        } else if (rows.data[guard_row+1].data[guard_col] == '#') {
          dir = MOVING_LEFT;
        } else {
          printf("Moving left!\n");
          rows.data[guard_row].data[guard_col] = 'X';
          guard_row++;
        }
        break;
      }
      case MOVING_LEFT: {
        if (guard_col == 0) {
          finished_moving = true;
          rows.data[guard_row].data[guard_col] = 'X';
        } else if (rows.data[guard_row].data[guard_col-1] == '#') {
          dir = MOVING_UP;
        } else {
          printf("Moving left!\n");
          rows.data[guard_row].data[guard_col] = 'X';
          guard_col--;
        }
        break;
      }
      case MOVING_RIGHT: {
        if (guard_col == rows.data[guard_row].length-1) {
          finished_moving = true;
          rows.data[guard_row].data[guard_col] = 'X';
        } else if (rows.data[guard_row].data[guard_col+1] == '#') {
          dir = MOVING_DOWN;
        } else {
          printf("Moving down!\n");
          rows.data[guard_row].data[guard_col] = 'X';
          guard_col++;
        }
        break;
      }
    }
  }

  printf("%s\n", working_data);

  size_t part1_ans = 0;
  for (size_t i=0; i<strlen(working_data); i++) {
    if (working_data[i] == 'X') part1_ans++;
  }

  printf("Part 1 = %zu\n", part1_ans);
  
  free(file_contents);

  return 0;
}

