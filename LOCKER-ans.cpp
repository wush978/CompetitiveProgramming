/**
 * https://www.spoj.com/problems/LOCKER/
 */

#include <iostream>
#include <unordered_map>

const std::size_t mod = 1000000000 + 7;

std::unordered_map<uint64_t, uint64_t> cache = {
  {1, 1},
  {2, 2},
  {3, 3},
  {4, 4}
};

const uint64_t get_answer(uint64_t n) {
  auto i = cache.find(n);
  if (i != cache.end()) return i->second;
  std::uint64_t y, r = 0;
  for(std::uint64_t x = 1; x <= n / 2;x++) {
    y = n - x;
    std::cout << n << " = " << x << " + " << y << " ==> " << get_answer(x) << " * " << get_answer(y) << " = " << get_answer(x) * get_answer(y) << std::endl;
    r = std::max(r, get_answer(x) * get_answer(y));
  }
  cache[n] = r;
  return r;
}

int main() {
  std::size_t t;
  std::uint64_t n;
  if (!(std::cin >> t)) return 1;
  while(t > 0) {
    t--;
    if (!(std::cin >> n)) return 1;
    std::cout << get_answer(n) << std::endl;
  }
  return 0;
}