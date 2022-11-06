#include <stdio.h>

#define CACHE_SIZE 90
#define INT_MIN -2147483647
#define INT_MAX 2147483647
struct CacheEntry {
  int last_called_index;
  int fib_n;
  int fib_val;
};
float call_index = 0;
int least_recently_used_idx = 0;
int most_recently_used_idx = 0;
int empty_slot = CACHE_SIZE;
int least_used = INT_MAX;
int most_used = INT_MIN;
float cache_hit = 0;
float cache_hit_ratio = 0;
const int DEBUG_MODE = 0; // 1: ON, 0: OFF
struct CacheEntry lru_cache[CACHE_SIZE] = {{0}};
struct CacheEntry mru_cache[CACHE_SIZE] = {{0}};

void calculate_fib_lru(int number_of_tests, int max_fib_val);
void calculate_fib_mru(int number_of_tests, int max_fib_val);
unsigned long long int fib_lru(int position);
unsigned long long int fib_mru(int position);

// helper functions
void print_slots(struct CacheEntry cache[CACHE_SIZE]);
void update_slot(int index, int position, int call_index,
                 unsigned long long int fib);

int main(int argc, char *argv[]) {
  char command = argv[1][1];
  int number_of_tests;
  int max_fib_val;
  switch (command) {
  case 'h':
    printf("\n Help commands \n \n"
           "-h: print this help \n \n"
           "-l [NUMBER_OF_TESTS] [MAX_FIB_VAL]: Calculate fibonacci using LRU "
           "cache \n \n"
           "-m [NUMBER_OF_TESTS] [MAX_FIB_VAL]: Calculate fibonacci using MRU "
           "cache \n \n"
           "Example: \n \n"
           "-l 30 60: Calculate fibonacci using LRU cache 30 times with "
           "fibonacci value position randomly between 0 to 60 \n \n"
           "-m 10 40: Calculate fibonacci using MRU cache 10 times with "
           "fibonacci value position randomly between 0 to 40 \n \n");
    break;
  case 'm':
    if (argc > 3) {
      number_of_tests = atoi(argv[2]);
      max_fib_val = atoi(argv[3]);
      calculate_fib_mru(number_of_tests, max_fib_val);
    } else {
      printf("Not enough arguments \n");
    }
    break;
  case 'l':
    if (argc > 3) {
      number_of_tests = atoi(argv[2]);
      max_fib_val = atoi(argv[3]);
      calculate_fib_lru(number_of_tests, max_fib_val);
    } else {
      printf("Not enough arguments \n");
    }
    break;
  default:
    printf("Invalid command \n");
  }
}

void calculate_fib_lru(number_of_tests, max_fib_val) {
  unsigned long long int fib_val = 0;
  if (DEBUG_MODE) {
    int debug_fib = 60;
    for (int ix = 0; ix < number_of_tests; ix++) {
      debug_fib = rand() % max_fib_val;
      printf("\n \n -------- RESULT -------- \n \n fib %d = %llu\n", debug_fib,
             fib_lru(debug_fib));
    }
    print_slots(lru_cache);
  } else {
    for (int ix = 0; ix < number_of_tests; ix++) {
      int fib_to_get = rand() % max_fib_val;
      fib_val = fib_lru(fib_to_get);
      cache_hit_ratio = (cache_hit / call_index) * 100;
      printf("\n Cache size = %d, n = %d, fib value = %llu, %f%% cache hit "
             "ratio \n ",
             CACHE_SIZE, fib_to_get, fib_val, cache_hit_ratio);
    }
  }
  return;
}

