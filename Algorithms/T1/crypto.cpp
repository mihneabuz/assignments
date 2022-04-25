#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct computer {
    uint64_t rate, cost;
};

// compare func for sorting by rate
inline bool compare(const computer a, const computer b) {
    return a.rate < b.rate;
}

int main() {
    std::ifstream in("crypto.in");
    std::ofstream out("crypto.out");

    uint32_t n;
    uint64_t credit;
    in >> n >> credit;

    std::vector<computer> computers(n, {0, 0});

    // read input into vector
    for (auto i = 0; i < n; i++) {
        uint64_t rate, cost;
        in >> rate >> cost;
        computers[i] = {rate, cost};
    }

    in.close();

    // sort by rate
    std::sort(computers.begin(), computers.end(), compare);

    // initialize to first computer
    uint64_t current_rate = computers[0].rate;  // current rate achieved
    uint64_t increase = computers[0].cost;     // cost to go to (current_rate+1)
    uint64_t current_cost = 0;                 // cost to go to current_rate

    // solve greedy
    for (auto i = 1; i < n; i++) {
        // if this computer has same the rate as last one
        if (computers[i].rate == current_rate) {
            increase += computers[i].cost;
        } else {
            uint64_t next_cost = (computers[i].rate - current_rate) * increase;

            if (current_cost + next_cost > credit) {
                // going to next computer means going over credit
                out << ((credit - current_cost) / increase) + current_rate;
                return 0;

            } else {
                // can go to next computer
                current_cost += next_cost;
            }

            // update rate and increase to next rate
            current_rate = computers[i].rate;
            increase += computers[i].cost;
        }
    }

    out << ((credit - current_cost) / increase) + current_rate;
    out.close();
    return 0;
}
