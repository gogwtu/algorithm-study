#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "generate_data.h"

//任何需要交换相邻两个元素的排序方法,其开销都是O(N^2)
//要想在排序时间上获得突破,基本思路是交换相距较远的元素
//这样也很好理解:交换相距较远的元素可以一次性消除更多的逆序对
//而交换相邻的元素只能一次消除一个逆序对
//每个逆序对就是i<j但是data[i] > data[j]

//冒泡排序:每次比较相邻两个元素,把大的交换到后面
//这样一轮下来最大的元素就被排序到了数组后面,就像气泡一样把大的气泡冒到最后
//进行n轮就完成了所有数据的排序
int bubble_sort(int *data, int size){
  int i, j;
  for (i = 0; i < size; i++){
    for (j = 0; j < size - 1; j++){
      if (data[j] > data[j + 1]){
        //swap data[j] and data[j+1]
        swap(&data[j], &data[j + 1]);
      }
    }
  }
  return 0;
}

//选择排序:每次选择剩余数组中最小的元素,把它们依次存放到数组前面的末尾
//这样数组的前面就是由小到大排序的元素,后面就是待排序的元素
int selection_sort(int *data, int size){
  int i, j;
  int element;//第i小的元素的值
  int position;//第i小的元素的数组位置
  for (i = 0; i < size; i++){
    //从i+1到size中选取最小的值data[position]放到i处
    //然后把原来的i处的值data[i]与data[position]交换
    element = data[i];
    position = i;
    for (j = i + 1; j < size; j++){
      if (data[j] < element){
        position = j;
        element = data[j];
      }
    }
    data[position] = data[i];
    data[i] = element;
  }

  return 0;
}

//插入排序:每次把一个数组元素插入到它前面已经排序的所有元素中
int insertion_sort(int *data, int size){
  int i, j;
  int element;

  for (i = 1; i < size; i++){
    element = data[i];
    //insert data[i] into data[0]~data[i - 1]
    for (j = i; j > 0; j--){
      if (element < data[j - 1]){
        data[j] = data[j - 1];
      } else {
        break;
      }
    }
    data[j] = element;
  }

  return 0;
}

//希尔排序:缩小增量排序,将相距为increment的每个子数组进行一次插入排序
//增量是由大逐渐缩小的,不同的增量序列性能不同,序列的最后一个数值都必须是1
//这里使用的是书上的序列,即n/2,n/4,...,1,将增量increment从n/2缩小到1的过程
//而当increment=1时就完全是插入排序了
int shell_sort(int *data, int size){
  int i, j;
  int increment;
  int element;

  for (increment = size / 2; increment > 0; increment /= 2){
    for (i = increment; i < size; i++){
      element = data[i];
      for (j = i; j >= increment; j -= increment){
        if (element < data[j - increment]){
          data[j] = data[j - increment];
        } else {
          break;
        }
      }
      data[j] = element;
    }
  }

  return 0;
}

//把两个已排序的子数组data[0 ~ (size/2 - 1)](数组a)和data[size/2 ~ (size - 1)](数组b)合并到数组c中
static void merge(int *data, int size, int *merged_c){
  int position_a = 0, position_b = size / 2, position_c = 0;
  int size_a = size / 2, size_b = size;

  //依次将a和b中的较小者放到数组c中去
  while (position_c < size){
    if (data[position_a] < data[position_b]){
      merged_c[position_c++] = data[position_a++];
      if (position_a == size_a){//a到底了,将b中的剩余元素都拷贝到c中
        while (position_b < size_b){
          merged_c[position_c++] = data[position_b++];
        }
      }
    } else {
      merged_c[position_c++] = data[position_b++];
      if (position_b == size_b){//b到底了,将a中的剩余元素都拷贝到c中
        while (position_a < size_a){
          merged_c[position_c++] = data[position_a++];
        }
      }
    }
  }
}

//将数组二分为两个子数组,分别进行归并排序,然后再将结果进行合并
//最后将合并后的结果拷贝回原来的数组中
static int merge_sort_recursive(int *data, int size, int *merged_c){
  if (size > 1){
    int i;

    merge_sort_recursive(data, size / 2, merged_c);
    merge_sort_recursive(data + (size / 2), size - (size / 2), merged_c + (size / 2));
    merge(data, size, merged_c);
    //将临时空间c中的内容拷贝到a和b中
    //strncpy((char *)data, (char *)merged_c, size * sizeof(int));
    for (i = 0; i < size; i++){
      data[i] = merged_c[i];
    }
  }
  return 0;
}

//归并排序:分冶+递归
//基本思想是将一个待排序的数组分成两个需要排序的部分
//然后各个部分分别排序,最后再对二者进行合并,将合并结果放到第三个地方
int merge_sort(int *data, int size){
  int *tmp_result = (int *)malloc(size * sizeof(int));
  if (tmp_result == NULL){
    printf("%s: error, out of memory\n", __FUNCTION__);
    return -1;
  }
  merge_sort_recursive(data, size, tmp_result);
  free(tmp_result);

  return 0;
}

