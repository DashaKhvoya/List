#include "BidirectionalList.h"

void ListConstruct(List* list, size_t capacity) {
  list -> data = (Node*)calloc(capacity, sizeof(Node));
  for (size_t i = 0; i < capacity - 1; i++) {
    list -> data[i + 1].next = i + 2;
    list -> data[i + 1].prev = -1;
  }
  list -> data[capacity - 1].next = 0;
  list -> head = 0;
  list -> tail = 0;
  list -> size = 0;
  list -> free_elem = 1;
  list -> is_sorted = false;
  list -> capacity = capacity;

  ASSERT_OK(list);
}

void ListDestruct(List* list) {
  ASSERT_OK(list);

  free(list -> data);
}

size_t ListInsertFront(List* list, Elem_t elem) {
  ASSERT_OK(list);

  size_t new_head = ListInsertBetween(list, elem, 0, list -> head);
  list -> head = new_head;
  if (list -> size == 1) {
    list -> tail = new_head;
  }

  ASSERT_OK(list);
  return new_head;
}

size_t ListInsertBack(List* list, Elem_t elem) {
  ASSERT_OK(list);

  size_t new_tail = ListInsertBetween(list, elem, list -> tail, 0);
  list -> tail = new_tail;
  if (list -> size == 1) {
    list -> head = new_tail;
  }

  ASSERT_OK(list);
  return new_tail;
}

size_t ListInsertBefore(List* list, Elem_t elem, size_t index) {
  ASSERT_OK(list);

  if (index != list -> head) {
    return ListInsertBetween(list, elem, list -> data[index].prev, index);
  } else {
    return ListInsertFront(list, elem);
  }
}

size_t ListInsertAfter(List* list, Elem_t elem, size_t index) {
  ASSERT_OK(list);

  if (index != list -> tail) {
    return ListInsertBetween(list, elem, index, list -> data[index].next);
  } else {
    return ListInsertBack(list, elem);
  }
}

size_t ListInsertBetween(List* list, Elem_t elem, size_t index_1, size_t index_2) {
  list -> size++;

  size_t index = list -> free_elem;
  list -> free_elem = list -> data[list -> free_elem].next;
  list -> data[index].value = elem;
  list -> data[index].prev = index_1;
  list -> data[index].next = index_2;
  list -> data[index_1].next = index;
  list -> data[index_2].prev = index;

  return index;
}

void ListErase(List* list, size_t index) {
  ASSERT_OK(list);

  list -> size--;
  list -> data[list -> data[index].prev].next = list -> data[index].next;
  list -> data[list -> data[index].next].prev = list -> data[index].prev;
  if (index == list->tail) {
        list->tail = list->data[index].prev;
    }

    if (index == list->head) {
        list->head = list->data[index].next;
    }
  list -> data[index].prev = -1;
  list -> data[index].next = list -> free_elem;
  list -> free_elem = index;

  ASSERT_OK(list);
}

void ListSort(List* list) {
  Elem_t* temp = (Elem_t*)calloc(list->size, sizeof(Elem_t));
  for (size_t i = list -> head, temp_i = 0; i != 0; i = list -> data[i].next) {
      temp[temp_i++] = list->data[i].value;
  }
  for (size_t temp_i = 1; temp_i <= list -> size; ++temp_i) {
    list->data[temp_i].value = temp[temp_i - 1];
  }
  free(temp);

  for (size_t i = 1; i <= list -> size; i++) {
    list -> data[i].next = i + 1;
    list -> data[i].prev = i - 1;
  }
  list -> data[list -> size].next = 0;

  for (size_t i = list -> size + 1; i < list -> capacity; i++) {
    list -> data[i].next = i + 1;
    list -> data[i].prev = -1;
  }
  list -> data[list -> capacity - 1].next = 0;

  list -> head = 1;
  list -> tail = list -> size;
  list -> free_elem = list -> size + 1;
  list -> is_sorted = true;
}

size_t ListFindIndex(List* list, size_t number) {
  ASSERT_OK(list);

  if (!list -> is_sorted) {
    size_t current = list -> head;
    for (size_t i = 0; i < number - 1; i++) {
      current = list -> data[current].next;
    }
    return current;
  }

  return number;
}

