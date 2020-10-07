#include <cuda.h>
#include <cuda_runtime.h>

#define BLOCK_SIZE 16

typedef struct{
    int width;
    int height;
    int stride;

    float *elements;
} Matrix;

__global__
void matMulKernel(Matrix C, const Matrix A, const Matrix B){
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int idx = row*C.width + col;

    float out = 0;
    if(idx < C.width * C.height){
        for(int j=0; j < A.width; ++j)
            out += A.elements[row*A.width + j] * B.elements[j*B.width + col];
    }

    C.elements[idx] = out;

}


void matMul_wrapper(Matrix &C, const Matrix &A, const Matrix &B, cudaDeviceProp devProp){
    dim3 block(BLOCK_SIZE, BLOCK_SIZE, 1);
    dim3 grid(  (C.width + block.x - 1) / block.x,
                (C.height + block.y - 1) / block.y,
                1);

    Matrix d_A {A.width, A.height, A.stride};
    size_t size = A.height * A.width * sizeof(float);
    cudaMallocManaged(&d_A.elements, size);
    cudaMemcpy(d_A.elements, A.elements, size, cudaMemcpyHostToDevice);

    Matrix d_B {B.width, B.height, B.stride};
    size = B.height * B.width * sizeof(float);
    cudaMallocManaged(&d_B.elements, size);
    cudaMemcpy(d_B.elements, B.elements, size, cudaMemcpyHostToDevice);

    Matrix d_C {C.width, C.height, C.stride};
    size = C.height * C.width * sizeof(float);
    cudaMallocManaged(&d_C.elements, size);
    cudaMemcpy(d_C.elements, C.elements, size, cudaMemcpyHostToDevice);

    matMulKernel<<<grid, block>>>(d_C, d_A, d_B);

    cudaDeviceSynchronize();

    cudaMemcpy(C.elements, d_C.elements, size, cudaMemcpyDeviceToHost);

    cudaFree(d_A.elements);
    cudaFree(d_B.elements);
    cudaFree(d_C.elements);
}