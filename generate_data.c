#include <stdio.h>
#include <stdlib.h>
#include "generate_data.h"

//从file_path的文件中读取size个元素到数组中并返回数组
int *get_data_from_file(const char *file_path, int size){
  int *data = (int *)malloc(size * sizeof(int));
  FILE *fp;
  int element;
  int i = 0;

  if (data == NULL){
    printf("@%s: error, data=NULL\n", __FUNCTION__);
    return NULL;
  }

  //先从文件中读取所有数据
  fp = fopen(file_path, "r");
  if (fp == NULL){
    printf("@%s: error, cannot open file %s\n", __FUNCTION__, file_path);
    return NULL;
  }
  while ((fscanf(fp, "%d", &element) != EOF)
      && (i < size)){
    data[i++] = element;
  }
  fclose(fp);

  return data;
}

//print all elements in the data array
void print_all_data(int *data, int size){
  int i;
  for (i = 0; i < size; i++){
    printf("%d ", data[i]);
  }
  printf("\n");
}
