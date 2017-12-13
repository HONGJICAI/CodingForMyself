#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <queue>
#include <memory>
#include <chrono>
#include "HungarianRings.h"
using namespace std;
class Solution{
	void search(unordered_map<uint64_t, pair<uint64_t, HungarianRings::ROTATE>>&parent, uint64_t child) {
		auto it = parent.find(child);
		if (it == parent.end())
			return;
		search(parent, it->second.first);
		cout << it->second.second << " ";
	}
public:
	void solve(char left[], char right[]) {
		std::queue<uint64_t>qe;
		std::unordered_map<uint64_t, pair<uint64_t, HungarianRings::ROTATE> >parent;
		std::shared_ptr<HungarianRings>root = std::make_shared<HungarianRings>(left, right);
		qe.push(root->compress());
		parent.insert(make_pair(root->compress(),make_pair(0,HungarianRings::ROTATE::NONE)));
		while (qe.size()) {
			uint64_t val = qe.front();
			auto c = HungarianRings::decompress(val);
			qe.pop();
			if (c.finish()) {
				cout << "finish " << endl;
				search(parent, c.compress());
				break;
			}
			for (int dir = HungarianRings::ROTATE::L; dir < HungarianRings::ROTATE::NONE; ++dir) {
				uint64_t temp = c.clone(HungarianRings::ROTATE(dir)).compress();
				if (parent.find(temp) == parent.end()) {
					qe.push(temp);
					parent[temp] = make_pair(val, HungarianRings::ROTATE(dir));
				}
			}
		}
	}
};
int main() {
	Solution c;
	char left[15]  = { 0,0,1,1,1,1,1,1,3,3,3,3,0,0,3 };
	char right[15] = { 2,2,2,2,2,2,2,3,0,3,3,1,0,0,3 };
	//char left[15] = { 2,0,0,0,0,0,0,1,1,1,1,1,1,1,3 };
	//char right[15] = { 0,2,2,2,2,2,2,3,3,3,3,3,3,3,1 };
	using namespace std::chrono; 
	steady_clock::time_point t1 = steady_clock::now();
	c.solve(left, right); 
	steady_clock::time_point t2 = steady_clock::now();
	cout <<duration_cast<duration<double>>(t2 - t1).count() << endl;
	int a;
	cin >> a;
}