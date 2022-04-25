#include <iostream>
#include <fstream>
#include <vector>

#define index(x, y, z) (x * (stop_loss+1)*(credit+1) + y * (credit+1) + z)

struct stock {
    int price, max_loss, max_profit;
};

int main() {
    std::ifstream in("stocks.in");
    std::ofstream out("stocks.out");

    int n, credit, stop_loss;
    in >> n >> credit >> stop_loss;

    std::vector<stock> stocks(n, {0, 0, 0});
    int *dp = new int[n * (credit + 1) * (stop_loss + 1)];

    for (auto i = 0; i < n; i++) {
        int price, min, max;
        in >> price >> min >> max;
        stocks[i] = {price, price - min, max - price};
    }

    in.close();

    // initialize for first stock
    for (int sl = 1; sl <= stop_loss; sl++)
        for (int cr = 1; cr <= credit; cr++)
            if (stocks[0].price <= cr && stocks[0].max_loss <= sl)
                dp[index(0, sl, cr)] = stocks[0].max_profit;

    // dp for rest of stocks
    for (int stock = 1; stock < n; stock++) {
        // current stock information
        int profit = stocks[stock].max_profit;
        int loss = stocks[stock].max_loss;
        int price = stocks[stock].price;

        for (int sl = 1; sl <= stop_loss; sl++) {
            for (int cr = 1; cr <= credit; cr++) {
                // value for current index in dp
                int val = 0;

                // if we can get current stock at current stop loss and credit
                if (loss <= sl && price <= cr) {
                    val = std::max(dp[index((stock - 1), sl, cr)],
                          profit + dp[index((stock-1), (sl-loss), (cr-price))]);
                } else {
                    val = dp[index((stock - 1), sl, cr)];
                }

                // put value in array
                dp[index(stock, sl, cr)] = val;
            }
        }
    }

    out << dp[index((n - 1), stop_loss, credit)];
    out.close();
    return 0;
}
