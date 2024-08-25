#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

struct card
{
    std::string name;
    std::string city;
    int         age;
    double      time;
};

struct analysis_result
{
    std::size_t           lyonnais_count;           // a)
    double                lyonnais_percentage;
    std::size_t           lyonnais_count_under_30;  // b)
    bool                  toulousan_begin_with_a;   // c)
    int                   min_age;                  // d)
    int                   max_age;
    std::string           min_age_name;
    std::string           max_age_name;
    double                age_mean;                                      // e)
    double                age_sd;
    bool                  p_better_than_m;                               // c)
    const char*           toulousans_data_file_name = "toulousans.txt";  // g)
    double                empirical_cov;                                 // h)
    std::set<std::string> cities;                                        // i)
};

bool load_data_from_file(std::vector<card>& datas, const char* file_name);
void print_analysis(const analysis_result& analisys_result);

void analysis(const std::vector<card>& datas, analysis_result& analisys_result);
void algorithm_analysis(const std::vector<card>& datas, analysis_result& analisys_result);

void time_sort_data(std::vector<card>& datas);
void smallest_step(const std::vector<card>& datas);
void age_filter(const std::vector<card>& datas);

void sort_data(std::vector<card>& datas);
void age_histogram(const std::vector<card>& datas);

int main()
{
    std::vector<card> datas;

    // ---------- 1.3.1 ----------
    if (!load_data_from_file(datas, "smalldata.txt"))
    {
        return 1;
    }

    {
        analysis_result analysis_result;
        analysis(datas, analysis_result);
        print_analysis(analysis_result);
    }

    // ---------- 1.3.2 ----------
    {
        analysis_result analysis_result;
        algorithm_analysis(datas, analysis_result);
        print_analysis(analysis_result);
    }
    time_sort_data(datas);

    // ---------- 1.3.3 ----------
    smallest_step(datas);
    age_filter(datas);
    sort_data(datas);
    age_histogram(datas);
}

std::ostream& operator<<(std::ostream& os, const card& c)
{
    return os << c.name << " " << c.city << " " << c.age << " " << c.time;
}

bool load_data_from_file(std::vector<card>& datas, const char* file_name)
{
    std::ifstream data_file(file_name);
    if (!data_file.is_open())
    {
        std::cerr << "Can not open " << file_name << " !\n";
        return false;
    }

    card c;
    while (data_file >> c.name >> c.city >> c.age >> c.time)
    {
        datas.push_back(c);
    }

    if (!data_file.eof())
    {
        std::cerr << "Can not extract data, wrong format !\n";
        return false;
    }
    return true;
}

void print_analysis(const analysis_result& ar)
{
    std::cout << "=========== Analisys results ===========\n\n"
              << "a)  Personnes habitant à Lyon : " << ar.lyonnais_count << "\n"
              << "    Pourcentage de lyonnais   : " << ar.lyonnais_percentage << "\n"
              << "b)  Personnes habitant à Lyon <30: " << ar.lyonnais_count_under_30 << "\n"
              << "c)  Toulousain dont le prénom commence par 'A' ? : " << std::boolalpha << ar.toulousan_begin_with_a
              << "\n"
              << "d)  Le plus agé : " << ar.max_age << " " << ar.max_age_name << "\n"
              << "    Le plus jeune : " << ar.min_age << " " << ar.min_age_name << "\n"
              << "e)  age moyen : " << ar.age_mean << "\n"
              << "    Écart-type : " << ar.age_sd << "\n"
              << "f)  Parisiens plus rapides en moyenne que les Marseillais ? : " << std::boolalpha
              << ar.p_better_than_m << "\n"
              << "g)  Le fichier " << ar.toulousans_data_file_name << " a été crée\n"
              << "h)  Covariance empirique entre âge et temps : " << ar.empirical_cov << "\n"
              << "i)  Liste des villes représentées :\n";
    for (auto& city : ar.cities)
    {
        std::cout << "      " << city << "\n";
    }
    std::cout << "\n======================================\n\n";
}

