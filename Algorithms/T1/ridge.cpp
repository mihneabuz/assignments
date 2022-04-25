#include <iostream>
#include <fstream>
#include <vector>

#define index(x, y) (x * 3 + y)

struct height {
    int level, cost;
};

int main() {
    std::ifstream in("ridge.in");
    std::ofstream out("ridge.out");

    int n;
    in >> n;

    std::vector<height> heights(n, {0, 0});
    uint64_t *dp = new uint64_t[n * 3];

    for (auto i = 0; i < n; i++)
        in >> heights[i].level >> heights[i].cost;

    in.close();

    // dp values for first height
    dp[index(0, 0)] = 0;
    dp[index(0, 1)] = heights[0].cost;
    dp[index(0, 2)] = heights[0].cost * 2;

    // fill dp array
    for (auto i = 1; i < n; i++) {
        for (auto j = 0; j < 3; j++) {
            int level = heights[i].level - j;

            // can't go below 0
            if (level >= 0) {
                uint64_t min = UINT64_MAX;

                // if it's different then previous
                if (level != heights[i - 1].level)
                    min = std::min(min, dp[index((i - 1), 0)]);

                // if it's different then previous - 1
                if (level != heights[i - 1].level - 1)
                    min = std::min(min, dp[index((i - 1), 1)]);

                // if it's different then previous - 2
                if (level != heights[i - 1].level - 2)
                    min = std::min(min, dp[index((i - 1), 2)]);

                // put minimum in dp array
                dp[index(i, j)] = j * heights[i].cost + min;
            } else {
                dp[index(i, j)] = UINT64_MAX;
            }
        }
    }

    // output min from last column
    out << std::min(std::min(dp[index((n - 1), 0)],
                             dp[index((n - 1), 1)]),
                             dp[index((n - 1), 2)]);
    out.close();
    return 0;
}
