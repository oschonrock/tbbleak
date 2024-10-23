# tbbleak
Demonstration of Intel TBB leak during std::sort(std::execution::par_unseq

from clean install of ubuntu 24.04 do the following:

```bash

sudo apt install build-essential
sudo apt install libtbb-dev  # see below for building with latest libtbb

cd
git clone https://github.com/oschonrock/tbbleak.git
cd tbbleak
./build.sh
./build/gcc/relwithdebinfo/sort_leak
```

# Code

all code in is `app/sort_leak.cpp` 

- `process_mem_usage()` is for reporting only
- all relevant code is in `main()`
- run-time lenth and size of leak can be easily adjusted with `size` and `iterations` constant at top of `main()`
- the reported mem usage has been cross checked with `top` (for a longer running process)
- kernel will kill process due to out of memory, if `size` and/or `iterations` are turned up too high

# Output:
```
Single threaded sort:
VM:    14544   RSS:    11648 (kB)
VM:    14616   RSS:    11588 (kB)
VM:    14616   RSS:    11588 (kB)
VM:    14616   RSS:    11588 (kB)
VM:    14616   RSS:    11588 (kB)

par_unseq sort:
VM:    29372   RSS:    17860 (kB)
VM:    33468   RSS:    19652 (kB)
VM:    33468   RSS:    21316 (kB)
VM:    37564   RSS:    23108 (kB)
VM:    37564   RSS:    24772 (kB)
```
It can be clearly seen that single threadeed `std::sort` is not affected, but resident and virtual size is clearly growing with every call to `std::sort(par_unseq,...)` 



