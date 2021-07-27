/**
 *
 * MIT License
 *
 * Copyright c)( [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files the (Software""), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.))
 * 
 * author: Siam Habib <hsiam261@gmail.com>
 *
 *
 * Four Russian Optimization to find the LCS of two strings
 * usage: ./four-russian-optimization.out 
 * enter the two strings in two lines (string can not have any white spaces between them)
 *
 */ 


#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// block_size of the four russian algorithm
// precompute memory requirement = 
// (|ALPHABET|+1)^(2*block_size)*4^block_size*size_of(pre_compute_dp_block)
// for block_size=4 and alphabet size = 4 (one byte characters) and using pair of 
// integers as index rep size
// 9.6 GiB
const int block_size = 3;


// change here to add more symbols
// two different sentinals are necessary for 
// making the sizes of the two strings divisible by block_size
namespace Alphabet {
	vector<char> symbol = {'A','C','G','T'};
	vector<char> sentinals = {'0','1'};
}




//precompute tasks are here
namespace Precompute {
	//maps all strings of length block_size using alphabet 
	//and appropriate centinals to integers
	unordered_map<string,int> block_strings0;
	unordered_map<string,int> block_strings1;
	
	//global variable used for block_string generation
	string str(block_size,Alphabet::sentinals[0]);
	

	//generates block_strings using recursion and backtracking
	void recurse(int at,int &cnt,unordered_map<string,int>  &block_strings) {
		block_strings[str]=cnt;
		cnt++;

		if(at==block_size) return;
		auto temp = str[at];
		for(auto c:Alphabet::symbol) {
			str[at]=c;
			recurse(at+1,cnt,block_strings);
		}
	
		str[at] = temp;
	}
	

	/**
	 * generates all the strings of length block_size 
	 * with the with the alphabet symbols that can only have
	 * trailing sentinal characters
	 */
	void init_block_strings() {
		for(auto &c: str) c= Alphabet::sentinals[0];
		int cnt=0;
		recurse(0,cnt,block_strings0);
	
		for(auto &c: str) c= Alphabet::sentinals[1];
		cnt=0;
		recurse(0,cnt,block_strings1);
	}


	// store indecies using bitwise will speed this up 
	// Most optimal size of INDEX_REP_SIZE = floor(lg(block_size*2+1))+1
	/**
	 * datastructure to store the dp values of a block
	 * worst case Memory Complexity: 
	 * 2*block_size*(WORD_SIZE+2*INDEX_REP_SIZE) + 2*WORD_SIZE
	 */ 
	struct pre_compute_dp_block{	
		/**
		 * strores the length of the lcs of the bottom 
		 * and right boundary cells
		 */ 
		vector<int> bottom;
		vector<int> right;
		
		/**
		 * stores lcs of the bottom and right boundary
		 * cells as string
		 */ 
		vector<string> bottom_lcs;
		vector<string> right_lcs;
		
		/**
		 * stores the (row,column) in the dp table that
		 * the path to the cell starts from
		 */ 
		vector<pair<int,int>> bottom_src;
		vector<pair<int,int>> right_src;
		
		pre_compute_dp_block() : bottom(block_size,0), right(block_size,0), 
			bottom_lcs(block_size), right_lcs(block_size),
			bottom_src(block_size,{-1,-1}), right_src(block_size,{-1,-1}) {}
	};

	/**
	 *   dp table 
	 *
	 *   0 _ _ _ _  -----> mask generated row 
	 *   _ x x x x \
	 *   _ x x x x |
	 *   _ x x x x |------> x marked squares[i][j] has the dp value of matching the prefix s0[:i]
	 *   _ x x x x /        and s1[:j] for given top and and left values, right index non-inclusive 
	 *   |
	 *   |
	 *   |
	 *   mask generated column 
	 */
	pre_compute_dp_block solve_block_dp(int row_mask, int column_mask,string s0,string s1) {
		int dp[block_size+1][block_size+1];
		char path[block_size+1][block_size+1];

		dp[0][0] = 0;
		path[0][0] = 0;

		//init_top_row
		for(int i=1;i<=block_size;i++) {
			dp[0][i]=dp[0][i-1]+(((1<<(i-1))&row_mask)>>(i-1));
			path[0][i]=0;
		}

		//init_left_column
		for(int i=1;i<=block_size;i++) {
			dp[i][0]=dp[i-1][0]+(((1<<(i-1))&column_mask)>>(i-1));
			path[i][0]=0;
		}

		for(int i=1;i<=block_size;i++) {
			for(int j=1;j<=block_size;j++) {
				if(s0[i-1]==s1[j-1]) {
					path[i][j]='D';
					dp[i][j] = dp[i-1][j-1]+1;
				} else if(dp[i-1][j]>dp[i][j-1]) {
					dp[i][j] = dp[i-1][j];
					path[i][j] = 'U';
				} else {
					dp[i][j] = dp[i][j-1];
					path[i][j] = 'L'; 
				}

			}
		}


		pre_compute_dp_block blk;
		for(int i=0;i<block_size;i++) blk.bottom[i] = dp[block_size][i+1];
		for(int i=0;i<block_size;i++) blk.right[i] = dp[i+1][block_size];
		
		//getting the path for the bottom row
		for(int i=0;i<block_size;i++) {
			int row = block_size;
			int col = i+1;
			while(path[row][col]!=0) {
				if(path[row][col]=='D') {
					row--;
					col--;
					blk.bottom_lcs[i]+=s0[row];
				} else if (path[row][col]=='U') row--;
				else col--; 
			}
			std::reverse(blk.bottom_lcs[i].begin(), blk.bottom_lcs[i].end());
			blk.bottom_src[i] = {row,col};
		}
		
		//getting the path for the right column
		for(int i=0;i<block_size;i++) {
			int row = i+1;
			int col = block_size;
			while(path[row][col]!=0) {
				if(path[row][col]=='D') {
					row--;
					col--;
					blk.right_lcs[i]+=s0[row];
				} else if (path[row][col]=='U') row--;
				else col--; 
			}
			std::reverse(blk.right_lcs[i].begin(),blk.right_lcs[i].end());
			blk.right_src[i] = {row,col};

		}

		return blk;

	}

