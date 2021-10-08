#include <iostream>
#include <iomanip>
#include <chrono>

#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>

int main()
{
  constexpr unsigned int num_steps = 1 << 22;

  std::vector<double> step_sum(num_steps);
  
  double pi = 0.0;

  constexpr double step = 1.0/(double) num_steps;

  // compute the sum and measure the time
  auto start = std::chrono::system_clock::now();

  // compute the terms of the sum
  tbb::parallel_for( tbb::blocked_range<int>(0,step_sum.size()),
                       [&](tbb::blocked_range<int> r)
    {
        for (int i=r.begin(); i<r.end(); ++i)
        {
            step_sum[i] = (i+0.5)/num_steps;
        }
    });

  // compute the sum
  auto sum = tbb::parallel_reduce(
                tbb::blocked_range<int>(0,step_sum.size()),
                0.0,
                [&](tbb::blocked_range<int> r, double running_sum)
                {
                    for (int i=r.begin(); i<r.end(); ++i)
                    {
                        running_sum += 4.0/(1.0+step_sum[i]*step_sum[i]);
                    }

                    return running_sum;
                },
                std::plus<double>() );

  auto stop = std::chrono::system_clock::now();

  std::chrono::duration<double> dur= stop - start;

  std::cout << dur.count() << " seconds" << std::endl;

  // finalize the computation
  pi = step * sum;

  std::cout << "result: " <<  std::setprecision (15) << pi << std::endl;

  return 0;
}
