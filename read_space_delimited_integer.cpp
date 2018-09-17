#include<iostream>
#include<cstdlib>

int main() {
  std::string input;
  if (!(std::getline(std::cin, input))) throw std::invalid_argument("");
  const char* start = input.c_str();
  char* end;
  while(*start != 0) {
    std::cout << strtol(start, &end, 10) << std::endl;
    start = end + 1;
  }
  return 0;
}
