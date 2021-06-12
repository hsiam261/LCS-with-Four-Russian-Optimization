#include <bits/stdc++.h>
#include <random>
#include <vector>
using namespace std;

seed_seq seq {
	(uint64_t) chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count(),
	(uint64_t) __builtin_ia32_rdtsc(),
	(uint64_t) (uintptr_t) make_unique<char>().get()
};

mt19937 rng(seq);
uniform_int_distribution<> character_distribution(0,3);
uniform_int_distribution<> length_distribution(100,1000);


vector<char> alphabet = {'A','C','G','T'};




string gen_random_string() {
	int len = length_distribution(rng);
	string s(len,'A');

	for(int i=0;i<len;i++) {
		int k = character_distribution(rng);
		//cout << k << endl;
		s[i] = alphabet[k];
	}

	return s;
}


int main() {
	cout << gen_random_string() << "\n";
	cout << gen_random_string() << endl;
	return 0;
}