void calculate_fib_mru(number_of_tests, max_fib_val) {
  unsigned long long int fib_val = 0;
  if (DEBUG_MODE) {
    int debug_fib = 60;
    for (int ix = 0; ix < number_of_tests; ix++) {
      debug_fib = rand() % max_fib_val;
      printf("\n \n -------- RESULT -------- \n \n fib %d = %llu\n", debug_fib,
             fib_mru(debug_fib));
    }
    print_slots(lru_cache);
  } else {
    for (int ix = 0; ix < number_of_tests; ix++) {
      int fib_to_get = rand() % max_fib_val;
      fib_val = fib_mru(fib_to_get);
      cache_hit_ratio = (cache_hit / call_index) * 100;
      printf("\n Cache size = %d, n = %d, fib value = %llu, %f%% cache hit "
             "ratio \n ",
             CACHE_SIZE, fib_to_get, fib_val, cache_hit_ratio);
    }
  }
  return;
}
unsigned long long int fib_lru(int position) {
  ++call_index;
  if (DEBUG_MODE) {
    printf("\nCurrently at position: %d\n", position);
    printf("With current cache: \n");
    print_slots(lru_cache);
  }

  // Base case
  if (position == 0) {
    return 0;
  }
  if (position == 1) {
    return 1;
  }

  // Find an existing position or find an empty slot
  for (int i = 0; i < CACHE_SIZE; i++) {
    // Find and update least called index
    if (lru_cache[i].last_called_index < least_used) {
      least_used = lru_cache[i].last_called_index;
      if (DEBUG_MODE) {
        printf("\n new lru: %d, index is: %d \n", least_used, i);
      }
      least_recently_used_idx = i;
    }
    // Found the exisitng position, mark it as recently used and return
    if (lru_cache[i].fib_n == position) {
      cache_hit += 1;
      lru_cache[i].last_called_index = call_index;
      if (DEBUG_MODE) {
        printf("\n Slot %d just been used, update last called index: %d", i,
               lru_cache[i].last_called_index);
        printf("\n Current cache: \n");
        print_slots(lru_cache);
      }
      return lru_cache[i].fib_val;
    }
  }
  unsigned long long int fib = fib_lru(position - 1) + fib_lru(position - 2);
  for (int i = 0; i < CACHE_SIZE; i++) {
    // Find an empty slot, update the slot
    if (lru_cache[i].fib_n == 0) {
      empty_slot -= 1;
      if (lru_cache[i].last_called_index < least_used) {
        least_used = lru_cache[i].last_called_index;
        if (DEBUG_MODE) {
          printf("\n new lru: %d, index is: %d \n", least_used, i);
        }
        least_recently_used_idx = i;
      }
      update_slot(i, position, call_index, fib);
      if (DEBUG_MODE) {
        printf("\n Current cache: \n");
        print_slots(lru_cache);
      }
      break;
    }
  }

  // If all slots are full, evict LRU slot and update the slot
  if (empty_slot == 0) {
    if (DEBUG_MODE) {
      printf("\n All slots are full, evicting LRU cache slot: %d \n",
             least_recently_used_idx);
    }
    least_used = INT_MAX;
    update_slot(least_recently_used_idx, position, call_index, fib);
    if (DEBUG_MODE) {
      printf("\n Current cache: \n");
      print_slots(lru_cache);
    }
  }
  return fib;
}

unsigned long long int fib_mru(int position) {
  ++call_index;
  if (DEBUG_MODE) {
    printf("\nCurrently at position: %d\n", position);
    printf("With current cache: \n");
    print_slots(mru_cache);
  }

  // Base case
  if (position == 0) {
    return 0;
  }
  if (position == 1) {
    return 1;
  }

  // Find an existing position or find an empty slot
  for (int i = 0; i < CACHE_SIZE; i++) {
    // Find and update most called index
    if (mru_cache[i].last_called_index > most_used) {
      most_used = mru_cache[i].last_called_index;
      if (DEBUG_MODE) {
        printf("\n new lru: %d, index is: %d \n", most_used, i);
      }
      most_recently_used_idx = i;
    }
    // Found the exisitng position, mark it as recently used and return
    if (mru_cache[i].fib_n == position) {
      cache_hit += 1;
      call_index++;
      mru_cache[i].last_called_index = call_index;
      if (DEBUG_MODE) {
        printf("\n Slot %d just been used, update last called index: %d", i,
               mru_cache[i].last_called_index);
        printf("\n Current cache: \n");
        print_slots(mru_cache);
      }
      return mru_cache[i].fib_val;
    }
  }
  unsigned long long int fib = fib_mru(position - 1) + fib_mru(position - 2);
  for (int i = 0; i < CACHE_SIZE; i++) {
    // Find an empty slot, update the slot
    if (mru_cache[i].fib_n == 0) {
      empty_slot -= 1;
      if (mru_cache[i].last_called_index > most_used) {
        most_used = mru_cache[i].last_called_index;
        if (DEBUG_MODE) {
          printf("\n new mru: %d, index is: %d \n", least_used, i);
        }
        most_recently_used_idx = i;
      }
      update_slot(i, position, call_index, fib);
      if (DEBUG_MODE) {
        printf("\n Current cache: \n");
        print_slots(mru_cache);
      }
      break;
    }
  }

  // If all slots are full, evict MRU slot and update the slot
  if (empty_slot == 0) {
    if (DEBUG_MODE) {
      printf("\n All slots are full, evicting LRU cache slot: %d \n",
             most_recently_used_idx);
    }
    most_used = INT_MIN;
    update_slot(most_recently_used_idx, position, call_index, fib);
    if (DEBUG_MODE) {
      printf("\n Current cache: \n");
      print_slots(mru_cache);
    }
  }
  return fib;
}

// helper function to update the slot with new position, new value, mark as
// recently used
void update_slot(int index, int position, int call_index,
                 unsigned long long int fib) {
  lru_cache[index].last_called_index = call_index;
  lru_cache[index].fib_val = fib;
  lru_cache[index].fib_n = position;
}

// helper function to print all slots in cache
void print_slots(struct CacheEntry cache[CACHE_SIZE]) {
  for (int i = 0; i < CACHE_SIZE; i++) {
    printf("Slot %d: fib_n: %d fib_val: %d, last_called_index %d \n", i,
           cache[i].fib_n, cache[i].fib_val, cache[i].last_called_index);
  }
}