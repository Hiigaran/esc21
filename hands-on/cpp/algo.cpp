#include <random>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <numeric>

std::ostream& operator<<(std::ostream& os, std::vector<int> const& c);
std::vector<int> make_vector(int N);

int main()
{
  // create a vector of N elements, generated randomly
  int const N = 10;
  std::vector<int> v = make_vector(N);
  std::cout << v << '\n';

  // create the iterators
  auto first = v.begin();
  auto last = v.end();
  auto middle = first + 0.5*N;

  // sum all the elements of the vector
  // use std::accumulate
  auto s = std::accumulate(first, last, 0);
  std::cout << s << std::endl; 

  // compute the average of the first half and of the second half of the vector
  auto mean1 = accumulate(first, middle, 0)/(0.5*N);
  auto mean2 = accumulate(middle, last, 0)/(0.5*N);
  std::cout << mean1 << std::endl;
  std ::cout << mean2 << std::endl;

  // move the three central elements to the beginning of the vector
  // use std::rotate
  std::rotate(first, first+3, last);
  std::cout << v << std::endl;

  // remove duplicate elements
  // use std::sort followed by std::unique/unique_copy
  std::sort(first, last);
  std::unique(first, last);
  std::cout << v << std::endl;
};

std::ostream& operator<<(std::ostream& os, std::vector<int> const& c)
{
  os << "{ ";
  std::copy(
            std::begin(c),
            std::end(c),
            std::ostream_iterator<int>{os, " "}
            );
  os << '}';

  return os;
}

std::vector<int> make_vector(int N)
{
  std::random_device rd;
  std::mt19937 eng{rd()};

  int const MAX_N = 100;
  std::uniform_int_distribution<int> dist{1, MAX_N};

  std::vector<int> result;
  result.reserve(N);
  std::generate_n(std::back_inserter(result), N, [&]() { return dist(eng); });

  return result;
}
