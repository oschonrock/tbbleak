# tbbleak
Demonstration of Intel TBB leak during std::sort(std::execution::par_unseq

The bug report is here: https://github.com/oneapi-src/oneTBB/issues/1533

To reproduce, from clean install of ubuntu 24.04, do the following:

```bash

sudo apt install build-essential git cmake
sudo apt install libtbb-dev  # see below for building with latest libtbb

cd
git clone https://github.com/oschonrock/tbbleak.git
cd tbbleak
./build.sh
./build/gcc/relwithdebinfo/sort_leak
```

to see the exact compiler command run `./build.sh --verbose clean` which gives:

```
...
/usr/bin/g++   -O2 -g -DNDEBUG -Wall -Wextra -Wconversion -Wsign-conversion -MD -MT CMakeFiles/sort_leak.dir/app/sort_leak.cpp.o -MF CMakeFiles/sort_leak.dir/app/sort_leak.cpp.o.d -o CMakeFiles/sort_leak.dir/app/sort_leak.cpp.o -c /home/oliver/tbbleak/app/sort_leak.cpp
...
/usr/bin/g++ -O2 -g -DNDEBUG CMakeFiles/sort_leak.dir/app/sort_leak.cpp.o -o sort_leak  -ltbb 
...
```

# Compiling with oneTBB master/HEAD

```
sudo apt remove libtbb-dev   # remove the tbb from ubuntu packages to avoid confusion
sudo apt autoremove          # cleanup related packages

cd
git clone https://github.com/oneapi-src/oneTBB.git
cd oneTBB/
cmake -DCMAKE_BUILD_TYPE=Release -DTBB_TEST=OFF -B build -S .
cmake --build build -- -j4
sudo cmake --install build
sudo ldconfig               # ensure the linker is aware of the newly installed lib

# and now back to building and running normally as above

cd ../tbbleak
./build.sh reset   # reset will remove the build dir and start again, so there is no doubt we have a clean compile
./build/gcc/relwithdebinfo/sort_leak

# output is the same as below

# to uninstall the manually compiled tbblib do the following
sudo xargs rm < ~/oneTBB/build/install_manifest.txt

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



