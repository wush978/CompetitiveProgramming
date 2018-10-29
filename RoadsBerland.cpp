/**
 * http://codeforces.com/contest/25/problem/D
 */

#include <vector>
#include <iostream>
#include <algorithm>
#include <climits>

int main() {
  std::size_t N;
  if (!(std::cin >> N)) return 1;
  std::vector<std::size_t> edges(2 * (N - 1), 0);
  for(std::size_t& edge : edges) {
    if (!(std::cin >> edge)) return 1;
  }
  std::vector<std::size_t> edge_order(2 * N, 0);
  for(std::size_t i = 0;i < 2 * N;i++) {
    edge_order[i] = i;
  }
  std::sort(edge_order.begin(), edge_order.end(), [&edges](const std::size_t i, const std::size_t j) {
    return edges[i] < edges[j];
  });

  std::vector<std::size_t> parent(N, 0);
  std::vector<std::size_t> size(N, 1);
  std::vector<std::size_t> redundant_edges;
  //'@param i index of node in [1,N]
  auto find_parent = [&edges, &edge_order, &parent, &redundant_edges](const std::size_t node) {
    if (parent[node - 1] != 0) return parent[node - 1];
    auto comp = [&edges](const std::size_t i, const std::size_t node) {
      return edges[i] < node;
    };
    auto start = std::lower_bound(edge_order.begin(), edge_order.end(), node, comp);
    auto end = std::upper_bound(edge_order.begin(), edge_order.end(), node, comp);
    auto it = start;
    std::vector<std::size_t> neighbors({node});
    while(it != end) {
      #ifdef DEBUG
      if (edges[*it] != node) throw std::logic_error("edges[*it] != node");
      #endif
      if (*it % 2) {
        neighbors.push_back(edges[*it + 1]);
      } else {
        neighbors.push_back(edges[*it - 1]);
      }
      it++;
    }
    if (neighbors.size() > 1) {
      std::sort(neighbors.begin(), neighbors.end());
      for(auto it = neighbors.begin() + 1;it != neighbors.end();it++) {
        if (*it != node) {
          redundant_edges.push_back(node);
          redundant_edges.push_back(*it);
        }
      }
      parent[node - 1] = neighbors[0];
    } else {
      parent[node - 1] = node;
    }
    return parent[node - 1];
  };
  auto find_root = [&find_parent, &parent](std::size_t node) {
    std::vector<std::size_t> path({node});
    std::size_t parent_node = find_parent(node);
    while(parent_node != node) {
      node = parent_node;
      path.push_back(node);
      parent_node = find_parent(node);
    }
    for(std::size_t path_node : path) {
      parent[path_node - 1] = parent_node;
    }
    return parent_node;
  };
  std::size_t count = 0;
  std::vector<std::size_t> added_edges;
  for(std::size_t i = 1;i <= N;i++) {
    std::size_t parent_node = find_root(i);
    if (parent_node != 1) {
      added_edges.push_back(i);
      added_edges.push_back(1);
      count += 1;
    }
  }
  std::cout << count << std::endl;
  for(std::size_t i = 0;i < count;i++) {
    std::cout << redundant_edges[2 * i] << " " << redundant_edges[2 * i + 1] << " " <<
      added_edges[2 * i] << " " << added_edges[2 * i + 1] << std::endl;;
  }
  return 0;
}
