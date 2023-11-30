#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::istringstream;
using std::ofstream;


struct Test {
    int key;          // key to be searched in array 'nums'
    vector<int> nums; // sorted vector to search a key within
};


// handle user input
vector<Test>& handleUserInput(vector<Test>& tests) {
    // INPUT: First line of input contains number of testcases T.
    /// For each testcase, first line of input contains number of elements in the array
    // and the number K seperated by space. Next line contains N elements.

    uint tests_num{0};
    uint nums_size{0};
    string line1{""};
    string line2{""};

    // get number of testcases to be checked
    cin >> tests_num;
    // cin.ignore() is needed because in cin has left unwanted newline character
    cin.ignore();

    if (tests_num <= 0) {
        cout << "Wrong number of tests to be checked!" << endl;

        return tests;
    }

    // loop for sequential writing the following 2 lines:
    // 1) number_of_elements_in_array key_to_search
    // 2) array_itself
    for(uint i = 0; i < tests_num; i++) {
        Test test;

        getline(cin, line1);
        istringstream iss1(line1);
        iss1 >> nums_size;
        iss1 >> test.key;

        getline(cin, line2);
        istringstream iss2(line2);

        for(uint j = 0; j < nums_size; j++) {
            unsigned int tmp{0};
            iss2 >> tmp;
            test.nums.push_back(tmp);
        }

        tests.push_back(test);
    }

    /* test correctness of handling user input */
    cout << "Test correctness of handling user input:" << endl;
    for(uint i = 0; i <= tests.size()-1; i++) {
        cout << "Test " << i+1 << ":\n\tnumbers: ";

        for(uint k = 0; k < tests[i].nums.size(); k++) {
            cout << tests[i].nums[k] << " ";
        }
        cout << "\n\tkey = " << tests[i].key << ", number of elements = " << tests[i].nums.size() << endl;
    }

    return tests;
}


// I wrote from scratch ITERATIVE binary search function
int binSearchMyIterativeSolution(const vector<int>& v, int key) {
    int low = 0;
    int high = v.size()-1;

    while(low <= high) {
        int mid = low + (high-low)/2;

        if(key == v[mid]) {
            return mid;
        } else if(key < v[mid]) {
            high = mid - 1;
        } else { // key > v[mid]
            low = mid + 1;
        }
    }
    return -1;
}


// I wrote from scratch RECURSIVE binary search function
int binSearchMyRecursiveSolution(const vector<int>& v, int key, int low, int high) {

    if(low <= high) {
        int mid = low + (high-low)/2;
        if(key == v[mid]) {
            return mid;
        } else if(key < v[mid]) {
            return binSearchMyRecursiveSolution(v, key, low, mid-1);
        } else {
            return binSearchMyRecursiveSolution(v, key, mid+1, high);
        }
    } else { // if(low > high)
        return -1;
    }
}

