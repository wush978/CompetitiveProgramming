#include "suffixArray.hpp"
#include <string>
#include <iostream>

int main() {
  std::string str;
  if (!std::getline(std::cin, str)) return 1;
  auto result(suffixArray(str));
  for(std::size_t i = 0;i < result.size();i++) {
    std::cout << (result[i]) << " ";
  }
  std::cout << std::endl;
  auto lcp(getLCP(str, result));
  for(std::size_t i = 0;i < lcp.size();i++) {
    std::cout << (lcp[i]) << " ";
  }
  std::cout << std::endl;
  return 0;
}
