#ifndef LIBMYCGEN_GG_TRICKS_H
#define LIBMYCGEN_GG_TRICKS_H

#include <mygen/genetic_generator.h>

struct gg_auto;

struct gg_auto *init_gg_auto(
    size_t solution_size,
    void (*generate_random_solution)(void *solution),
    signed long int (*fit)(const void *solution),
    void (*crossover)(void *solution, const void *solution_a, const void *solution_b)
  );
struct gg_auto *copy_gg_auto(struct gg_auto *self);
void free_gg_auto(struct gg_auto *self);

struct genetic_generator *gga_get_genetic_generator_copy(const struct gg_auto *self);

void gga_run_generations(struct gg_auto *self, size_t generations);

void gga_get_top_solution(const struct gg_auto *self, void *solution, size_t idx);

void get_automatic_genetic_solution(
    size_t solution_size,
    void (*generate_random_solution)(void *solution),
    signed long int (*fit)(const void *solution),
    void (*crossover)(void *solution, const void *solution_a, const void *solution_b),
    size_t generations,
    void *solution
  );


#endif
