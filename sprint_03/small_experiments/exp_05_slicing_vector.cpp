// C++ program for the above approach
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Template class to slice a vector
// from range X to Y
template <typename T>
vector<T> slicing(vector<T> const& v,
				int X, int Y)
{

	// Begin and End iterator
	auto first = v.begin() + X;
	auto last = v.begin() + Y + 1;

	// Copy the element
	vector<T> vector(first, last);

	// Return the results
	return vector;
}

// Template class to print the element
// in vector v
template <typename T>
void printResult(vector<T> const& v)
{

	// Traverse the vector v
	for (auto i : v) {
		cout << i << ' ';
	}
	cout << '\n';
}

// Driver Code
int main()
{

	// Given vector
	vector<int> arr = { 1, 3, 4, 2,
						4, 2, 1 };

	// Given range
	int X = 2, Y = 5;

	// To store the sliced vector
	vector<int> ans;

	// Function Call
	ans = slicing(arr, X, Y);

	// Print the sliced vector
	printResult(ans);
}
