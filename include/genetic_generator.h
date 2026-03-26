#ifndef CGENETICALG_GENETIC_GENERATOR_H
#define CGENETICALG_GENETIC_GENERATOR_H

#include <stddef.h>

struct genetic_generator;

struct genetic_generator *init_genetic_generator(
    size_t solution_size,
    void (*generate_random_solution)(void *solution),
    signed long int (*fit)(const void *solution),
    void (*crossover)(void *solution, const void *solution_a, const void *solution_b),
    size_t population_size
  );

struct genetic_generator *copy_genetic_generator(const struct genetic_generator *self);

void free_genetic_generator(struct genetic_generator *self);

void set_fit(struct genetic_generator *self, signed long int (*fit)(const void *solution));
void set_crossover(struct genetic_generator *self, void (*crossover)(void *solution, const void *solution_a, const void *solution_b));

void set_population_size(struct genetic_generator *self, size_t population_size);

void run_generations(struct genetic_generator *self, size_t cut, unsigned short int crossover_per_mille, size_t generations);

void get_top_solution(const struct genetic_generator *self, void *solution, size_t idx);

#endif
