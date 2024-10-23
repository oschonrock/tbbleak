#include "memusage.hpp"
#include <algorithm>
#include <cstddef>
#include <execution>
#include <iostream>
#include <random>

std::vector<int> make_random_numbers(std::size_t size, int maxval) {
  std::mt19937                       generator{std::random_device{}()};
  std::uniform_int_distribution<int> distribution{0, maxval};

  std::vector<int> random_numbers;
  random_numbers.reserve(size);
  for (std::size_t i = 0; i != size; i++) {
    random_numbers.push_back(distribution(generator));
  }
  return random_numbers;
}

int main() {
  constexpr std::size_t size       = 1'000'000;
  constexpr int         maxval     = 1'000'000;
  constexpr std::size_t iterations = 5;

  auto random_numbers = make_random_numbers(size, maxval);

  std::cout << "Single threaded sort:\n";
  for (std::size_t i = 0; i != iterations; i++) {
    auto sorted_numbers = random_numbers;
    std::sort(sorted_numbers.begin(), sorted_numbers.end());
    report_memory_usage();
  }

  std::cout << "\npar_unseq sort:\n";
  for (std::size_t i = 0; i != iterations; i++) {
    auto sorted_numbers = random_numbers;
    std::sort(std::execution::par_unseq, sorted_numbers.begin(), sorted_numbers.end());
    report_memory_usage();
  }
}
