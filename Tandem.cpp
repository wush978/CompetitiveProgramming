#include <vector>
#include <deque>
#include <functional>
#include <string>
#include <iostream>
#include <stack>
#include <algorithm>
#include <cmath>
#include <limits>

// #define TANDEM_DEBUG

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
    pc.p.swap(buf);
    pc.c.swap(pcTmp.c);
    cyclicSize *= 2;
    shiftForward = false;
  }
  return pc.p;
}

std::deque<std::size_t> getSuffixArray(std::string& S) {
  S.push_back('a' - 1);
  std::deque<std::size_t> result = cyclicSort(S);
  result.pop_front();
  S.pop_back();
  return result;
}

std::vector<std::size_t> getRank(const std::deque<std::size_t>& sa) {
  std::size_t n = sa.size();
  std::vector<std::size_t> rank(n, 0);
  for(std::size_t i = 0;i < n;i++) {
    rank[sa[i]] = i;
  }
  return rank;
}

std::vector<std::size_t> getLCP(const std::string& S, const std::deque<std::size_t>& sa, const std::vector<std::size_t>& rank) {
  std::size_t n = S.size();
  if (n == 0) throw std::invalid_argument("empty string");
  std::vector<std::size_t> result(n, 0);
  std::size_t current_value = 0;
  for(std::size_t i = 0;i < n;i++) {
    std::size_t j = rank[i];
    if (j + 1 == n) {
      current_value = 0;
    } else {
      std::size_t i_next = sa[j + 1];
      while(i + current_value < n && i_next + current_value < n && S[i + current_value] == S[i_next + current_value]) {
        current_value += 1;
      }
      result[j] = current_value;
      if (current_value > 0) current_value -= 1;
    }
  }
  return result;
}

void getLR(const std::vector<std::size_t>& lcp, std::vector<std::size_t>& L, std::vector<std::size_t>& R) {
  const std::size_t n = lcp.size();
  L.resize(n, 0);
  R.resize(n, 0);
  std::stack<std::size_t> s;
  std::size_t i = 0;
  for(;i < n;i++) {
    while(!s.empty()) {
      std::size_t j = s.top();
      if (lcp[j] <= lcp[i]) break;
      // lcp[j] > lcp[i]
      s.pop();
      R[j] = i;
      if (s.empty()) L[j] = 0;
      else L[j] = s.top() + 1;
    }
    s.push(i);
  }
  i = n;
  while(!s.empty()) {
    std::size_t j = s.top();
    s.pop();
    R[j] = i;
    if (s.empty()) L[j] = 0;
    else L[j] = s.top() + 1;
  }
}

struct SegmentTree {

  std::vector<std::size_t> left, right, value;

  const std::vector<std::size_t>& input;

  const std::size_t default_value;

  SegmentTree(std::vector<std::size_t> _input) :
  input(_input), default_value(std::numeric_limits<std::size_t>::max()) {
    const auto n = input.size();
    const auto depth = std::ceil(std::log2(input.size())) + 1;
    const auto size = std::pow(2, depth);
    left.resize(size, 0);
    right.resize(size, 0);
    value.resize(size, 0);

    auto getCut = [](std::size_t l, std::size_t r) {
      std::size_t len = r - l;
      len = len / 2;
      return l + len;
    };

    std::stack<std::size_t>  left_stack, right_stack, index_stack;
    left_stack.push(0);
    right_stack.push(n);
    index_stack.push(0);
    while(!left_stack.empty()) {
      
      auto l = left_stack.top();
      left_stack.pop();
      auto r = right_stack.top();
      right_stack.pop();
      auto i = index_stack.top();
      index_stack.pop();

      left[i] = l;
      right[i] = r;

      if (r - l == 1) continue;
      auto cut = getCut(l, r);
      left_stack.push(l);
      right_stack.push(cut);
      index_stack.push(2 * i + 1);
      left_stack.push(cut);
      right_stack.push(r);
      index_stack.push(2 * i + 2);
      #ifdef TANDEM_DEBUG
      if (2 * i + 2 >= size) throw std::logic_error("2 * i + 2 >= size (1)");
      #endif // TANDEM_DEBUG
    }

    std::size_t i = size;
    while(i > 0) {
      i -= 1;
      auto l = left[i];
      auto r = right[i];
      if (l == 0 && r == 0) continue;
      if (r - l == 1) value[i] = input[l];
      else {
        #ifdef TANDEM_DEBUG
        if (2 * i + 2 >= size) throw std::logic_error("2 * i + 2 >= size (2)");
        #endif // TANDEM_DEBUG
        value[i] = std::min(value[2 * i + 1], value[2 * i + 2]);
      }
    }
  }

