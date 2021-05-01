#ifndef _UTILS_H
#define _UTILS_H

#define MEASURE_TIME

//read data from file_path, size is the total number of elements
int *get_data_from_file(const char *file_path, int size);
//print all elements in the data array
void print_all_data(int *data, int size);

//swap two elements
static inline void swap(int *a, int *b){
  int tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}
#endif  //_UTILS_H
