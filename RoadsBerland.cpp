/**
 * http://codeforces.com/contest/25/problem/D
 */

#include <vector>
#include <iostream>

struct DisjointUnion {
  
  std::vector<std::size_t> parents;

  std::vector<std::size_t> size;

  DisjointUnion(const std::size_t N) :
  parents(N, 0), size(N, 1) 
  {
    for(std::size_t i = 0;i < N;i++) {
      parents[i] = i;
    }
  }

  std::size_t find_set(std::size_t v) {
    if (parents[v] == v) return v;
    parents[v] = find_set(parents[v]);
    return parents[v];
  }

  bool union_set(std::size_t a, std::size_t b) {
    a = find_set(a);
    b = find_set(b);
    if (a == b) return true;
    if (size[a] < size[b]) std::swap(a, b);
    parents[b] = a;
    size[a] += size[b];
    return false;
  }
};

int main() {
  std::size_t N;
  if (!(std::cin >> N)) return 1;

  DisjointUnion x(N);

  std::vector<std::size_t> redundant_edges;
  for(std::size_t i = 0;i + 1 < N;i++) {
    std::size_t a, b;
    if (!(std::cin >> a >> b)) return 1;
    if (x.union_set(a - 1, b - 1)) {
      redundant_edges.push_back(a);
      redundant_edges.push_back(b);
    }
  }

  std::cout << redundant_edges.size() / 2 << std::endl;

  auto it = redundant_edges.begin();
  std::size_t root = x.find_set(0);
  for(std::size_t i = 1;i < N;i++) {
    if (root != x.find_set(i)) {
      x.union_set(root, i);
      std::cout << *(it++) << " " << *(it++) << " " << root + 1 << " " << i + 1 << std::endl;
      root = x.find_set(i);
    }
  }

  return 0;
}