void analysis(const std::vector<card>& datas, analysis_result& ar)
{
    const double card_count = datas.size();

    ar.min_age = datas.front().age;
    ar.max_age = datas.front().age;

    double variance = 0.0f;

    int    parisians_count = 0;
    double parisians_time_mean = 0.0f;
    int    marseillais_count = 0;
    double marseillais_time_mean = 0.0f;

    double covariance = 0.0f;
    int    toulousains_count = 0;
    double toulousains_time_mean = 0.0f;
    double toulousains_age_mean = 0.0f;

    std::ofstream toulousans_data(ar.toulousans_data_file_name);

    for (const card& c : datas)
    {
        if (c.city == "Lyon")
        {
            // Question a)
            ar.lyonnais_count++;
            // Question b)
            ar.lyonnais_count_under_30 += (c.age < 30);
        }

        if (c.city == "Toulouse")
        {
            // Question c)
            if (c.name[0] == 'A')
            {
                ar.toulousan_begin_with_a = true;
            }
            // Question g)
            toulousans_data << c.name << "\t" << 2018 - c.age << "\t" << c.time << "\n";

            // Question h)
            toulousains_time_mean += c.time;
            toulousains_age_mean += c.age;
            toulousains_count++;
            covariance += c.age * c.time;
        }

        // Question d)
        if (ar.min_age > c.age)
        {
            ar.min_age = c.age;
            ar.min_age_name = c.name;
        }
        if (ar.max_age < c.age)
        {
            ar.max_age = c.age;
            ar.max_age_name = c.name;
        }

        // Question e)
        ar.age_mean += c.age;
        variance += c.age * c.age;

        // Question f)
        if (c.city == "Paris")
        {
            parisians_time_mean += c.time;
            parisians_count++;
        }
        if (c.city == "Marseille")
        {
            marseillais_time_mean += c.time;
            marseillais_count++;
        }

        // Question i)
        ar.cities.insert(c.city);
    }
    toulousans_data.close();

    // Question a)
    ar.lyonnais_percentage = 100 * ar.lyonnais_count / card_count;

    // Question e)
    ar.age_mean /= card_count;
    variance /= card_count;
    variance -= ar.age_mean * ar.age_mean;
    ar.age_sd = std::sqrt(variance);

    // Question f)
    parisians_time_mean /= parisians_count;
    marseillais_time_mean /= marseillais_count;
    ar.p_better_than_m = parisians_time_mean < marseillais_time_mean;

    // Question h)
    toulousains_time_mean /= toulousains_count;
    toulousains_age_mean /= toulousains_count;
    covariance /= toulousains_count;
    covariance -= toulousains_age_mean * toulousains_time_mean;
    ar.empirical_cov = covariance;
}

void algorithm_analysis(const std::vector<card>& datas, analysis_result& ar)
{
    const double card_count = datas.size();

    // Question a)
    ar.lyonnais_count = std::count_if(datas.begin(), datas.end(), [](const card& c) { return c.city == "Lyon"; });
    ar.lyonnais_percentage = 100 * ar.lyonnais_count / card_count;

    // Question b)
    ar.lyonnais_count_under_30 =
        std::count_if(datas.begin(), datas.end(), [](const card& c) { return c.city == "Lyon" && c.age < 30; });

    // Question c)
    ar.toulousan_begin_with_a =
        std::any_of(datas.begin(), datas.end(), [](const card& c) { return c.city == "Toulouse" && c.name[0] == 'A'; });

    // Question d)
    auto minMax =
        std::minmax_element(datas.begin(), datas.end(), [](const card& c1, const card& c2) { return c1.age < c2.age; });
    ar.min_age = minMax.first->age;
    ar.min_age_name = minMax.first->name;
    ar.max_age = minMax.second->age;
    ar.max_age_name = minMax.second->name;

    // Question e)
    ar.age_mean =
        std::accumulate(datas.begin(), datas.end(), 0, [](int x, const card& c) { return x + c.age; }) / card_count;

    double variance =
        std::accumulate(datas.begin(), datas.end(), 0, [](int x, const card& c) { return x + c.age * c.age; }) /
        card_count;
    variance -= ar.age_mean * ar.age_mean;
    ar.age_sd = std::sqrt(variance);

    // Question f)
    double parisians_time_mean =
        std::accumulate(
            datas.begin(),
            datas.end(),
            0,
            [](int x, const card& c) { return x + (c.city == "Paris" ? c.time : 0); }) /
        static_cast<double>(std::count_if(datas.begin(), datas.end(), [](card c) { return c.city == "Paris"; }));
    double marseillais_time_mean =
        std::accumulate(
            datas.begin(),
            datas.end(),
            0,
            [](int x, const card& c) { return x + (c.city == "Marseille" ? c.time : 0); }) /
        static_cast<double>(std::count_if(datas.begin(), datas.end(), [](card c) { return c.city == "Marseille"; }));
    ar.p_better_than_m = parisians_time_mean < marseillais_time_mean;

    // Question g)
    std::ofstream toulousans_data(ar.toulousans_data_file_name);
    std::for_each(datas.begin(), datas.end(), [&toulousans_data](const card& c) {
        if (c.city == "Toulouse")
        {
            toulousans_data << c.name << "\t" << 2018 - c.age << "\t" << c.time << std::endl;
        }
    });
    toulousans_data.close();

    // Question h)
    double toulousains_count =
        std::count_if(datas.begin(), datas.end(), [](const card& c) { return c.city == "Toulouse"; });
    double age_mean = std::accumulate(
                          datas.begin(),
                          datas.end(),
                          0.0,
                          [](double x, const card& c) { return x + (c.city == "Toulouse" ? c.age : 0); }) /
                      toulousains_count;
    double time_mean = std::accumulate(
                           datas.begin(),
                           datas.end(),
                           0.0,
                           [](double x, const card& c) { return x + (c.city == "Toulouse" ? c.time : 0); }) /
                       toulousains_count;

    ar.empirical_cov = std::accumulate(
                           datas.begin(),
                           datas.end(),
                           0.0,
                           [](double x, card c) { return x + (c.city == "Toulouse" ? c.age * c.time : 0); }) /
                       toulousains_count;
    ar.empirical_cov -= age_mean * time_mean;

    // Question i)
    std::for_each(datas.begin(), datas.end(), [&ar](const card& c) { ar.cities.insert(c.city); });
}

