#include <stdio.h>

#define LRU_CACHE_SIZE 1000

struct LRUEntry {
  int last_called_index;
  int fib_n;
  int fib_val;
};



int main(void) {
  struct LRUEntry lru_cache[LRU_CACHE_SIZE];
  lru_cache->fib_n = 0;
  for(int i=0; i < 3; i++) {
    printf(" value: %d", lru_cache[i].fib_n);
  }
  // for (int ix = 0; ix < 100; ix++) {
  //   int fib_to_get = rand() % 100;
  //   printf("fib %d = %ull\n", ix, fib_lru(fib_to_get, lru_cache));
  // }
  return 0;
}


