#pragma once
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

typedef Node* Elem_t;

enum ListState {
  LIST_OK,
  POINTER_ERROR,
  MEMORY_ERROR,
  SIZE_ERROR,
  FREE_ELEM_ERROR,
  LOOP_ERROR
};

struct NodeList {
  Elem_t value;
  size_t next;
  size_t prev;
};

struct List {
  NodeList* data;
  size_t head;
  size_t tail;
  size_t size;
  size_t free_elem;
  bool is_sorted;
  size_t capacity;
};

#define DEBUG
#ifdef DEBUG
  #define ASSERT_OK(list) if(IsOk(list) != LIST_OK) { ListDump(list); assert(0); }
#else
  #define ASSERT_OK(list)
 #endif

void ListConstruct(List* list, size_t capacity);
void ListDestruct(List* list);

size_t ListInsertFront(List* list, Elem_t elem);
size_t ListInsertBack(List* list, Elem_t elem);
size_t ListInsertBefore(List* list, Elem_t elem, size_t index);
size_t ListInsertAfter(List* list, Elem_t elem, size_t index);
size_t ListInsertBetween(List* list, Elem_t elem, size_t index_1, size_t index_2);

void ListErase(List* list, size_t index);

void   ListSort(List* list);
size_t ListFindIndex(List* list, size_t number);

ListState IsOk(List* list);
bool IsLoop(List* list);

void ListDump(List* list);
void LogFile(List* list);
void GraphFileReal(List* list);
