#include <chrono>
#include <ctime>  // std::mktime
#include <fstream>
#include <iomanip>  // std::get_time
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using namespace std::literals;

#define _GLIBCXX_DEBUG 1  // включить режим отладки

#ifdef _GLIBCXX_DEBUG
class RedirectStandardInput {
   public:
    RedirectStandardInput(std::ifstream& input) {
        // сохраняем указатель на "streambuf"
        _cinbuf_bkp = std::cin.rdbuf();

        // перенаправляем поток ввода std::cin на файл
        std::cin.rdbuf(input.rdbuf());
    }

    ~RedirectStandardInput() {
        std::cin.rdbuf(_cinbuf_bkp);  // восстанавливаем standard input
    }

   private:
    std::streambuf* _cinbuf_bkp{nullptr};
};
#endif  //_GLIBCXX_DEBUG

void ProcessQuery(const std::string& line) {
    static double earnings[55000];
    std::string query_type;
    std::stringstream ss(line);
    std::tm date_from_tm{}, date_to_tm{};

    ss >> query_type >> std::get_time(&date_from_tm, "%Y-%m-%d") >> std::get_time(&date_to_tm, "%Y-%m-%d");
    std::time_t date_from_time_t = std::mktime(&date_from_tm);
    std::time_t date_to_time_t = std::mktime(&date_to_tm);
    long int from = date_from_time_t / (60 * 60 * 24);
    long int to = date_to_time_t / (60 * 60 * 24);

    if (query_type == "Earn"s) {
        double income;
        ss >> income;
        int delta_in_days = static_cast<int>(std::difftime(to, from)) + 1;
        double income_per_day = income / delta_in_days;

        for (auto i = from; i != to+1; ++i) {
            earnings[i] += income_per_day;
        }
    } else if (query_type == "ComputeIncome"s) {
        std::cout << std::setprecision(25) << std::accumulate(std::begin(earnings) + from, std::begin(earnings) + to + 1, 0.0) << std::endl;
    }
}

int main() {
#ifdef _GLIBCXX_DEBUG
    // =========== Standard input redirection, debug mode ===========
    std::string path = "24_input.txt"s;
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("File \""s + path + "\" is not opened"s);
    }
    RedirectStandardInput redirection(input);
    // =================== End of input redirection ==================
#endif  //_GLIBCXX_DEBUG

    int n{};
    std::cin >> n >> std::ws;

    for (int i = 0; i < n; ++i) {
        std::string line;
        std::getline(std::cin, line);
        ProcessQuery(line);
    }

    return 0;
}
