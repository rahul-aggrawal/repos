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