void time_sort_data(std::vector<card>& datas)
{
    std::sort(datas.begin(), datas.end(), [](const card& c1, const card& c2) { return c1.time < c2.time; });

    constexpr int data_limit = 100;
    std::ofstream sort_data_file("data_tri.txt");
    for (int i = data_limit - 1; i >= 0; --i)
    {
        sort_data_file << datas[i] << "\n";
    }
    std::cout << "Le fichier data_tri.txt a été crée\n";
}

void smallest_step(const std::vector<card>& datas)
{
    std::vector<double> times(datas.size());
    std::transform(datas.begin(), datas.end(), std::back_inserter(times), [](const card& c) { return c.time; });

    std::sort(times.begin(), times.end());
    std::adjacent_difference(times.begin(), times.end(), times.begin());

    double minStep = *std::min_element(times.begin() + 1, times.end());
    std::cout << "Le plus petit écart entre les temps de courses est : " << minStep << " secondes\n";
}

void age_filter(const std::vector<card>& datas)
{
    std::vector<card> youngest;
    std::vector<card> oldest;

    std::partition_copy(
        datas.begin(),
        datas.end(),
        std::back_inserter(youngest),
        std::back_inserter(oldest),
        [](const card& c) { return c.age < 40; });

    // for (const card& c : youngest)
    // {
    //     std::cout << c << std::endl;
    // }
    // for (const card& c : oldest)
    // {
    //     std::cout << c << std::endl;
    // }
}

void sort_data(std::vector<card>& datas)
{
    std::sort(datas.begin(), datas.end(), [](const card& c1, const card& c2) {
        if (c1.name != c2.name)
        {
            return c1.name < c2.name;
        }
        if (c1.city != c2.city)
        {
            return c1.city < c2.city;
        }
        if (c1.age != c2.age)
        {
            return c1.age > c2.age;
        }
        return c1.time < c2.time;
    });

    std::ofstream sort_data_file("ordre.dat");
    for (const auto& c : datas)
    {
        sort_data_file << c << "\n";
    }
    std::cout << "Le fichier ordre.dat a été crée\n";
}

void age_histogram(const std::vector<card>& datas)
{
    std::map<int, int> age_histogram;
    for (const card& c : datas)
    {
        age_histogram[c.age]++;
    }

    std::cout << "Histogramme des âges:\n\n";
    std::for_each(age_histogram.begin(), age_histogram.end(), [](std::pair<int, int> x) {
        std::cout << x.first << " : " << x.second << "\n";
    });
    auto max = std::max_element(
                   age_histogram.begin(),
                   age_histogram.end(),
                   [](std::pair<int, int> x1, std::pair<int, int> x2) { return x1.second < x2.second; })
                   ->first;
    std::cout << "Classe d'âge la plus nombreuse : " << max << "\n";
}
