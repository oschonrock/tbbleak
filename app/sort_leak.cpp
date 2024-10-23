#include <algorithm>
#include <cstddef>
#include <execution>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <unistd.h>

std::pair<std::size_t, std::size_t> process_mem_usage() {
  std::ifstream stat_stream("/proc/self/stat", std::ios_base::in);

  // dummy vars for leading entries in stat that we don't care about
  // NOLINTNEXTLINE multiple decl
  std::string pid, comm, state, ppid, pgrp, session, tty_nr, tpgid, flags, minflt, cminflt, majflt,
      cmajflt, utime, stime, cutime, cstime, priority, nice, O, itrealvalue, starttime;

  // the two fields we want
  std::size_t vsize{};
  std::size_t rss{};

  stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >>
      minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >>
      nice >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

  stat_stream.close();

  auto page_size_kb = static_cast<std::size_t>(sysconf(_SC_PAGE_SIZE)) / 1024;

  std::size_t vm_usage_kB     = vsize / 1024;
  std::size_t resident_set_kB = rss * page_size_kb;
  return {vm_usage_kB, resident_set_kB};
}

int main() {
  constexpr std::size_t size       = 1'000'000;
  constexpr int         maxval     = 1'000'000;
  constexpr std::size_t iterations = 10;

  std::mt19937                       generator{std::random_device{}()};
  std::uniform_int_distribution<int> distribution{0, maxval};

  std::vector<int> random_numbers;
  random_numbers.reserve(size);
  for (std::size_t i = 0; i != size; i++) {
    random_numbers.push_back(distribution(generator));
  }

  for (std::size_t i = 0; i != iterations; i++) {
    auto sorted_numbers = random_numbers; // make a copy
    std::sort(std::execution::par_unseq, sorted_numbers.begin(), sorted_numbers.end());
    // report memory usage
    auto [vm, rss] = process_mem_usage();
    std::cout << "VM: " << std::setw(8) << vm << "   RSS: " << std::setw(8) << rss << " (kB)\n";
    // allow the sorted copy to be destroyed
  }
}