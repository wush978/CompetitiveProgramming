#include <iostream>

bool next(std::string& S, std::size_t i = 0) {
  if (S[i] != 'd') {
    S[i] += 1;
    return true;
  }
  else {
    S[i] = 'a';
    if (i + 1 < S.size()) return next(S, i + 1);
    else return false;
  }
}

std::size_t count(const std::string& S) {
  std::size_t result = 0;
  if (S[0] != S[1]) result += 1;
  for(std::size_t i = 1;i + 1 < S.size();i++) {
    if (S[i] != S[i-1] && S[i] != S[i+1]) result+=1;
  }
  if (S[S.size() - 2] != S[S.size() - 1]) result += 1;
  return result;
}

int main() {
  std::size_t n;
  if (!(std::cin >> n)) return 1;
  std::string S(n - 1, 'a');
  std::size_t result = 0;
  do {
    result += count(S);
  } while(next(S, 0));
  std::cout << result << std::endl;
  return 0;
}