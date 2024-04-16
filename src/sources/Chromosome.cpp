#include "Chromosome.h"
#include <cmath>

Chromosome::Chromosome(Interval a_interval, int a_length, long double a_discretization_step, const std::vector<bool>& a_genes) : m_interval(a_interval), m_length(a_length), m_discretization_step(a_discretization_step), m_genes(std::move(a_genes))
{
    if (a_genes.empty())
    {
        generate_genes();
    }
}

void Chromosome::generate_genes()
{
    for (int i = 0; i < m_length; ++i)
    {
        m_genes.push_back(get_random() < 0.5);
    }
}

void Chromosome::flip_gene(int a_idx)
{
    m_genes[a_idx] = !m_genes[a_idx];
}

void Chromosome::set_genes(const std::vector<bool>& a_genes)
{
    m_genes = std::move(a_genes);
}

void Chromosome::set_selection_probability(long double a_selection_probability)
{
    m_selection_probability = a_selection_probability;
}

const std::vector<bool>& Chromosome::get_genes() const
{
    return m_genes;
}

long double Chromosome::get_selection_probability() const
{
    return m_selection_probability;
}

long long Chromosome::base2_to_base10() const
{
    long long result{ 0 };
    for (int i = 0; i < m_length; ++i)
    {
        if (m_genes[i])
        {
            result += (1 << i);
        }
    }
    return result;
}

long double Chromosome::decode() const
{
    return m_interval.m_left + base2_to_base10() * m_discretization_step;
}

bool Chromosome::operator==(const Chromosome& a_chromosome) const
{
    return m_genes == a_chromosome.m_genes;
}

std::ostream& operator<<(std::ostream& a_out, const Chromosome& a_chromosome)
{
    a_chromosome.print_genes(a_out);
    align_sign(a_out, " | x = ", a_chromosome.decode());
    return a_out;
}

void Chromosome::print_genes(std::ostream& a_out) const
{
    for (const auto& gene : m_genes)
    {
        a_out << gene;
    }
}