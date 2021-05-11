//
// Created by kamiyoru on 2021/5/11.
//

#include <simple/Format.h>

int main() {
  sim::format("{} {}\n", -1LL, sim::align(2, 1ULL));
  sim::format("{1} {0}\n", -1LL, sim::align(2, 1ULL));
  sim::format("{1} {0}\n", sim::red(-1LL), sim::yellow(sim::align(2, 1ULL)));
  sim::format("{1} {0}\n", -1LL, sim::align(2, 1ULL));
}
