#include "Genetics.h"
#include <algorithm>
#include <iomanip>

std::unique_ptr<Genetics> Genetics::m_instance{ nullptr };

Genetics::Genetics()
{
    m_print_info = true;
    IO("Genetics");
}

void Genetics::set_length()
{
    m_length = static_cast<int>(std::ceil(std::log2((m_interval.m_right - m_interval.m_left) * std::pow(10, m_precision))));
}

void Genetics::set_discretization_step()
{
    m_discretization_step = ((m_interval.m_right - m_interval.m_left) / (1 << m_length));
}

void Genetics::generate_population()
{
    m_population.clear();
    m_population.reserve(m_population_size);
    for (int i = 0; i < m_population_size; ++i)
    {
        m_population.emplace_back(m_interval, m_length, m_discretization_step);
    }
}

long double Genetics::fitness(long double a_value) const
{
    return a_value * a_value * std::get<0>(m_parameters) + a_value * std::get<1>(m_parameters) + std::get<2>(m_parameters);
}

long double Genetics::total_fitness() const
{
    long double total{ 0 };
    for (const auto& chromosome : m_population)
    {
        total += fitness(chromosome.decode());
    }
    return total;
}

long double Genetics::average_fitness() const
{
    return total_fitness() / m_population_size;
}

long double Genetics::elite_fitness() const
{
    return fitness(get_elite().decode());
}

Chromosome Genetics::get_elite() const
{
    Chromosome elite{ m_population[0] };
    long double max_fitness{ fitness(elite.decode()) };
    for (int i = 1; i < m_population_size; ++i)
    {
        long double current_fitness{ fitness(m_population[i].decode()) };
        if (current_fitness > max_fitness)
        {
            max_fitness = current_fitness;
            elite = m_population[i];
        }
    }
    return elite;
}

void Genetics::set_selection_probabilities()
{
    if (m_print_info)
    {
        std::cout << "\nSelection probabilities\n"; 
    }
    for (int i = 0; i < m_population_size; ++i)
    {
        Chromosome& chromosome{ m_population[i] };
        chromosome.set_selection_probability(fitness(chromosome.decode()) / total_fitness());
        if (m_print_info)
        {
            align_index(std::cout, i);
            std::cout << "Probability = " << chromosome.get_selection_probability() << '\n';
        }
    }
}

void Genetics::set_selection_intervals()
{
    if (m_print_info)
    {
        std::cout << "\nSelection intervals\n0 ";
    }
    m_intervals.clear();
    m_intervals.reserve(m_population_size + 2);
    long double sum{ 0 };
    m_intervals.emplace_back(sum);
    for (int i = 0; i < m_population_size; ++i)
    {
        sum += m_population[i].get_selection_probability();
        if (m_print_info && std::abs(sum - 1) > std::numeric_limits<long double>::epsilon())
        {
            std::cout << sum << ' ';
        }
        m_intervals.emplace_back(sum);
    }
    if (m_print_info)
    {
        std::cout << "1\n";
    }
    m_intervals.emplace_back(1);
    std::sort(m_intervals.begin(), m_intervals.end());
}

int Genetics::binary_search_chromosome(long double a_chromosome) const
{
    int left{ 1 }, right{ m_population_size - 1 };
    while (left <= right)
    {
        int middle{ left + (right - left) / 2 };
        if (m_intervals[middle - 1] <= a_chromosome && m_intervals[middle + 1] >= a_chromosome)
        {
            return middle;
        }
        else if (m_intervals[middle] > a_chromosome)
        {
            right = middle - 1;
        }
        else
        {
            left = middle + 1;
        }
    }
    return left - 1;
}

std::vector<Chromosome> Genetics::selection() const
{
    std::vector<Chromosome> new_population{ get_elite() };
    for (int i = 0; i < m_population_size - 1; ++i)
    {
        long double u{ get_random() };
        int idx{ binary_search_chromosome(u) };
        new_population.push_back(m_population[idx]);
        if (m_print_info)
        {
            std::cout << "u = " << u << " | Choose chromosome " << idx + 1 << '\n';
        }
    }
    return new_population;
}

