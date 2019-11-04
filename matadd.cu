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
