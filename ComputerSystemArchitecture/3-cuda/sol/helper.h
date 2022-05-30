#ifndef ACCPOP_HELPER_H
#define ACCPOP_HELPER_H
#include <stdio.h>
#include <vector>

#define DEGREE_TO_RADIANS		0.01745329252f

struct city {
    unsigned long long pop, reachable;
    float phi, theta;

    city() {}
    
    city(float lat, float lon, unsigned long long pop_) {
        phi = (90.f - lat) * DEGREE_TO_RADIANS;
        theta = lon * DEGREE_TO_RADIANS;
        pop = pop_;
        reachable = pop_;
    }
};

void readCities(std::vector<city> &cities, const char *fileIn);
void writeCities(std::vector<city> &cities, const char *fileOut);

#define DIE(assertion, call_description)                    \
do {                                                        \
    if (assertion) {                                        \
            fprintf(stderr, "(%d): ",                       \
                            __LINE__);                      \
            perror(call_description);                       \
            exit(EXIT_FAILURE);                             \
    }                                                       \
} while(0);

#endif //ACCPOP_HELPER_H