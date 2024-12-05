#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

typedef struct {
  size_t capacity;
  size_t length;
  int *data;
} DynRulesArray;

typedef struct {
  size_t capacity;
  size_t length;
  DynRulesArray *data;
} DynUpdateArray;

int get_middle(DynRulesArray update);
bool update_consistent_with_rules(DynRulesArray update, DynRulesArray lhs, DynRulesArray rhs);

int main(void) {
  // char *input_file = "./test.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);
  sdm_string_view contents_view = sdm_cstr_as_sv(file_contents);
  
  DynRulesArray lhs = {0}, rhs = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(lhs, 1024);
  SDM_ENSURE_ARRAY_MIN_CAP(rhs, 1024);
  while (contents_view.data[0] != '\n') {
    sdm_string_view rule = sdm_sv_pop_by_delim(&contents_view, '\n');
    SDM_ARRAY_PUSH(lhs, sdm_sv_pop_integer(&rule));
    if (rule.data[0] != '|') {
      fprintf(stderr, "Something is weird with the input data for the rules\n");
      exit(1);
    }
    sdm_sv_pop_one_char(&rule);
    SDM_ARRAY_PUSH(rhs, sdm_sv_pop_integer(&rule));
  }

  sdm_sv_pop_one_char(&contents_view);

  assert(lhs.length == rhs.length && "Rule sets are different sizes!");

  DynUpdateArray updates = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(updates, 1024);
  while (contents_view.length) {
    sdm_string_view update_sv = sdm_sv_pop_by_delim(&contents_view, '\n');
    SDM_ARRAY_PUSH(updates, (DynRulesArray){0});
    SDM_ENSURE_ARRAY_MIN_CAP(updates.data[updates.length - 1], 1024);
    while (update_sv.length) {
      if (update_sv.data[0] == '\n') break;
      int next_val = sdm_sv_pop_integer(&update_sv);
      SDM_ARRAY_PUSH(updates.data[updates.length - 1], next_val);
      assert((update_sv.data[0] == ',') || (update_sv.data[0] == '\n') && "bad input data?");
      if (update_sv.data[0] == ',') sdm_sv_pop_one_char(&update_sv);
    }
  }

  int part1_answer = 0;
  int part2_answer = 0;
  for (size_t i=0; i<updates.length; i++) {
    if (update_consistent_with_rules(updates.data[i], lhs, rhs)) {
      part1_answer += get_middle(updates.data[i]);
    } else {
      while (!update_consistent_with_rules(updates.data[i], lhs, rhs));
      part2_answer += get_middle(updates.data[i]);
    }
  }

  printf("Part 1 answer = %d\n", part1_answer);
  printf("Part 2 answer = %d\n", part2_answer);

  free(file_contents);

  return 0;
}

int get_middle(DynRulesArray update) {
  assert(update.length % 2 == 1 && "Even lengths have no middle!");
  return update.data[(update.length - 1) / 2];
}

bool update_consistent_with_rules(DynRulesArray update, DynRulesArray lhs, DynRulesArray rhs) {
  assert(lhs.length == rhs.length && "Rule arrays have inconsistent lengths");
  bool result = true;

  for (size_t i=0; i<lhs.length; i++) {
    int lhs_i = lhs.data[i];
    int rhs_i = rhs.data[i];
    int index_lhs = -1;
    int index_rhs = -1;
    for (size_t j=0; j<update.length; j++) {
      if (update.data[j] == lhs_i) index_lhs = j;
      if (update.data[j] == rhs_i) index_rhs = j;
      if ((index_lhs > 0) && (index_rhs > 0)) break;
    }
    if ((index_lhs < 0) || (index_rhs < 0) || (index_lhs < index_rhs)) {
      continue;
    } else {
      result = false;
      SDM_ARRAY_SWAP(update, index_lhs, index_rhs);
    }
  }

  return result;
}

