#include <iostream>
#include <fstream>
#include <vector>

int main() {
    std::ifstream in("valley.in");
    std::ofstream out("valley.out");

    int n;
    in >> n;

    std::vector<int> heights(n, 0);

    // read into array and find minimums and indices
    int min = INT32_MAX, first_min, last_min;
    for (auto i = 0; i < n; i++) {
        in >> heights[i];

        if (heights[i] < min) {
            if (heights[i] == min) {
                last_min = i;
            } else {
                first_min = i;
                last_min = i;
            }

            min = heights[i];
        }
    }

    in.close();

    int lefth = heights[0];
    int righth = heights[n - 1];

    unsigned long long count = 0;

    // check for edge case min first
    int min_first_case = 0;
    if (last_min == 0) {
        count += (heights[1] - heights[0]);
        min_first_case = 1;
    }

    // check for edge case min last
    int min_last_case = 0;
    if (first_min == n - 1) {
        count += (heights[n - 2] - heights[n - 1]);
        min_last_case = 1;
    }

    // calculate from start to first min
    for (auto i = 0; i < first_min - min_last_case; i++) {
        if (heights[i] < lefth)
            lefth = heights[i];
        else
            count += (heights[i] - lefth);
    }

    // calculate from first min to last min
    for (auto i = first_min + 1; i < last_min; i++)
        count += (heights[i] - min);

    // calculate from last min to end
    for (auto i = n - 1; i > last_min + min_first_case; i--) {
        if (heights[i] < righth)
            righth = heights[i];
        else
            count += (heights[i] - righth);
    }

    out << count;
    out.close();
    return 0;
}
