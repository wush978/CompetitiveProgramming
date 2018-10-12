#include <deque>
#include <vector>
#include <iostream>
#include <functional>

#define ANUSAR_DEBUG

typedef std::pair< std::vector<std::size_t> , std::vector<std::size_t> > CountingSultResult;

template<typename Vector>
void countingSort(const Vector& input, std::size_t K, CountingSultResult& result, std::function<std::size_t(const typename Vector::value_type)> converter = [](const typename Vector::value_type v) {
  return (std::size_t) v;
}) {
  std::size_t n = input.size();
  if (n == 0) return;
  typedef typename Vector::value_type Value;
  result.first.resize(n);
  result.second.resize(n);
  std::vector<std::size_t> count(K, 0);
  for(std::size_t i = 0;i < n;i++) {
    count[converter(input[i])] += 1;
  }
  for(std::size_t i = 1;i < K;i++) {
    count[i] += count[i-1];
  }
  for(std::size_t i = 0;i < n;i++) {
    std::size_t j = converter(input[i]);
    result.first[count[j]++] = i;
  }
  result.second[0] = 0;
  for(std::size_t i = 1;i < n;i++) {
    std::size_t vNow = converter(input[result.first[i]]), vLast = converter(input[result.first[i-1]]);
    if (vNow > vLast) {
      result.second[i] = result.second[i-1] + 1;
    } else {
      #ifdef ANUSAR_DEBUG
      if (vNow < vLast) throw std::logic_error("vNow < vLast");
      #endif
      result.second[i] = result.second[i-1];
    }
  }
  return;
}

const std::deque<std::size_t> getCyclicSort(const std::string& S) {
  CountingSultResult pc, pcTmp;
  std::size_t cyclicSize = 1;
  countingSort(S, 26, pc, [](const char v) {
    return v - 'a';
  });
  cyclicSize *= 2;
  std::vector<std::size_t> index(n, 0);
  for(std::size_t i = 0;i < n;i++) {
    index[i] = i;
  }
  while(cyclicSize < S.size()) {
    cyclicSize *= 2;
  }

  std::deque<std::size_t> result(pc.first.begin(), pc.first.end());
  return result;
}

const std::deque<std::size_t> getSuffixArray(std::string& S) {
  S.push_back(0x0);
  auto result(getCyclicSort(S));
  S.pop_back();
  result.pop_front();
  return result;
}

int main() {
  std::size_t T;
  if (!(std::cin >> T)) return 1;
  // problems
  std::string S;
  std::size_t Q;
  std::vector<std::size_t> F;
  while(T > 0) {
    if (!(std::cin >> S)) return 1;
    if (!(std::cin >> Q)) return 1;
    F.resize(Q);
    for(auto& f : F) {
      if (!(std::cin >> f)) return 1;
    }

    const auto suffixArray(getSuffixArray(S));
    T--;
  }
}