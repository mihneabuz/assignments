#include <iostream>
#include <stdlib.h>
#include <vector>

#include "helper.h"

__device__ inline float geoDistanceDevice(float phi1, float theta1, float phi2, float theta2)
{
    float cs = __sinf(phi1) * __sinf(phi2) * __cosf(theta1 - theta2) + __cosf(phi1) * __cosf(phi2);
    cs = max(min(cs, 1.f), -1.f);
    return 6371.f * acos(cs);
}

__global__ void kernel(city* cities, float kmRange, int offsetx, int offsety, int size) {
    const auto x = offsetx + threadIdx.x;
    const auto y = offsety + blockIdx.x;

    if (x >= y || x >= size || y >= size)
        return;

    const auto dist = geoDistanceDevice(cities[x].phi, cities[x].theta,
                                        cities[y].phi, cities[y].theta);

    if (dist <= kmRange) {
        atomicAdd(&(cities[x].reachable), cities[y].pop);
        atomicAdd(&(cities[y].reachable), cities[x].pop);
    }
}

int main(int argc, char** argv) {
    DIE(argc == 1, "./accpop <kmrange1> <file1in> <file1out> ...");
    DIE((argc - 1) % 3 != 0, "./accpop <kmrange1> <file1in> <file1out> ...");

    float kmRange;
    int size, offset[2];
    const int dim = 1024;

    for (int argcID = 1; argcID < argc; argcID += 3) {
        kmRange = atof(argv[argcID]);

        std::vector<city> cities(0);
        readCities(cities, argv[argcID + 1]);
        size = cities.size();
        
        city* citiesDevice;
        cudaMalloc(&citiesDevice, cities.size() * sizeof(city));
        cudaMemcpy(citiesDevice, &cities.front(), cities.size() * sizeof(city), cudaMemcpyHostToDevice);

        offset[0] = 0;
        offset[1] = 0;

        while (offset[0] <= size) {
            while (offset[1] <= size) {
                kernel<<<dim, dim>>>(citiesDevice, kmRange, offset[0], offset[1], size);
                offset[1] += dim;
            }

            offset[0] += dim;
            offset[1] = offset[0];
        }
        cudaDeviceSynchronize();
        
        cudaMemcpy(&cities[0], citiesDevice, cities.size() * sizeof(city), cudaMemcpyDeviceToHost);

        writeCities(cities, argv[argcID + 2]);

        cudaFree(citiesDevice);
    }
}
