// This program has not strictly related to coursera course
// This is supplementary program I used to grasp heap sort to implement
// my solution for challenge 3.7. Maximum Salary

// ============================================================================
//  Name        : heapsort.cpp
//  Author      : Aleksey Kravchuk
// ============================================================================

// C++ program for implementation of Heap Sort
#include <iostream>

using namespace std;

// To heapify a subtree rooted with node i which is
// an index in arr[]. The size of heap = n
void maxHeapify(int arr[], int n, int i)
{
	int largest = i; // Initialize largest as root
	int left = 2*i + 1; // left = 2*i + 1
	int right = 2*i + 2; // right = 2*i + 2

	// If left child is larger than root
	if (left < n && arr[left] > arr[largest])
		largest = left;

	// If right child is larger than largest so far
	if (right < n && arr[right] > arr[largest])
		largest = right;

	// If largest is not root
	if (largest != i)
	{
		swap(arr[i], arr[largest]);

		// Recursively maxHeapify the affected sub-tree
		maxHeapify(arr, n, largest);
	}
}


void minHeapify(int arr[], int n, int i)
{
	int smallest = i;    // Initialize smallest as root
	int left = 2*i + 1;  // left = 2*i + 1
	int right = 2*i + 2; // right = 2*i + 2

	// If left child is smaller than root
	if (left < n && arr[left] < arr[smallest])
		smallest = left;

	// If right child is smaller than smallest so far
	if (right < n && arr[right] < arr[smallest])
		smallest = right;

	// If smallest is not root
	if (smallest != i)
	{
		swap(arr[i], arr[smallest]);

		// Recursively minHeapify the affected sub-tree
		minHeapify(arr, n, smallest);
	}
}


// main function to do heap sort
void maxHeapSort(int arr[], int n)
{
	// Build heap (rearrange array)
	for (int i = n/2 - 1; i >= 0; i--)
		maxHeapify(arr, n, i);

	// One by one extract an element from heap's root and put it to the last position of our array
	for (int i = n-1; i >= 0; i--)
	{
		// Move current root to end
		swap(arr[0], arr[i]);

		// call max heapify on the reduced heap
		maxHeapify(arr, i, 0);
	}
}

// main function to do heap sort
void minHeapSort(int arr[], int n)
{
	// Build heap (rearrange array)
	for (int i = n/2 - 1; i >= 0; i--)
		minHeapify(arr, n, i);

	// One by one extract an element from heap's root and put it to the last position of our array
	for (int i = n-1; i >= 0; i--)
	{
		// Move current root to end
		swap(arr[0], arr[i]);

		// call min heapify on the reduced heap
		minHeapify(arr, i, 0);
	}
}


/* A utility function to print array of size n */
void printArray(int arr[], int n)
{
	for (int i=0; i<n; ++i)
		cout << arr[i] << " ";
	cout << "\n";
}

// Driver program
int main()
{
	/* int arr[] = {12, 11, 13, 5, 6, 7}; */
	/* int arr[] = {2, 7, 11, 10, 5}; */

	// according to the https://www.youtube.com/watch?v=51JGP4VVlDc tutorial
	/* int arr[] = {40, 60, 10, 20, 50, 30}; */
	int arr[] = {4, 9, 11, 59, 3};

	int n = sizeof(arr)/sizeof(arr[0]);

	cout << "Initial array is \n";
	printArray(arr, n);

	maxHeapSort(arr, n);
	cout << "Sorted array in ASCENDING order is \n";
	printArray(arr, n);

	minHeapSort(arr, n);
	cout << "Sorted array in DESCENDING order is \n";
	printArray(arr, n);

}
