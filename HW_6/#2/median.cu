#include <iostream>
#include <fstream>
#include <stdio.h>
    using namespace std;

__global__ void median_filter(int *img, int rows, int cols, int filter_size, int *res)
{

    int pt_x = blockIdx.x * blockDim.x + threadIdx.x;
    int pt_y = blockIdx.y * blockDim.y + threadIdx.y;
    int double_filter = filter_size * filter_size;
    int temp_arr[1000];
    if (pt_x < rows - filter_size + 1 && pt_y < cols - filter_size + 1)
    {
        int final_arr_idx = pt_x * (cols - filter_size + 1) + pt_y;

        for (int i = -filter_size / 2; i <= filter_size / 2; ++i)
        {
            for (int j = -filter_size / 2; j <= filter_size / 2; ++j)
            {
                int arr_idx = (i + filter_size / 2) * filter_size + j + filter_size / 2;
                int temp_x = pt_x + (filter_size - 1) / 2 + i;
                int temp_y = pt_y + (filter_size - 1) / 2 + j;

                temp_arr[arr_idx] = img[temp_x * cols + temp_y];
            }
        }
        for (int i = 0; i < double_filter - 1; ++i)
            for (int j = 1; j < double_filter; ++j)
                if (temp_arr[i] > temp_arr[j])
                {
                    temp_arr[i] = temp_arr[i] ^ temp_arr[j];
                    temp_arr[j] = temp_arr[j] ^ temp_arr[i];
                    temp_arr[i] = temp_arr[i] ^ temp_arr[j];
                }

        res[final_arr_idx] = temp_arr[double_filter / 2 + 1];
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
    int width = 347;
    int height = 320;
    int *host_img = read_txt(width, height);
    int *image_on_device;
    int *result_on_host;
    int *result_on_device;
    cudaMalloc((void **)&image_on_device, width * height * sizeof(int));
    cudaMemcpy(image_on_device, host_img, width * height * sizeof(int), cudaMemcpyHostToDevice);
    cudaMalloc((void **)&result_on_device, (width - 2) * (height - 2) * sizeof(int));

    result_on_host = new int[(width - 2) * (height - 2)];

    median_filter<<<dim3(256, 256, 1), dim3(3, 3, 1)>>>(image_on_device, width, height, 3, result_on_device);

    cudaMemcpy(result_on_host, result_on_device, (width - 2) * (height - 2) * sizeof(int), cudaMemcpyDeviceToHost);
    write(result_on_host, width - 2, height - 2);
    cudaFree(result_on_device);
    cudaFree(image_on_device);
    return 0;
}