void Genetics::cross_chromosomes(Chromosome& a_chromosome1, Chromosome& a_chromosome2, int a_idx) const
{
    if (m_print_info)
    {
        a_chromosome1.print_genes(std::cout);
        std::cout << " ";
        a_chromosome2.print_genes(std::cout);
        std::cout << " | point = " << a_idx;
    }

    std::vector<bool> copy1{ a_chromosome1.get_genes() };
    std::vector<bool> copy2{ a_chromosome2.get_genes() };

    for (int i = 0; i < a_idx; ++i)
    {
        copy1[i] = a_chromosome2.get_genes()[i];
        copy2[i] = a_chromosome1.get_genes()[i];
    }

    a_chromosome1.set_genes(copy1);
    a_chromosome2.set_genes(copy2);

    if (m_print_info)
    {
        std::cout << "\nResult: ";
        a_chromosome1.print_genes(std::cout);
        std::cout << " ";
        a_chromosome2.print_genes(std::cout);
        std::cout << '\n';
    }
}

void Genetics::mark_for_crossover(std::vector<int>& a_marked) const
{
    if (m_print_info)
    {
        std::cout.unsetf(std::ios::fixed);
        std::cout << "\nCrossover probability: " << m_crossover_probability << '\n';
        std::cout.setf(std::ios::fixed);
    }
    Chromosome elite{ get_elite() };
    for (int i = 0; i < m_population_size; ++i)
    {
        if (m_population[i] == elite)
        {
            continue;
        }
        long double u{ get_random() };
        if (m_print_info)
        {
            align_index(std::cout, i);
            m_population[i].print_genes(std::cout);
            std::cout << " | u = " << u;
        }
        if (u < m_crossover_probability)
        {
            a_marked.push_back(i);
            if (m_print_info)
            {
                std::cout.unsetf(std::ios::fixed);
                std::cout << " < " << m_crossover_probability << " participant";
                std::cout.setf(std::ios::fixed);
            }
        }
        if (m_print_info)
        {
            std::cout << '\n';
        }
    }
}

std::vector<Chromosome> Genetics::crossover() const
{
    std::vector<Chromosome> new_population{ m_population };
    std::vector<int> marked{};
    mark_for_crossover(marked);
    int size{ static_cast<int>(marked.size()) };

    if (size == 1)
    {
        return new_population;
    }

    if (size & 1)
    {
        int idx{ static_cast<int>(get_random(1, m_length - 1)) };

        // Cross the last 3 chromosomes
        if (m_print_info)
        {
            std::cout << "Triple crossing between " << marked[size - 3] + 1 << ", " << marked[size - 2] + 1 << " and " << marked[size - 1] + 1 << ":\n";
        }
        cross_chromosomes(new_population[marked[size - 3]], new_population[marked[size - 2]], idx);
        cross_chromosomes(new_population[marked[size - 3]], new_population[marked[size - 1]], idx);

        size -= 3;
    }

    for (int i = 0; i < size; i += 2)
    {
        if (m_print_info)
        {
            std::cout << "Crossing chromosomes " << marked[i] + 1 << " and " << marked[i + 1] + 1 << ":\n";
        }
        cross_chromosomes(new_population[marked[i]], new_population[marked[i + 1]], get_random(1, m_length - 1));
    }

    return new_population;
}

std::vector<Chromosome> Genetics::mutation() const
{
    if (m_print_info)
    {
        std::cout.unsetf(std::ios::fixed);
        std::cout << "\nMutation probability: " << m_mutation_probability << '\n';
        std::cout.setf(std::ios::fixed);
        std::cout << "The following chromosomes have been modified:\n";
    }

    Chromosome elite{ get_elite() };
    std::vector<Chromosome> new_population{ m_population };
    for (int i = 0; i < m_population_size; ++i)
    {
        if (new_population[i] == elite)
        {
            continue;
        }
        Chromosome& chromosome{ new_population[i] };
        bool printed{ false };
        for (int j = 0; j < m_length; ++j)
        {
            if (get_random() < m_mutation_probability)
            {
                chromosome.flip_gene(j);
                if (m_print_info && !printed)
                {
                    std::cout << i + 1 << ' ';
                    printed = true;
                }
            }
        }
    }
    return new_population;
}

