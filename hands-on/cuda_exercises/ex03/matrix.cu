#include <iostream>
#include <assert.h>
// Here you can set the device ID that was assigned to you
#define MYDEVICE 0


// Simple utility function to check for CUDA runtime errors
void checkCUDAError(const char *msg);
// Part 2 of 4: implement the kernel
__global__ void kernel( int *a, int dimx, int dimy ) {
    auto xindex = threadIdx.x + blockIdx.x * blockDim.x;
    auto yindex = threadIdx.y + blockIdx.y * blockDim.y;
    if(xindex < dimx && yindex < dimy){
       a[xindex * dimx + yindex] = xindex * dimx + yindex;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Program main
////////////////////////////////////////////////////////////////////////////////
int main() {
    cudaSetDevice(MYDEVICE);
// Part 1 and 4 of 4: set the dimensions of the matrix
    int dimx = 4;
    int dimy = 4;
    int num_bytes = dimx*dimy*sizeof(int);

    int *d_a=0, *h_a=0; // device and host pointers

    h_a = (int*)malloc(num_bytes);
    //allocate memory on the device
    cudaMalloc(&d_a, num_bytes);

    if( NULL==h_a || NULL==d_a ) {
        std::cerr << "couldn't allocate memory" << std::endl;
        return 1;
    }

    // Part 2 of 4: define grid and block size and launch the kernel
    // NOTA: se le risorse assegnate sono minori della matrice devo
    // definire un secondo kernel
    dim3 grid, block;
    block.x = 5;
    block.y = 5;
    grid.x  = 10;
    grid.y  = 10;

    kernel<<<grid, block>>>( d_a, dimx, dimy );
    // block until the device has completed
    cudaDeviceSynchronize();
    // check if kernel execution generated an error
    checkCUDAError("kernel execution");
    // device to host copy
    cudaMemcpy(h_a, d_a, num_bytes, cudaMemcpyDeviceToHost);

    // Check for any CUDA errors
    checkCUDAError("cudaMemcpy");
    // verify the data returned to the host is correct
    for(int row=0; row<dimx; row++)
    {
        for(int col=0; col<dimy; col++){
            std::cout << "index = " << row * dimx + col << std::endl;
            std::cout << "h_a = " << h_a[row * dimx + col] << std::endl;
            assert(h_a[row * dimx + col] == row * dimx + col);
        }
    }
    // free host memory
    free( h_a );
    // free device memory
    cudaFree( d_a );

    // If the program makes it this far, then the results are correct and
    // there are no run-time errors.  Good work!
    std::cout << "Correct!" << std::endl;

    return 0;
}


void checkCUDAError(const char *msg)
{
    cudaError_t err = cudaGetLastError();
    if( cudaSuccess != err) 
    {
        std::cerr << "Cuda error: " << msg << " " << cudaGetErrorString(err) << std::endl;
        exit(-1);
    }                         
}
