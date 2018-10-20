#include <vector>
#include <deque>
#include <functional>
#include <string>
#include <iostream>

struct CountingSortResult {

  std::deque<std::size_t> p;

  std::vector<std::size_t> c;

};

template<typename T, typename V = typename T::value_type>
void countingSort(const T& input, const std::size_t K, CountingSortResult& result, std::function<std::size_t(const V&)> converter = [](const V& v) {
  return (std::size_t) v;
}, bool isClass = true) {
  std::vector<std::size_t> count(K, 0);
  for(const V& v : input) {
    count[converter(v)] += 1;
  }
  std::size_t tmp, csum = 0;
  for(std::size_t i = 0;i < K;i++) {
    tmp = count[i];
    count[i] = csum;
    csum += tmp;
  }
  result.p.resize(input.size(), 0);
  result.c.resize(input.size(), 0);
  for(std::size_t i = 0;i < input.size();i++) {
    std::size_t j = converter(input[i]);
    result.p[count[j]] = i;
    count[j] += 1;
  }
  if (!isClass) return;
  result.c.resize(input.size(), 0);
  result.c[result.p[0]] = 0;
  for(std::size_t i = 1;i < input.size();i++) {
    std::size_t j1 = result.p[i-1], j2 = result.p[i];
    std::size_t v1 = converter(input[j1]), v2 = converter(input[j2]);
    if (v1 < v2) {
      result.c[j2] = result.c[j1] + 1;
    } 
    else if (v1 == v2) {
      result.c[j2] = result.c[j1];
    }
    else {
      throw std::logic_error("v1 > v2");
    }
  }
  return;
}

std::deque<std::size_t> cyclicSort(const std::string& S) {
  std::size_t n = S.size();
  CountingSortResult pc, pcTmp;
  std::deque<std::size_t> buf(n, 0);
  countingSort<std::string, char>(S, 27, pc, [](const char c) {
    return (std::size_t) (c - 'a' + 1);
  });
  std::size_t cyclicSize = 1;
  bool shiftForward = false;
  auto cyclicShift = [&n, &cyclicSize, &shiftForward](const std::size_t i) {
    if (shiftForward) {
      return (i + cyclicSize < n ? i + cyclicSize : i + cyclicSize - n);
    } else {
      return (i < cyclicSize ? i + n - cyclicSize : i - cyclicSize);
    }
  };
  std::vector<std::size_t> index(S.size(), 0);
  for(std::size_t i = 0;i < index.size();i++) {
    index[i] = i;
  }
  while(cyclicSize < S.size()) {
    countingSort< std::vector<std::size_t> , std::size_t >(index, pc.c[*pc.p.rbegin()] + 1, pcTmp, [&cyclicShift, &pc](const std::size_t i) {
      std::size_t j = pc.p[i];
      return pc.c[cyclicShift(j)];
    }, false);
    for(std::size_t i = 0;i < n;i++) {
      buf[i] = cyclicShift(pc.p[pcTmp.p[i]]);
    }
    pcTmp.c[buf[0]] = 0;
    shiftForward = true;
    for(std::size_t i = 1;i < n;i++) {
      std::size_t 
        v1_1 = pc.c[buf[i-1]], 
        v1_2 = pc.c[cyclicShift(buf[i-1])],
        v2_1 = pc.c[buf[i]],
        v2_2 = pc.c[cyclicShift(buf[i])];
      if (v1_1 < v2_1) {
        pcTmp.c[buf[i]] = pcTmp.c[buf[i-1]] + 1;
      } else if (v1_1 == v2_1) {
        if (v1_2 < v2_2) {
          pcTmp.c[buf[i]] = pcTmp.c[buf[i-1]] + 1;
        } else if (v1_2 == v2_2) {
          pcTmp.c[buf[i]] = pcTmp.c[buf[i-1]];
        } else {
          throw std::logic_error("v1_2 > v2_2");
        }
      } else {
        throw std::logic_error("v1_1 > v2_1");
      }
    }
    cyclicSize *= 2;
    shiftForward = false;
  }
  return pc.p;
}

std::deque<std::size_t> getSuffixArray(std::string& S) {
  S.push_back('a' - 1);
  std::deque<std::size_t> result = cyclicSort(S);
  result.pop_front();
  return result;
}

std::vector<std::size_t> getLCP(const std::string& S, const std::deque<std::size_t>& sa) {
  std::size_t n = S.size();
  if (n == 0) throw std::invalid_argument("empty string");
  std::vector<std::size_t> result(n - 1, 0);
  std::size_t current_value = 0;
  std::vector<std::size_t> rank(n, 0);
  for(std::size_t i = 0;i < n;i++) {
    rank[sa[i]] = i;
  }
  for(std::size_t i = 0;i < n;i++) {
    std::size_t j = rank[i];
    if (j + 1 == n) {
      current_value = 0;
    } else {
      std::size_t i_next = sa[j + 1];
      while(i + current_value + 1 < n && i_next + current_value + 1 < n && S[i + current_value + 1] == S[i_next + current_value + 1]) {
        current_value += 1;
      }
      result[j] = current_value;
      current_value -= 1;
    }
  }
  return result;
}

int main() {
  std::string S;
  if (!(std::cin >> S)) return 1;
  const auto sa(getSuffixArray(S));
  const auto lcp(getLCP(S, sa));
  return 0;
}