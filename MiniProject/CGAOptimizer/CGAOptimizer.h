//
// Created by Jakub Radzik on 22/12/2021.
//

#ifndef MINIPROJECT_CGAOPTIMIZER_H
#define MINIPROJECT_CGAOPTIMIZER_H


#include <vector>
#include <unistd.h>
#include "CGAIndividual/CGAIndividual.h"

class CGAOptimizer {
public:
    CGAOptimizer(int populationSize, double mutationProbability, double crossoverProbability);

    ~CGAOptimizer();

    void v_initialize();

    void v_run_iteration();

    CGAIndividual *v_select_parent();

    std::tuple<CGAIndividual *, CGAIndividual *> v_crossover(CGAIndividual &parent1, CGAIndividual &parent2);

    void v_mutate(CGAIndividual &individual);

private:
    int i_population_size;
    double i_crossover_probability;
    double i_mutation_probability;
    int i_genotype_size;
    std::vector<CGAIndividual *> population;
};

#endif //MINIPROJECT_CGAOPTIMIZER_H

CGAOptimizer::CGAOptimizer(int populationSize, double mutationProbability, double crossoverProbability) {
    i_population_size = populationSize;
    i_mutation_probability = mutationProbability;
    i_crossover_probability = crossoverProbability;
    i_genotype_size = 100;
}

CGAOptimizer::~CGAOptimizer() {
    population.clear();
}

void CGAOptimizer::v_initialize() {
    std::cout << "Initializing population" << std::endl;

    for (int i = 0; i < i_population_size; i++) {
        population.push_back(new CGAIndividual(i_genotype_size, "Parent-" + std::to_string(i)));
    }

    for (int i = 0; i < i_population_size; i++) {
        population[i]->v_randomize();
        std::cout << population[i]->getSName() << " initialized with genotype: "
                  << population[i]->s_genotype_to_string() << std::endl;
    }
}

void CGAOptimizer::v_run_iteration() {
    std::cout << "Running iteration" << std::endl;
    std::vector new_population = std::vector<CGAIndividual *>();
    CGAIndividual *parent1;
    CGAIndividual *parent2;
    CGAIndividual *child1;
    CGAIndividual *child2;

    while (new_population.size() < i_population_size) {
        parent1 = v_select_parent();
        do {
            parent2 = v_select_parent();
        } while (parent1 == parent2);

        std::tie(child1, child2) = v_crossover(*parent1, *parent2);
        v_mutate(*child1);
        v_mutate(*child2);

        new_population.push_back(child1);
        new_population.push_back(child2);
    }
    population = std::move(new_population);

    std::cout << "New population:" << std::endl;
    for (int i = 0; i < i_population_size; i++) {
        std::cout << population[i]->getSName() << " genotype: "
                  << population[i]->s_genotype_to_string() << std::endl;
    }
}

CGAIndividual *CGAOptimizer::v_select_parent() {
    return population[rand() % i_population_size];
}

std::tuple<CGAIndividual *, CGAIndividual *>
CGAOptimizer::v_crossover(CGAIndividual &parent1, CGAIndividual &parent2) {
    if (((double) (rand() % 100) / 100) < i_crossover_probability) {
        CGAIndividual *child1 = new CGAIndividual(i_genotype_size);
        CGAIndividual *child2 = new CGAIndividual(i_genotype_size);

        child1->v_crossover(parent1, parent2);
        child2->v_crossover(parent1, parent2);

        child1->setSName("[" + parent1.getSName() + "-" + parent2.getSName() + "]");
        child2->setSName("[" + parent1.getSName() + "-" + parent2.getSName() + "]");

        std::tuple<CGAIndividual *, CGAIndividual *> children = std::make_tuple(child1, child2);

        child1 = nullptr;
        child2 = nullptr;

        delete child1;
        delete child2;

        return children;
    }
    return std::make_tuple(new CGAIndividual(parent1), new CGAIndividual(parent2));
}

void CGAOptimizer::v_mutate(CGAIndividual &individual) {
    individual.v_mutation(i_mutation_probability);
}
