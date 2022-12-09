#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

__global__ void filter(int *image_arr, int rows, int cols, int filter_size, int *result)
{

    int pt_x = blockIdx.x * blockDim.x + threadIdx.x;
    int pt_y = blockIdx.y * blockDim.y + threadIdx.y;
    int sum = filter_size * filter_size;
    if (pt_x < rows - filter_size + 1 && pt_y < cols - filter_size + 1)
    {
        int array_index = pt_x * (cols - filter_size + 1) + pt_y;

        for (int i = -filter_size / 2; i <= filter_size / 2; ++i)
        {
            for (int j = -filter_size / 2; j <= filter_size / 2; ++j)
            {
                int temp_x = (pt_x + (filter_size - 1) / 2 + i);
                int temp_y = pt_y + (filter_size - 1) / 2 + j;
                result[array_index] += image_arr[temp_x * cols + temp_y] / sum;
            }
        }
    }
}

int *read_txt(int rows, int cols)
{
    int *matr = new int[rows * cols];

    ifstream myfile;
    myfile.open("flopa.txt");

    for (int i = 0; i < rows * cols; ++i)
    {
        myfile >> matr[i];
    }
    return matr;
}

void write(int *matr, int rows, int cols)
{
    ofstream file;
    file.open("flopa_out.txt");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            file << matr[i * cols + j] << ' ';
        }
        file << "\n";
    }
    file.close();
}

int main()
{
    int width = 606;
    int height = 497;
    int *host_img = read_txt(width, height);
    int *image_on_device;
    int *result_on_host;
    int *result_on_device;
    cudaMalloc((void **)&image_on_device, width * height * sizeof(int));
    cudaMemcpy(image_on_device, host_img, width * height * sizeof(int), cudaMemcpyHostToDevice);
    cudaMalloc((void **)&result_on_device, (width - 2) * (height - 2) * sizeof(int));

    result_on_host = new int[(width - 2) * (height - 2)];

    filter<<<dim3(256, 256, 1), dim3(3, 3, 1)>>>(image_on_device, width, height, 3, result_on_device);

    cudaMemcpy(result_on_host, result_on_device, (width - 2) * (height - 2) * sizeof(int), cudaMemcpyDeviceToHost);

    write(result_on_host, width - 2, height - 2);
    cudaFree(result_on_device);
    cudaFree(image_on_device);
    return 0;
}