// ordinary ITERATIVE binary search
int binSearch(const vector<unsigned int>& vec, unsigned int key) {
    unsigned int low{0};
    unsigned int high = vec.size() - 1;

    while(low <= high) {
        unsigned int mid = low + (high-low)/2;
        if(key == vec[mid]) {
            return mid;
        }

        if(key < vec[mid]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return -1;
}


// ordinary RECURSIVE binary search
int binSearchRecursive(const vector<int>& v, int key, int low, int high) {
    if(high >= low) {
        int mid = low + (high - low)/2;

        if(key == v[mid]) {
            return mid;
        }

        if(key < v[mid]) {
            return binSearchRecursive(v, key, low, mid-1);
        } else { // if key > v[mid]
            return binSearchRecursive(v, key, mid+1, high);
        }
    }

    // base case for recursion, i.e. we return -1 when high becomes < low
    // and it means that element was not found in the array
    return -1;
}


// A recursive binary search function. It returns
// location of x in given array arr[l..r] is present,
// otherwise -1
int binarySearchFromGeeksForGeeks(const vector<uint> v, uint key, int l, int r) {

    if (r >= l) {
        int mid = l + (r - l) / 2;

        // If the element is present at the middle
        // itself
        if (key == v[mid])
            return mid;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (key < v[mid])
            return binarySearchFromGeeksForGeeks(v, key, l, mid - 1);

        // Else the element can only be present
        // in right subarray
        return binarySearchFromGeeksForGeeks(v, key, mid + 1, r);
    }

    // We reach here when element is not
    // present in array
    return -1;
}


// searches the left most occurence of a key in sorted array ITERATIVELY
int binSearchLeftMost(const vector<uint>& vec, uint key) {
    uint low{0};
    uint high = vec.size()-1;

    while(low <= high) {
        uint mid = low + (high-low)/2;
        if(key == vec[mid] and (mid == 0 or vec[mid - 1] != key)) {
            return mid;
        }

        if(key <= vec[mid]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return -1;
}


// searches the left most occurence of key in sorted array RECURSIVELY
int binSearchLeftMostRecursive(const vector<uint>& vec, uint key, uint low, uint high) {
    if(high < low) return -1; // base case for recursive vesion of binSearch

    uint mid = low + (high-low)/2;
    if(key == vec[mid] and (mid == 0 or vec[mid-1] != key)) {
        return mid;
    }

    if(key <= vec[mid]) {
        binSearchLeftMostRecursive(vec, key, low, mid-1);
    } else {
        binSearchLeftMostRecursive(vec, key, mid+1, high);
    }
}


int binSearchLeftMostMySolution(const vector<uint>& vec, uint key) {
    uint low{0};
    int high = vec.size() - 1;

    while(low <= high) {
        uint mid = low + (high-low)/2;

        if(key == vec[mid]) {
            if(mid == 0 or vec[mid-1] != key) {
                return mid;
            }
            high = mid - 1;
        } else if (key < vec[mid]){
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return -1;
}


// function to do binSearch and to be used for finding number of occurrence in array
int searchLeftMost(const vector<uint>& vec, uint key, uint low, uint high) {
    uint lower_bound{low};

    while(low <= high) {
        uint mid = low + (high-low)/2;

        if( key == vec[mid] and (mid == lower_bound or vec[mid - 1] != key) ) {
            return mid;
        }

        if(key <= vec[mid]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
}


// function to do binSearch and to be used for finding number of occurrence in array
int searchRightMost(const vector<uint>& vec, uint key, uint low, uint high) {
    uint upper_bound{high};

    while(low <= high) {
        uint mid = low + (high-low)/2;

        if( key == vec[mid] and (mid == upper_bound or vec[mid + 1] != key) ) {
            return mid;
        }

        if(key >= vec[mid]) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
}


// count the number of occurrences of an element(key) in array
int countOccurrences(const vector<uint>& vec, uint key) {
    uint low{0};
    uint high = vec.size()-1;
    int left_idx{-1};
    int right_idx{-1};

    while(low <= high) {
        uint mid = low + (high-low)/2;
        if(key == vec[mid]) {

            // check for corner case in LEFT half
            if(mid == 0 or vec[mid-1] != key) {
                left_idx = mid;
            } else {
                left_idx = searchLeftMost(vec, key, low, mid-1);
            }

            // check for corner case in RIGHT half
            if(mid == vec.size()-1 or vec[mid+1] != key) {
                right_idx = mid;
            } else {
                right_idx = searchRightMost(vec, key, mid+1, high);
            }

            cout << "left_idx = " << left_idx << "\t" << ", right_idx ="
                 << right_idx << endl;

            return right_idx - left_idx + 1;
        }

        if(key <= vec[mid]) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return -1;
}


// applying binSearch when searching in huge array
int searchInInfArray(const vector<uint>& v, uint key) {
    bool isEnough = true; // flag to indicate whether the array is too high to allow next multiplying by 2
    int idx{-1};

    if(v[0] == key) {
        return 0;
    }

    // begin from index = 1
    int i = 1;

    while(key > v[i]) {
        if( i*2 < (v.size()-1) ) {
            i *= 2;
        } else {
            isEnough = false;
            break;
        }
    }

    if(v[i] == key) {
        return i;
    }

    if(isEnough) {
        cout << "low = " << i/2 << ", high = " << i <<  ", v.size() = " << v.size() << endl;
        idx = binSearchLeftMostRecursive(v, key, i/2, i);
        cout << "key = " << key << " was found at index: " << idx << endl;
    } else {
        idx = binSearchLeftMostRecursive(v, key, i, v.size()-1);
        cout << "low = " << i << ", high = " << v.size() - 1 <<  ", v.size() = " << v.size() << endl;
    }

    return idx;
}


// generate huge array and write it to the file
int generateHugeSortedVector(uint size, string fileName) {
    cout << "Creating file and generating sorted sequence of numbers ..." << endl;
    vector<uint> v;

    // open a file in write mode.
    ofstream ost(fileName);
    if(ost.is_open()) {
        for(auto i = 0; i < size; i++) {
            v.push_back(rand() % 1000000);
        }

        sort(v.begin(), v.end());

        for(auto x: v) {
            ost << x << " ";
        }
        ost << endl;
    } else {
        cout << "Couldn't open file: " << fileName << endl;
        return -1;
    }

    if(ost) {
        ost.close();
    }

    return 0;
}


// generate huge array and write it to the vector and to the file
vector<uint>& generateHugeSortedVector(vector<uint>& v, uint size, string fileName) {

    for(auto i = 0; i < size; i++) {
        v.push_back(rand() % 100);
    }

    sort(v.begin(), v.end());

    // print generated array to the file
    ofstream ost(fileName);
    if(ost.is_open()) {
        cout << "printing vector elements into the file: " << fileName << " ..." << endl;
        for(auto i = 0; i < v.size(); i++) {
            ost << v[i] << "(" << i << ") ";
        }
        ost << endl;
    }

    if(ost) {
        ost.close();
    }

    return v;
}


int countOnes(const vector<uint>& v) {
    int idx = binSearchLeftMost(v, 1);
    if( idx != -1) {
        return v.size() - idx; // actually this formula goes from (v.size() - 1) - index + 1
    } else {
        return 0;
    }
}


int findPivotIdxInRotatedArray(const vector<int>& v) {
    int low = 0;
    int high = v.size() - 1;

    // check if the array is rotated
    if(v[high] > v[low] or low == high) {
        cout << "\tArray is not rotated or have only 1 element." << endl;
        return -1;
    }

    while(low <= high) {
        int mid = low + (high-low)/2;

        // if pivot element is found
        // once we've found index of pivot element, we know that there are 2 sorted subarrays
        // and we can apply binary search to these parts of original array
        if(mid < high and v[mid] > v[mid+1]) {
            cout << "\t\tPivot element = " << v[mid] << " is found at index: " << mid << endl;
            return mid;
        }

        // if the middle element is smaller than the first element in subarray
        // then it means that sort property is broken in the left subarray
        if(v[mid] < v[low]) {
            high = mid;
            continue;
        }

        // if the middle element is greater than the last element in current subarray
        // then it means that sort property is broken in the right part of subarray
        if(v[mid] > v[high]) {
            low = mid;
            continue;
        }

        cout << "\tBEFORE RETURN FROM FUNCTION: Array is not rotated or it has only one element." << endl;
        return -1;
    }

    return -1;
}


/* Recursive function to get pivot. For array 3, 4, 5, 6, 1, 2
   it returns 3 (index of 6) */
int findPivotIdxInRotatedArrayRecursive(const vector<int>& v, int low, int high) {

    // base case: it means that pivot element was not found
    if (high < low or high == low) {
        return -1;
    }

    int mid = low + (high - low)/2;

    // if pivot element has index mid
    if (mid < high and v[mid] > v[mid + 1]) {
        return mid;
    }

    // if pivot element right before middle most
    if (mid > low and v[mid] < v[mid - 1]) {
        return (mid-1);
    }

    if (v[low] >= v[mid])
        return findPivotIdxInRotatedArrayRecursive(v, low, mid-1);

    return findPivotIdxInRotatedArrayRecursive(v, mid + 1, high);
}


// A binary search based function that returns index of a peak element
int findPeakElement(const vector<int>& v) {
    static int counter = 0;

    int n = v.size();
    int low = 0;
    int high = n-1;

    while(low <= high) {
        counter++;
        // calculate middle element
        // int m = (low + high + 1)/2;
        int m = low + (high - low)/2;


        // Compare middle element with its neighbours (if neighbours exist)
        if( (m == low or v[m-1] < v[m]) and (m == high or v[m+1] < v[m]) ) {
            cout << "While loop was executed " << counter << " times." << endl;
            return m;
        }

        // If middle element is not peak and its left neighbour is greater
        // than it, then LEFT half must contain a peak element
        if( m > low and v[m-1] > v[m] ) {
            high = m-1;
            continue;
        }

        // If middle element is not peak and its right neighbour is greater
        // than it, then RIGHT half must have a peak element
        if( m < high and v[m+1] > v[m] ) {
            low = m+1;
            continue;
        }
    }
    cout << "While loop was executed " << counter << " times." << endl;
    cout << "Peak element wasn't found." << endl;
    return -1;
}


int findPeakElementFromLeetCode(const vector<int>& v) {
    static int counter = 0;
    int n = v.size();
    int low = 0;
    int high = n - 1;

    while(low < high) {
        counter++;

        int mid = low + (high - low + 1)/2;

        if(v[mid-1] > v[mid]) {
            high = mid - 1;
        }
        else {
            low = mid;
        }
    }

    // low = high
    cout << "While loop was executed " << counter << " times." << endl;
    return high;
}


int findPeakElementRecursive(const vector<int>& v, int low, int high) {
    cout << "findPeakElementRecursive() called" << endl;
    int mid = low + (high - low)/2;

    if( (mid == 0 or v[mid] > v[mid-1]) and (mid == high or v[mid] > v[mid+1]) ) {
        return mid;
    }

    if(mid > low and v[mid-1] > v[mid]) {
        return findPeakElementRecursive(v, low, mid-1);
    }

    if(mid < high and v[mid+1] > v[mid]) {
        return findPeakElementRecursive(v, mid+1, high);
    }
}



// Function to find square root of x using binary search algorithm
// If x is not a perfect square, return floor of the square root
int floorSqrt(int x) {
    // base case
    if(x < 2) {
        return x;
    }

    int L = 1;
    int R = x;

    // to store the floor of sqrt(x)
    int res = x;

    while(L <= R) {
        // find middle element
        int m = L + (R-L)/2;

        // return mid if x is a perfect square
        if(m*m == x) {
            return m;
        }

        if(m*m > x) {
            // discard right search space
            R = m - 1;
            res = R;
            continue;
        }

        if(m*m < x) {
            // res = m;
            L = m + 1;
        }
    }

    return res;
}


int main() {
    // driver code

    // vector<Test> tests;
    // handleUserInput(tests);

    // Task 1: simple binary search
    // Description: Given a sorted array arr[] of N integers and a number K (key) is given.
    // The task is to check if the element key is present in the array or not.
    // If it is present in the array, return it's index, otherwise return -1.

    // for(auto i: tests) {
    //   cout << "\t\tIndex of searched key = " << binSearch(i.nums, i.key) << endl;
    // }

    // for(auto i: tests) {
    //   // cout << "\t\tbinSearchMyIterativeSolution() is working ..." << endl;
    //   // cout << "\t\tIndex of searched key = " << binSearchMyIterativeSolution(i.nums, i.key)<< endl;

    //   cout << "\t\tbinSearchMyRecursiveSolution() is working ..." << endl;
    //   cout << "\t\tIndex of searched key = " << binSearchMyRecursiveSolution(i.nums, i.key, 0, i.nums.size()-1)<< endl;
    // }

    // Test recursive implementation of binary search:
    // for(auto i: tests) {
    //   cout << "\t\tIndex of searched key = " << binSearchRecursive(i.nums, i.key, 0, i.nums.size()-1) << endl;
    // }


    // Task 2: find leftmost element ()
    // Given a sorted array WITH REPETITIONS, find the leftmost element (specified by key) in it.
    // for(auto i: tests) {
    //   cout << "\t\tLeftMostIndex of searched key = " << binSearchLeftMost(i.nums, i.key) << endl;
    // }

    // Task 3: count occurrences of an element
    // for(auto i: tests) {
    //   cout << "\t\tThe number of occurrences of searched key = " << countOccurrences(i.nums, i.key) << endl;
    // }

    // // Task 4: count 1 in a sorted binary array, for example in: 0 0 0 0 0 1 1
    // for(auto i: tests) {
    //   cout << "\t\tThe number of 1 in binary array = " << countOnes(i.nums) << endl;
    // }

    // Task 5: apply binary search to infinite sized (huge)array
    // ********************************************************************************
    // for(auto i: tests) {
    //   int idx = searchInInfArray(i.nums, i.key);
    //   cout << "\t\tThe index of key in INFINITE  array = " << idx  << endl;
    // }

    // Task 6: Given sorted and rotated array of distinct elements and an element key.
    //         Find if key is present in the array or not
    // ********************************************************************************
    // for(auto i: tests) {
    //   // test my own implementation of finding pivot element:
    //   int pivotIdx = findPivotIdxInRotatedArray(i.nums);

    //   // test RECURSIVE implementation of finding pivot element:
    //   // int pivotIdx = findPivotIdxInRotatedArrayRecursive(i.nums, 0, i.nums.size()-1);

    //   if(pivotIdx == -1) {
    //     // cout << "\t\tPivot element was not found." << endl;
    //     cout << "\t\tIndex of searched key = " << binSearchRecursive(i.nums, i.key, 0, i.nums.size()-1) << endl;
    //     continue;
    //   }

    //   if(i.key == i.nums[pivotIdx]) {
    //     cout << "\t\tIndex of searched key = " << pivotIdx << endl;
    //   } else if(i.key >= i.nums[0] and i.key < i.nums[pivotIdx]) {
    //     cout << "\t\tLeft half of initial array was used." << endl;
    //     cout << "\t\tIndex of searched key = " << binSearchRecursive(i.nums, i.key, 0, pivotIdx-1) << endl;
    //   } else {
    //     cout << "\t\tRight half of initial array was used." << endl;
    //     cout << "\t\tIndex of searched key = " << binSearchRecursive(i.nums, i.key, pivotIdx+1, i.nums.size()-1) << endl;
    //   }
    // }

    // Task 7: Given UNsorted array. The task is to find ANY PEAK ELEMENT.
    //         An element is called a PEAK element when it is not smaller than its neighbours
    // ********************************************************************************

    // for(auto i: tests) {
    //   cout << "\t\tFinding peak element in the following numbers: ";
    //   for(auto elm: i.nums){
    //     cout << elm  << " ";
    //   }
    //   cout << endl;
    //   // cout << "\t\t index of peak element is: " << findPeakElement(i.nums) << endl;
    //   // cout << "\t\t index of peak element is: " << findPeakElementRecursive(i.nums, 0, i.nums.size()-1) << endl;
    //   int peak_nums = findPeakElementFromLeetCode(i.nums);
    //   // int peak_nums = findPeakElement(i.nums);
    //   cout << "\t\t index of peak element is: " << peak_nums << endl;
    // }

    cout << "Type the number for which there is needed to compute floor(sqrt(x)): ";
    int x;
    cin >> x;

    int floor = floorSqrt(x);
    cout << "floor of sqrt(" << x << ") = " << floor << endl;
}
