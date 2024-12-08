#include <stdbool.h>
#include <string.h>
#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"

int b_gcd(int num1, int num2);

typedef struct {
  size_t capacity;
  size_t length;
  sdm_string_view *data;
} SVArray;

typedef struct {
  int rownum, colnum;
} Coord;

bool coord_in_grid(Coord x, int numrows, int numcols) {
  return (
    (0 <= x.rownum) && (x.rownum < numrows) &&
    (0 <= x.colnum) && (x.colnum < numcols)
  );
}

Coord coord_add(Coord a, Coord b) {
  return (Coord){
    .rownum = a.rownum + b.rownum,
    .colnum = a.colnum + b.colnum,
  };
}

Coord coord_sub(Coord a, Coord b) {
  return (Coord){
    .rownum = a.rownum - b.rownum,
    .colnum = a.colnum - b.colnum,
  };
}

Coord coord_scale(Coord x, int c) {
  x.rownum *= c;
  x.colnum *= c;
  return x;
}

Coord coord_simplify(Coord a) {
  int gcd = b_gcd(a.rownum, a.colnum);
  a.rownum /= gcd;
  a.colnum /= gcd;
  return a;
}

typedef struct {
  size_t capacity;
  size_t length;
  Coord *data;
} CoordArray;

#define NUMCOORDS 128
CoordArray coords[NUMCOORDS] = {0};

typedef struct {
  size_t capacity;
  size_t length;
  char *data;
} SizedStr;

bool char_in_sstr(SizedStr sstr, char c) {
  for (size_t i=0; i<sstr.length; i++) {
    if (sstr.data[i] == c) return true;
  }
  return false;
}

/**
* Stein's Algorithm .
* @author Andrei Ciobanu
* @date DEC 12, 2010
*/
int b_gcd(int num1, int num2) {
	if (num1 == num2) return (num1);
	if (!num1 && !num2) {
		/* Convention: GCD(0, 0) = 0 */
		return 0;
	}
	if (!num1 || !num2) {
		// GCD(0, num2) = num2 
		// GCD(num1, 0) = num1
		return (num1 | num2);
	}
	if ( !(num1 & 1) && !(num2 & 1)) {
		// num1 and num2 are even, 
		// then gcd(num1, num2) = 2 * gcd(num1/2, num2/2)
		return (b_gcd(num1 >> 1, num2 >> 1) << 1);
	}
	if ( !(num1 & 1) && (num2 & 1)) {
		// num1 is even, and num2 is odd 
		// then gcd(num1, num2) = gcd(num1/2, num2)
		return b_gcd(num1 >> 1, num2);
	}
	if ( (num1 & 1) && !(num2 & 1)) {
		// num1 is odd, and num2 is even
		// then gcd(num1, num2) = gcd(num1, num2/2)
		return b_gcd(num1, num2 >> 1);
	}
	if ( (num1 & 1) && (num2 & 1)) {
		// num1 and num2 are odd
		if(num1 >= num2) {
			return b_gcd((num1 - num2) >> 1, num1);
		}
		else {
			return b_gcd((num2 - num1) >> 1, num1);
		}
	}
	return 0;
}

int main(void) {
  // char *input_file = "./test.txt";
  char *input_file = "./input.txt";
  char *file_contents = sdm_read_entire_file(input_file);

  char *grid_copy = malloc(strlen(file_contents) * sizeof(char));
  if (grid_copy == NULL) {
    fprintf(stderr, "Memory error :(\n");
    return 1;
  }
  strcpy(grid_copy, file_contents);
  sdm_string_view grid_copy_sv = sdm_cstr_as_sv(grid_copy);

  SVArray gridrows = {0};
  SDM_ENSURE_ARRAY_MIN_CAP(gridrows, 1024);
  while (grid_copy_sv.length > 0) {
    sdm_sv_trim(&grid_copy_sv);
    SDM_ARRAY_PUSH(gridrows, sdm_sv_pop_by_delim(&grid_copy_sv, '\n'));
  }

  size_t rownum = 0, colnum = 0;
  for (size_t i=0; i<strlen(file_contents); i++) {
    if (file_contents[i] == '\n') {
      rownum++;
      colnum = 0; 
      continue;
    }
    if (file_contents[i] != '.') {
      SDM_ARRAY_PUSH(coords[(size_t)file_contents[i]], ((Coord){.rownum=rownum, .colnum=colnum}));
    }
    colnum++;
  }

  for (size_t i=0; i<NUMCOORDS; i++) {
    if (coords[i].length ==0) continue;
    CoordArray c_arr = coords[i];
    for (size_t j=0; j<c_arr.length; j++) {
      Coord start = c_arr.data[j];
      for (size_t k=0; k<c_arr.length; k++) {
        if (j == k) continue;
        Coord end = c_arr.data[k];
        Coord diff = coord_sub(end, start);
        Coord back = coord_sub(start, diff);
        Coord forward = coord_add(end, diff);
        if (coord_in_grid(back, gridrows.length, gridrows.data[0].length)) {
          gridrows.data[back.rownum].data[back.colnum] = '#';
        }
        if (coord_in_grid(forward, gridrows.length, gridrows.data[0].length)) {
          gridrows.data[forward.rownum].data[forward.colnum] = '#';
        }
      }
    }
  }

  size_t part1_ans = 0;
  for (size_t i=0; i<strlen(grid_copy); i++) {
    if (grid_copy[i] == '#') part1_ans++;
  }
  printf("Part 1 = %zu\n", part1_ans);

  strcpy(grid_copy, file_contents);

  for (size_t i=0; i<NUMCOORDS; i++) {
    if (coords[i].length ==0) continue;
    CoordArray c_arr = coords[i];
    for (size_t j=0; j<c_arr.length; j++) {
      Coord start = c_arr.data[j];
      for (size_t k=0; k<c_arr.length; k++) {
        if (j == k) continue;
        Coord end = c_arr.data[k];
        Coord diff = coord_simplify(coord_sub(end, start));
        int scale = 0;
        Coord new_coord = coord_add(start, coord_scale(diff, scale));
        while (coord_in_grid(new_coord, gridrows.length, gridrows.data[0].length)) {
          gridrows.data[new_coord.rownum].data[new_coord.colnum] = '#';
          scale++;
          new_coord = coord_add(start, coord_scale(diff, scale));
        }
        scale = -1;
        new_coord = coord_add(start, coord_scale(diff, scale));
        while (coord_in_grid(new_coord, gridrows.length, gridrows.data[0].length)) {
          gridrows.data[new_coord.rownum].data[new_coord.colnum] = '#';
          scale--;
          new_coord = coord_add(start, coord_scale(diff, scale));
        }
      }
    }
  }

  size_t part2_ans = 0;
  for (size_t i=0; i<strlen(grid_copy); i++) {
    if (grid_copy[i] == '#') part2_ans++;
  }
  printf("Part 2 = %zu\n", part2_ans);
  free(file_contents);

  return 0;
}

