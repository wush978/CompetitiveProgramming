#include <vector>
#include <cmath>
#include <climits>

/**
 * @param T The typename of the sorted objects
 * @param k The number of classes of the sorted objects
 * Reference: https://cp-algorithms.com/string/suffix-array.html
 */
template<typename T, int K>
std::vector<std::size_t> sortCyclicShifts(const std::vector<T>& input, int (*key)(const T&)) {
  if (input.size() == 0) return std::vector<std::size_t>();
  const std::size_t n = input.size();

  std::vector<std::size_t> p(n, 0);
  std::vector<int> c(n, 0);

  // counting sort
  std::vector<std::size_t> count(K, 0);
  for(auto i = input.begin();i != input.end();i++) {
    count[key(*i)] += 1;
  }
  std::size_t csum = 0, tmp;
  for(auto i = count.begin();i != count.end();i++) {
    tmp = *i;
    *i += csum;
    csum += tmp;
  }
  for(std::size_t i = 0;i < n;i++) {
    int j = key(input[i]);
    p[count[j]] = i;
    c[i] = j;
    count[j] += 1;
  }

  // recursion
  std::size_t step = 1, stepBound = std::ceil(std::log(n));
  std::size_t shift = 1;
  while(step <= stepBound) {
    // counting sort i - 2^k
    std::vector<int> ctmp(n, 0);
    std::fill(count.begin(), count.end(), 0);
    for(std::size_t i = 0;i < shift;i++) {
      count[c[i + (n - shift)]] += 1;
    }
    for(std::size_t i = shift;i < n;i++) {
      count[c[i - shift]] += 1;
    }
    csum = 0;
    for(auto i = count.begin();i != count.end();i++) {
      tmp = *i;
      *i += csum;
      csum += tmp;
    }
    for(std::size_t i = 0;i < shift;i++) {
      int j = c[i + (n - shift)];
      p[count[j]] = i;
      ctmp[i] = j;
      count[j] += 1;
    }
    for(std::size_t i = shift;i < n;i++) {
      int j = c[i - shift];
      p[count[j]] = i;
      ctmp[i] = j;
      count[j] += 1;
    }
    shift = (shift * 2) % n;
    step++;
    std::swap(c, ctmp);
  }
  return p;
}

template<typename T, int K>
std::vector<std::size_t> sortCyclicShifts(const std::vector<T>& input) {
  typedef int (*KeyType)(const T&);
  KeyType key = [](const T& t) {
    return ((int) t);
  };
  return sortCyclicShifts<T, K>(input, key);
}
