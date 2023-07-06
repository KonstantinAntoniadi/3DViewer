#ifndef BACKEND_AFFINE_TRANSFORMATIONS_H
#define BACKEND_AFFINE_TRANSFORMATIONS_H

#include "3d_v.h"

void move_object(data_t *A, double mov_x, double mov_y, double mov_z);

void rotate_object(data_t *A, double angle_x, double angle_y, double angle_z);

void scale_object(data_t *A, double scale);

void free_struct(data_t *A);

#endif  // BACKEND_AFFINE_TRANSFORMATIONS_H
