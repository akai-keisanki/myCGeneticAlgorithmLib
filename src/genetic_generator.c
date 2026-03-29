#include "../include/genetic_generator.h"

#include <stdlib.h>
#include <string.h>

void gg_sort_population(struct genetic_generator *self);
void gg_run_generation(struct genetic_generator *self, size_t cut, unsigned short int crossover_per_mille);

struct genetic_generator
{
  size_t solution_size;
  void (*generate_random_solution)(void *solution);
  signed long int (*fit)(const void *solution);
  void (*crossover)(void *solution, const void *solution_a, const void *solution_b);
  size_t population_size;
  void *population;
  signed long int *fits;
};

struct genetic_generator *init_genetic_generator(
    const size_t solution_size,
    void (* const generate_random_solution)(void *solution),
    signed long int (* const fit)(const void *solution),
    void (* const crossover)(void *solution, const void *solution_a, const void *solution_b),
    const size_t population_size
  )
{
  struct genetic_generator *self = malloc(sizeof(struct genetic_generator));

  self->solution_size = solution_size;
  self->generate_random_solution = generate_random_solution;
  self->fit = fit;
  self->crossover = crossover;
  self->population_size = population_size;

  self->population = malloc(self->solution_size * self->population_size);
  self->fits = malloc(sizeof(signed long int) * self->population_size);
  gg_run_generation(self, 0, 0);

  return self;
}

struct genetic_generator *copy_genetic_generator(const struct genetic_generator * const self)
{
  struct genetic_generator *copy = malloc(sizeof(struct genetic_generator));

  copy->solution_size = self->solution_size;
  copy->generate_random_solution = self->generate_random_solution;
  copy->fit = self->fit;
  copy->crossover = self->crossover;
  copy->population_size = self->population_size;

  copy->population = malloc(self->solution_size * self->population_size);
  memcpy(copy->population, self->population, self->solution_size * self->population_size);

  copy->fits = malloc(sizeof(signed long int) * self->population_size);
  memcpy(copy->fits, self->fits, sizeof(signed long int) * self->population_size);

  return copy;
}

void free_genetic_generator(struct genetic_generator *self)
{
  free(self->population);
  free(self->fits);
  free(self);
}

void gg_set_fit(struct genetic_generator * const self, signed long int (* const fit)(const void *solution))
{
  self->fit = fit;
}

void gg_set_crossover(struct genetic_generator * const self, void (* const crossover)(void *solution, const void *solution_a, const void *solution_b))
{
  self->crossover = crossover;
}

void gg_set_population_size(struct genetic_generator *self, size_t population_size)
{
  void *population = self->population;
  signed long int *fits = self->fits;

  self->population_size ^= population_size;
  population_size ^= self->population_size;
  self->population_size ^= population_size;

  self->population = malloc(self->solution_size * self->population_size);
  memcpy(self->population, population, self->solution_size * population_size);

  self->fits = malloc(sizeof(signed long int) * self->population_size);
  memcpy(self->fits, fits, sizeof(signed long int) * population_size);

  gg_run_generation(self, population_size, 0);

  free(population);
  free(fits);
}

void gg_sort_population_step(struct genetic_generator * const self, const size_t left, const size_t right)
{
  if (right - left <= 1) return;

  size_t middle = (left + right) / 2;
  gg_sort_population_step(self, left, middle);
  gg_sort_population_step(self, middle, right);

  char *pop_copy = malloc(self->solution_size * (right - left));
  signed long int *fits_copy = malloc(sizeof(signed long int) * (right - left));
  for(size_t i = left; i < right; ++ i)
  {
    memcpy(pop_copy + (i - left) * self->solution_size, (char*)self->population + i * self->solution_size, self->solution_size);
    fits_copy[i - left] = self->fits[i];
  }

  size_t i = left;
  size_t il = 0;
  size_t ir = middle - left;
  while (il < middle - left && ir < right - left)
  {
    if (fits_copy[il] >= fits_copy[ir])
    {
      memcpy((char*)self->population + i * self->solution_size, pop_copy + il * self->solution_size, self->solution_size);
      self->fits[i] = fits_copy[il];
      ++il;
    }
    else
    {
      memcpy((char*)self->population + i * self->solution_size, pop_copy + ir * self->solution_size, self->solution_size);
      self->fits[i] = fits_copy[ir];
      ++ir;
    }
    ++i;
  }

  while (il < middle - left)
  {
    memcpy((char*)self->population + i * self->solution_size, pop_copy + il * self->solution_size, self->solution_size);
    self->fits[i] = fits_copy[il];
    ++ il;
    ++i;
  }

  while (ir < right - left)
  {
    memcpy((char*)self->population + i * self->solution_size, pop_copy + ir * self->solution_size, self->solution_size);
    self->fits[i] = fits_copy[ir];
    ++ ir;
    ++i;
  }

  free(pop_copy);
  free(fits_copy);
}

void gg_sort_population(struct genetic_generator * const self)
{
  gg_sort_population_step(self, 0, self->population_size);
}

void gg_run_generation(struct genetic_generator * const self, size_t cut, const unsigned short int crossover_per_mille)
{
  if (cut > self->population_size) return;

  for (size_t i = cut; i < self->population_size; ++i)
  {
    if (cut > 0 && rand() % 1000 < crossover_per_mille)
      self->crossover((char*)self->population + i * self->solution_size, (char*)self->population + rand() % cut * self->solution_size, (char*)self->population + rand() % cut * self->solution_size);
    else
      self->generate_random_solution((char*)self->population + i * self->solution_size);

    self->fits[i] = self->fit((char*)self->population + i * self->solution_size);
  }

  gg_sort_population(self);
}

void gg_run_generations(struct genetic_generator * const self, const size_t cut, const unsigned short int crossover_per_mille, size_t generations)
{
  for (size_t i = 0; i < generations; ++i)
    gg_run_generation(self, cut, crossover_per_mille);
}

void gg_get_top_solution(const struct genetic_generator * const self, void * const solution, size_t idx)
{
  if (idx >= self->population_size) idx = self->population_size - 1;
  memcpy(solution, (char*)self->population + idx * self->solution_size, self->solution_size);
}

signed long int gg_get_top_fit(const struct genetic_generator * const self, size_t idx)
{
  if (idx >= self->population_size) idx = self->population_size - 1;
  return self->fits[idx];
}
