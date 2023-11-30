// This program has not strictly related to coursera course
// This is supplementary program I used to grasp heap sort to implement
// my solution for challenge 3.7. Maximum Salary
// ============================================================================
//  Name        : bh_from_array.cpp
//  Author      : Aleksey Kravchuk
// ============================================================================

#include <iostream>


// Driver Code
int main() {
	// Binary Tree Representation
	// of input array
	//         1
	//      /     \
	//    3        5
	//  /    \    /  \
	// 4      6  13  10
	/// \    / \
	//9  8  15 17
	int arr[] = { 1, 3, 5, 4, 6, 13, 10, 9, 8, 15, 17 };
	/* auto arr = { 1, 3, 5, 4, 6, 13, 10, 9, 8, 15, 17 }; */

	/* auto n = sizeof(arr) / sizeof(arr[0]); */
	auto  n = sizeof(arr) / sizeof(*arr);

	std::cout << sizeof(arr) << std::endl;
	std::cout << "Array holds: " << n << " elements." << std::endl;

	for(auto elm: arr) {
		std::cout << elm << " ";
	}

	std::cout << std::endl;


	/* buildHeap(arr, n); */

	/* printHeap(arr, n); */

	// Final Heap:
	// 17
	//           /      \
	// 15         13
	//       /    \      /  \
	// 9      6    5   10
	//     / \    /  \
	// 4   8  3    1

	return 0;
}
