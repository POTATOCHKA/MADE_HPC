#include "matmul.h"

using namespace std;

int main()
{
    int sizes[6] = {500, 512, 1000, 1024, 2000, 2048};
    char indexes[3] = {'i', 'j', 'k'};
    for (size_t i = 0; i < 3; i++)
    {
        cout << indexes[i] << endl;
        for (size_t j = 0; j < 6; j++)
        {
            cout << calc_time(sizes[j], indexes[i]) << "s  ";
        }
        cout << endl;
    }

    return 0;
}
/*
OUTPUT
-O3
i
0.372581s  0.385315s  3.0426s  3.42204s  30.8544s  34.8527s  
j
0.369892s  0.379651s  2.88171s  3.23371s  24.7629s  28.1297s  
k
0.36866s  0.377903s  2.75658s  2.95073s  22.1773s  23.7965s 
-g
i
0.401106s  0.401272s  3.13944s  3.46926s  31.6079s  36.6354s  
j
0.372396s  0.379184s  2.99171s  3.34291s  25.2766s  28.6669s  
k
0.359614s  0.373424s  2.78576s  2.94454s  22.1649s  23.7853s 
*/