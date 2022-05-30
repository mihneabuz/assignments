#include <math.h>
#include <fstream>

#include "helper.h"

using namespace std;

void readCities(std::vector<city> &cities, const char *fileIn) {
    string geon;
    float lat;
    float lon;
    unsigned long long pop;

    ifstream ifs(fileIn);

    while(ifs >> geon >> lat >> lon >> pop) {
        cities.emplace_back(lat, lon, pop);
    }

    ifs.close();
}

void writeCities(std::vector<city> &cities, const char *fileOut) {
    ofstream ofs(fileOut);

    for (auto &city : cities) {
        ofs << city.reachable << '\n';        
    }
    
    ofs.close();
}