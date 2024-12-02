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

size_t safe_reports_part1(DynDynIntArray array_of_arrays);
size_t safe_reports_part2(DynDynIntArray array_of_arrays);

int main(void) {
  // char *input_file = "./test.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);
  sdm_string_view contents_view = sdm_cstr_as_sv(file_contents);

  DynDynIntArray array_of_arrays = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(array_of_arrays, 1024);
  size_t line_num = 0;
  while (contents_view.length) {
    array_of_arrays.data[line_num] = (DynIntArray){0};
    SDM_ENSURE_ARRAY_MIN_CAP(array_of_arrays.data[line_num], 1024);
    array_of_arrays.length = line_num + 1;
    while (*contents_view.data != '\n') {
      SDM_ARRAY_PUSH(array_of_arrays.data[line_num], sdm_pop_integer(&contents_view));
      if (*contents_view.data != '\n') sdm_sv_trim(&contents_view);
    }
    sdm_sv_trim(&contents_view);
    line_num++;
  }

  printf("Part 1 = %zu\n", safe_reports_part1(array_of_arrays));
  printf("Part 2 = %zu\n", safe_reports_part2(array_of_arrays));

  for (size_t i=0; i< array_of_arrays.length; i++) 
    SDM_ARRAY_FREE(array_of_arrays.data[i]);
  SDM_ARRAY_FREE(array_of_arrays);
  free(file_contents);

  return 0;
}

DynIntArray get_array_diff(DynIntArray arr) {
  DynIntArray retval = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(retval, arr.capacity);

  for (size_t i=0; i<arr.length-1; i++) SDM_ARRAY_PUSH(retval, arr.data[i+1] - arr.data[i]);

  return retval;
}

size_t count_zeros(DynIntArray arr) {
  size_t retval = 0;
  for (size_t i=0; i<arr.length; i++) {
    if (arr.data[i] == 0) retval++;
  }

  return retval;
}

bool report_is_safe_part1(DynIntArray report) {
  bool passed = true;
  DynIntArray diff_arr = get_array_diff(report);
  if (count_zeros(diff_arr) > 0) passed = false;
  for (size_t i=0; i<diff_arr.length; i++) {
    if (abs(diff_arr.data[i]) > 3) passed = false;
  }
  
  for (size_t i=1; i<diff_arr.length; i++) {
    if (diff_arr.data[i-1] * diff_arr.data[i] < 0) passed = false;
  }

  SDM_ARRAY_FREE(diff_arr);
  return passed;
}

bool report_is_safe_part2(DynIntArray report) {
  DynIntArray diff_arr = get_array_diff(report);
  size_t num_zeros = count_zeros(diff_arr);

  size_t sign_flips = 0;
  bool sign_flip_last_or_first = false;
  for (size_t i=1; i<diff_arr.length; i++) {
    int product = diff_arr.data[i-1] * diff_arr.data[i];
    if (product < 0) sign_flips++;
    if ((i==1) && (product<0)) sign_flip_last_or_first = true;
    if ((i==diff_arr.length-1) && (product<0)) sign_flip_last_or_first = true;
  }

  size_t big_steps = 0;
  bool big_step_last = false;
  for (size_t i=0; i<diff_arr.length; i++) {
    if (abs(diff_arr.data[i]) > 3) big_steps++;
    if ((i==diff_arr.length-1) && (abs(diff_arr.data[i]) > 3)) big_step_last = true;
  }
  
  bool passed = false;
  if ((num_zeros == 0) && (sign_flips == 0) && (big_steps == 0)) {
    passed = true;
  } else if ((num_zeros < 2) && (sign_flips == 0) && (big_steps == 0)) {
    passed = true;
  } else if ((num_zeros == 0) && (sign_flips%2 == 0) && (big_steps == 0)) {
    passed = true;
  } else if ((num_zeros == 0) && (sign_flips == 1) && (sign_flip_last_or_first) && (big_steps == 0)) {
    passed = true;
  } else if ((num_zeros == 0) && (sign_flips == 0) && (big_step_last) && (big_steps == 1)) {
    passed = true;
  }

  SDM_ARRAY_FREE(diff_arr);
  return passed;
}

size_t safe_reports_part1(DynDynIntArray array_of_arrays) {
  size_t num_safe = 0;
  for (size_t report_num=0; report_num<array_of_arrays.length; report_num++) {
    DynIntArray report = array_of_arrays.data[report_num];
    if (report_is_safe_part1(report)) num_safe++;
  }

  return num_safe;
}

size_t safe_reports_part2(DynDynIntArray array_of_arrays) {
  size_t num_safe = 0;
  for (size_t report_num=0; report_num<array_of_arrays.length; report_num++) {
    DynIntArray report = array_of_arrays.data[report_num];
    if (report_is_safe_part2(report)) num_safe++;
  }

  return num_safe;
}

