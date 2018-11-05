/**
 * https://www.spoj.com/problems/CHAIN/
 */

#include <iostream>
#include <vector>
#include <array>
#include <utility>

class AnimalSpecies {

  typedef std::size_t Species;

  typedef std::array<Species, 3> Chain;

  typedef std::pair<Chain, std::size_t> SpeciesChain;

  std::vector<Species> species;

  std::vector<std::size_t> size;

  std::vector<Species> eats;

  std::vector<Species> eat_chain;

public:

  AnimalSpecies(const std::size_t N) :
  species(N, 0), size(N, 1), eats(N, 0), eat_chain(N, 0) {
    for(std::size_t i = 0;i < N;i++) {
      species[i] = i + 1;
      eat_chain[i] = i + 1;
    }
  }

  Species find_species(Species v) {
    if (v == 0) return 0;
    if (v == species[v-1]) return v;
    species[v-1] = find_species(species[v-1]);
    return species[v-1];
  }

  Species find_eat_chain(Species v, std::size_t depth = 0) {
    if (depth > 3) throw std::logic_error("eat chain cycle");
    v = find_species(v);
    if (eat_chain[v-1] == v) return v;
    if (eat_chain[v-1] == 0) return 0;
    eat_chain[v-1] = find_eat_chain(eat_chain[v-1], depth + 1);
    return eat_chain[v-1];
  }

  const SpeciesChain get_chain(Species v) {
    SpeciesChain result = {{0, 0, 0}, 3};
    v = find_species(v);
    Species r = find_eat_chain(v);
    for(std::size_t i = 0;i < 3;i++) {
      result.first[i] = r;
      if (v == r) result.second = i;
      if (r != 0) r = find_species(eats[r-1]);
    }
    #ifdef DEBUG
    if (result.second == 3) {
      throw std::logic_error("chain not matched!");
    }
    #endif
    return result;
  }

  void merge_species_internal(Species x, Species y) {
    if (x == 0 || y == 0) return;
    x = find_species(x);
    y = find_species(y);
    if (size[x-1] < size[y-1]) std::swap(x, y);
    species[y-1] = x;
    size[x-1] += size[y-1];
  }

  bool check_chain(const SpeciesChain& chain_x, const SpeciesChain& chain_y, const std::size_t shift) {
    for(std::size_t i = 0;i < 3;i++) {
      Species u = find_species(chain_x.first[i]);
      for(std::size_t j = 1;j < 3;j++) {
        Species v = find_species(chain_y.first[(i + shift + j) % 3]);
        if (u != 0 && u == v) return false;
      }
    }
    return true;
  }

  void merge_chain(const SpeciesChain& chain_x, const SpeciesChain& chain_y, const std::size_t shift) {
    for(std::size_t i = 0;i < 3;i++) {
      merge_species_internal(chain_x.first[i], chain_y.first[(i + shift) % 3]);
    }
    Species r;
    if (shift == 1) r = find_species(chain_y.first[0]);
    else r = find_species(chain_x.first[0]);
    for(std::size_t i = 0;i < 3;i++) {
      Species v = find_species(std::max(chain_x.first[i], chain_y.first[(i + shift) % 3]));
      if (v == 0) continue;
      eats[v-1] = find_species(std::max(chain_x.first[(i + 1) % 3], chain_y.first[(i + shift + 1) % 3]));
      eat_chain[v-1] = r;
    }
    #ifdef DEBUG
    for(Species i = 1;i <= species.size();i++) {
      get_chain(i);
    }
    #endif
  }

};

int main() {
  std::size_t T;
  if (!(std::cin >> T)) return 1;
  while(T > 0) {
    std::size_t N, K, count = 0;
    if (!(std::cin >> N >> K)) return 1;
    AnimalSpecies A(N);
    for(std::size_t k = 0;k < K;k++) {
      std::size_t D, X, Y;
      if (!(std::cin >> D >> X >> Y)) return 1;
      if (X > N || Y > N || X == 0 || Y == 0) {
        #ifdef DEBUG
        printf("(%zu) error\n", k);
        #endif
        count += 1;
        continue;
      }
      const auto chain_x(A.get_chain(X));
      const auto chain_y(A.get_chain(Y));
      std::size_t shift;
      if (D == 1) {
        shift = (chain_x.second <= chain_y.second ? chain_y.second - chain_x.second : chain_y.second + 3 - chain_x.second);
      } else if (D == 2) {
        shift = (chain_x.second + 1<= chain_y.second ? chain_y.second - (chain_x.second + 1) : chain_y.second + 2 - chain_x.second);
      }
      if (A.check_chain(chain_x, chain_y, shift)) {
        A.merge_chain(chain_x, chain_y, shift);
      } else {
        #ifdef DEBUG
        printf("(%zu) error\n", k);
        #endif
        count += 1;
      }
    }
    std::cout << count << std::endl;
    T--;
  }
  return 0;
}