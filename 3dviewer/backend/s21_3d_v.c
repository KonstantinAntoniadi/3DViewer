#include "s21_3d_v.h"

#include <stdio.h>

/**
 *
 * считаем кол-во вершин и фасетов
 */
int parse_obj_file(char *file_name, struct data *obj) {
  int status = -1;
  FILE *file = NULL;
  char buffer[1000000];
  obj->count_of_vertexes = 0;
  obj->count_of_facets = 0;
  obj->numbers_of_vertexes_in_facets = 0;
  if (!(file = fopen(file_name, "r"))) {
    printf("Ошибка: не удалось открыть файл\n");  // LCOV_EXCL_LINE
    status = -1;                                  // LCOV_EXCL_LINE
  } else {
    while (fgets(buffer, 1000000, file)) {
      if (strncmp(buffer, "v ", 2) == 0) {  // сравнивает первые 2 симв
        (obj->count_of_vertexes)++;
      } else if (strncmp(buffer, "f ", 2) == 0) {
        (obj->count_of_facets)++;
      } else if (obj->count_of_vertexes == 0) {
        status = -1;
      }
    }

    fclose(file);
    status = 1;
  }

  return status;
}
/**
 *
 * выделяем помять под массив кол-во вершин в фасетах
 */
int allocate_memory_for_array_vertexes_in_facets(char *file_name,
                                                 struct data *obj) {
  int status = 0;
  char buffer[1000000];
  char *istr = NULL;
  int i = 0;

  // Изменяем выделение памяти на основе obj->count_of_facets
  obj->vertexes = (int *)calloc(obj->count_of_facets, sizeof(int));
  if (obj->vertexes == NULL) {
    printf(  // LCOV_EXCL_LINE
        "Ошибка: не удалось выделить память под массив вершин "
        "многоугольников\n");  // LCOV_EXCL_LINE
    status = -1;               // LCOV_EXCL_LINE
  }

  FILE *file = fopen(file_name, "r");
  if (!file) {
    // Возвращаем -1 в случае ошибки открытия файла
    printf("Ошибка: не удалось открыть файл\n");  // LCOV_EXCL_LINE
    status = -1;                                  // LCOV_EXCL_LINE
  } else {
    while (fgets(buffer, 1000000, file) != NULL) {
      // Используем функцию sscanf для парсинга строк с помощью шаблона
      int vertex_num;
      if (sscanf(buffer, "f %d", &vertex_num) ==
          1) {  //проверить, начинается ли строка, содержащаяся в buffer, с
                //подстроки "f " (буква "f" и пробел), и если да, то извлекает
                //целое число
        obj->vertexes[i] = 0;
        // Используем функцию strtok для разделения строки на отдельные токены
        istr = strtok(buffer, "f ");  // указывает на первый токен
        while (istr != NULL) {
          int vertex_idx;
          // проверяем каждый токен на соответствие шаблону %d
          if (sscanf(istr, "%d", &vertex_idx) == 1) {
            (obj->vertexes[i]) += 1;
          }
          istr = strtok(NULL, " ");
        }
        i++;
      }
    }
    status = 1;
    fclose(file);
  }

  return status;
}

/**
 *
 * выделяем память для вершин в двойной массив т е в матрицу
 */
