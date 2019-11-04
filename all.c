//HELLO.C


#include <mpi.h>
#include <stdio.h>
#define MASTER 0
int main(int argc,char *argv[])
{
	int numtasks,taskid,len;
	char hostname[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	MPI_Get_processor_name(hostname, &len);

	printf ("Hello from task %d on %s!\n", taskid, hostname);
	if(taskid == MASTER)
	{
		printf("number of MPI task is: %d\n",numtasks);
	}
	MPI_Finalize();
}




//SUM.C


#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

// size of array 
#define n 10 

int a[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; 

// Temporary array for slave process 
int a2[1000]; 

int main(int argc, char *argv[]) 
{ 

	int pid, np, elements_per_process, n_elements_recieved; 
	// np -> no. of processes 
	// pid -> process id 

	MPI_Status status; 

	// Creation of parallel processes 
	MPI_Init(&argc, &argv); 

	// find out process ID, 
	// and how many processes were started 
	MPI_Comm_rank(MPI_COMM_WORLD, &pid); 
	MPI_Comm_size(MPI_COMM_WORLD, &np); 

	// master process 
	if (pid == 0) 
	{ 
		int index, i; 
		elements_per_process = n / np; 

		// check if more than 1 processes are run 
		if (np > 1) 
		{ 
			// distributes the portion of array 
			// to child processes to calculate 
			// their partial sums 
			for (i = 1; i < np - 1; i++) 
			{ 
				index = i * elements_per_process; 

				MPI_Send(&elements_per_process, 1, MPI_INT, i, 0, MPI_COMM_WORLD); 
				MPI_Send(&a[index], elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD); 
			} 
			
			//because last process can get more than n/p elements
			// last process adds remaining elements 
			index = i * elements_per_process; 
			int elements_left = n - index; 

			MPI_Send(&elements_left, 1, MPI_INT, i, 0, MPI_COMM_WORLD); 
			MPI_Send(&a[index], elements_left, MPI_INT, i, 0, MPI_COMM_WORLD); 
		} 

		// master process add its own sub array 
		int sum = 0; 
		for (i = 0; i < elements_per_process; i++) 
			sum += a[i]; 
		//sum of values in the master process
		printf("%d\n",sum);
		// collects partial sums from other processes 
		int tmp; 
		for (i = 1; i < np; i++) 
		{ 
			MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status); 
			
			int sender = status.MPI_SOURCE; 
			sum += tmp; 
		} 

		// prints the final sum of array 
		printf("Sum of array is : %d\n", sum); 
	} 
	// slave processes 
	else 
	{ 
		MPI_Recv(&n_elements_recieved, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 

		// stores the received array segment in local array a2 
		MPI_Recv(&a2, n_elements_recieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 

		// calculates its partial sum 
		int partial_sum = 0; 
		for (int i = 0; i < n_elements_recieved; i++) 
			partial_sum += a2[i]; 

		printf("%d\n",partial_sum);
		// sends the partial sum to the root process 
		MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
	} 

	// cleans up all MPI state before exit of process 
	MPI_Finalize(); 

	return 0; 
}



//DOTPRODUCT.C


#include <mpi.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

// size of array 
#define n 10 

int a1[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int a2[] = {1,2,3,4,5,6,7,8,9,10}; 

// Temporary array for slave process 
int temp1[1000];
int temp2[1000]; 

int main(int argc, char* argv[]) 
{ 

	int pid, np, elements_per_process, n_elements_recieved; 
	// np -> no. of processes 
	// pid -> process id 

	MPI_Status status; 

	// Creation of parallel processes 
	MPI_Init(&argc, &argv); 

	// find out process ID, 
	// and how many processes were started 
	MPI_Comm_rank(MPI_COMM_WORLD, &pid); 
	MPI_Comm_size(MPI_COMM_WORLD, &np); 

	// master process 
	if (pid == 0) 
	{ 
		int index, i; 
		elements_per_process = n / np; 

		// check if more than 1 processes are run 
		if (np > 1) 
		{ 
			// distributes the portion of array 
			// to child processes to calculate 
			// their partial sums 
			for (i = 1; i < np - 1; i++) 
			{ 
				index = i * elements_per_process; 

				MPI_Send(&elements_per_process, 1, MPI_INT, i, 0, MPI_COMM_WORLD); 
				MPI_Send(&a1[index], elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&a2[index], elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD); 
			} 
			
			//because last process can get more than n/p elements
			// last process adds remaining elements 
			index = i * elements_per_process; 
			int elements_left = n - index; 

			MPI_Send(&elements_left, 1, MPI_INT, i, 0, MPI_COMM_WORLD); 
			MPI_Send(&a1[index], elements_left, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&a2[index], elements_left, MPI_INT, i, 0, MPI_COMM_WORLD); 
		} 

		// master process add its own sub array 
		int sum = 0; 
		for (i = 0; i < elements_per_process; i++) 
			sum += a1[i]*a2[i]; 
		//sum of values in the master process
		printf("%d\n",sum);
		// collects partial sums from other processes 
		int tmp; 
		for (i = 1; i < np; i++) 
		{ 
			MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status); 
			
			//int sender = status.MPI_SOURCE; 
			sum += tmp; 
		} 

		// prints the final sum of array 
		printf("Sum of array is : %d\n", sum); 
	} 
	// slave processes 
	else 
	{ 
		MPI_Recv(&n_elements_recieved, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status); 

		// stores the received array segment in local array temp1 and temp2 
		MPI_Recv(&temp1, n_elements_recieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv(&temp2, n_elements_recieved, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		// calculates its partial sum 
		int partial_sum = 0; 
		for (int i = 0; i < n_elements_recieved; i++) 
			partial_sum += temp1[i]*temp2[i]; 

		printf("%d\n",partial_sum);
		// sends the partial sum to the root process 
		MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); 
	} 

	// cleans up all MPI state before exit of process 
	MPI_Finalize(); 

	return 0; 
} 



//INTEGRAL.C


#include <mpi.h>
#include <math.h>
#include <stdio.h>
float fct(float x)
{
	return x*x*x;
}
/* Prototype */
float integral(float a, int n, float h);
float simpsons_(float a,int n,float h);
int main(int argc,char *argv[])
{
      int n, p, i, j, ierr,num;
      float h, result, a, b, pi;
      float my_a, my_range;

      int myid, source, dest, tag;
      MPI_Status status;
      float my_result;

      pi = acos(-1.0);  /* = 3.14159... */
      a = 0.;           /* lower limit of integration */
      b = 1.0;     /* upper limit of integration */
      n = 100000;          /* number of increment within each process */

      dest = 0;         /* define the process that computes the final result */
      tag = 123;        /* set the tag to identify this particular job */

/* Starts MPI processes ... */

      MPI_Init(&argc,&argv);              /* starts MPI */
      MPI_Comm_rank(MPI_COMM_WORLD, &myid);  /* get current process id */
      MPI_Comm_size(MPI_COMM_WORLD, &p);     /* get number of processes */

      h = (b-a)/n;    /* length of increment */
      num = n/p;        /* number of intervals calculated by each process*/
      my_range = (b-a)/p;
      my_a = a + myid*my_range;
     // my_result = integral(my_a,num,h);
        my_result=simpsons_(my_a,num,h);

      printf("Process %d has the partial result of %f\n", myid,my_result);

      if(myid == 0) 
			{
        result = my_result;
        for (i=1;i<p;i++)
        {
          source = i;           /* MPI process number range is [0,p-1] */
          MPI_Recv(&my_result, 1, MPI_REAL, source, tag,MPI_COMM_WORLD, &status);
          result += my_result;
        }
        printf("The result =%f\n",result);
      }
      else
			{
				MPI_Send(&my_result, 1, MPI_REAL, dest, tag,MPI_COMM_WORLD);      /* send my_result to intended dest.*/
			}
      MPI_Finalize();                       /* let MPI finish up ... */
}
float integral(float a, int n, float h)
{
      int j;
      float h2, aij, integ;

      integ = 0.0;                 /* initialize integral */
      h2 = h/2.0;
      for (j=0;j<n;j++) {             /* sum over all "j" integrals */
        aij = a + j*h;        /* lower limit of "j" integral */
        integ += fct(aij+h2)*h;
      }
      return (integ);
}
float simpsons_(float ll, int n,float h)
{
    // Calculating the value of h
   // float h = (ul - ll) / n;

    // Array for storing value of x and f(x)
    float x[100000], fx[100000];

    // Calculating values of x and f(x)
    for (int i = 0; i <= n; i++) {
        x[i] = ll + i * h;
        fx[i] = fct(x[i]);
    }

    // Calculating result
    float res = 0;
    for (int i = 0; i <= n; i++) 
		{
        if (i == 0 || i == n)
            res += fx[i];
        else if (i % 2 != 0)
            res += 4 * fx[i];
        else
            res += 2 * fx[i];
    }
    res = res * (h / 3);
    return res;
}



//LAGRANGE.C



//for this program number of processors is equal to number of points that we have
#include<stdio.h>
#include<string.h>
#include<mpi.h>

int main(int argc,char *argv[])
{
    int my_rank;
    int p,i;
    int x[]={0,1,2,5};
    int y[]={2,3,12,147};
    int X=3;
    float sum;
    int tag = 0;
    int temp1,temp2;
    float temp;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
/*  
  if(my_rank==0)
    {
        for(i=0;i<4;i++)
         scanf("%d",&x[i]);
        for(i=0;i<4;i++)
         scanf("%d",&y[i]);
    }
*/
    MPI_Bcast(x,4,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(y,4,MPI_INT,0,MPI_COMM_WORLD);

   temp1=1;
   for(i=0;i<4;i++)
   {
     if(my_rank==i);
     else
       temp1*=(X-x[i]);
   }
   
	temp2=1;
	for(i=0;i<4;i++)
	{
     if(my_rank==i);
     else
       temp2*=(x[my_rank]-x[i]);
	}
	temp=(float)temp1/(float)temp2;
	temp=temp*(float)y[my_rank];
	MPI_Reduce(&temp,&sum,1,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
	if(my_rank == 0)
        printf("%f",sum);
    
MPI_Finalize();
}



//PREFIXSUM.C


#include<stdio.h>
#include<mpi.h>

int computeSum(int a[],int my_rank)
{
        int sum=0;
        int i;
        for(i=0;i<=my_rank;i++)
        {
                sum=sum+a[i];
        }
        return sum;
}
int main(int argc, char *argv[])
{
        int my_rank;
        int p;
        int a[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
        int pSum;
        MPI_Init(&argc,&argv);
        MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
        MPI_Comm_size(MPI_COMM_WORLD,&p);
        if(my_rank==0)
        {
                printf("Enter index\n");
                int x;
                scanf("%d",&x);
                pSum=computeSum(a,x);
                printf("Index = %d, sum = %d\n ",x,pSum);
        }
        MPI_Finalize();
}




//MATMUL.C



//for this program number of processors is equal to N+1
#include <stdio.h>
#include "mpi.h"
#define N 4
/* number of rows and columns in matrix */
MPI_Status status;
double a[N][N],b[N][N],c[N][N];
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
			for (j=0; j<N; j++) 
			{
				a[i][j]= 1.0;
				b[i][j]= 2.0;
			}	
		}
		rows = N/numworkers;
		offset = 0;
		for (dest=1; dest<=numworkers; dest++)
		{
			MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
			MPI_Send(&b, N*N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
			offset = offset + rows;
		}
		/* wait for results from all worker tasks */
		for (i=1; i<=numworkers; i++)
		{
			source = i;
			MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
		}
		printf("Here is the result matrix:\n");
		for (i=0; i<N; i++) 
		{
			for (j=0; j<N; j++)
				printf("%6.2f ", c[i][j]);
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
		MPI_Recv(&b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
		/* Matrix multiplication */
		for (k=0; k<N; k++)
			for (i=0; i<rows; i++) 
			{
				c[i][k] = 0.0;
				for (j=0; j<N; j++)
					c[i][k] = c[i][k] + a[i][j] * b[j][k];
			}
			MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
			MPI_Send(&c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
	}
	MPI_Finalize();
}



//MATVEC.C



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




//ODDEVEN.C



#include<stdio.h>
#include<string.h>
#include <mpi.h>
void main(int argc, char* argv[])
{
	int my_rank,p,a[100],source,dest;
	int tag=0;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	if(my_rank==0)
	{
		int n=4;
		a[0]=4;
		a[1]=3;
		a[2]=2;
		a[3]=1;
		/*printf("Enter total elements: \n");
		scanf("%d",&n);
		int i;
		for(i=0;i<n;i++)
			scanf("%d",&a[i]);
		*/
		int i;
		for(i=0;i<n;i++)
		{
			int b=a[i];
			MPI_Send(&b,1,MPI_INT,i+1,tag,MPI_COMM_WORLD);
		}
		for(source=1;source<p;source++)
		{
			int x;	
			MPI_Recv(&x,1,MPI_INT,source,tag,MPI_COMM_WORLD,&status);
			a[source-1]=x;
			//printf("Message from process %d element = %d\n",source-1,x);
		}
		//printing the final result
		for(int i=0;i<n;i++)
		{
			printf("%d ",a[i]);
		}
	}
	else
	{
		int x,z;
		MPI_Recv(&x,1,MPI_INT,0,tag, MPI_COMM_WORLD, &status);
		int j;
		int k=(p-1)/2;
		if((p-1)%2!=0)
			k++;
		for(j=1;j<=k;j++)
		{
			if(my_rank+1<p && my_rank%2==1)
			{
				MPI_Send(&x,1,MPI_INT,my_rank+1,tag,MPI_COMM_WORLD);
				MPI_Recv(&x,1,MPI_INT,my_rank+1,tag,MPI_COMM_WORLD,&status);
			}
			if(my_rank-1>=1 && my_rank%2==0)
			{
				MPI_Recv(&z,1,MPI_INT,my_rank-1,tag,MPI_COMM_WORLD,&status);
				if(z>x)
				{
					int temp=z;
					z=x;
					x=temp;
				}
				MPI_Send(&z,1,MPI_INT,my_rank-1,tag,MPI_COMM_WORLD);	
			}
			if(my_rank+1<p && my_rank%2==0)
			{
				MPI_Send(&x,1,MPI_INT,my_rank+1,tag,MPI_COMM_WORLD);
				MPI_Recv(&x,1,MPI_INT,my_rank+1,tag,MPI_COMM_WORLD,&status);
			}
			if(my_rank-1>=1 && my_rank%2==1)
			{
				MPI_Recv(&z,1,MPI_INT,my_rank-1,tag,MPI_COMM_WORLD,&status);
				if(z>x)
				{
					int temp=z;
					z=x;
					x=temp;
				}
				MPI_Send(&z,1,MPI_INT,my_rank-1,tag,MPI_COMM_WORLD);
			}
		}
		MPI_Send(&x,1,MPI_INT,0,tag,MPI_COMM_WORLD);
	}
MPI_Finalize();
}



//TRAPEZOIDAL.C



#include <mpi.h>
#include <math.h>
#include <stdio.h>
float fct(float x)
{
	return (1.0000/(1.0000+(x*x)));
}
/* Prototype */
float integral(float a, int n, float h);
float simpsons_(float a,int n,float h);
int main(int argc,char *argv[])
{
      int n, p, i, j, ierr,num;
      float h, result, a, b, pi;
      float my_a, my_range;

      int myid, source, dest, tag;
      MPI_Status status;
      float my_result;

      pi = acos(-1.0);  /* = 3.14159... */
      a = 0.;           /* lower limit of integration */
      b = 1.0;     /* upper limit of integration */
      n = 100000;          /* number of increment within each process */

      dest = 0;         /* define the process that computes the final result */
      tag = 123;        /* set the tag to identify this particular job */

/* Starts MPI processes ... */

      MPI_Init(&argc,&argv);              /* starts MPI */
      MPI_Comm_rank(MPI_COMM_WORLD, &myid);  /* get current process id */
      MPI_Comm_size(MPI_COMM_WORLD, &p);     /* get number of processes */

      h = (b-a)/n;    /* length of increment */
      num = n/p;        /* number of intervals calculated by each process*/
      my_range = (b-a)/p;
      my_a = a + myid*my_range;
     // my_result = integral(my_a,num,h);
        my_result=simpsons_(my_a,num,h);

      printf("Process %d has the partial result of %f\n", myid,my_result);

      if(myid == 0) 
			{
        result = my_result;
        for (i=1;i<p;i++)
        {
          source = i;           /* MPI process number range is [0,p-1] */
          MPI_Recv(&my_result, 1, MPI_REAL, source, tag,MPI_COMM_WORLD, &status);
          result += my_result;
        }
        printf("The result =%f\n",result);
      }
      else
			{
				MPI_Send(&my_result, 1, MPI_REAL, dest, tag,MPI_COMM_WORLD);      /* send my_result to intended dest.*/
			}
      MPI_Finalize();                       /* let MPI finish up ... */
}
float integral(float a, int n, float h)
{
      int j;
      float h2, aij, integ;

      integ = 0.0;                 /* initialize integral */
      h2 = h/2.0;
      for (j=0;j<n;j++) {             /* sum over all "j" integrals */
        aij = a + j*h;        /* lower limit of "j" integral */
        integ += fct(aij+h2)*h;
      }
      return (integ);
}
float simpsons_(float ll, int n,float h)
{
    // Calculating the value of h
   // float h = (ul - ll) / n;

    // Array for storing value of x and f(x)
    float x[100000], fx[100000];

    // Calculating values of x and f(x)
    for (int i = 0; i <= n; i++) {
        x[i] = ll + i * h;
        fx[i] = fct(x[i]);
    }

    // Calculating result
    float res = 0;
    for (int i = 0; i <= n; i++) 
		{
        if (i == 0 || i == n)
            res += fx[i];
        else
            res += 2 * fx[i];
    }
    res = res * (h / 2);
    return res;
}



//SHUFFLE.C



#include<mpi.h>
#include<stdio.h>

#define N 50

int arr[N];

int shuffle(int x,int p)
{
	x <<= 1;
	if(x >= p)
	{
		x -= p;
		x += 1;
	}
	return x;
}

int rev_shuffle(int x,int p)
{
	if(x&1)
	x += p;
	x >>= 1;
	return x;
}

int main()
{
	int tag=0,rank,p,a,e,x;
	MPI_Status status;
	MPI_Comm w = MPI_COMM_WORLD;
	MPI_Init(NULL,NULL);
	MPI_Comm_size(w,&p);
	MPI_Comm_rank(w,&rank);

	if(rank == 0)
	{
		arr[0]=1;
		arr[1]=1;
		arr[2]=1;
		arr[3]=2;
		//printf("Enter the coefficients\n");
		//for(int i=0;i<p-1;i++)
		//scanf("%i",&arr[i]);
		//printf("Enter x: ");
		//scanf("%i",&arr[p-1]);
	}
	MPI_Scatter(arr,1,MPI_INT,&a,1,MPI_INT,0,w);
	if(rank%2 == 0)
		e = 0;
	else
		e = 1;
	if(rank == p-1)
		x = a;
	for(int i=1;i<p;i*=2)
	{
		MPI_Bcast(&x,1,MPI_INT,p-1,w);
		if(e == 1)
			a *=x;
		if(shuffle(rank,p) != rank)
		{
			MPI_Send(&e,1,MPI_INT,shuffle(rank,p),tag,w);
			MPI_Recv(&e,1,MPI_INT,rev_shuffle(rank,p),tag,w,&status);
		}
		x = x*x;
	}
	if(rank != p-1)
		printf("processor %i has %i\n",rank,a);

	MPI_Finalize();
}



//PRIMES.C




#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LIMIT     20     /* Increase this to find more primes */
#define FIRST     0           /* Rank of first task */

int isprime(int n) {
int i,squareroot;
if (n>10) {
   squareroot = (int) sqrt(n);
   for (i=3; i<=squareroot; i=i+2)
      if ((n%i)==0)
         return 0;
   return 1;
   }
/* Assume first four primes are counted elsewhere. Forget everything else */
else
   return 0;
}


int main (int argc, char *argv[])
{
int   ntasks,
      rank,                 
      n,                    
      pc,                   
      pcsum,                
      foundone,             
      maxprime,             
      mystart,              
      stride;             

double start_time,end_time;
	
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);
MPI_Comm_size(MPI_COMM_WORLD,&ntasks);
if (((ntasks%2) !=0) || ((LIMIT%ntasks) !=0)) {
   printf("Sorry - this exercise requires an even number of tasks.\n");
   printf("evenly divisible into %d.  Try 4 or 8.\n",LIMIT);
   MPI_Finalize();
   exit(0);
   }

start_time = MPI_Wtime();
mystart = (rank*2)+1;       
stride = ntasks*2;          
pc=0;                       
foundone = 0;               


if (rank == FIRST) {
   printf("Using %d tasks to scan %d numbers\n",ntasks,LIMIT);
   pc = 4;                  /* Assume first four primes are counted here */
   for (n=mystart; n<=LIMIT; n=n+stride) {
      if (isprime(n)) {
         pc++;
         foundone = n;
         /***** Optional: print each prime as it is found
         printf("%d\n",foundone);
         *****/
         }
      }
   MPI_Reduce(&pc,&pcsum,1,MPI_INT,MPI_SUM,FIRST,MPI_COMM_WORLD);
   MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX,FIRST,MPI_COMM_WORLD);
   end_time=MPI_Wtime();
   printf("Done. Largest prime is %d Total primes %d\n",maxprime,pcsum);
   printf("Wallclock time elapsed: %.2lf seconds\n",end_time-start_time);
   }



if (rank > FIRST) {
   for (n=mystart; n<=LIMIT; n=n+stride) {
      if (isprime(n)) {
         pc++;
         foundone = n;
         /***** Optional: print each prime as it is found
         printf("%d\n",foundone);
         *****/
         }
      }
   MPI_Reduce(&pc,&pcsum,1,MPI_INT,MPI_SUM,FIRST,MPI_COMM_WORLD);
   MPI_Reduce(&foundone,&maxprime,1,MPI_INT,MPI_MAX,FIRST,MPI_COMM_WORLD);
   }

MPI_Finalize();
}
