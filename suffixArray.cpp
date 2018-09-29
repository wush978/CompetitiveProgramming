#include "suffixArray.hpp"
#include <string>
#include <iostream>

int main() {
  std::string str;
  if (!std::getline(std::cin, str)) return 1;
  std::vector<char> input(str.begin(), str.end());
  input.push_back('$');
  auto result = sortCyclicShifts<char, 256>(input);
  result.erase(result.begin());
  for(std::size_t i = 0;i < result.size();i++) {
    std::cout << (result[i]) << " ";
  }
  std::cout << std::endl;
  return 0;
}
