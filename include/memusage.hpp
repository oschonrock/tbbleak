#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>

// returns virtual and resident memory of current process in kB
inline std::pair<std::size_t, std::size_t> process_mem_usage() {
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

  return {vsize / 1024, rss * page_size_kb};
}

inline void report_memory_usage() {
  auto [vm, rss] = process_mem_usage();
  std::cout << "VM: " << std::setw(8) << vm << "   RSS: " << std::setw(8) << rss << " (kB)\n";
}
