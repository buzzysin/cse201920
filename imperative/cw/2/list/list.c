// Implementation of list module.

// ---------- Add headers here ----------
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Each node in a doubly linked list is stored in this structure. The user of the
// module does not have any knowledge of nodes.
struct node
{
  struct node *back;
  item x;
  struct node *next;
};
typedef struct node node;

// A list is to be represented as a circular list. It has to contain a doubly linked list
// of item nodes and one special sentinel node that links to the two ends of the item list.
// The none field holds this sentinel node. It has a default item, must always be present for
// a list and always sits before the first item node and after the last item node. For a list
// that holds no item nodes the back and next fields of the sentinel node point to the sentinel
// node itself. The current node refers to the currently selected item node in the list. If
// the current node is the none node then no item is selected.
struct list
{
  node *none, *current;
};
typedef struct list list;

// ---------- Add functions here ----------

// *********************** //
//  *** Implementation *** //
// *********************** //

// Create a new empty list, with the default item to return in case no item is selected.
// No item is selected in an empty list.
list *newList(item e)
{
  // * Allocate the necessary memory for the list
  list *l = malloc(sizeof(list));
  // * Allocate the necessary memory for the sentinel node
  node *none = malloc(sizeof(node));

  // * Set the fields of the struct
  none->next = none;
  none->back = none;
  none->x = e;

  // * Assign the node to the list
  l->none = none;
  l->current = none;

  // * Return a pointer to the list
  return l;
}

void freeList(list *xs)
{
  // * Define a reference point to start
  // * freeing nodes
  node *start = xs->none->next;

  while (start != xs->none)
  {
    // * The node we want to free is the one
    // * at the start
    node *to_free = start;

    // * The sentinel points to the item after
    // * the node to remove
    xs->none->next = to_free->next;

    // * The neighbour to the node to remove
    // * now points to the end of the list
    to_free->back = xs->none;

    // * At this point, the node to free should
    // * no longer have any pointers to it, so
    // * it should itself be safe to free
    free(to_free);

    // * Reassign start
    start = xs->none->next;
  }

  // * After iteration, all except the sentinel remains.
  free(xs->none);

  // * Free the wrapper struct
  free(xs);
}

void first(list *xs)
{
  // * We can skip this entire function iff:
  // ? The list is empty
  if (xs->none->next == xs->none->back)
    return;

  xs->current = xs->none->next;
}
void last(list *xs)
{
  // * We can skip this entire function iff:
  // ? The list is empty
  if (xs->none->next == xs->none->back)
    return;

  xs->current = xs->none->back;
}

bool none(list *xs)
{
  // * Return whether or not the current pointer is
  // * pointing at the sentinel
  return xs->current == xs->none;
}

bool after(list *xs)
{
  // * Null case - no item is selected
  if (xs->current == xs->none)
    return false;

  // * Otherwise
  xs->current = xs->current->next;
  return true;
}
bool before(list *xs)
{
  // * Null case - no item is selected
  if (xs->current == xs->none)
    return false;

  // * Otherwise
  xs->current = xs->current->back;
  return true;
}

item get(list *xs)
{
  // * Null case - no item is selected
  if (xs->current == xs->none)
    // * Return the default item
    return xs->none->x;

  // * Otherwise
  return xs->current->x;
}
bool set(list *xs, item x)
{
  // * Null case - no item is selected
  if (xs->current == xs->none)
    // * Return false
    return false;

  // * Otherwise
  xs->current->x = x;
  return true;
}

void insertAfter(list *xs, item x)
{
  // * Create a new node to hold item x
  node *n = malloc(sizeof(node));
  n->x = x;

  // ? Rearrange pointers to accomodate new node:

  // * New node next => After current
  n->next = xs->current->next;
  // * New node back => Current node
  n->back = xs->current;

  // * After current back => New node
  xs->current->next->back = n;
  // * Current next => New Node
  xs->current->next = n;

  // * Finally, reassign the current pointer
  xs->current = n;
}
void insertBefore(list *xs, item x)
{
  // * Create a new node to hold item x
  node *n = malloc(sizeof(node));
  n->x = x;

  // ? Rearrange pointers to accomodate new node:

  // * New node next => Current node
  n->next = xs->current;
  // * New node back => Before Current
  n->back = xs->current->back;

  // * Before current next => New node
  xs->current->back->next = n;
  // * Current back => New Node
  xs->current->back = n;

  // * Finally, reassign the current pointer
  xs->current = n;
}

