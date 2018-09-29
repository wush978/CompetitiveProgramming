#include <vector>
#include <cmath>
#include <climits>

/**
 * @param T The typename of the sorted objects
 * @param k The number of classes of the sorted objects
 * Reference: https://cp-algorithms.com/string/suffix-array.html
 */
template<typename T, std::size_t K>
std::vector<std::size_t> sortCyclicShifts(const std::vector<T>& input, std::size_t (*key)(const T&)) {
  if (input.size() == 0) return std::vector<std::size_t>();
  const std::size_t n = input.size();

  std::vector<std::size_t> p(n, 0);
  std::vector<std::size_t> c(n, 0);

  // counting sort
  std::vector<std::size_t> count(K, 0);
  for(auto i = input.begin();i != input.end();i++) {
    count[key(*i)] += 1;
  }
  std::size_t csum = 0, tmp;
  for(auto i = count.begin();i != count.end();i++) {
    tmp = *i;
    *i = csum;
    csum += tmp;
  }
  for(std::size_t i = 0;i < n;i++) {
    std::size_t j = key(input[i]);
    p[count[j]] = i;
    c[i] = j;
    count[j] += 1;
  }

  // recursion
  std::size_t step = 1, stepBound = std::ceil(std::log(n));
  std::size_t shift = 1;
  while(step <= stepBound) {
    // counting sort i - 2^k
    std::fill(count.begin(), count.end(), 0);
    for(std::size_t i = 0;i < shift;i++) {
      count[c[i + (n - shift)]] += 1;
    }
    for(std::size_t i = shift;i < n;i++) {
      count[c[i - shift]] += 1;
    }
    // reset count / c
    std::vector<std::size_t> cmap(count.size(), 0);
    std::size_t cmapSize = 0;
    std::vector<std::size_t> countTmp;
    for(std::size_t i = 0;i < count.size();i++) {
        if (count[i] > 0) {
            cmap[i] = cmapSize++;
            countTmp.push_back(count[i]);
        }
    }
    std::swap(count, countTmp);
    std::vector<std::size_t> ctmp(n);
    csum = 0;
    for(auto i = count.begin();i != count.end();i++) {
      tmp = *i;
      *i = csum;
      csum += tmp;
    }
    std::size_t countSize = count.size();
    std::vector<std::size_t> ptmp(n);
    for(auto i = p.begin();i != p.end();i++) {
      std::size_t shifted = (*i < shift ? *i + n - shift : *i - shift);
      std::size_t shiftedC = cmap[c[shifted]];
      ptmp[count[shiftedC]] = shifted;
      ctmp[shifted] = shiftedC * cmapSize + cmap[c[*i]];
      if (ctmp[shifted] + 1 > countSize) countSize = ctmp[shifted] + 1;
      count[shiftedC] += 1;
    }
    count.resize(countSize);
    shift = (shift * 2) % n;
    std::swap(c, ctmp);
    std::swap(p, ptmp);
    step++;
  }
  return p;
}

template<typename T, std::size_t K>
std::vector<std::size_t> sortCyclicShifts(const std::vector<T>& input) {
  typedef std::size_t (*KeyType)(const T&);
  KeyType key = [](const T& t) {
    return ((std::size_t) t);
  };
  return sortCyclicShifts<T, K>(input, key);
}

