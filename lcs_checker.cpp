#include <bits/stdc++.h>
#include <cstdlib>
#include <fstream>
#include <ios>
#include <iostream>
using namespace std;

const int MAXN = 10001;

int dp[MAXN][MAXN];

int lcs_size(const string& s0,const string& s1) {
	for(int i=0;i<=s0.size();i++) dp[i][0]=0;
	for(int i=0;i<=s1.size();i++) dp[0][i]=0;

	for(int i=1;i<=s0.size();i++) {
		for(int j=1;j<=s1.size();j++) {
			if(s0[i-1]==s1[j-1]) dp[i][j] = dp[i-1][j-1]+1;
			else dp[i][j] = max(dp[i-1][j],dp[i][j-1]);
		}
	}

	return dp[s0.size()][s1.size()];
}

bool is_subsequece(const string& s, const string& key) {
	int i=0;
	for(int j=0;j<key.size();j++) {
		while(i<s.size() && s[i]!=key[j]) {
			i++;
		}
		if(i==s.size()) return false;
		i++;
	}

	return true;
}


int main(int argc,char** argv) {
	fstream input_file;
	fstream solve_file;
	
	if(argc==3) {
		input_file.open(argv[1],ios::in);
		if(input_file.fail()) {
			cerr << "failed to open input file " << argv[1] << "endl";
			exit(-1);
		}
		
		solve_file.open(argv[2],ios::in);
		
		if(solve_file.fail()) {
			cerr << "failed to open solution file " << argv[2] << "endl";
			exit(-1);
		}

	} else {
		cerr << "Usage : lcs_checker input_file solution_file" << endl;
		exit(-1);
	}

	string s0,s1;
	input_file >> s0 >> s1;
	input_file.close();


	int n = lcs_size(s0,s1);

	int k;
	solve_file >> k;
	if(n!=k) {
		cerr << "size of lcs mismatch : " << n << " vs " << k << endl;
		exit(1);
	}
	
	string key;
	solve_file >> key;
	solve_file.close();
	
	if(key.size()!=n) {
		cerr << "size of lcs mismatch : " << n << " vs " << k << endl;
		exit(1);
	}


	if(!is_subsequece(s0,key)) {
		cerr << key << " not a subsequence of " << s0 << endl;
		exit(1);
	}

	if(!is_subsequece(s1,key)) {
		cerr << key << " not a subsequence of " << s1 << endl;
		exit(1);
	}

	return 0;

}