bool deleteToAfter(list *xs)
{
  // * Null case - no item is selected
  if (xs->current == xs->none)
    return false;

  // * Otherwise

  // * Identify the node to free ...
  node *to_free = xs->current;
  // * ... and the new next node
  node *next = xs->current->next;

  // * Dissociate the node to free from the list
  next->back = to_free->back;
  to_free->back->next = next;

  // * Assign the new current node
  xs->current = next;
  // * Free the old current node
  free(to_free);

  return true;
}
bool deleteToBefore(list *xs)
{
  // * Null case - no item is selected
  if (xs->current == xs->none)
    return false;

  // * Otherwise

  // * Identify the node to free ...
  node *to_free = xs->current;
  // * ... and the new next node
  node *next = xs->current->back;

  // * Dissociate the node to free from the list
  next->next = to_free->next;
  to_free->next->back = next;

  // * Assign the new current node
  xs->current = next;
  // * Free the old current node
  free(to_free);

  return true;
}

// !!!!!!!!!!!!!!!!!!!!!!! //
// !!! End of new code !!! //
// !!!!!!!!!!!!!!!!!!!!!!! //

// Test the list module, using int as the item type. Strings are used as
// 'pictograms' to describe lists. Single digits represent items and the '|' symbol
// in front of a digit indicates that this is the current item. If the '|' symbol
// is at the end of the string then 'none' of the items is selected. The strings
// "|37", "3|7", "37|" represent a list of two items, with the current position
// at the first item, the last item, and a situation where 'none' of the items
// is selected.
#ifdef test_list

// Build a list from a pictogram, with -1 as the default item.
list *build(char *s)
{
  list *xs = malloc(sizeof(list));
  int n = strlen(s);
  node *nodes[n];
  for (int i = 0; i < n; i++)
    nodes[i] = malloc(sizeof(node));
  for (int i = 0; i < n; i++)
    nodes[i]->next = nodes[(i + 1) % n];
  for (int i = 1; i < n + 1; i++)
    nodes[i % n]->back = nodes[i - 1];
  xs->none = nodes[0];
  xs->none->x = -1;
  node *p = xs->none->next;
  for (int i = 0; i < strlen(s); i++)
  {
    if (s[i] == '|')
      xs->current = p;
    else
    {
      p->x = s[i] - '0';
      p = p->next;
    }
  }
  return xs;
}

// Destroy a list which was created with the build function and which matches a
// pictogram.
void destroy(list *xs, char *s)
{
  int n = strlen(s);
  node *nodes[n];
  nodes[0] = xs->none;
  for (int i = 1; i < n; i++)
    nodes[i] = nodes[i - 1]->next;
  for (int i = 0; i < n; i++)
    free(nodes[i]);
  free(xs);
}

// Check that a list matches a pictogram.
bool match(list *xs, char *s)
{
  int n = strlen(s);
  node *nodes[n];
  nodes[0] = xs->none;
  for (int i = 1; i < n; i++)
    nodes[i] = nodes[i - 1]->next;
  if (nodes[n - 1]->next != xs->none)
    return false;
  for (int i = 1; i < n; i++)
  {
    if (nodes[i]->back != nodes[i - 1])
      return false;
  }
  node *p = xs->none->next;
  for (int i = 0; i < strlen(s); i++)
  {
    if (s[i] == '|')
    {
      if (p != xs->current)
        return false;
    }
    else
    {
      if (p->x != s[i] - '0')
        return false;
      p = p->next;
    }
  }
  return true;
}

// Use constants to say which function to call.
enum
{
  First,
  Last,
  None,
  After,
  Before,
  Get,
  Set,
  InsertAfter,
  InsertBefore,
  DeleteToAfter,
  DeleteToBefore
};
typedef int function;

// A replacement for the library assert function.
void assert(int line, bool b)
{
  if (b)
    return;
  printf("The test on line %d fails.\n", line);
  exit(1);
}

// Call a given function with a possible integer argument, returning a possible
// integer or boolean result (or -1).
int call(function f, list *xs, int arg)
{
  int result = -1;
  switch (f)
  {
  case None:
    result = none(xs);
    break;
  case First:
    first(xs);
    break;
  case Last:
    last(xs);
    break;
  case After:
    result = after(xs);
    break;
  case Before:
    result = before(xs);
    break;
  case Get:
    result = get(xs);
    break;
  case Set:
    result = set(xs, arg);
    break;
  case InsertAfter:
    insertAfter(xs, arg);
    break;
  case InsertBefore:
    insertBefore(xs, arg);
    break;
  case DeleteToBefore:
    result = deleteToBefore(xs);
    break;
  case DeleteToAfter:
    result = deleteToAfter(xs);
    break;
  default:
    assert(__LINE__, false);
  }
  return result;
}

// Check that a given function does the right thing. The 'in' value is passed to
// the function or is -1. The 'out' value is the expected result, or -1.
bool check(function f, int in, char *before, char *after, int out)
{
  list *xs = build(before);
  int result = call(f, xs, in);
  //check circular list structure
  assert(__LINE__, (xs->none == xs->none->next->back));
  assert(__LINE__, (xs->none == xs->none->back->next));
  //check correct workings of the function
  bool ok = (match(xs, after) && (result == out));
  destroy(xs, after);
  return ok;
}

