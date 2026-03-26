#include "../include/gg_tricks.h"

struct gg_auto
{
    size_t solution_size,
    void (*generate_random_solution)(void *solution),
    signed long int (*fit)(const void *solution),
    void (*crossover)(void *solution, const void *solution_a, const void *solution_b),
    size_t population_size;
};

struct gg_auto *init_gg_auto(
    size_t solution_size,
    void (*generate_random_solution)(void *solution),
    signed long int (*fit)(const void *solution),
    void (*crossover)(void *solution, const void *solution_a, const void *solution_b),
  )
{
  // TODO WIP
}

struct gg_auto *copy_gg_auto(struct gg_auto *self)
{
  // TODO WIP
}

void free_gg_auto(struct gg_auto *self);
{
  // TODO WIP
}

struct genetic_generator *get_genetic_generator_copy(const struct gg_auto *self)
{
  // TODO WIP
}

void run_generations(struct gg_auto *self, size_t generations)
{
  // TODO WIP
}

void get_top_solution(const struct gg_auto *self, void *solution, size_t idx)
{
  // TODO WIP
}
