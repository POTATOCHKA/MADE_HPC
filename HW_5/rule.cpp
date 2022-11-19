#include <time.h>
#include <mpi.h>
#include <iostream>

using namespace std;

#define BOUNDARIES_CONDITIONS 1

int get_value(int left, int mid, int right)
{
    int exchange[] = {0, 1, 1, 1, 0, 1, 1, 0};
    int val = exchange[right + 2 * mid + 4 * left];
    return val;
}

void one_game(int *arr, int size)
{
    int *copy = new int[size];
    for (size_t i = 0; i < size; ++i)
    {
        copy[i] = arr[i];
    }
    for (size_t i = 1; i < size - 1; ++i)
    {
        arr[i] = get_value(copy[i - 1], copy[i], copy[i + 1]);
    }
    delete[] copy;
}

void show_array(int *array, int size)
{

    for (size_t i = 1; i < size - 1; ++i)
    {
        if (array[i])
        {
            printf("#");
        }
        else
        {
            printf(" ");
        }
    }
}

void exchange_ghost_cells(int left, int right, int Size, int *array, int psize)
{
    if (BOUNDARIES_CONDITIONS)
    {
        if (left == -1)
        {
            left = psize - 1;
        }

        if (right == psize)
        {
            right = 0;
        }
        MPI_Send(&array[1], 1, MPI_INT, left, 228, MPI_COMM_WORLD);
        MPI_Send(&array[Size - 1], 1, MPI_INT, right, 228, MPI_COMM_WORLD);
        MPI_Recv(&array[0], 1, MPI_INT, left, 228, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&array[Size], 1, MPI_INT, right, 228, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else
    {
        if (left != -1)
        {
            MPI_Send(&array[1], 1, MPI_INT, left, 228, MPI_COMM_WORLD);
        }
        if (right != psize)
        {
            MPI_Send(&array[Size - 1], 1, MPI_INT, right, 228, MPI_COMM_WORLD);
        }

        if (left != -1)
        {
            MPI_Recv(&array[0], 1, MPI_INT, left, 228, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (right != psize)
        {
            MPI_Recv(&array[Size], 1, MPI_INT, right, 228, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
}

int main(int argc, char **argv)
{
    int Size = 40;
    int num_games = 50;

    int psize;
    int prank;
    MPI_Status status;
    int ierr;

    ierr = MPI_Init(&argc, &argv);
    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &psize);
    srand(time(NULL));

    int *array = new int[Size];

    for (size_t i = 0; i < Size; ++i)
    {
        array[i] = rand() % 2;
    }

    for (int i = 0; i < num_games; ++i)
    {
        int left = prank - 1;
        int right = prank + 1;

        exchange_ghost_cells(left, right, Size, array, psize);

        one_game(array, Size);
        if (prank == 0)
        {
            cout << endl;

            show_array(array, Size);
            for (size_t process = 1; process < psize; ++process)
            {
                int *temp_array = new int[Size];
                MPI_Recv(temp_array, Size, MPI_INT, process, 228, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                show_array(temp_array, Size);
            }
        }
        else
        {
            MPI_Send(array, Size, MPI_INT, 0, 228, MPI_COMM_WORLD);
        }
    }
    ierr = MPI_Finalize();
    return 0;
}
/*
mpic++ rule.cpp -o kek
mpirun -n 4 ./kek
*/