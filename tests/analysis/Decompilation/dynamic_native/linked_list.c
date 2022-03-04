/*
 * This file is distributed under the MIT License. See LICENSE.md for details.
 */

#include <malloc.h>
#include <stdint.h>

struct __attribute__((packed)) Node {
  uint32_t content_uint32;
  struct Node *prev;
  struct Node *next;
};

int getPrevious(struct Node *n) {
  if (n->prev)
    return n->prev->content_uint32;
  return 0;
}

int getNext(struct Node *n) {
  if (n->next)
    return n->next->content_uint32;
  return 0;
}

static struct Node Nodes[11] = {0};

static struct Node* getGlobalNodes() {
  return &Nodes[0];
}


int main(int argc, char **argv) {
  struct Node *first = getGlobalNodes();
  first->prev = NULL;

  struct Node *cur = first;
  for (int i = 0; i < 10; i++) {
    struct Node *new_node = first + i + 1;

    cur->next = new_node;
    new_node->prev = cur;
    new_node->next = NULL;
    new_node->content_uint32 = i;
    cur = cur->next;
  }

  int sum = 0;
  cur = first;
  while (cur) {
    sum += getPrevious(cur);
    sum += getNext(cur);
    cur = cur->next;
  }

  cur = first;
  while (cur) {
    struct Node *next = cur->next;
    free(cur);
    cur = next;
  }

  return sum;
}
