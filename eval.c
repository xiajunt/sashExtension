#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define ADD	0
#define SUB	1
#define MUL	2
#define DIV	3
#define NUM	4

typedef struct arith_tree
{
  int type;
  int num_val;
  struct arith_tree *left;
  struct arith_tree *right;
}arith_tree;

void free_tree(arith_tree *tree);
void memory_error(arith_tree *a, arith_tree *b, arith_tree *c);
arith_tree* make_tree(const char *expr);
int arith_eval(arith_tree *tree);
int eval_expr(const char *expr);

void free_tree(arith_tree *tree)
{
  if (tree == NULL)
    return;
  if (tree->left != NULL)
    free_tree(tree->left);
  if (tree->right != NULL)
    free_tree(tree->right);
  free(tree);
}

void memory_error(arith_tree *a, arith_tree *b, arith_tree *c)
{
  fprintf(stderr, "Allocating error\n");
  if (a != NULL)
    free_tree(a);
  if (b != NULL)
    free_tree(b);
  if (c != NULL)
    free_tree(c);
}

arith_tree* make_tree(const char *expr)
{
  arith_tree *tree = NULL;
  arith_tree *end_tree = NULL;
  arith_tree *leaf1 = NULL;
  arith_tree *leaf2 = NULL;
  const char *ch = expr;

  while (*ch != '\0')
    {
      if (isdigit(*ch))
	{
	  leaf1 = malloc(sizeof (arith_tree));
	  if (leaf1 == NULL)
	    {
	      memory_error(tree, leaf1, leaf2);
	      return NULL;
	    }
	  leaf1->left = NULL;
	  leaf1->right = NULL;
	  leaf1->type = NUM;
	  leaf1->num_val = atoi(ch);
	  while (isdigit(*ch))
	    ch++;
	  if (*ch == '\0')
	    {
	      if (tree == NULL)
		tree = leaf1;
	      else
		end_tree->right = leaf1;
	    }
	}
      else
	{
	  leaf2 = malloc(sizeof (arith_tree));
	  if (leaf2 == NULL)
	    {
	      memory_error(tree, leaf1, leaf2);
	      return NULL;
	    }
	  leaf2->left = NULL;
	  leaf2->right = NULL;
	  switch (*ch)
	    {
	    case '+':
	      leaf2->type = ADD;
	      break;
	    case '-':
	      leaf2->type = SUB;
	      break;
	    case 'x':
	      leaf2->type = MUL;
	      break;
	    case '/':
	      leaf2->type = DIV;
	      break;
	    default:
	      /*Never happen*/
	      break;
	    }
	  if (leaf2->type == ADD || leaf2->type == SUB)
	    {
	      if (tree == NULL)
		{
		  tree = leaf2;
		  tree->left = leaf1;
		  end_tree = tree;
		}
	      else
		{
		  end_tree->right = leaf1;
		  leaf2->left = tree;
		  end_tree = tree = leaf2;
		}
	    }
	  else
	    {
	      if (tree == NULL)
		{
		  tree = leaf2;
		  tree->left = leaf1;
		  end_tree = tree;		  
		}
	      else
		{
		  end_tree->right = leaf2;
		  leaf2->left = leaf1;
		  end_tree = leaf2;
		}
	    }
	  ch++;
	}
    }
  return tree;
}

int arith_eval(arith_tree *tree)
{
  if (tree->type == NUM)
    return tree->num_val;
  switch (tree->type)
    {
    case ADD:
      return arith_eval(tree->left) + arith_eval(tree->right);
    case SUB:
      return arith_eval(tree->left) - arith_eval(tree->right);
    case MUL:
      return arith_eval(tree->left) * arith_eval(tree->right);
    case DIV:
      return arith_eval(tree->left) / arith_eval(tree->right);
    default:
      return 0;
    }
}

int eval_expr(const char *expr)
{
  arith_tree *tree = make_tree(expr);
  int res = arith_eval(tree);
  free_tree(tree);
  return res;
}