void Genetics::next_generation()
{
    set_selection_probabilities();
    set_selection_intervals();

    m_population = std::move(selection());
    if (m_print_info)
    {
        std::cout << "\nAfter selection\n";
        print_generation();
    }

    m_population = std::move(crossover());
    if (m_print_info)
    {
        std::cout << "\nAfter crossover\n";
        print_generation();
    }

    m_population = std::move(mutation());
    if (m_print_info)
    {
        std::cout << "\nAfter mutation\n";
        print_generation();
    }
}

std::istream& operator>>(std::istream& a_in, Genetics& a_genetics)
{
    a_genetics.m_population_size = read<int>(a_in);
    a_genetics.m_interval = read<Interval>(a_in);
    std::get<0>(a_genetics.m_parameters) = read<long double>(a_in);
    std::get<1>(a_genetics.m_parameters) = read<long double>(a_in);
    std::get<2>(a_genetics.m_parameters) = read<long double>(a_in);
    a_genetics.m_precision = read<int>(a_in);
    a_genetics.m_crossover_probability = read<long double>(a_in);
    a_genetics.m_mutation_probability = read<long double>(a_in);
    a_genetics.m_steps = read<int>(a_in);

    a_genetics.set_length();
    a_genetics.set_discretization_step();
    a_genetics.generate_population();

    return a_in;
}

void Genetics::print_generation(std::ostream& a_out) const
{
    std::cout << std::setprecision(m_length);
    for (int i = 0; i < m_population_size; ++i)
    {
        align_index(a_out, i);
        const auto& chromosome{ m_population[i] };
        a_out << chromosome;
        align_sign(a_out, " | f(x) = ", fitness(chromosome.decode()));
        a_out << '\n';
    }
    std::cout << std::setprecision(m_precision);
}

std::ostream& operator<<(std::ostream& a_out, const Genetics& a_genetics)
{
    a_out << "Input\n";
    a_out << "Population size: " << a_genetics.m_population_size << '\n';
    a_out << "Interval: " << a_genetics.m_interval << '\n';
    a_out << "Parameters: " << std::get<0>(a_genetics.m_parameters) << ' ' << std::get<1>(a_genetics.m_parameters) << ' ' << std::get<2>(a_genetics.m_parameters) << '\n';
    a_out << "Precision: " << a_genetics.m_precision << '\n';
    a_out << "Crossover probability: " << a_genetics.m_crossover_probability << '\n';
    a_out << "Mutation probability: " << a_genetics.m_mutation_probability << '\n';
    a_out << "Steps: " << a_genetics.m_steps << '\n';
    a_out << "\nInitial population\n" << std::setprecision(a_genetics.m_precision) << std::fixed;
    a_genetics.print_generation(a_out);

    return a_out;
}

Genetics& Genetics::get_instance()
{
    if (m_instance == nullptr)
    {
        m_instance = std::unique_ptr<Genetics>(new Genetics());
    }
    return *m_instance;
}

void Genetics::run()
{
    std::cin >> *this;
    std::cout << *this;

    next_generation();
    m_print_info = false;
    std::cout << "\nMax evolution (" + std::to_string(m_steps) + " steps)\n";
    for (int i = 1; i < m_steps; ++i)
    {
        std::cout << std::setprecision(m_length);
        align_index(std::cout, i - 1);
        align_sign(std::cout, "X = ", get_elite().decode());
        align_sign(std::cout, " | MAX = ", elite_fitness());
        align_sign(std::cout, " | AVG = ", average_fitness());
        std::cout << '\n' << std::setprecision(m_precision);

        next_generation();
    }

    std::cout << std::setprecision(m_length);
    align_index(std::cout, m_steps - 1);
    align_sign(std::cout, "X = ", get_elite().decode());
    align_sign(std::cout, " | MAX = ", elite_fitness());
    align_sign(std::cout, " | AVG = ", average_fitness());
}