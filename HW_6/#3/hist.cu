#include <iostream>
#include <fstream>
#include <stdio.h>
using namespace std;

__global__ void calc_hist(int *image_arr, int rows, int cols, int *res)
{

    int pt_x = blockIdx.x * blockDim.x + threadIdx.x;
    int pt_y = blockIdx.y * blockDim.y + threadIdx.y;
    if (pt_x < rows && pt_y < cols)
    {
        atomicAdd(res + image_arr[pt_x * cols + pt_y], 1);
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

void write(int *arr)
{
    ofstream file;
    file.open("hist.txt");
    for (int i = 0; i < 256; ++i)
    {
        file << arr[i] << ' ';
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
    cudaMalloc((void **)&result_on_device, 256 * sizeof(int));

    result_on_host = new int[256];
    for (size_t i = 0; i < 256; i++)
    {
        result_on_host[i] = 0;
    }

    calc_hist<<<dim3(256, 256, 1), dim3(3, 3, 1)>>>(image_on_device, width, height, result_on_device);

    cudaMemcpy(result_on_host, result_on_device, 256 * sizeof(int), cudaMemcpyDeviceToHost);
    write(result_on_host);
    cudaFree(result_on_device);
    cudaFree(image_on_device);
    return 0;
}