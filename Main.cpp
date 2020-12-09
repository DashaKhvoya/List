#include "BidirectionalList.h"

void Test();

int main() {
  Test();
  return 0;
}

void Test() {
  const int answer[] = { 15, 55, 325, 586, 4, 32, 79, 13, 500, 7, 40 };

  List list = {};
  ListConstruct(&list, 30);

  ListInsertBack(&list, 15); //15 55 325 586 4 32 79 13 500 40
  ListInsertBack(&list, 55);
  ListInsertBack(&list, 325);
  ListInsertBack(&list, 586);
  ListInsertBack(&list, 500);
  ListInsertBack(&list, 7);
  ListInsertBack(&list, 40);
  int idx = ListFindIndex(&list, 4);
  idx = ListInsertAfter(&list, 4, idx);
  ListInsertAfter(&list, 13, idx);
  idx = ListInsertAfter(&list, 14, idx);
  ListInsertBefore(&list, 32, idx);
  ListInsertBefore(&list, 79, idx);
  ListErase(&list, idx);

  bool pass_test = true;
  for (size_t index = list.head, i = 0; index != 0; index = list.data[index].next) {
    if (list.data[index].value != answer[i++]) {
      pass_test = false;
    }
  }

  if (pass_test) {
    printf("YES\n");
  } else {
    printf("NO\n");
  }

  ListDestruct(&list);
}