//使用最大堆,即根元素是堆中最大的数
//下滤:将指定位置的节点和它的子节点比较,如果比子节点中最大的元素小则与之交换,相当于不断下滤
//为了避免中途进行元素交换,只在下滤的最后才把需要把最后一个准备交换的元素放到该节点
//从最下层的父节点开始,一个个往上层构建,这样从下往上层的元素就是有堆序的,
//然后在将指定元素下滤的过程,当发现它比某个父节点的儿子都大的时候,就不用再往下滤了
//因为儿子和孙子之间已经是有序的
static void percolate_down(int *data, int size, int position){
  int i;
  int child;
  int element;
  element = data[position];
  for (i = position; (i * 2 + 1) < size; i = child){
    child = (i * 2 + 1);
    //先找出子节点中的最大者
    if ((child != size - 1) && data[child] < data[child + 1]){
      child++;
    }
    if (element < data[child]){
      data[i] = data[child];
    } else {
      break;
    }
  }
  data[i] = element;
}

//堆排序:利用堆(优先队列)的性质:所有父节点的数值都比其子节点大(最大堆)或小(最小堆)
//然后不断地移除堆的根节点,这样得到的就是由大小到或由小到大排序后的结果
int heap_sort(int *data, int size){
  int position;
  int element;

  //直接将data[]当作下标从0开始的原始堆数据,size是它的数据数量
  //先构建max-heap,size/2的位置就是它最下层的父节点,从该节点开始逐渐往上层构建堆
  for (position = size / 2; position >= 0; position--){
    percolate_down(data, size, position);
  }

  //排序:每次移除根节点,然后将它存放到末尾
  //每次移除根元素后,堆的实际大小会减小1,这样堆的末尾就会空出一项出来,用它来存放被移除的根元素
  //最后得到的data[]就是由小到大排序后的元素
  for (position = size - 1; position >= 0; position--){
    //交换根节点和堆的末尾节点,此时堆的末尾节点data[position]就是第position+1小的元素
    element = data[0];
    data[0] = data[position];
    data[position] = element;
    //由于原始堆的末尾节点被放到根节点,需要再重新从它开始下滤以满足堆序特性
    percolate_down(data, position, 0);
  }
  return 0;
}

//快速排序的思想,是对于一个数组,先确定一个枢纽元素,再把所有小于它的数都移到左边,大于它的都移到右边
//采用三数值分割法确定枢纽元素,即以数组左,中,右三个点的值进行排序,大小中等的元素作为枢纽元素
//三个排序的数中,按照由小到大的顺序,分别放在数组的left(最左边),right-1和right(数组最右边)的位置
//然后使用两个游标i和j,其中i从left+1的位置开始向右移动,j从right-2(枢纽元素左边)的位置开始向左移动.
//i一直向右移动,跳过比枢纽元素小的所有元素
//j一直向左移动,跳过比枢纽元素大的所有元素
//当二者无法移动时,就说明i处的元素大于(或等于)枢纽元素,j处的元素小于(或等于)枢纽元素,
//于是将i和j处的元素交换
//然后继续上述过程,直到i和j的位置交错,即i到了j的右边,而j到了i的左边
//再将枢纽元素与i交换,这样枢纽元素的左边都是比它小的,右边都是比它大的
//然后再对枢纽元素的左边和右边进行同样的操作,即递归地处理左边和右边即可

//采用left,middle,right三数值分割法来确定枢纽元素
//枢纽是left,right,(left+right)/2位置的中间大小元素
//确定枢纽:确定了枢纽后,就把它和right-1位置的元素进行交换
//由调用者来保证至少有3个元素,即right > left + 1
static void quick_sort_pivot(int *data, int left, int right){
  int min, max;
  int p = (left + right) / 2;

  //对left,p,right三者进行排序
  if (data[left] > data[p]){
    swap(&data[left], &data[p]);
  }
  if (data[p] > data[right]){
    swap(&data[p], &data[right]);
  }
  if (data[left] > data[p]){
    swap(&data[left], &data[p]);
  }
  //再将p和right-1处的元素进行交换
  swap(&data[p], &data[right - 1]);
}

