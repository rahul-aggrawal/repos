//for this program number of processors is equal to N+1
#include <stdio.h>
#include "mpi.h"
#define N 4
/* number of rows and columns in matrix */
MPI_Status status;
double a[N][N],b[N][1],c[N][1];
int main(int argc, char **argv)
{
	int numtasks,taskid,numworkers,source,dest,rows,offset,i,j,k;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	numworkers = numtasks-1;
	/*---------------------------- master ----------------------------*/
	if (taskid == 0) 
	{
		for (i=0; i<N; i++) 
		{
			b[i][0]= 2.0;
			for (j=0; j<N; j++) 
			{
				a[i][j]= 1.0;
			}	
		}
		rows = N/numworkers;
		offset = 0;
		for (dest=1; dest<=numworkers; dest++)
		{
			MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
			MPI_Send(&b, N*1, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
			offset = offset + rows;
		}
		/* wait for results from all worker tasks */
		for (i=1; i<=numworkers; i++)
		{
			source = i;
			MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[offset][0], rows*1, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
		}
		printf("Here is the result matrix:\n");
		for (i=0; i<N; i++) 
		{
			//for (j=0; j<N; j++)
				printf("%6.2f ", c[i][0]);
			printf ("\n");
		}
	}
	/*---------------------------- worker----------------------------*/
	if (taskid > 0) 
	{
		source = 0;
		MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&a, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&b, N*1, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
		/* Matrix multiplication */
		//for (k=0; k<N; k++)
			for (i=0; i<rows; i++) 
			{
				c[i][0] = 0.0;
				for (j=0; j<N; j++)
					c[i][0] = c[i][0] + a[i][j] * b[j][0];
			}
			MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
			MPI_Send(&c, rows*1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}
