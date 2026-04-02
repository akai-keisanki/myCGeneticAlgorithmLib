#include "../include/gg_tricks.h"

#include <stdlib.h>
#include <math.h>

#include <stdio.h>

struct gg_auto
{
  struct genetic_generator *gg;
  size_t solution_size;
  unsigned short int crossover_per_mille;
  size_t population_size;
  size_t population_cut;
};

struct gg_auto *init_gg_auto(
    const size_t solution_size,
    void (* const generate_random_solution)(void *solution),
    signed long int (* const fit)(const void *solution),
    void (* const crossover)(void *solution, const void *solution_a, const void *solution_b)
  )
{
  struct gg_auto *self = malloc(sizeof(struct gg_auto));

  self->solution_size = solution_size;

  self->population_size = 0x20 * (size_t)round(pow((double)solution_size, 3.0/4.0));
  self->crossover_per_mille = 120 + 880 / sqrt(self->population_size);
  self->population_cut = (self->population_size + 100) / 8;

  self->gg = init_genetic_generator(self->solution_size, generate_random_solution, fit, crossover, self->population_size);

  return self;
}

struct gg_auto *copy_gg_auto(struct gg_auto *self)
{
  struct gg_auto *copy = malloc(sizeof(struct gg_auto));

  copy->solution_size = self->solution_size;

  copy->population_size = self->population_size;
  copy->crossover_per_mille = self->crossover_per_mille;
  copy->population_cut = self->population_cut;

  copy->gg = gga_get_genetic_generator_copy(self);

  return copy;
}

void free_gg_auto(struct gg_auto *self)
{
  free_genetic_generator(self->gg);
  free(self);
}

struct genetic_generator *gga_get_genetic_generator_copy(const struct gg_auto *self)
{
  return copy_genetic_generator(self->gg);
}

double gga_medium_head_fit(const struct gg_auto *self)
{
  const size_t cut = sqrt(self->population_cut);
  double m = 0;

  for (size_t i = 0; i < cut; ++i)
    m += (double)gg_get_top_fit(self->gg, i) / cut;

  return m; 
}

void gga_run_generations(struct gg_auto *self, size_t generations)
{
  double head;
  double p_head = gga_medium_head_fit(self);
  double improvement;

  for (size_t g = 0; g < generations; ++g)
  {
    gg_run_generations(self->gg, self->population_cut, self->crossover_per_mille, 1);

    head = gga_medium_head_fit(self);

    improvement = (head - p_head) / (abs(p_head) + 1e-9);

    if (improvement < 5e-2)
    {
      if (self->crossover_per_mille > 50)
	self->crossover_per_mille -= 7;
    }
    else
      if (self->crossover_per_mille < 950)
	self->crossover_per_mille += 5;

    p_head = head;
  }
}

void gga_get_top_solution(const struct gg_auto *self, void *solution, size_t idx)
{
  gg_get_top_solution(self->gg, solution, idx);
}

void get_automatic_genetic_solution(
    size_t solution_size,
    void (*generate_random_solution)(void *solution),
    signed long int (*fit)(const void *solution),
    void (*crossover)(void *solution, const void *solution_a, const void *solution_b),
    size_t generations,
    void *solution
  )
{
  struct gg_auto *gen = init_gg_auto(
      solution_size,
      generate_random_solution,
      fit,
      crossover
    );

  gga_run_generations(gen, generations);

  gga_get_top_solution(gen, solution, 0);

  free_gg_auto(gen);
}
