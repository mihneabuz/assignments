#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct graph_entry {
	vector<string> adj;
	bool seen;
};

struct person {
	string name;
	vector<string> emails;
};

typedef unordered_map<string, graph_entry> string_graph;

// cmp function for sorting
inline bool my_cmp(person& x, person& y) {
	if (x.emails.size() == y.emails.size())
		return x.name > y.name;
	return x.emails.size() < y.emails.size();
}

// some helper functions
inline bool is_digit(char c) {
	return c >= '0' && c <= '9';
}

inline bool is_email(string& str) {
	return !is_digit(str.back());
}

// dfs, builds person
void dfs(string key, string_graph& graph, person& pers) {
	auto& node = graph[key];

	if (node.seen)
		return;

	if (is_email(key))
		pers.emails.push_back(key);
	else
		pers.name = min(pers.name, key);

	node.seen = true;
	for (auto adj : node.adj)
		dfs(adj, graph, pers);
}

int main() {
	ifstream in("adrese.in");
    ofstream out("adrese.out");

	int n;
	in >> n;

	// make undirected graph of emails and names
	int id = 0;
	string_graph graph;
	for (int i = 0; i < n; i++) {
		string name;
		int email_count;
		in >> name >> email_count;

		// add a unique number to end of name for duplicate names
		name = name + to_string(id++);

		for (int j = 0; j < email_count; j++) {
			string email;
			in >> email;

			graph[name].adj.push_back(email);
			graph[email].adj.push_back(name);
		}
	}
	in.close();

	// use dfs to find all conex pieces (all unique persons)
	vector<person> unique_pers;
	for (auto v : graph) {
		if (!v.second.seen) {
			// create dummy person to be written by dfs
			person aux{"ZZZZZZZZZZZ", {}};
			dfs(v.first, graph, aux);

			// remove id number from end of name
			while (is_digit(aux.name.back()))
				aux.name.pop_back();

			// put person into list
			unique_pers.push_back(aux);
		}
	}

	// sort lists and show output
	out << unique_pers.size() << '\n';
	sort(unique_pers.begin(), unique_pers.end(), my_cmp);
	for (auto pers : unique_pers) {
		out << pers.name << ' ' << pers.emails.size() << '\n';
		sort(pers.emails.begin(), pers.emails.end());
		for (auto e : pers.emails)
			out << e << '\n';
	}
	out.close();

	return 0;
}