ListState IsOk(List* list) {
#ifdef DEBUG
  if (!list) {
    return POINTER_ERROR;
  }

  if (!list -> data) {
    return MEMORY_ERROR;
  }

  if (list -> size > list -> capacity) {
    return SIZE_ERROR;
  }

  if(list -> data[list -> free_elem].prev != -1) {
    return FREE_ELEM_ERROR;
  }

  if (IsLoop(list)) {
    return LOOP_ERROR;
  }
#endif

  return LIST_OK;
}

bool IsLoop(List* list) {
  size_t temp = list -> head;
  for (size_t i = 0; i < list -> size; i++) {
    temp = list -> data[temp].next;
  }

  if (temp == 0) {
    return false;
  }

  return true;
}

void ListDump(List* list) {
  #ifdef DEBUG
    LogFile(list);
    GraphFileReal(list);
  #endif
}

void GraphFileReal(List* list) {
  if (!list || !list -> data) {
    return;
  }

  FILE* graph_file = fopen ("graph.gv", "w");

  fprintf (graph_file, "digraph {\n");
  fprintf (graph_file, "\"%p\" [shape=\"record\", style=\"filled\", color=\"#000088\", fillcolor=\"#C0C0FF\", label=\"{NULL|{prev:%lu|pos: 0|next:%lu}}\"]\n",
  &list -> data[0], list -> data[0].prev, list -> data[0].next);

  for (size_t i = 1; i < list -> capacity; i++) {
    if (list -> data[i].prev != -1) {
      fprintf (graph_file, "\"%p\" [shape=\"record\", style=\"filled\", color=\"#008000\", fillcolor=\"#C0FFC0\", label=\"{%d|{prev:%lu|pos:%lu|next:%lu}}\"]\n",
      &list -> data[i], list -> data[i].value, list -> data[i].prev, i, list -> data[i].next);
    } else {
      fprintf (graph_file, "\"%p\" [shape=\"record\", style=\"filled\", color=\"#000000\", fillcolor=\"#FFFFFF\", label=\"{NAN|{prev:-1|pos:%lu|next:%lu}}\"]\n",
      &list -> data[i], i, list -> data[i].next);
    }
    fprintf (graph_file, "\"%p\"->\"%p\"[style=\"dashed\", color=\"#A0A0A0\"];\n",  &list -> data[i - 1],  &list -> data[i]);
  }

  for (size_t i = 1; i < list -> capacity; i++) {
    if (list -> data[i].prev != -1) {
      fprintf (graph_file, "\"%p\"->\"%p\"[color=\"pink\", constraint=\"false\"];\n",  &list -> data[i],  &list -> data[list -> data[i].prev]);
      fprintf (graph_file, "\"%p\"->\"%p\"[color=\"blue\", constraint=\"false\"];\n",  &list -> data[i],  &list -> data[list -> data[i].next]);
    }
  }
  fprintf (graph_file, "}\n");

  fclose (graph_file);

  system("rm graph.png");
  system("dot -Tpng graph.gv -o graph.png");
}

void LogFile(List* list) {
  FILE* log_file = fopen ("log_file.txt", "w");

  if (!list) {
    fprintf(log_file, "Pointer not exist %p\n", list);
    return;
  }
  fprintf (log_file, "list logs\n\t{\n");

  if (list -> size > list -> capacity) {
    fprintf (log_file, "OMG SIZE = %lu > CAPACITY = %lu HELP PLEASE\n", list -> size, list -> capacity);
    return;
  }
  fprintf (log_file, "\tsize = %lu\n", list -> size);
  fprintf (log_file, "\tcapacity = %lu\n", list -> capacity);

  fprintf (log_file, "\thead = %lu\n", list -> head);
  fprintf (log_file, "\ttail = %lu\n", list -> tail);
  fprintf (log_file, "\tfree = %lu\n", list -> free_elem);

  if (!list -> data) {
    fprintf (log_file, "Reading not impossible %p\n", list -> data);
    return;
  }
  fprintf (log_file, "\tdata = %p\n", list -> data);
  fprintf (log_file, "\t\t{\n");
  for (int i = 0; i < list -> capacity; i++) {
    fprintf (log_file, "\t\tdata[%d] = %d next[%d] = %lu prev[%d] = %lu\n", i, list -> data[i].value, i, list -> data[i].next, i, list -> data[i].prev);
  }
  fprintf (log_file, "\n");
  fprintf (log_file, "\t\t}\n");
  fprintf (log_file, "\t}\n");

  fclose (log_file);
}