int allocate_memory_for_array_of_vertexes(char *file_name, struct data *obj) {
  int status = 0;
  char buffer[1000000];
  char *istr = NULL;

  obj->array_of_vertexes =
      (double **)calloc(obj->count_of_vertexes, sizeof(double *));
  if (obj->array_of_vertexes == NULL) {
    status = -1;  // LCOV_EXCL_LINE
  }
  for (unsigned int i = 0; i < obj->count_of_vertexes; i++) {
    obj->array_of_vertexes[i] = (double *)calloc(3, sizeof(double));
    if (obj->array_of_vertexes[i] == NULL) {
      // Ошибка выделения памяти, освобождаем выделенную ранее память
      for (unsigned int j = 0; j < i; j++) {  // LCOV_EXCL_LINE
        free(obj->array_of_vertexes[j]);      // LCOV_EXCL_LINE
      }
      free(obj->array_of_vertexes);  // LCOV_EXCL_LINE
      status = -1;                   // LCOV_EXCL_LINE
    }
  }

  FILE *file = fopen(file_name, "r");
  if (!file) {
    printf("Ошибка: не удалось открыть файл\n");  // LCOV_EXCL_LINE
    status = -1;                                  // LCOV_EXCL_LINE
  } else {
    for (int i = 0; !feof(file);) {
      fgets(buffer, 1000000, file);
      if (!strncmp(buffer, "v ", 2)) {
        istr = strtok(buffer, "v ");
        obj->array_of_vertexes[i][0] = atof(istr);
        istr = strtok(NULL, " ");
        obj->array_of_vertexes[i][1] = atof(istr);
        istr = strtok(NULL, " ");
        obj->array_of_vertexes[i][2] = atof(istr);
        i++;
      }
    }

    obj->single_array_for_vertexes =
        (double *)calloc((obj->count_of_vertexes * 3), sizeof(double));
    if (obj->single_array_for_vertexes) {
      for (unsigned int i = 0, k = 0; i < obj->count_of_vertexes; i++) {
        obj->single_array_for_vertexes[k] = obj->array_of_vertexes[i][0];
        obj->single_array_for_vertexes[k + 1] = obj->array_of_vertexes[i][1];
        obj->single_array_for_vertexes[k + 2] = obj->array_of_vertexes[i][2];
        k += 3;
      }
      status = 1;
    } else {
      status = -1;  // LCOV_EXCL_LINE
    }

    fclose(file);
  }

  return status;  // Успешное завершение
}

int allcocate_memory_for_array_of_facets(char *file_name, struct data *obj) {
  int status = 0;
  char buffer[1000000];

  obj->array_of_facets = (int **)calloc(obj->count_of_facets, sizeof(int *));
  if (obj->array_of_facets == NULL) {
    status = -1;  // LCOV_EXCL_LINE
  }
  for (unsigned int i = 0; i < obj->count_of_facets; i++) {
    obj->array_of_facets[i] = (int *)calloc(obj->vertexes[i], sizeof(int));
    if (obj->array_of_facets[i] == NULL) {
      status = -1;  // LCOV_EXCL_LINE
    }
  }

  FILE *file = fopen(file_name, "r");
  if (!file) {
    printf("Ошибка: не удалось открыть файл\n");  // LCOV_EXCL_LINE
    status = -1;                                  // LCOV_EXCL_LINE
  } else {
    int i = 0;
    while (fgets(buffer, 1000000, file)) {
      if (strncmp(buffer, "f ", 2) == 0) {
        char *istr = strtok(buffer, "f ");
        obj->array_of_facets[i][0] = atoi(istr);
        obj->numbers_of_vertexes_in_facets =
            obj->numbers_of_vertexes_in_facets + 1;
        for (int j = 1; j < obj->vertexes[i]; j++) {
          istr = strtok(NULL, " ");
          obj->array_of_facets[i][j] = atoi(istr);
          obj->numbers_of_vertexes_in_facets =
              obj->numbers_of_vertexes_in_facets + 1;
        }
        i++;
      }
      status = 1;
    }

    obj->numbers_of_vertexes_in_facets *= 2;
    obj->single_array_for_facets =
        (int *)calloc(obj->numbers_of_vertexes_in_facets, sizeof(int));

    if (obj->single_array_for_facets) {
      for (unsigned int i = 0, k = 0; i < obj->count_of_facets; i++) {
        for (int j = 0; j < (obj->vertexes[i]); j++) {
          if (j == 0) {
            obj->single_array_for_facets[k] = obj->array_of_facets[i][0];
            obj->single_array_for_facets[k + obj->vertexes[i] * 2 - 1] =
                obj->array_of_facets[i][j];
          } else {
            obj->single_array_for_facets[k - 1] = obj->array_of_facets[i][j];
            obj->single_array_for_facets[k] = obj->array_of_facets[i][j];
          }
          k += 2;
        }
      }
    } else {
      status = -1;  // LCOV_EXCL_LINE
    }

    fclose(file);
  }

  return status;
}

