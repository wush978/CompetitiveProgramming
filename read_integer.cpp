#include <iostream>

int main() {
  int input;
  while(true) {
    if (std::cin >> input) {
      if (input != 42) std::cout << input << std::endl;
      else break;
    } else if (std::cin.bad()) {
      std::cerr << "bad" << std::endl;
    } else if (std::cin.eof()) {
      std::cerr << "eof" << std::endl;
    } else {
      std::cerr << "other error" << std::endl;
    }
    std::cin.clear();
    std::cin.ignore();
  }
  return 0;
}