//递归进行排序:先将所有小于枢纽元素的数移到它左边,再将所有大于它的数移到它右边
//再分别对枢纽左边的子数组和右边的子数组进行递归的快速排序
static int quick_sort_recursive(int *data, int left, int right){
  int i, j; //两个游标
  int pivot;  //枢纽元素位置

  //只有1-2个元素的时候不需要使用快速排序
  if (right <= left + 1){
    if (data[left] > data[right]){
      swap(&data[left], &data[right]);
    }
    return 0;
  }

  //先确定left和right之间的枢纽
  quick_sort_pivot(data, left, right);
  //如果刚好只有3个元素,那么在经过上面的确定枢纽操作后它们就已经排好序了,直接结束
  if (right == left + 2){
    return 0;
  }

  pivot = right - 1;
  i = left + 1;
  j = right - 2;

  while (i <= j){
    //此时data[left] < data[right -1] < data[right]
    //将data[right - 1]作为枢纽元素,把数组中所有小于它的元素都移动到它的左边
    //大于它的元素都移动到它的右边
    //方法是使用两个游标i和j
    while (data[i] < data[pivot]){
      i++;
    }
    while (data[j] > data[pivot]){
      j--;
    }

    if (i < j){
      if (data[i] != data[j]){
        swap(&data[i], &data[j]);
      } else {
        //单独处理data[i(j)]和data[pivot]相等的情况
        //不能放到上面的移动i(j)游标的while循环中判断
        //原因是那样的话在data[i]和边界值相等时游标会超出边界
        if (data[i] == data[pivot]){
          i++;
        }
        if (data[j] == data[pivot]){
          j--;
        }
      }
    } else {
      swap(&data[i], &data[pivot]);
      break;
    }
  }

  //至此,i处的位置是枢纽元素,i左边的元素都小于它,右边的元素都大于它
  quick_sort_recursive(data, left, i - 1);
  quick_sort_recursive(data, i + 1, right);

  return 0;
}

int quick_sort(int *data, int size){
  if (size > 1){
    quick_sort_recursive(data, 0, size - 1);
  }

  return 0;
}

int main(int argc, char *argv[]){
  char file_path[256];
  int *data;
  int size = 0;
  int m = 0;
  int opt;
#ifdef MEASURE_TIME
  struct timespec time_start, time_end;
  unsigned long elapsed_time;
#endif

  //parse command line parameters
  //-f: the file path containing all data to be sorted, each integer is separated by a space
  //-s: the size of the data
  //-m: choosing a sorting method
  //-h: print help
  while ((opt = getopt(argc, argv, "f:s:m:h")) != -1){
    switch (opt){
      case 'f'://data source file
        sprintf(file_path, "%s", optarg);
        break;
      case 's'://data size
        size = atoi(optarg);
        break;
      case 'm'://sorting method
        m = atoi(optarg);
        break;
      case 'h'://help
      default:
        printf("./sorts -f file_path -s size -m sort_method [-h]\n");
        printf("0-bubble_sort\n1-selection_sort\n2-insertion_sort\n3-shell_sort\n4-merge-sort\n5-heap-sort\n6-quick_sort\n");
        return 0;
    }
  }

  if (size == 0){
    printf("@%s: error, size=0\n", __FUNCTION__);
    return -1;
  }
  if (m > 6){
    printf("sort_method includes:\n");
    printf("0-bubble_sort\n1-selection_sort\n2-insertion_sort\n3-shell_sort\n4-merge-sort\n5-heap-sort\n6-quick_sort\n");
    return -1;
  }

  //read all data to be sorted to a data array
  data = get_data_from_file(file_path, size);
  if (data == NULL){
    return -1;
  }

  //before: original data
  printf("+++++++++++++++++++++++++++++++++++++initial data+++++++++++++++++++++++++++++++++++++++\n");
  print_all_data(data, size);

  switch (m){
    case 0: //bubble_sort
      printf("-----------------------------------bubble_sort result-----------------------------------\n");
      break;
    case 1: //selection-sort
      printf("----------------------------------selection_sort result---------------------------------\n");
      break;
    case 2: //insertion-sort
      printf("----------------------------------insertion_sort result---------------------------------\n");
      break;
    case 3: //shell-sort
      printf("------------------------------------shell_sort result-----------------------------------\n");
      break;
    case 4: //merge-sort
      printf("------------------------------------merge_sort result-----------------------------------\n");
      break;
    case 5: //heap-sort
      printf("-------------------------------------heap_sort result-----------------------------------\n");
      break;
    case 6: //quick-sort
      printf("------------------------------------quick_sort result-----------------------------------\n");
      break;
    default:
      break;
  }

  //start sorting
#ifdef MEASURE_TIME
      clock_gettime(CLOCK_MONOTONIC, &time_start);
#endif
  switch (m){
    case 0: //bubble_sort
      bubble_sort(data, size);
      break;
    case 1: //selection-sort
      selection_sort(data, size);
      break;
    case 2: //insertion-sort
      insertion_sort(data, size);
      break;
    case 3: //shell-sort
      shell_sort(data, size);
      break;
    case 4: //merge-sort
      merge_sort(data, size);
      break;
    case 5: //heap-sort
      heap_sort(data, size);
      break;
    case 6: //quick-sort
      quick_sort(data, size);
      break;
    default:
      break;
  }
#ifdef MEASURE_TIME
      clock_gettime(CLOCK_MONOTONIC, &time_end);
#endif

  //after: sorted data
  print_all_data(data, size);

#ifdef MEASURE_TIME
  elapsed_time = (time_end.tv_sec - time_start.tv_sec) * 1e9
    + (time_end.tv_nsec - time_start.tv_nsec);
  printf("------------------------------------total %lu ns-----------------------------------\n",
      elapsed_time);
#endif

  //free data space
  if (data){
    free(data);
  }

  return 0;
}
