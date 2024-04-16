#pragma once

#include "Chromosome.h"
#include <iostream>
#include <memory>
#include <tuple>

class Genetics
{
    // Singleton
    static std::unique_ptr<Genetics> m_instance;

    // Members
    int m_population_size{};
    Interval m_interval{};
    std::tuple<long double, long double, long double> m_parameters{};
    int m_precision{};
    long double m_crossover_probability{};
    long double m_mutation_probability{};
    int m_steps{};
    std::vector<Chromosome> m_population{};
    std::vector<long double> m_intervals{};
    int m_length{};
    long double m_discretization_step{};
    bool m_print_info{};

    // Private constructor
    Genetics();

    // Setters
    void set_length();
    void set_discretization_step();
    void generate_population();

    // Fitness
    long double fitness(long double a_value) const; // fitness(x) = f(x)
    long double total_fitness() const;
    long double average_fitness() const;
    long double elite_fitness() const;
    Chromosome get_elite() const;

    // Selection
    void set_selection_probabilities(); // p_i = fitness(x_i) / fitness(x)
    void set_selection_intervals(); // q_i = p_1 + ... + p_i
    int binary_search_chromosome(long double a_chromosome) const; // [q_i, q_{i + 1}) -> i
    std::vector<Chromosome> selection() const;

    // Crossover
    void cross_chromosomes(Chromosome& a_chromosome1, Chromosome& a_chromosome2, int a_idx) const;
    void mark_for_crossover(std::vector<int>& a_marked) const;
    std::vector<Chromosome> crossover() const;

    // Mutation
    std::vector<Chromosome> mutation() const;

    // Full step
    void next_generation();

    // I/O
    friend std::istream& operator>>(std::istream& a_in, Genetics& a_genetics);
    void print_generation(std::ostream& a_out = std::cout) const;
    friend std::ostream& operator<<(std::ostream& a_out, const Genetics& a_genetics);

public:
    static Genetics& get_instance();

    void run();
};
