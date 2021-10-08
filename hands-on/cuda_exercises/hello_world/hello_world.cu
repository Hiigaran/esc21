#include <iostream>

__global__
void mykernel(){}

int main()
{

  mykernel<<<1,1>>>();
  std::cout << "Hello World!" << std::endl;

  return 0;
}
