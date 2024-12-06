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
  *row = 0;
  *col = 0;
  for (*row=0; *row<rows.length; (*row)++) {
    for (*col=0; *col<rows.data[*row].length; (*col)++) {
      if (rows.data[*row].data[*col] == '^') {
        return;
      }
    }
  }
}

int main(void) {
  // char *input_file = "./test.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);

  char *working_data = malloc(strlen(file_contents));
  if (working_data == NULL) {
    fprintf(stderr, "Memory problem :(\n");
    return 1;
  }
  strcpy(working_data, file_contents);
  sdm_string_view contents_view = sdm_cstr_as_sv(working_data);

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
          rows.data[guard_row].data[guard_col] = 'X';
          guard_col++;
        }
        break;
      }
    }
  }

  size_t part1_ans = 0;
  for (size_t i=0; i<strlen(working_data); i++) {
    if (working_data[i] == 'X') part1_ans++;
  }
  printf("Part 1 = %zu\n", part1_ans);
  
  strcpy(working_data, file_contents);
  contents_view = sdm_cstr_as_sv(working_data);

  size_t part2_ans = 0;
  for (size_t i=0; i<strlen(working_data); i++) {
    printf("Test %zu of %zu\n", i, strlen(working_data));
    strcpy(working_data, file_contents);

    if (working_data[i] != '.') {
      continue;
    }
    working_data[i] = '#';

    contents_view = sdm_cstr_as_sv(working_data);
    SDM_ENSURE_ARRAY_MIN_CAP(rows, 1024);
    SDM_ARRAY_RESET(rows);
    while (contents_view.length) {
      SDM_ARRAY_PUSH(rows, sdm_sv_pop_by_delim(&contents_view, '\n'));
    }
    // printf("%s\n", working_data);

    bool changed_dir = false;
    finished_moving = false;
    find_guard(rows, &guard_row, &guard_col);
    dir = MOVING_UP;

    while (!finished_moving) {
      if (rows.data[guard_row].data[guard_col] == '+') {
        part2_ans++;
        finished_moving = true;
      } else {
        switch (dir) {
          case MOVING_UP: {
            if (rows.data[guard_row].data[guard_col] == 'U') {
              part2_ans++;
              finished_moving = true;
            } else if (guard_row == 0) {
              finished_moving = true;
              if (changed_dir) {
                changed_dir = false;
                rows.data[guard_row].data[guard_col] = 'U';
              } else
                rows.data[guard_row].data[guard_col] = 'U';
            } else if (rows.data[guard_row-1].data[guard_col] == '#') {
              dir = MOVING_RIGHT;
              changed_dir = true;
            } else {
              if (changed_dir) {
                changed_dir = false;
                rows.data[guard_row].data[guard_col] = 'U';
              } else
                rows.data[guard_row].data[guard_col] = 'U';
              guard_row--;
            }
            break;
          }
          case MOVING_DOWN: {
            if (rows.data[guard_row].data[guard_col] == 'D') {
              part2_ans++;
              finished_moving = true;
            } else if (guard_row == rows.length-1) {
              finished_moving = true;
              if (changed_dir) {
                changed_dir = false;
                rows.data[guard_row].data[guard_col] = 'D';
              } else
                rows.data[guard_row].data[guard_col] = 'D';
            } else if (rows.data[guard_row+1].data[guard_col] == '#') {
              dir = MOVING_LEFT;
              changed_dir = true;
            } else {
              if (changed_dir) {
                changed_dir = false;
                rows.data[guard_row].data[guard_col] = 'D';
              } else
                rows.data[guard_row].data[guard_col] = 'D';
              guard_row++;
            }
            break;
          }
          case MOVING_LEFT: {
            if (rows.data[guard_row].data[guard_col] == 'L') {
              part2_ans++;
              finished_moving = true;
            } else if (guard_col == 0) {
              finished_moving = true;
              if (changed_dir) {
                changed_dir = false;
                rows.data[guard_row].data[guard_col] = 'L';
              } else
                rows.data[guard_row].data[guard_col] = 'L';
            } else if (rows.data[guard_row].data[guard_col-1] == '#') {
              changed_dir = true;
              dir = MOVING_UP;
            } else {
              if (changed_dir) {
                changed_dir = false;
                rows.data[guard_row].data[guard_col] = 'L';
              } else
                rows.data[guard_row].data[guard_col] = 'L';
              guard_col--;
            }
            break;
          }
          case MOVING_RIGHT: {
            if (rows.data[guard_row].data[guard_col] == 'R') {
              part2_ans++;
              finished_moving = true;
            } else if (guard_col == rows.data[guard_row].length-1) {
              finished_moving = true;
              if (changed_dir) {
                changed_dir = false;
                rows.data[guard_row].data[guard_col] = 'R';
              } else
                rows.data[guard_row].data[guard_col] = 'R';
            } else if (rows.data[guard_row].data[guard_col+1] == '#') {
              changed_dir = true;
              dir = MOVING_DOWN;
            } else {
              if (changed_dir) {
                changed_dir = false;
                rows.data[guard_row].data[guard_col] = 'R';
              } else
                rows.data[guard_row].data[guard_col] = 'R';
              guard_col++;
            }
            break;
          }
        }
      }
    }
  }

  // 2978 is too high
  printf("Part 2 = %zu (2978 is too high, and 935 is too low)\n", part2_ans);

  free(working_data);
  free(file_contents);

  return 0;
}

