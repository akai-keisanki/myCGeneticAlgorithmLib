# myCGeneticAlgorithmLib

A simple library for genetic algorithm solutions in C.

## Introduction

A genetic algorithm is a heuristic solution to a problem based on the principles of natural selection and genetics.

The goal is to generate and select an optimal solution with specific scoring criteria (fitness) by iterating over generations that select the best cut of solutions and replace the rest by other or mixed (crossover) ones.

This library provides a concise, generic and straightforward way to implement this process.

## Usage

In order to implement your custom solution:

- You must define:
    - The data structure which represents a solution (e.g. `struct solution`);
    - A generator function for a random solution (`void (struct solution *)`);
    - A fitness scorer function for a solution (`signed long int (const struct solution *)`) [^1];
    - A crossover function to mix 2 solutions (`void (struct solution *, const struct solution *, const struct solution *)`);
    - The size of the population;

- Then use the generator:
    - Initialize a `struct genetic_generator *` object with `init_genetic_generator(...)` according to the previous definitions (e.g. `struct genetic_generator *gen = init_genetic_generator(sizeof(struct solution), generate_random_solution, fit, crossover, POPULATION_SIZE)`[^2]);
    - Run generations with `run_generations(...)` (e.g. 200 generations with a cut of the 100 best solutions and 25% of crossover rate: `run_generations(gen, 100, 250, 200)`);
    - Copy a solution from the population to a solution object with `get_top_solution(...)` (e.g. the top one: `get_top_solution(gen, &solution, 0)`);
    - Free the generator object with `free_genetic_generator`;

[^1]: Higher fitness scores mean a better solution. Since the algorithm will compare the scores, you might want to have unique scores for every possible solution.
[^2]: The crossover rate is given per mille (i.e. 0-1000 where 1000 = 100%).

### How to test it

Create a `test.c` file.
You can write your own code or use one of the examples at the end of this file.

To run your test:
```sh
make run_test
```

This will compile your script and generate an executable binary file named `test` with the library linked and run it.

### Examples

#### 1. Square root

Code:
```c
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "include/genetic_generator.h"

const size_t POPULATION_SIZE = 0x100;
const size_t CUT = 0x20;
const size_t CROSSOVER_PER_MILLE = 333;
const size_t GENERATIONS = 0x20;
double target = 0.0;

struct solution
{
  double x;
};

void generate_random_solution(struct solution * const solution)
{
  solution->x = rand() % 2000000000 / 10000000.0 - 100.0;
}

signed long int fit(const struct solution * const solution)
{
  return -round(fabs(solution->x * solution->x - target) * 10000000000.0);
}

void crossover(struct solution * const solution, const struct solution * const solution_a, const struct solution * const solution_b)
{
  solution->x = (solution_a->x + solution_b->x) / 2;
}

signed int main(void)
{
  srand(time(0));

  puts("==============================\nGenetic Square Root Calculator\n==============================\n\n");
  puts("Input number: ");
  
  scanf("%lf", &target);

  struct genetic_generator *gen = init_genetic_generator(
      sizeof(struct solution),
      generate_random_solution,
      fit,
      crossover,
      POPULATION_SIZE
    );
  struct solution solution;

  gg_run_generations(gen, CUT, CROSSOVER_PER_MILLE, GENERATIONS);

  for (size_t i = 0; i < 10; ++i)
  {
    gg_get_top_solution(gen, &solution, i);

    printf("\n%luth best solution: %.10lf\n", i, solution.x);
    printf("%luth best fitness: %li\n", i, fit(&solution));
  }

  free_genetic_generator(gen);

  return 0;
}
```

