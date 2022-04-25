#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

using namespace std;
#define THRESHOLD 5

// generation of all possible solutions
vector<vector<int>> solutions;
void generate(int max, vector<int>& current, int left, int right) {
	int to_choose = max - current.size();

	if (to_choose == 0) {
		solutions.push_back(current);
		return;
	}

	for (int i = left; i <= right - to_choose + 1; i++) {
		current.push_back(i);
		generate(max, current, i + 1, right);
		current.pop_back();
	}
}

/* perfect hashing for can_sum
 *
 * set of numbers from 2 to 50 (1 is always picked!) -> can be encoded in 49 bits
 * t <= 20 -> can be encoded in 4 bits
 * target <= 50 * 20 = 1000 -> can be encoded i 11 bits
 *
 * that's exactly 64 bits! Yay!
 */

uint64_t h(vector<int>& vec, int t, int target) {
	uint64_t res = 0;
	for (auto x : vec)
		res |= 1ull << (x + 13);
	res |= (target << 5);
	res |= t;
	return res;
}

unordered_map<uint64_t, int8_t> cache;
bool can_sum_with_cache(int target, vector<int>& sol, int t) {
	// check cache
	auto& from_cache = cache[h(sol, t, target)];
	if (from_cache == 1)
		return true;
	if (from_cache == -1)
		return false;

	// check early stopping conditions
	if (target == 0 || target <= t) {
		from_cache = 1;
		return true;
	}

	if (t == 0) {
		from_cache = -1;
		return false;
	}

	if (t == 1) {
		for (auto x : sol)
			if (x == target) {
				from_cache = 1;
				return true;
			}
		from_cache = -1;
		return false;
	}

	// recurse
	for (auto x : sol)
		if (x <= target && can_sum_with_cache(target - x, sol, t - 1)) {
			from_cache = 1;
			return true;
		}

	from_cache = -1;
	return false;
}

bool can_sum_no_cache(int target, vector<int>& sol, int t) {
	// check early stopping conditions
	if (target == 0 || target <= t)
		return true;

	if (t == 0)
		return false;

	if (t == 1) {
		for (auto x : sol)
			if (x == target)
				return true;
		return false;
	}

	// recurse
	for (auto x : sol)
		if (x <= target && can_sum_no_cache(target - x, sol, t - 1))
			return true;

	return false;
}

bool can_sum(int target, vector<int>& sol, int t) {
	// use best version based on t
	if (t < THRESHOLD)
		return can_sum_no_cache(target, sol, t);
	else
		return can_sum_with_cache(target, sol, t);
}
int maxi, best;

int main() {
	int k, n, t;
	cin >> k >> n >> t;

	// generate all solutions
	vector<int> tmp;
	generate(n - 1, tmp, 2, k);

	// check all solutions, find best one
	for (auto& sol : solutions) {
		// calculezi suma maxima la care poate ajunge solutia
		int max_sum = sol[sol.size() - 1] * t;

		// calculezi sirul maxim de sume consecutive
		for (int i = 1; i <= max_sum; i++) {
			//find max
		}
	
		// compare maxim curent cu cel mai bun de pana atunci
		if (maxi > best)
			best = maxi;
	}
	return 0;
}
