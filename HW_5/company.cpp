#include <time.h>
#include <mpi.h>
#include <iostream>

using std::cout;
using std::endl;

int get_next_process(int *info, int psize)
{
    int *done = new int[psize];
    int num_of_alive_proc = 0;
    for (size_t i = 0; i < psize; i++)
    {
        done[i] = info[psize + i];
        if (done[i] == -1)
        {
            num_of_alive_proc += 1;
        }
    }
    if (num_of_alive_proc == 0)
    {
        return -1;
    }
    int *alive_proc = new int[num_of_alive_proc];
    int idx = 0;
    for (size_t i = 0; i < psize; i++)
    {
        if (done[i] != 1)
        {
            alive_proc[idx] = i;
            idx++;
        }
    }
    int next_proc = alive_proc[rand() % num_of_alive_proc];
    info[psize + next_proc] = 1;
    for (size_t i = 0; i < psize; i++)
    {
        if (info[i] == -1)
        {
            info[i] = next_proc;
            break;
        }
    }
    delete[] done;
    delete[] alive_proc;
    return next_proc;
}

int main(int argc, char **argv)
{

    int psize;
    int prank;
    MPI_Status status;
    int ierr;

    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &psize);

    int N = 2 * psize;
    int *information = new int[N];
    srand(time(NULL));

    if (prank == 0)
    {
        cout << "num of processes:" << psize << endl;

        for (int i = 0; i < N; ++i)
        {
            information[i] = -1;
        }
        information[0] = 0;
        information[psize] = 1;
        int recevier_process = get_next_process(information, psize);
        MPI_Ssend(information, N, MPI_INT, recevier_process, 228, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(information, N, MPI_INT, MPI_ANY_SOURCE, 228, MPI_COMM_WORLD, &status);
        int recevier_process = get_next_process(information, psize);
        if (recevier_process > 0)
        {
            MPI_Ssend(information, N, MPI_INT, recevier_process, 228, MPI_COMM_WORLD);
        }
        else
        {

            cout << "The final consequence:" << endl;
            for (size_t i = 0; i < psize; i++)
            {
                if (i != psize - 1)
                {
                    cout << information[i] << "->";
                }
                else
                {
                    cout << information[i] << endl;
                }
            }
        }
    }
    delete[] information;

    ierr = MPI_Finalize();
    return 0;
}