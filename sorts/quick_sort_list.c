#include <stdio.h>
#include <stdlib.h>

struct list_node {
  int val;
  struct list_node *next;
};

//当node->next = end时,表示待排序的链表结束
static struct list_node *qsort_list_recursive(struct list_node *head, struct list_node *end){
  if (head == end || head->next == end){
    return head;
  }
  struct list_node *pivot;
  struct list_node *low_node_start;
  struct list_node *low_node_end;//比pivot->val小的节点
  struct list_node *current;//当前遍历的节点

  //把小于Pivot的整个链表插入到head节点前面
  //大于pivot的整个链表直接绕过
  //这样当一轮结束时,pivot左边的所有节点值都小于等于它
  //而右边的所有节点值都大于等于它
  //先把head和head->next的位置进行确定
  if (head->next->val < head->val){
    //把head和head->next进行交换
    //临时变量:head,pivot,current
    //需要交换成为pivot,head,current
    pivot = head->next;
    current = pivot->next;
    head->next = current;
    pivot->next = head;
    head = pivot;
  }
  //此时的大小关系为head(pivot)<=current
  pivot = head;
  current = head->next;

  while (current != end && current->next != end){
    //current是肯定大于Pivot的
    //绕过那些大于pivot的大链表节点
    do {
      if (current->next->val >= pivot->val){
        current = current->next;
      } else {
        break;
      }
    } while (current->next != end);

    //从current->next开始的节点值小于pivot
    if (current->next != end){
      low_node_start = current->next;
      low_node_end = low_node_start;
      while (low_node_end->next != end){
        if (low_node_end->next->val <= pivot->val){
          low_node_end = low_node_end->next;
        } else {
          break;
        }
      }
      //此时low_node_start~low_node_end区间的所有节点值都小于pivot
      //将该小链表都移到链表的头部
      current->next = low_node_end->next;
      current = current->next;
      low_node_end->next = head;
      head = low_node_start;
    }
  }
  //至此,head~pivot的值都小于等于pivot
  //pivot->next~end的值都大于等于pivot
  if (head != pivot){
    head = qsort_list_recursive(head, pivot);
  }
  if (pivot->next != end){
    pivot->next = qsort_list_recursive(pivot->next, end);
  }

  return head;
}

struct list_node *quick_sort_list(struct list_node *head){
  if (head && head->next){
    head = qsort_list_recursive(head, NULL);
  }

  return head;
}

int main(int argc, char *argv[]){
  struct list_node *list_head = NULL;
  struct list_node *node;
  struct list_node *prev_node;
  struct list_node *next_node;
  int size = 100; //data set size
  int i;

  for (i = 0; i < size; i++){
    node = (struct list_node *)malloc(sizeof(struct list_node));
    node->val = rand() % 100;
    node->next = NULL;
    if (!list_head){
      list_head = node;
    } else {
      prev_node->next = node;
    }
    prev_node = node;
  }

  printf("@original data:\n");
  for (node = list_head; node != NULL; node = node->next){
    printf("%d ", node->val);
  }

  printf("\n");

  list_head = quick_sort_list(list_head);

  printf("@qsorted data:\n");
  for (node = list_head; node != NULL; node = node->next){
    printf("%d ", node->val);
  }
  printf("\n");

  for (node = list_head; node != NULL; ){
    next_node = node->next;
    free(node);
    node = next_node;
  }

  return 0;
}