	/**
	 * Array storing all the precomputed lcs between block strings while masks 
	 * are in consideration
	 * May consider storing in file or database
	 * For block_size=4 and alphabet.size()=4, required >14GiB of data 
	 */
	vector<vector<pre_compute_dp_block>> pre[1<<block_size][1<<block_size];

	/**
	 * Function that fills up pre
	 */ 
	void precompute() {
		for(auto &t1:pre) {
			for(auto &t2:t1) {
				t2.resize(block_strings0.size(),vector<pre_compute_dp_block>(block_strings1.size()));
			}
		}
		
		for(int row_mask=0;row_mask<(1<<block_size);row_mask++) {
			for(int col_mask=0;col_mask<(1<<block_size);col_mask++) {
				for(auto &t0:block_strings0) {
					for(auto &t1:block_strings1) {
						pre[row_mask][col_mask][t0.second][t1.second] =
							solve_block_dp(row_mask,col_mask,t0.first,t1.first);
					}
				}
			}
		}
		
	}
	

}

/**
 * Takes a string and fills a vetctor with strings of size block_size
 * May add sentinal in the end of the string if string size is not divisible
 * by block_size
 */ 
void make_block_arr(string s,vector<string> &block_arr,char sentinal) {
	if(s.size()%block_size) {
		int new_size = (s.size()+block_size-1)/block_size*block_size;
		s.resize(new_size,sentinal);
	}
	
	for(int i=0;i<s.size();i+=block_size) block_arr.push_back(s.substr(i,block_size));
}

/**
 * Generates row/column mask using the dp values computed in a previous iteration
 */ 
int make_mask(int ref,const vector<int>& v) {
	int mask = 0;
	for(int i=0;i<v.size();i++) {
		if(ref<v[i]) {
			mask|=(1<<i);
			ref=v[i];
		}
	}

	return mask;
}

/**
 * the dp table for the block level dp
 * the size of this table is a bottle neck
 * might want to consider using memory optimized 
 * lcs here 
 */ 
Precompute::pre_compute_dp_block dp[400][400];

/**
 * computes lcs
 */ 
string  lcs(string s0,string s1) {
	vector<string> block_arr0,block_arr1;
	make_block_arr(s0,block_arr0,Alphabet::sentinals[0]);
	make_block_arr(s1,block_arr1,Alphabet::sentinals[1]);

	//Precompute::pre_compute_dp_block dp[block_arr0.size()+1][block_arr1.size()+1];


	for(int i=1;i<=block_arr0.size();i++) {
		for(int j=1;j<=block_arr1.size();j++) {
			int ref = dp[i-1][j-1].bottom[block_size-1];
			int row_mask = make_mask(ref,dp[i-1][j].bottom);
			int col_mask = make_mask(ref,dp[i][j-1].right);

			dp[i][j] = Precompute::pre[row_mask][col_mask][Precompute::block_strings0[block_arr0[i-1]]][Precompute::block_strings1[block_arr1[j-1]]];
			for(auto& x:dp[i][j].bottom) x+=ref;
			for(auto& x:dp[i][j].right) x+=ref;

		}
	}

	int row=block_arr0.size();
	int col=block_arr1.size();
	bool is_row = true;
	int idx = block_size-1;
	
	vector<string> block_lcs;
	while ((is_row?dp[row][col].bottom_src[idx]:dp[row][col].right_src[idx])!=make_pair(-1,-1)) {
		pair<int,int> src;
		if(is_row) {
			block_lcs.push_back(dp[row][col].bottom_lcs[idx]);
			src = dp[row][col].bottom_src[idx];	
		} else {
			block_lcs.push_back(dp[row][col].right_lcs[idx]);
			src = dp[row][col].right_src[idx];
		}

		if(src.first==0 && src.second==0) {
			row--;
			col--;
			is_row=true;
			idx = block_size-1;
		} else if(src.first==0) {
			row--;
			is_row=true;
			idx=src.second-1;
		} else {
			col--;
			is_row=false;
			idx = src.first-1;
		}
	}
	
	string s="";
	for(int i=block_lcs.size()-1;i>=0;i--) {
		s+=block_lcs[i];
	}

	return s;

}

/**
 * reads two strings from stdin
 * writes size of lcs and lcs in two separate lines
 */ 
int main() {
	Precompute::init_block_strings();
	Precompute::precompute();

	string s0,s1;
	cin >> s0 >> s1;
	string l = lcs(s0,s1);
	cout << l.size() << "\n";
	cout << l << endl;

	return 0;
}

