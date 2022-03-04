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

uint32_t getContent(struct Node *n) {
  return n ? n->content_uint32 : 0U;
}

void initNode(struct Node *n) {
  if (!n)
    return;
  n->prev = NULL;
  n->next = NULL;
  n->content_uint32 = 0U;
}

static struct Node Nodes[11] = {0};

static struct Node* getGlobalNodes() {
  return &Nodes[0];
}

int main(int argc, char **argv) {
  struct Node *first = getGlobalNodes();
  initNode(first);

  struct Node *cur = first;
  for (int i = 0; i < 10; i++) {
    struct Node *new_node = first + i + 1;
    initNode(new_node);
    new_node->content_uint32 = i;
    new_node->prev = cur;

    cur->next = new_node;
    cur = new_node;
  }

  int sum = 0;
  cur = first;
  while (cur) {
    sum += getContent(cur);
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
