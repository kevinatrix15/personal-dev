#include <algorithm>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <numeric>
#include <vector>

using namespace std::chrono;

struct gen_rand {
  double range;

public:
  gen_rand(float r = 1.0) : range(r) {}
  float operator()() { return (rand() / (float)RAND_MAX) * range; }
};

std::vector<float> add(const std::vector<float>& a, const std::vector<float>& b)
{
  std::vector<float> c(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] + b[i];
  }
  return c;
}

int main(int argc, char** argv)
{
  // initialize a to natural numbers
  std::vector<float> a(102400 * 8);
  std::iota(a.begin(), a.end(), 1);

  // initialize b to random numbers
  std::vector<float> b(a.size());
  std::generate_n(b.begin(), b.size(), gen_rand());

  /*
  std::vector<int> c(a.size());

  for (size_t i = 0; i < a.size(); ++i) {
    c[i] = a[i] + b[i];
  }
  */

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  const std::vector<float> c = add(a, b);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();

  duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
  std::cout << "It took me " << time_span.count() << " seconds.";
  std::cout << std::endl;

  return 0;
}
