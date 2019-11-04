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
