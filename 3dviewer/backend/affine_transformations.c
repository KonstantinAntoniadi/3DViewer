#include "affine_transformations.h"

void move_object(data_t *A, double mov_x, double mov_y, double mov_z) {
  for (unsigned int i = 0; i < A->count_of_vertexes * 3; i += 3) {
    A->single_array_for_vertexes[i] += mov_x;
    A->single_array_for_vertexes[i + 1] += mov_y;
    A->single_array_for_vertexes[i + 2] += mov_z;
  }
}

// Эта функция вращает объект на заданные углы по осям x, y и z.
void rotate_object(data_t *A, double angle_x, double angle_y, double angle_z) {
  double angleToRad = angle_x * M_PI / 180;
  double val_sin_x = sin(angleToRad);
  double val_cos_x = cos(angleToRad);

  angleToRad = angle_y * M_PI / 180;
  double val_sin_y = sin(angleToRad);
  double val_cos_y = cos(angleToRad);

  angleToRad = angle_z * M_PI / 180;
  double val_sin_z = sin(angleToRad);
  double val_cos_z = cos(angleToRad);

  for (unsigned int i = 0; i < A->count_of_vertexes * 3; i += 3) {
    double x = A->single_array_for_vertexes[i];
    double y = A->single_array_for_vertexes[i + 1];
    double z = A->single_array_for_vertexes[i + 2];

    // Вращение по оси X
    double temp_y = val_cos_x * y - val_sin_x * z;
    double temp_z = val_sin_x * y + val_cos_x * z;

    // Вращение по оси Y
    double temp_x = val_cos_y * x + val_sin_y * temp_z;
    temp_z = -val_sin_y * x + val_cos_y * temp_z;

    // Вращение по оси Z
    x = val_cos_z * temp_x - val_sin_z * temp_y;
    y = val_sin_z * temp_x + val_cos_z * temp_y;

    A->single_array_for_vertexes[i] = x;
    A->single_array_for_vertexes[i + 1] = y;
    A->single_array_for_vertexes[i + 2] = temp_z;
  }
}

void scale_object(data_t *A, double scale) {
  for (unsigned int i = 0; i < A->count_of_vertexes * 3; i++) {
    A->single_array_for_vertexes[i] *= scale;
  }
}

void free_struct(data_t *A) {
  if (A) {
    if (A->single_array_for_vertexes_copy) {
      free(A->single_array_for_vertexes_copy);   // LCOV_EXCL_LINE
      A->single_array_for_vertexes_copy = NULL;  // LCOV_EXCL_LINE
    }
    if (A->single_array_for_facets) {
      free(A->single_array_for_facets);
      A->single_array_for_facets = NULL;
    }
    if (A->vertexes) {
      free(A->vertexes);
      A->vertexes = NULL;
    }
    if (A->single_array_for_vertexes) {
      free(A->single_array_for_vertexes);
      A->single_array_for_vertexes = NULL;
    }

    for (unsigned int i = 0; i < A->count_of_vertexes; i++) {
      if (A->array_of_vertexes[i]) {
        free(A->array_of_vertexes[i]);
        A->array_of_vertexes[i] = NULL;
      }
    }

    if (A->array_of_vertexes) {
      free(A->array_of_vertexes);
      A->array_of_vertexes = NULL;
    }

    for (unsigned int i = 0; i < A->count_of_facets; i++) {
      if (A->array_of_facets[i]) {
        free(A->array_of_facets[i]);
        A->array_of_facets[i] = NULL;
      }
    }

    if (A->array_of_facets) {
      free(A->array_of_facets);
      A->array_of_facets = NULL;
    }

    if (A) {
      free(A);
      A = NULL;
    }
  }
}
