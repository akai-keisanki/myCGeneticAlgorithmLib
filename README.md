# myCGeneticAlgorithmLib

Trying to make a genetic algorithm library in C

# What is a genetic algorithm?

A genetic algorithm is a heuristic solution to a problem based on the principles of natural selection and genetics.

The goal is to generate and select an optimal solution with specific scoring criteria (fitness), iterating over generations that select the best cut of solutions and replace the rest by other or mixed (crossover) ones.

# Examples

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

void crossover(struct solution * const solution, struct solution * const solution_a, struct solution * const solution_b)
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

  run_generations(gen, CUT, CROSSOVER_PER_MILLE, GENERATIONS);

  for (size_t i = 0; i < 10; ++i)
  {
    get_top_solution(gen, &solution, i);

    printf("\n%lluth best solution: %.10lf\n", i, solution.x);
    printf("%luth best fitness: %li\n\n", i, fit(&solution));
  }

  free_genetic_generator(gen);

  return 0;
}
```

# How to test it

Create a `test.c` file.
You can write your own code or use one of the examples.

Run:
```sh
make test
```

There must be an executable `test` file with the library linked.