double findMinValue(double *vertexes, int count_vertexes,  // LCOV_EXCL_LINE
                    char find) {                           // LCOV_EXCL_LINE
  double res = vertexes[0];                                // LCOV_EXCL_LINE
  int index = 0;                                           // LCOV_EXCL_LINE
  if (find == 'y') {                                       // LCOV_EXCL_LINE
    index = 1;                                             // LCOV_EXCL_LINE
  } else if (find == 'z') {                                // LCOV_EXCL_LINE
    index = 2;                                             // LCOV_EXCL_LINE
  }

  for (int i = 0; i < count_vertexes * 3; i += 3) {  // LCOV_EXCL_LINE
    if (vertexes[i + index] < res) {                 // LCOV_EXCL_LINE
      res = vertexes[i + index];                     // LCOV_EXCL_LINE
    }
  }

  return res;  // LCOV_EXCL_LINE
}

double findMaxValue(double *vertexes, int count_vertexes,  // LCOV_EXCL_LINE
                    char find) {                           // LCOV_EXCL_LINE
  double res = vertexes[0];                                // LCOV_EXCL_LINE
  int index = 0;                                           // LCOV_EXCL_LINE
  if (find == 'y') {                                       // LCOV_EXCL_LINE
    index = 1;                                             // LCOV_EXCL_LINE
  } else if (find == 'z') {                                // LCOV_EXCL_LINE
    index = 2;                                             // LCOV_EXCL_LINE
  }

  for (int i = 0; i < count_vertexes * 3; i += 3) {  // LCOV_EXCL_LINE
    if (vertexes[i + index] > res) {                 // LCOV_EXCL_LINE
      res = vertexes[i + index];                     // LCOV_EXCL_LINE
    }
  }

  return res;  // LCOV_EXCL_LINE
}

double findMaxOfThree(double a, double b, double c) {  // LCOV_EXCL_LINE
  double max_value = a;                                // LCOV_EXCL_LINE
  if (b >= a && b >= c) {                              // LCOV_EXCL_LINE
    max_value = b;                                     // LCOV_EXCL_LINE
  } else if (c >= a && c >= b) {                       // LCOV_EXCL_LINE
    max_value = c;                                     // LCOV_EXCL_LINE
  }

  return max_value;  // LCOV_EXCL_LINE
}

void centering(double *vertexes, int count_vertexes) {         // LCOV_EXCL_LINE
  double x_min = findMinValue(vertexes, count_vertexes, 'x');  // LCOV_EXCL_LINE
  double y_min = findMinValue(vertexes, count_vertexes, 'y');  // LCOV_EXCL_LINE
  double z_min = findMinValue(vertexes, count_vertexes, 'z');  // LCOV_EXCL_LINE

  double x_max = findMaxValue(vertexes, count_vertexes, 'x');  // LCOV_EXCL_LINE
  double y_max = findMaxValue(vertexes, count_vertexes, 'y');  // LCOV_EXCL_LINE
  double z_max = findMaxValue(vertexes, count_vertexes, 'z');  // LCOV_EXCL_LINE

  double x_diff = x_max - x_min;           // LCOV_EXCL_LINE
  double y_diff = y_max - y_min;           // LCOV_EXCL_LINE
  double z_diff = z_max - z_min;           // LCOV_EXCL_LINE
  double x_middle = x_min + x_diff / 2.0;  // LCOV_EXCL_LINE
  double y_middle = y_min + y_diff / 2.0;  // LCOV_EXCL_LINE
  double z_middle = z_min + z_diff / 2.0;  // LCOV_EXCL_LINE

  double k = findMaxOfThree(x_diff, y_diff, z_diff) / 2.0;  // LCOV_EXCL_LINE

  for (int i = 0; i < count_vertexes * 3; i += 3) {  // LCOV_EXCL_LINE
    vertexes[i] -= x_middle;                         // LCOV_EXCL_LINE
    vertexes[i + 1] -= y_middle;                     // LCOV_EXCL_LINE
    vertexes[i + 2] -= z_middle;                     // LCOV_EXCL_LINE
    vertexes[i] /= k;                                // LCOV_EXCL_LINE
    vertexes[i + 1] /= k;                            // LCOV_EXCL_LINE
    vertexes[i + 2] /= k;                            // LCOV_EXCL_LINE
  }
}  // LCOV_EXCL_LINE