Execution example:
```
==============================
Genetic Square Root Calculator
==============================


Input number:
2

0th best solution: 1.4142135624
0th best fitness: 0

1th best solution: 1.4142135624
1th best fitness: 0

2th best solution: 1.4142135624
2th best fitness: 0

3th best solution: 1.4142135624
3th best fitness: 0

4th best solution: 1.4142135624
4th best fitness: 0

5th best solution: 1.4142135624
5th best fitness: 0

6th best solution: 1.4142135624
6th best fitness: 0

7th best solution: 1.4142135624
7th best fitness: 0

8th best solution: 1.4142135624
8th best fitness: 0

9th best solution: 1.4142135624
9th best fitness: 0

```

#### 1. Square root with `gg_auto`

Code:
```c
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "include/gg_tricks.h"

const size_t GENERATIONS = 0x20;
double target = 0.0;

struct solution
{
  double x;
};

void generate_random_solution(struct solution * const solution)
{
  solution->x = rand() % 2000000000 / 10000000.0 - 100.0;
}

signed long int fit(const struct solution * const solution)
{
  return -round(fabs(solution->x * solution->x - target) * 10000000000.0);
}

void crossover(struct solution * const solution, const struct solution * const solution_a, const struct solution * const solution_b)
{
  solution->x = (solution_a->x + solution_b->x) / 2;
}

signed int main(void)
{
  srand(time(0));

  puts("==============================\nGenetic Square Root Calculator\n==============================\n\n");
  puts("Input number: ");
  
  scanf("%lf", &target);

  struct gg_auto *gen = init_gg_auto(
      sizeof(struct solution),
      generate_random_solution,
      fit,
      crossover
    );
  struct solution solution;

  gga_run_generations(gen, GENERATIONS);

  for (size_t i = 0; i < 10; ++i)
  {
    gga_get_top_solution(gen, &solution, i);

    printf("\n%luth best solution: %.10lf\n", i, solution.x);
    printf("%luth best fitness: %li\n", i, fit(&solution));
  }

  free_gg_auto(gen);

  return 0;
}
```

Execution example:
```
==============================
Genetic Square Root Calculator
==============================


Input number:
2

0th best solution: -1.4142135624
0th best fitness: 0

1th best solution: -1.4142135624
1th best fitness: 0

2th best solution: -1.4142135623
2th best fitness: -1

3th best solution: -1.4142135623
3th best fitness: -1

4th best solution: -1.4142135624
4th best fitness: -1

5th best solution: -1.4142135624
5th best fitness: -1

6th best solution: -1.4142135624
6th best fitness: -1

7th best solution: -1.4142135624
7th best fitness: -1

8th best solution: -1.4142135624
8th best fitness: -2

9th best solution: -1.4142135623
9th best fitness: -2

```

#### 1. Square root with `get_automatic_genetic_solution`

Code:
```c
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdio.h>

#include "include/gg_tricks.h"

const size_t GENERATIONS = 0x20;
double target = 0.0;

struct solution
{
  double x;
};

void generate_random_solution(struct solution * const solution)
{
  solution->x = rand() % 2000000000 / 10000000.0 - 100.0;
}

signed long int fit(const struct solution * const solution)
{
  return -round(fabs(solution->x * solution->x - target) * 10000000000.0);
}

void crossover(struct solution * const solution, const struct solution * const solution_a, const struct solution * const solution_b)
{
  solution->x = (solution_a->x + solution_b->x) / 2;
}

signed int main(void)
{
  srand(time(0));

  puts("==============================\nGenetic Square Root Calculator\n==============================\n\n");
  puts("Input number: ");
  
  scanf("%lf", &target);
  
  struct solution solution;

  get_automatic_genetic_solution(
      sizeof(struct solution),
      generate_random_solution,
      fit,
      crossover,
      GENERATIONS,
      &solution
    );

  printf("\nBest solution: %.10lf\n", solution.x);
  printf("Best fitness: %li\n", fit(&solution));

  return 0;
}
```

Execution example:
```
==============================
Genetic Square Root Calculator
==============================


Input number:
2

Best solution: -1.4142135624
Best fitness: 0
```
