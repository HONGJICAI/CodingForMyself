#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <queue>
#include <chrono>
#include <functional>
#include <array>
#include "HungarianRings.h"
using namespace std;
class Solution{
	void bfs(HungarianRings &root) {
		std::queue<uint64_t>qe;
		std::unordered_map<uint64_t, pair<uint64_t, HungarianRings::ROTATE> >parent;
		std::function<void(uint64_t)>search = [&parent,&search](uint64_t child) {
			auto it = parent.find(child);
			if (it == parent.end())
				return;
			search(it->second.first);
			cout << it->second.second << " ";
		};

		qe.push(root.compress());
		parent.insert(make_pair(root.compress(),make_pair(0,HungarianRings::ROTATE::NONE)));
		while (qe.size()) {
			uint64_t val = qe.front();
			auto c = HungarianRings::decompress(val);
			qe.pop();
			if (c.finish()) {
				cout << "finish " << endl;
				search(c.compress());
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
	void dfs(HungarianRings &root) {
		unordered_set<uint64_t>visited;
		bool finished = false;
		std::function<void(HungarianRings&)> recursion = [&visited,&recursion,&finished](HungarianRings &root)->void {
			if (root.finish()) {
				cout << "finish" << endl;
				finished = true;
				return;
			}
			for (int dir = HungarianRings::ROTATE::L; dir < HungarianRings::ROTATE::NONE && finished == false; ++dir) {
				auto c = root.clone(HungarianRings::ROTATE(dir));
				auto compressVal = c.compress();
				if (visited.find(compressVal) == visited.end()) {
					visited.insert(compressVal);
					recursion(c);
				}
			}
		};
		
		visited.insert(root.compress());
		recursion(root);
	}
	void A_star(HungarianRings&root) {
		struct HungarianRingsEx :public HungarianRings{
			int times=0;
			int estimateVal;
			HungarianRingsEx() = default;
			HungarianRingsEx(HungarianRings &parent) :
				HungarianRings(parent) 
			{
				this->estimateVal = estimate();
			}
			int estimate() {
				int res = 0, T = 0;
				std::array<int, 4>color1,color2;
				for (int T = 0; T < 2;++T) {
					color1.fill(0);
					color2.fill(0);
					for (int i = T * 7; i < T * 7 + 7; ++i) {
						color1[int(getColor("left", i))]++;
						color2[int(getColor("right", i))]++;
					}
					res += *std::max_element(begin(color1), end(color1)) + *std::max_element(begin(color2), end(color2));
				}
				return res;
			}
			bool operator()(const HungarianRingsEx &a, const HungarianRingsEx &b) {
				return a.estimateVal < b.estimateVal || (a.estimateVal == b.estimateVal && a.times > b.times);
			}
			HungarianRingsEx clone(ROTATE dir) {
				HungarianRingsEx c(HungarianRings::clone(dir));
				c.times = this->times + 1;
				return c;
			}
		};
		std::priority_queue<HungarianRingsEx,vector<HungarianRingsEx>, HungarianRingsEx>qe;
		std::unordered_map<uint64_t, pair<uint64_t, HungarianRings::ROTATE> >parent;
		std::function<void(uint64_t)>search = [&parent, &search](uint64_t child) {
			auto it = parent.find(child);
			if (it == parent.end())
				return;
			search(it->second.first);
			cout << it->second.second << " ";
		};
		HungarianRingsEx r(root);
		qe.push(r);
		parent.insert(make_pair(root.compress(), make_pair(0, HungarianRings::ROTATE::NONE)));
		while (qe.size()) {
			auto c = qe.top();
			auto val = c.compress();
			qe.pop();
			if (c.finish()) {
				cout << "finish " << endl;
				search(val);
				break;
			}
			for (int dir = HungarianRings::ROTATE::L; dir < HungarianRings::ROTATE::NONE; ++dir) {
				auto ex = c.clone(HungarianRings::ROTATE(dir));
				auto temp = ex.compress();
				if (parent.find(temp) == parent.end()) {
					qe.emplace(ex);
					parent[temp] = make_pair(val, HungarianRings::ROTATE(dir));
				}
			}
		}
	}
public:
	void solve(char left[], char right[]) {
		HungarianRings root(left, right);
		if(root.valid())
			A_star(root);
	}
};
int main() {
	Solution c;
	const int B = HungarianRings::COLOR::BLUE,
		Y = HungarianRings::COLOR::YELLOW,
		G = HungarianRings::COLOR::GREEN,
		P = HungarianRings::COLOR::PINK;
	char left[15] = { Y,Y,G,G,G,G,P,G,B,Y,P,B,P,B,P };
	char right[15] = { P,Y,B,B,Y,P,Y,B,Y,G,P,P,G,B,P };
	using namespace std::chrono; 
	steady_clock::time_point t1 = steady_clock::now();
	c.solve(left, right); 
	steady_clock::time_point t2 = steady_clock::now();
	cout << "\nit takes " << duration_cast<duration<double>>(t2 - t1).count() << " second" << endl;
	int a;
	cin >> a;
}