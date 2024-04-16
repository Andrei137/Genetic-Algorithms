#pragma once

#include "Utils.h"
#include <vector>

class Chromosome
{
    // Members
    Interval m_interval{};
    int m_length{};
    long double m_discretization_step{};
    std::vector<bool> m_genes{};
    long double m_selection_probability{};

public:
    // Initializers
    explicit Chromosome(Interval a_interval, int a_length, long double a_discretization_step, const std::vector<bool>& a_genes = {});
    void generate_genes();

    // Setters
    void flip_gene(int a_idx);
    void set_genes(const std::vector<bool>& a_genes);
    void set_selection_probability(long double a_selection_probability);

    // Getters
    const std::vector<bool>& get_genes() const;
    long double get_selection_probability() const;

    // Decoding
    long long base2_to_base10() const;
    long double decode() const;

    // Operators
    bool operator==(const Chromosome& a_chromosome) const;

    // I/O
    void print_genes(std::ostream& a_out) const;
    friend std::ostream& operator<<(std::ostream& a_out, const Chromosome& a_chromosome);
};