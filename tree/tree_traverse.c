#include <stdio.h>
#include <stdlib.h>

struct tree_node {
  int val;
  struct tree_node *left;
  struct tree_node *right;
};

struct cstack_node {
    struct tree_node *tnode;
    struct cstack_node *next;
};

struct cstack {
  struct cstack_node *top;
  struct cstack_node *base;
};

struct cstack *cstack_create(){
  struct cstack *stackp = (struct cstack *)malloc(sizeof(struct cstack));
  stackp->base = NULL;
  stackp->top = NULL;
  return stackp;
}

void cstack_push(struct cstack *stackp, struct tree_node *nodep){
  struct cstack_node *sn = (struct cstack_node *)malloc(sizeof(struct cstack_node));
  sn->tnode = nodep;
  sn->next = stackp->top;

  stackp->top = sn;
}

static inline int cstack_empty(struct cstack *stackp){
  return (stackp->top == stackp->base);
}
//获取栈的顶点元素但是不弹出
struct tree_node *cstack_top(struct cstack *stackp){
  if (cstack_empty(stackp)){
    return NULL;
  } else {
    return stackp->top->tnode;
  }
}
//弹出栈的顶点元素
struct tree_node *cstack_pop(struct cstack *stackp){
  struct tree_node *tnode = NULL;
  if (!cstack_empty(stackp)){
    struct cstack_node *top = stackp->top;
    tnode = top->tnode;
    stackp->top = top->next;
    free(top);
  }
  return tnode;
}

//先序遍历是先遍历根,再分别对左右子树进行同样的遍历
//那么如果一个节点没有
//先序遍历也要用到栈,但是与后序遍历不同,它栈中的节点元素
//的内容已经放入到了输出当中
int *preorder_nonrecursive(struct tree_node *root, int *num_node){
  *num_node = 0;
  if (!root){
    return NULL;
  }
  struct cstack *tree_stack = cstack_create();
  struct tree_node *stack_top;
  int *output_array = (int *)malloc(sizeof(int));

  while (root){
    //将root加入到输出中
    (*num_node)++;
    output_array = (int *)realloc(output_array,
        (*num_node) * sizeof(int));
    output_array[(*num_node) - 1] = root->val;
    //将root入栈
    cstack_push(tree_stack, root);

    if (root->left){
      root = root->left;
    } else if (root->right){
      root = root->right;
    } else {
pop_stack:
      //如果一个节点没有左子树和右子树就出栈
      root = cstack_pop(tree_stack);

      stack_top = cstack_top(tree_stack); 
      if (!stack_top){
        break;
      }

      if (root == stack_top->left){
        //如果当前节点是栈顶元素的左孩子
        //那就对栈顶元素的右孩子所在的子树进行先序遍历
        if (stack_top->right){
          root = stack_top->right;
        } else {
          //如果栈顶元素没有右孩子,那就说明该栈顶元素的所有子节点都已经
          //遍历完,而该元素自身本身也已经遍历过
          //于是将其弹出
          goto pop_stack;
        }
      } else if (root == stack_top->right){
        //栈顶的左右元素都已经处理完,弹出栈顶
        goto pop_stack;
      }
    }
  }

  return output_array;
}

//递归前序遍历树
void preorder_recursive(struct tree_node *root){
  printf("%d,", root->val);
  if (root->left){
    preorder_recursive(root->left);
  }
  if (root->right){
    preorder_recursive(root->right);
  }
}

//非递归中序遍历树
int *inorder_nonrecursive(struct tree_node *root, int *num_node){
  *num_node = 0;
  if (!root){
    return NULL;
  }
  int *output_array = (int *)malloc(sizeof(int));
  struct cstack *tree_stack = cstack_create();
  struct tree_node *stack_top;

  while (root){
    cstack_push(tree_stack, root);
    if (root->left) {
      root = root->left;
    } else {
output_mid_node:  //输出中间节点值
      //如果没有左儿子,那就将该节点输出
      (*num_node)++;
      output_array = (int *)realloc(output_array,
          (*num_node) * sizeof(int));
      output_array[(*num_node) - 1] = root->val;
      if (root->right){
        root = root->right;
      } else {
        //如果没有右儿子,那就弹出栈
pop_stack:
        root = cstack_pop(tree_stack);

        stack_top = cstack_top(tree_stack);
        if (!stack_top){
          break;
        }

        if (root == stack_top->left){
          //当前节点是栈顶元素的左儿子,由于当前节点已经输出
          //那就得将栈顶元素的值输出
          root = stack_top;
          goto output_mid_node;
        } else if (root == stack_top->right){
          //如果当前节点是栈顶元素的右儿子,那就说明
          //栈顶元素的左右儿子都已经输出完
          //栈顶元素应该出栈
          goto pop_stack;
        }
      }
    }
  }

  return output_array;
}

