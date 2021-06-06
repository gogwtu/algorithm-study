#include <stdio.h>
#include <stdlib.h>
#include "generate_data.h"

//read size elements from file_path and return a data array
int *get_data_from_file(const char *file_path, int size){
  int *data = (int *)malloc(size * sizeof(int));
  FILE *fp;
  int element;
  int i = 0;

  if (data == NULL){
    printf("@%s: error, data=NULL\n", __FUNCTION__);
    return NULL;
  }

  //open the file
  fp = fopen(file_path, "r");
  if (fp == NULL){
    printf("@%s: error, cannot open file %s\n", __FUNCTION__, file_path);
    return NULL;
  }

  //read file data into an array
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
