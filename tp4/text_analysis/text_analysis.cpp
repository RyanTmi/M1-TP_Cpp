#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <numeric>
#include <string>

void write_stats(const std::map<std::string, std::uint32_t>& frequencies);
void numerics(const std::map<std::string, std::uint32_t>& frequencies);

int main()
{
    std::ifstream                        declaration_file("declaration.txt");
    std::map<std::string, std::uint32_t> frequencies;

    std::string word;
    while (declaration_file >> word)
    {
        ++frequencies[word];
    }
    declaration_file.close();

    write_stats(frequencies);
    numerics(frequencies);
}

void write_stats(const std::map<std::string, std::uint32_t>& frequencies)
{
    std::ofstream stats_file("stats.dat");
    for (const auto& p : frequencies)
    {
        stats_file << p.first << " : " << p.second << '\n';
    }
    stats_file.close();

    std::ofstream stats_rev_file("rev_stats.dat");
    for (auto it = frequencies.rbegin(); it != frequencies.rend(); ++it)
    {
        stats_rev_file << it->first << " : " << it->second << '\n';
    }
    stats_rev_file.close();
}

void numerics(const std::map<std::string, std::uint32_t>& f)
{
    using pair = std::pair<std::string, std::uint32_t>;

    std::cout << "Nombre de mots différents :     " << f.size() << '\n';
    std::cout << "Nombre de mots différents > 7 : "
              << std::count_if(f.begin(), f.end(), [](const pair& p) { return p.first.size() >= 7; }) << '\n';

    auto max_it =
        std::max_element(f.begin(), f.end(), [](const pair& p1, const pair& p2) { return p1.second < p2.second; });
    std::cout << "Le plus frequent :              \"" << max_it->first << "\" : " << max_it->second << '\n';

    std::uint64_t word_count = std::accumulate(f.begin(), f.end(), 0UL, [](std::uint64_t i, const pair& p) {
        return i + p.first.size() * p.second;
    });
    std::cout << "Nombre de lettres :             " << word_count << '\n';


    auto is_vowel = [](char c) {
        c = std::tolower(c);
        return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y';
    };
    auto vowel_count = [&is_vowel](const std::string& s) { return std::count_if(s.begin(), s.end(), is_vowel); };
    std::cout << "Mots avec 2 voyelles :          " << std::count_if(f.begin(), f.end(), [&vowel_count](const pair& p) {
        return vowel_count(p.first) == 2;
    }) << '\n';

    auto e_count = [](const std::string& s) {
        return std::count_if(s.begin(), s.end(), [](char c) { return c == 'e'; });
    };
    max_it = std::max_element(f.begin(), f.end(), [&e_count](const pair& p1, const pair& p2) {
        return e_count(p1.first) < e_count(p2.first);
    });
    std::cout << "Contient le plus de 'e' :       \"" << max_it->first << "\" : " << max_it->second << '\n';
}
