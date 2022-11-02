#include <stdio.h>

#define NUMBER_OF_TESTS 1000
#define LRU_CACHE_SIZE 90
#define INT_MAX 2147483647
#define MAX_FIB_VAL 60

struct LRUEntry {
  int last_called_index;
  int fib_n;
  int fib_val;
};
float call_index = 0;
int least_recently_used_idx = 0;
int least_used = INT_MAX;
float cache_hit = 0;
float cache_hit_ratio = 0;
const int DEBUG_MODE = 0; // 1: ON, 0: OFF

unsigned long long fib_lru(int position, struct LRUEntry lru_cache[LRU_CACHE_SIZE]);


int main(void) {
  struct LRUEntry lru_cache[LRU_CACHE_SIZE] = {{0}};
  unsigned long long int fib_val = 0;
  if (DEBUG_MODE) {
    int debug_fib = 60;
    printf("\n \n -------- RESULT -------- \n \n fib %d = %llu\n", debug_fib, fib_lru(debug_fib, lru_cache));
    for (int i = 0; i < LRU_CACHE_SIZE; i++) {
      printf("Slot %d: fib_n: %d fib_val: %d, last_called_index %d \n", i,
             lru_cache[i].fib_n, lru_cache[i].fib_val,
             lru_cache[i].last_called_index);
    }
  } else {
    for (int ix = 0; ix < NUMBER_OF_TESTS; ix++) {
      int fib_to_get = rand() % MAX_FIB_VAL;
      //Cache size 1, n=2, 50% cache hit ratio
      fib_val = fib_lru(fib_to_get, lru_cache);
      cache_hit_ratio = (cache_hit / call_index) * 100;
      printf("\n Cache size = %d, n = %d, fib value = %llu, %f%% cache hit ratio \n ",LRU_CACHE_SIZE, fib_to_get, fib_val,  cache_hit_ratio);
    }
  }
  return 0;
}

unsigned long long fib_lru(int position, struct LRUEntry lru_cache[LRU_CACHE_SIZE]) {
  int empty_slot = 0;
  ++call_index;
  long long unsigned int fib = 1;

  // Base case
  if (position == 0) {
    return 0;
  }
  if (position == 1) {
    return 1;
  }

  // Find an existing position or find an empty slot
  for (int i = 0; i < LRU_CACHE_SIZE; i++) {
    // Find and update least called index
    if (lru_cache[i].last_called_index > 0 &&
        lru_cache[i].last_called_index < least_used) {
      least_used = lru_cache[i].last_called_index;
      least_recently_used_idx = i;
      if (DEBUG_MODE) {
        printf("LRU Slot: %d", i);
      }
    }
    // Found the exisitng position, mark it as recently used and return
    if (lru_cache[i].fib_n == position) {
      cache_hit += 1;
      lru_cache[i].last_called_index = call_index;
      if (DEBUG_MODE) {
        printf("\n Slot %d just been used, update last called index: %d", i,
               lru_cache[i].last_called_index);
      }
      return lru_cache[i].fib_val;
    }

    // Find an empty slot, update new value, posiion, mark it as recently used
    else if (lru_cache[i].fib_n == 0) {
      empty_slot += 1;
      if (DEBUG_MODE) {
        printf("\n Last called index of slot %d is updated to %d \n", i,
               call_index);
      }
      lru_cache[i].last_called_index = call_index;
      lru_cache[i].fib_n = position;
      lru_cache[i].fib_val =
          fib_lru(position - 1, lru_cache) + fib_lru(position - 2, lru_cache);
      if (DEBUG_MODE) {
        printf("\n Adding to slot %d: fib_n: %d fib_val: %d \n", i,
               lru_cache[i].fib_n, lru_cache[i].fib_val);
      }
      break;
    }
  }

  // If all slots are full
  if (empty_slot == 0) {
    if (DEBUG_MODE) {
      printf("\n All slots are full, evicting LRU cache slot: %d \n",
             lru_cache[least_recently_used_idx].fib_n);
    }
    
  // Replace the least recently used slot with new position and new value
    lru_cache[least_recently_used_idx].fib_n = position;
    lru_cache[least_recently_used_idx].fib_val =
        fib_lru(position - 1, lru_cache) + fib_lru(position - 2, lru_cache);
  // Mark it as recently used
lru_cache[least_recently_used_idx].last_called_index = call_index;

    if (DEBUG_MODE) {
      printf("\n NEW SLOT %d, with position %d, value: %d \n",
             least_recently_used_idx, lru_cache[least_recently_used_idx].fib_n,
             lru_cache[least_recently_used_idx].fib_val);
    }
  }
}