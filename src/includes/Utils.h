#pragma once

#include <iostream>
#include <random>

inline void IO(const std::string& a_file_name = "")
{
    if (a_file_name != "")
    {
        FILE* stream{};
        if (freopen_s(&stream, (a_file_name + ".in").c_str(), "r", stdin))
        {
            return;
        }
        if (freopen_s(&stream, (a_file_name + ".out").c_str(), "w", stdout))
        {
            return;
        }
    }
    else
    {
        std::cin.tie(0)->sync_with_stdio(0);
    }
}

template <typename T>
T read(std::istream& a_in = std::cin)
{
    T temp{};
    a_in >> temp;
    return temp;
}

inline long double get_random(int a_left = 0, int a_right = 1)
{
    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_real_distribution<long double> distr(a_left, a_right);
    return distr(eng);
}

inline void align_sign(std::ostream& a_out, const std::string& a_label, long double a_value)
{
    a_out << a_label << (a_value < 0 ? "" : " ") << a_value;
}

inline void align_index(std::ostream& a_out, int a_idx)
{
    if (a_idx < 9)
    {
        a_out << ' ';
    }
    a_out << a_idx + 1 << ": ";
}

struct Interval
{
    long double m_left{}, m_right{};

    friend std::istream& operator>>(std::istream& a_in, Interval& a_interval)
    {
        a_interval.m_left = read<long double>(a_in);
        a_interval.m_right = read<long double>(a_in);
        return a_in;
    }

    friend std::ostream& operator<<(std::ostream& a_out, const Interval& a_interval)
    {
        a_out << a_interval.m_left << ' ' << a_interval.m_right;
        return a_out;
    }
};
