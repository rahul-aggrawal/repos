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