//递归中序遍历树
void inorder_recursive(struct tree_node *root){
  if (root->left){
    inorder_recursive(root->left);
  }
  printf("%d,", root->val);
  if (root->right){
    inorder_recursive(root->right);
  }
}

//用非递归方式实现树的后序遍历
//以前序遍历方式遍历树并将节点入栈
//一旦当前节点没有左儿子，就遍历右儿子子树并入栈
//一旦当前节点既没有左儿子也没有右儿子，就将其从栈中弹出并放到输出中
//返回输出数组
int *postorder_nonrecursive(struct tree_node* root, int *num_node){
  *num_node = 0;
  if (!root){
    return NULL;
  }
  struct cstack *tree_stack = cstack_create();
  int *output_array = (int *)malloc(sizeof(int));
  struct tree_node *stack_top;

  while (root){
    cstack_push(tree_stack, root);
    if (root->left){
      root = root->left;
    } else if (root->right){
      root = root->right;
    } else {
pop_stack:
      //将当前节点root从栈中弹出并放到输出数组中
      root = cstack_pop(tree_stack);
      (*num_node)++;
      output_array = (int *)realloc(output_array, (*num_node) * sizeof(int));
      output_array[(*num_node) - 1] = root->val;

      stack_top = cstack_top(tree_stack);
      if (!stack_top){
        break;  //结束
      }

      if (root == stack_top->left){
        //当前从栈中弹出的是栈顶元素的左儿子
        //对栈顶元素的右子树进行遍历
        root = stack_top->right;
        if (!root){//右子树没有可遍历的，于是弹出栈顶元素
          goto pop_stack;
        }
      } else if (root == stack_top->right){
        //从栈中弹出的是栈顶元素的右儿子
        //说明栈顶元素的左儿子和右儿子已经被输出
        //轮到输出它了，于是将它从栈中弹出
        goto pop_stack;
      }
    }
  }

  return output_array;
}

//以递归方式后续遍历树
void postorder_recursive(struct tree_node *root){
  if (root->left){
    postorder_recursive(root->left);
  }
  if (root->right){
    postorder_recursive(root->right);
  }
  printf("%d,", root->val);
}

//按照二叉查找树的方式构建树
void insert_tree(struct tree_node *root, int val){
  if (val < root->val){
    if (root->left){
      insert_tree(root->left, val);
    } else {
      root->left = (struct tree_node *)malloc(sizeof(struct tree_node));
      root->left->val = val;
    }
  } else {
    if (root->right){
      insert_tree(root->right, val);
    } else {
      root->right = (struct tree_node *)malloc(sizeof(struct tree_node));
      root->right->val = val;
    }
  }
}

struct tree_node *build_tree(int *data, int size){
  struct tree_node *root = NULL;
  int i;

  if (size >= 1){
    root = (struct tree_node *)malloc(sizeof(struct tree_node));
    root->val = data[0];
    for (i = 1; i < size; i++){
      insert_tree(root, data[i]);
    }
  }
  return root;
}

void free_tree(struct tree_node *root){
  if (root){
    if (root->left){
      free_tree(root->left);
      root->left = NULL;
    }
    if (root->right){
      free_tree(root->right);
      root->right = NULL;
    }
    free(root);
  }
}

/*
                    10 
          6                     15
     3        8            13         18
  1     5   7   9       11    14   17    19

pre_order:10,6,3,1,5,8,7,9,15,13,11,14,18,17,19
in_order:1,3,5,6,7,8,9,10,11,13,14,15,17,18,19
post_order:1,5,3,7,9,8,6,11,14,13,17,19,18,15,10
*/
int main(int argc, char *argv[]){
  int data[15] = {10, 6, 3, 8, 1, 5, 7, 9, 15, 13, 18, 11, 14, 17, 19};
  struct tree_node *root;
  int size;
  int i;

  //build-tree
  root = build_tree(data, 15);

  //分别以(递归与非递归的)先序,中序和后序遍历来遍历整个树
  int *result;

  printf("preorder_recursive: begin\n");
  preorder_recursive(root);
  printf("\n");

  printf("preorder_nonrecursive: begin\n");
  result = preorder_nonrecursive(root, &size);
  for (i = 0; i < size; i++){
    printf("%d,", result[i]);
  }
  free(result);
  printf("\n");

  printf("inorder_recursive: begin\n");
  inorder_recursive(root);
  printf("\n");

  printf("inorder_nonrecursive: begin\n");
  result = inorder_nonrecursive(root, &size);
  for (i = 0; i < size; i++){
    printf("%d,", result[i]);
  }
  free(result);
  printf("\n");

  printf("postorder_recursive: begin\n");
  postorder_recursive(root);
  printf("\n");

  printf("postorder_nonrecursive: begin\n");
  result = postorder_nonrecursive(root, &size);
  for (i = 0; i < size; i++){
    printf("%d,", result[i]);
  }
  free(result);
  printf("\n");

  free_tree(root);

  return 0;
}

