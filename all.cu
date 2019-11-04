//HELLO.CU
#include "stdio.h"
int main()
{
 printf("Hello, world\n");
 return 0;
}



//ADD.CU



#include <stdio.h>
#include <stdlib.h>
__global__ 
void add(int *a, int *b, int *c) 
{
*c = *a + *b;
}
int main() 
{
  int a, b, c;
  // host copies of variables a, b & c
  int *d_a, *d_b, *d_c;
  // device copies of variables a, b & c
  int size = sizeof(int);
  // Allocate space for device copies of a, b, c
  cudaMalloc((void **)&d_a, size);
  cudaMalloc((void **)&d_b, size);
  cudaMalloc((void **)&d_c, size);
  // Setup input values  
  c = 0;
  a = 3;
  b = 6;
  // Copy inputs to device
  cudaMemcpy(d_a, &a, size, cudaMemcpyHostToDevice);
  cudaMemcpy(d_b, &b, size, cudaMemcpyHostToDevice);
  // Launch add() kernel on GPU
  add<<<1,1>>>(d_a, d_b, d_c);
  // Copy result back to host
  cudaError err = cudaMemcpy(&c, d_c, size, cudaMemcpyDeviceToHost);
  if(err!=cudaSuccess) 
  {
      printf("CUDA error copying to Host: %s\n", cudaGetErrorString(err));
  }
  printf("result is %d\n",c);
  // Cleanup
  cudaFree(d_a);
  cudaFree(d_b);
  cudaFree(d_c);
  return 0;
}



//VECADD.CU



#include "stdio.h"
#define N 10
__global__ void add(int *a, int *b, int *c)
{
 int tID = blockIdx.x;
 if (tID < N)
 {
 c[tID] = a[tID] + b[tID];
 }
}
int main()
{
 int a[N], b[N], c[N];
 int *dev_a, *dev_b, *dev_c;
 cudaMalloc((void **) &dev_a, N*sizeof(int));
 cudaMalloc((void **) &dev_b, N*sizeof(int));
 cudaMalloc((void **) &dev_c, N*sizeof(int));
 // Fill Arrays
 for (int i = 0; i < N; i++)
 {
 a[i] = i,
 b[i] = 1;
 }
 cudaMemcpy(dev_a, a, N*sizeof(int), cudaMemcpyHostToDevice);
 cudaMemcpy(dev_b, b, N*sizeof(int), cudaMemcpyHostToDevice);
 add<<<N,1>>>(dev_a, dev_b, dev_c);
 cudaMemcpy(c, dev_c, N*sizeof(int), cudaMemcpyDeviceToHost);
 for (int i = 0; i < N; i++)
 {
 printf("%d + %d = %d\n", a[i], b[i], c[i]);
 }
 return 0;
}



//MATADD.CU



#include "stdio.h"
#define COLUMNS 3
#define ROWS 2
__global__ void matadd(int *a, int *b, int *c)
{
 int x = blockIdx.x;
 int y = blockIdx.y;
 int i = (COLUMNS*y) + x;
 c[i] = a[i] + b[i];
}

/* ------------- COMPUTATION DONE ON GPU ----------------------------*/

int main()
{
 int a[ROWS][COLUMNS], b[ROWS][COLUMNS], c[ROWS][COLUMNS];
 int *dev_a, *dev_b, *dev_c;
 cudaMalloc((void **) &dev_a, ROWS*COLUMNS*sizeof(int));
 cudaMalloc((void **) &dev_b, ROWS*COLUMNS*sizeof(int));
 cudaMalloc((void **) &dev_c, ROWS*COLUMNS*sizeof(int));
 for (int i = 0; i < ROWS; i++) // Fill Arrays
 for (int j = 0; j < COLUMNS; j++)
 {
 a[i][j] = 1;
 b[i][j] = 3;
 }
 cudaMemcpy(dev_a, a, ROWS*COLUMNS*sizeof(int),cudaMemcpyHostToDevice);
 cudaMemcpy(dev_b, b, ROWS*COLUMNS*sizeof(int),cudaMemcpyHostToDevice);
 dim3 grid(COLUMNS,ROWS);
 matadd<<<grid,1>>>(dev_a, dev_b, dev_c);
 cudaMemcpy(c, dev_c, ROWS*COLUMNS*sizeof(int),cudaMemcpyDeviceToHost);

/* ------------- COMPUTATION DONE ON HOST CPU ---------------------------*/

 for (int i = 0; i < ROWS; i++) // Output Arrays
 {
 for (int j = 0; j < COLUMNS; j++)
 {
 printf("[%d][%d]=%d ",i,j,c[i][j]);
 }
 printf("\n");
 }
 return 0;
}




//MATMUL.CU



#include "stdio.h"
#define COLUMNS 3
#define ROWS 3
__global__ void matmul(int *a, int *b, int *c, int width)
{
  int row = blockIdx.y*width+threadIdx.y;
  int col = blockIdx.x*width+threadIdx.x;
  if(row<width && col <width) 
  {
    int product_val = 0;
    for(int k=0;k<width;k++) 
    {
      product_val += a[row*width+k]*b[k*width+col];
    }
    c[row*width+col] = product_val;
  }
}

/* ------------- COMPUTATION DONE ON GPU ----------------------------*/

int main()
{
 int a[ROWS][COLUMNS], b[ROWS][COLUMNS], c[ROWS][COLUMNS];
 int *dev_a, *dev_b, *dev_c;
 int N = 3;
 cudaMalloc((void **) &dev_a, ROWS*COLUMNS*sizeof(int));
 cudaMalloc((void **) &dev_b, ROWS*COLUMNS*sizeof(int));
 cudaMalloc((void **) &dev_c, ROWS*COLUMNS*sizeof(int));
 for (int i = 0; i < ROWS; i++) // Fill Arrays
 for (int j = 0; j < COLUMNS; j++)
 {
 a[i][j] = 1;
 b[i][j] = 3;
 }
 cudaMemcpy(dev_a, a, ROWS*COLUMNS*sizeof(int),cudaMemcpyHostToDevice);
 cudaMemcpy(dev_b, b, ROWS*COLUMNS*sizeof(int),cudaMemcpyHostToDevice);
 dim3 grid(COLUMNS,ROWS);
 matmul<<<grid,1>>>(dev_a, dev_b, dev_c,N);
 cudaMemcpy(c, dev_c, ROWS*COLUMNS*sizeof(int),cudaMemcpyDeviceToHost);

/* ------------- COMPUTATION DONE ON HOST CPU ---------------------------*/

 for (int i = 0; i < ROWS; i++) // Output Arrays
 {
 for (int j = 0; j < COLUMNS; j++)
 {
 printf("[%d][%d]=%d ",i,j,c[i][j]);
 }
 printf("\n");
 }
 return 0;
}
