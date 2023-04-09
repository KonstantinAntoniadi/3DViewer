#ifndef BACKEND_S21_3D_V_H
#define BACKEND_S21_3D_V_H

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data {
  unsigned int count_of_vertexes;
  unsigned int count_of_facets;
  double **array_of_vertexes;
  int **array_of_facets;
  int *vertexes;
  unsigned int numbers_of_vertexes_in_facets;
  int *single_array_for_facets;
  double *single_array_for_vertexes;
  double *single_array_for_vertexes_copy;
} data_t;

int parse_obj_file(char *file_name, struct data *obj);
int allocate_memory_for_array_vertexes_in_facets(char *file_name,
                                                 struct data *obj);
int allocate_memory_for_array_of_vertexes(char *file_name, struct data *obj);
int allcocate_memory_for_array_of_facets(char *file_name, struct data *obj);
double findMinValue(double *vertexes, int count_vertexes, char find);
double findMaxValue(double *vertexes, int count_vertexes, char find);
double findMaxOfThree(double a, double b, double c);
void centering(double *vertexes, int count_vertexes);

#endif  // BACKEND_S21_3D_V_H
