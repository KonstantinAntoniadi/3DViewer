#include <check.h>
#include <stdio.h>
#include "../3dviewer/backend/affine_transformations.h"


#define eps 1e-7

START_TEST(viewer_test) {
    char* filepath = "/Users/edwinevi/Desktop/C8_3DViewer_v1.0-2/src/tests/cube.obj";
    FILE* file = fopen(filepath, "r");
    ck_assert_ptr_nonnull(file);
    if (file) {
        fclose(file);
    }

    data_t * obj = calloc(1, sizeof(data_t));
    ck_assert_int_eq(parse_obj_file(filepath, obj), 1);
    ck_assert_int_eq(allocate_memory_for_array_of_vertexes(filepath, obj), 1);
    obj->numbers_of_vertexes_in_facets = 0;
    ck_assert_int_eq(allocate_memory_for_array_vertexes_in_facets(filepath, obj), 1);
    ck_assert_int_eq(allcocate_memory_for_array_of_facets(filepath, obj), 1);
    ck_assert_uint_eq(obj->count_of_vertexes, 8);
    ck_assert_uint_eq(obj->count_of_facets, 10);
    ck_assert_uint_eq(obj->numbers_of_vertexes_in_facets, 60);
    
    double standart_array_vertexs[] = {
            -1.0,  -1.0,  -1.0,
            -1.0,  -1.0,  1.0,
            -1.0,  1.0,  -1.0,
            -1.0,  1.0,  1.0,
            1.0,  -1.0,  -1.0,
            1.0,  -1.0,  1.0,
            1.0,  1.0,  -1.0,
            1.0,  1.0,  1.0
    };

    unsigned int standart_array_facets[] = {
           1, 7, 7, 5, 5, 1,
           1, 3, 3, 7, 7, 1,
           1, 4, 4, 3, 3, 1,
           1, 2, 2, 4, 4, 1,
           3, 8, 8, 7, 7, 3,
           3, 4, 4, 8, 8, 3,
           5, 7, 7, 8, 8, 5,
           5, 8, 8, 6, 6, 5,
           1, 5, 5, 6, 6, 1,
           1, 6, 6, 2, 2, 1
    };

    for (unsigned int i = 0; i < obj->count_of_vertexes * 3; i++) {
        ck_assert_double_eq_tol(obj->single_array_for_vertexes[i], standart_array_vertexs[i], eps);
    }

    for (unsigned int i = 0; i < obj->numbers_of_vertexes_in_facets; i++) {
        ck_assert_uint_eq(obj->single_array_for_facets[i], standart_array_facets[i]);
    }

    double standart_array_vertexs_after_move[] = {
        9.0,  14.0,  -6.0,
        9.0,  14.0, -4.0,
        9.0,  16.0,  -6.0,
        9.0,  16.0,  -4.0,
        11.0,  14.0,  -6.0,
        11.0,  14.0,  -4.0,
        11.0,  16.0,  -6.0,
        11.0,  16.0,  -4.0
    };

    move_object(obj, 10.0, 15.0, -5.0);

    for (unsigned int i = 0; i < obj->count_of_vertexes * 3; i++) {
        ck_assert_double_eq_tol(obj->single_array_for_vertexes[i], standart_array_vertexs_after_move[i], eps);
    }

    double standart_array_vertexs_after_scale[] = {
        4.5,  7.0,  -3.0,
        4.5,  7.0,  -2.0,
        4.5,  8.0,  -3.0,
        4.5,  8.0,  -2.0,
        5.5,  7.0,  -3.0,
        5.5,  7.0,  -2.0,
        5.5,  8.0,  -3.0,
        5.5,  8.0,  -2.0
    };

    scale_object(obj, 0.5);
    for (unsigned int i = 0; i < obj->count_of_vertexes * 3; i++) {
        ck_assert_double_eq_tol(obj->single_array_for_vertexes[i], standart_array_vertexs_after_scale[i], eps);
    }

    rotate_object(obj, 360, 360, 360);
    for (unsigned int i = 0; i < obj->count_of_vertexes * 3; i++) {
        ck_assert_double_eq_tol(obj->single_array_for_vertexes[i], standart_array_vertexs_after_scale[i], eps);
    }

    free_struct(obj);

}
END_TEST


Suite *viewer_test_suite(void) {
  Suite *s1 = suite_create("3dviewer_test: ");
  TCase *tc1_1 = tcase_create("3dviewer_test: ");
  tcase_add_test(tc1_1, viewer_test);
  

  suite_add_tcase(s1, tc1_1);
  return s1;
}

void srunner_test(Suite *s, int *no_failed) {
  SRunner *srunner = srunner_create(s);
  srunner_run_all(srunner, CK_NORMAL);
  *no_failed = srunner_ntests_failed(srunner);
  srunner_free(srunner);
}

int main() {
  int no_failed = 0;
  srunner_test(viewer_test_suite(), &no_failed);

  return (no_failed == 0) ? 0 : 1;
}
