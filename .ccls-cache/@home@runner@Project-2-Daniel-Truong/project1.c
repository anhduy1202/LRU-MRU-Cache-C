#include <stdio.h>
#define MAX_INPUT_VALUE (100000 - 1)

int iterative_nth_fib(int position);
long long unsigned int recursion_nth_fib(int position);
long long unsigned int cache_nth_fib(int position, int memo[]);

int test(int argc, char *argv[]) {
  FILE *file;
  int number1 = atoi(argv[1]);
  int number2, total;
  unsigned long int output;
  char option = argv[2][0];
  char filename[50];
  strcpy(filename, argv[3]);

  if (number1 >= MAX_INPUT_VALUE) {
    printf("Sorry, %d is the largest input value\n", MAX_INPUT_VALUE);
    exit(1);
  }

  file = fopen(filename, "r");

  if (file == NULL) {
    printf("Error! file doesn't exist or wrong file name \n");
    exit(1);
  }

  fscanf(file, "%d", &number2);
  total = number1 + number2;
  int memo[total + 1];
  memset(memo, 0, (total + 1) * sizeof(int));

  switch (option) {
  case 'i':
    output = iterative_nth_fib(total);
    break;
  case 'r':
    output = recursion_nth_fib(total);
    break;
  case 'c':
    output = cache_nth_fib(total, memo);
    break;
  default:
    printf("Invalid input \n");
  }

  printf("the %dth position of Fibonacci Number: %lu \n", total, output);
  fclose(file);

  return 0;
}

int iterative_nth_fib(int position) {
  // Pre-defined first 2 values in fibonacci sequence to build up later value
  int fib_sequence[position + 1];
  fib_sequence[0] = 0;
  fib_sequence[1] = 1;
  for (int i = 2; i <= position; i++) {
    fib_sequence[i] = fib_sequence[i - 1] + fib_sequence[i - 2];
  }
  return fib_sequence[position];
}

long long unsigned int recursion_nth_fib(int position) {
  // Base case
  long long unsigned int fib = 1;
  if (position == 0) {
    return 0;
  }
  if (position == 1) {
    return 1;
  }

  fib = recursion_nth_fib(position - 1) + recursion_nth_fib(position - 2);
  return fib;
}

long long unsigned int cache_nth_fib(int position, int memo[]) {
  long long unsigned int fib = 1;
  if (position == 0) {
    return 0;
  }
  if (position == 1) {
    return 1;
  }
  // If result already stored in cache, return the result right away
  if (memo[position] > 0) {
    return memo[position];
  }
  // If not, store new result in cache
  memo[position] =
      cache_nth_fib(position - 1, memo) + cache_nth_fib(position - 2, memo);
  fib = memo[position];
  return fib;
}