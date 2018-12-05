// Jing Jiang
// ECE 365 DSA2 Assignment 4 Merge

#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstdlib>
#include <cstring>

using namespace std;

bool dp[1001][1001];

int main(){
	int i,j;
	string input, output, A, B, C;
	ifstream inFile;
	ofstream outFile;
	cout << "Enter name of input file: ";
	cin >> input;
	cout << "Enter name of output file: ";
	cin >> output;
	inFile.open (input);
	outFile.open (output);

	if (inFile.is_open () && outFile.is_open ()) {
		while (inFile >> A && inFile >> B && inFile >> C) {
			if ((A.size() + B.size()) != C.size() || (C[C.size()-1]!=A[A.size()-1] && C[C.size()-1]!=B[B.size()-1])) {
				outFile << "*** NOT A MERGE ***" << endl;
				continue;
			}

			// dp algorithm
			for (i = 0; i <= B.size (); i++) {
				for (j = 0; j <= A.size (); j++) {
					if (i == 0 && j > 0) {
						if (C[j - 1] == A[j - 1]) {
							dp[i][j] = (j == 1) ? true : dp[i][j - 1];
							continue;
						}
					}
					else if (j == 0 && i > 0) {
						if (C[i - 1] == B[i - 1]) {
							dp[i][j] = (i == 1) ? true : dp[i - 1][j];
							continue;
						}
					}

					dp[i][j] = false;
				}
			}

			for (i = 1; i <= B.size(); i++) {
				for (j = 1; j <= A.size(); j++) {
					dp[i][j] = false;

					if (dp[i - 1][j] || dp[i][j - 1]) {
						if (B[i - 1] == A[j - 1] && C[i + j -1] == A[j - 1]) {
							dp[i][j] = true;
						}
						else if (dp[i - 1][j]) {
							if (C[i + j - 1] == B[i - 1]) {
								dp[i][j] = true;
							}
						}
						else if (dp[i][j - 1]) {
							if (C[i + j - 1] == A[j - 1]) {
								dp[i][j] = true;
							}
						}
					}
				}
			}

			// Show the result and convert A to uppercase 
			if (dp[B.size()][A.size()]==1) {
				i = B.size ();
				j = A.size ();

				while (j > 0 && i >= 0) {
					if (dp[i][j] && (i == 0 || !dp[i - 1][j])) {
						C[i + j - 1] = toupper (C[i + j - 1]);
						j--;
					}
					else
						i--;
				}

				outFile << C << endl;

			}
			else
				outFile << "*** NOT A MERGE ***" << endl;
		}
		
	}
	// Error check, show the error message
	else {
		cerr << "ERROR: Cannot open file! :" << strerror(errno) << endl;
	}

	inFile.close();
	outFile.close();

	return 0;
}
