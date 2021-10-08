// non compila perché nella nuova libreria intel ha campiato tbb mettendolo sotto oneapi

#include <iostream>
#include <random>
#include <utility>
#include <chrono>
#include <vector>
#include <algorithm>
#include <execution>

int main() {
  
  constexpr int N = 10000;
  std::vector<int> input;
  std::vector<int> output;
  input.resize(N);
  output.resize(N);
  
  std::mt19937 engine;
  std::uniform_int_distribution<> uniformDist(-5,5);  // default è int

  // fill the vector
  for (int i = 0; i < N; ++i) {
    input.emplace_back(uniformDist(engine));
  }

  // sort it in parallel
  std::sort(std::execution::par, input.begin(), input.end());

  auto foo = [](int i) -> int { return i * -2; };

  // apply a function foo to each element of input 
  std::transform(std::execution::par_unseq, input.begin(), input.end(), output.begin(), foo);

  return 0;
}
