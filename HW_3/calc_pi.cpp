#include <omp.h>
#include <iostream>

int main (int argc, char *argv[])
{
    int N = 1e9;
    int num_in_round = 0;
    float random_x = 0;
    float random_y = 0;



    #pragma omp parallel default(none) shared(N) private(random_x, random_y) reduction(+ : num_in_round) num_threads(4)
    {
        unsigned int seed = 123 + omp_get_thread_num();
        #pragma omp for
            for (int i = 0; i < N; ++i) {
                random_x = rand_r(&seed) / static_cast<float>(RAND_MAX);
                random_y = rand_r(&seed) / static_cast<float>(RAND_MAX);
                if (((random_x * random_x) + (random_y * random_y)) < 1){
                    num_in_round +=1;
                }
            }
    }
    float pi = 4 * (float) (num_in_round) / N;
    std::cout << pi << std::endl;

    return 0;
}