  const std::size_t find(std::size_t l, std::size_t r, std::size_t i, const std::size_t L) const {
    if (l >= right[i]) return L;
    if (left[i] >= r) return L;
    if (l <= left[i] && r >= right[i]) return value[i];
    if (left[i] <= l && right[i] >= r && value[i] >= L) return L;
    return std::min(find(l, r, 2 * i + 1, L), find(l, r, 2 * i + 2, L)); 
  }
};

int main() {
  std::string S;
  if (!(std::cin >> S)) return 1;
  const std::size_t N = S.size();
  const auto sa(getSuffixArray(S));
  const auto s_rank(getRank(sa));
  const auto s_lcp(getLCP(S, sa, s_rank));
  const auto s_lcp_tree = SegmentTree(s_lcp);
  #ifdef TANDEM_DEBUG
  for(std::size_t i = 0;i < s_lcp.size();i++) {
    for(std::size_t j = i + 1;j < s_lcp.size();j++) {
      std::size_t result = s_lcp[i];
      for(std::size_t k = i + 1;k < j;k++) {
        result = std::min(result, s_lcp[k]);
      }
      if (result != s_lcp_tree.find(i,j,0)) throw std::runtime_error("Incorrect segment tree find");
    }
  }
  #endif // TANDEM_DEBUG
  std::reverse(S.begin(), S.end());
  const auto pa(getSuffixArray(S));
  const auto p_rank(getRank(pa));
  const auto p_lcp(getLCP(S, pa, p_rank));
  const auto p_lcp_tree = SegmentTree(p_lcp);
  auto getLCP123 = [](const std::size_t x1, const std::size_t x2, const std::size_t x3, const SegmentTree& tree, const std::size_t L) {
    std::size_t l, r;
    if (x1 < x2) {
      l = x1;
      r = x2;
    } else {
      l = x2;
      r = x1;
    }
    if (x3 < l) l = x3;
    else if (x3 > r) r = x3;
    return tree.find(l, r, 0, L);
  };
  std::size_t boring_count = 0, interesting_count = 0;
  for(std::size_t L = 1;3 * L <= N;L++) {
    std::size_t start = 0;
    std::size_t s1 = s_rank[start], s2 = s_rank[start + L], s3 = s_rank[start + 2 * L];
    std::size_t p1 = p_rank[N - 1 - start], p2 = p_rank[N - 1 - start - L], p3 = p_rank[N - 1 - start - 2 * L];
    while(start + 2 * L < N) {
      if (start > 0) {
        s1 = s2;
        s2 = s3;
        s3 = s_rank[start + 2 * L];
        p1 = p2;
        p2 = p3;
        p3 = p_rank[N - 1 - start - 2 * L];
      }
      std::size_t s_lcp123 = getLCP123(s1, s2, s3, s_lcp_tree, L + 1);
      std::size_t p_lcp123 = getLCP123(p1, p2, p3, p_lcp_tree, L);
      std::size_t lower_bound = std::max(L, p_lcp123) + start + 2;
      std::size_t upper_bound_all = std::min(s_lcp123 + 1, L + 1) + start + p_lcp123;
      std::size_t upper_bound_boring = std::min(s_lcp123, L + 1) + start + p_lcp123;
      if (upper_bound_all >= lower_bound) {
        if (upper_bound_boring >= lower_bound) {
          boring_count += upper_bound_boring - lower_bound + 1;
          interesting_count += upper_bound_all - upper_bound_boring;
        } else {
          interesting_count += upper_bound_all - lower_bound + 1;
        }
      }
      start += L;
    }
  }
  std::cout << interesting_count << " " << boring_count << std::endl;
  return 0;
}