// Test newList, and call freeList. The test for freeList is that the memory
// leak detector in the -fsanitize=address or -fsanitize=leak compiler option
// reports no problems.
void testNewList()
{
  list *xs = newList(-1);
  //check circular list structure
  assert(__LINE__, (xs->none == xs->none->next));
  assert(__LINE__, (xs->none == xs->none->back));
  //check that an empty list is produced with a sentinel correctly
  assert(__LINE__, match(xs, "|"));
  freeList(xs);
}

// Test the various functions.
void testFirst()
{
  assert(__LINE__, check(First, -1, "|", "|", -1));
  assert(__LINE__, check(First, -1, "|37", "|37", -1));
  assert(__LINE__, check(First, -1, "3|7", "|37", -1));
  assert(__LINE__, check(First, -1, "37|", "|37", -1));
}

void testLast()
{
  assert(__LINE__, check(Last, -1, "|", "|", -1));
  assert(__LINE__, check(Last, -1, "|37", "3|7", -1));
  assert(__LINE__, check(Last, -1, "3|7", "3|7", -1));
  assert(__LINE__, check(Last, -1, "37|", "3|7", -1));
}

void testNone()
{
  assert(__LINE__, check(None, -1, "|", "|", true));
  assert(__LINE__, check(None, -1, "|37", "|37", false));
  assert(__LINE__, check(None, -1, "3|7", "3|7", false));
  assert(__LINE__, check(None, -1, "37|", "37|", true));
}

void testAfter()
{
  assert(__LINE__, check(After, -1, "|", "|", false));
  assert(__LINE__, check(After, -1, "|37", "3|7", true));
  assert(__LINE__, check(After, -1, "3|7", "37|", true));
  assert(__LINE__, check(After, -1, "37|", "37|", false));
}

void testBefore()
{
  assert(__LINE__, check(Before, -1, "|", "|", false));
  assert(__LINE__, check(Before, -1, "|37", "37|", true));
  assert(__LINE__, check(Before, -1, "3|7", "|37", true));
  assert(__LINE__, check(Before, -1, "37|", "37|", false));
}

void testGet()
{
  assert(__LINE__, check(Get, -1, "|", "|", -1));
  assert(__LINE__, check(Get, -1, "|37", "|37", 3));
  assert(__LINE__, check(Get, -1, "3|7", "3|7", 7));
  assert(__LINE__, check(Get, -1, "37|", "37|", -1));
}

void testSet()
{
  assert(__LINE__, check(Set, 5, "|", "|", false));
  assert(__LINE__, check(Set, 5, "|37", "|57", true));
  assert(__LINE__, check(Set, 5, "3|7", "3|5", true));
  assert(__LINE__, check(Set, 5, "37|", "37|", false));
}

void testInsertAfter()
{
  assert(__LINE__, check(InsertAfter, 5, "|", "|5", -1));
  assert(__LINE__, check(InsertAfter, 5, "|37", "3|57", -1));
  assert(__LINE__, check(InsertAfter, 5, "3|7", "37|5", -1));
  assert(__LINE__, check(InsertAfter, 5, "37|", "|537", -1));
}

void testInsertBefore()
{
  assert(__LINE__, check(InsertBefore, 5, "|", "|5", -1));
  assert(__LINE__, check(InsertBefore, 5, "|37", "|537", -1));
  assert(__LINE__, check(InsertBefore, 5, "3|7", "3|57", -1));
  assert(__LINE__, check(InsertBefore, 5, "37|", "37|5", -1));
}

void testDeleteToAfter()
{
  assert(__LINE__, check(DeleteToAfter, -1, "|", "|", false));
  assert(__LINE__, check(DeleteToAfter, -1, "|37", "|7", true));
  assert(__LINE__, check(DeleteToAfter, -1, "3|7", "3|", true));
  assert(__LINE__, check(DeleteToAfter, -1, "37|", "37|", false));
  assert(__LINE__, check(DeleteToAfter, -1, "|5", "|", true));
}

void testDeleteToBefore()
{
  assert(__LINE__, check(DeleteToBefore, -1, "|", "|", false));
  assert(__LINE__, check(DeleteToBefore, -1, "|37", "7|", true));
  assert(__LINE__, check(DeleteToBefore, -1, "3|7", "|3", true));
  assert(__LINE__, check(DeleteToBefore, -1, "37|", "37|", false));
  assert(__LINE__, check(DeleteToBefore, -1, "|5", "|", true));
}

int main()
{
  testNewList();
  testFirst();
  testLast();
  testNone();
  testAfter();
  testBefore();
  testGet();
  testSet();
  testInsertAfter();
  testInsertBefore();
  testDeleteToAfter();
  testDeleteToBefore();
  printf("List module tests run OK.\n");
  return 0;
}
#endif
