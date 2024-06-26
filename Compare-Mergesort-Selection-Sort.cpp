/*

S1201-Recursive-Sortmerge-App.cpp

The Sort-Merge algorithm is a prime example of the divide-and-conquer paradigm.
The algorithm consists of three steps:

Divide:
- Divide the array into smaller chunks recursively until each chunk has only one element
  (or a 'very small' size, say k).
- This is the base case.
 -For simplicity, let's assume we divide the array into halves.
Optional Step:
- Sort each chunk of size == k, 1 < k < m, use any sorting algorithm (s.a. insertion sort).
  This idea is not used in this solution (we stop splitting when the chunk size is 1).    
Merge:
- Combine the sorted chunks back together into a single sorted array.

The key advantage of the Sort-Merge algorithm is its efficiency
in sorting large datasets that cannot fit into memory all at once.

Dividing the data into smaller chunks and sorting them individually
reduces the memory requirement. 

Additionally, the merge step is efficient since it only requires 
comparing and merging already sorted lists.
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;
//Prototypes -------------------------------------------------------------------------
void mergeSort(vector<int>& v);
void mergeSort(vector<int>& v, vector<int>& aux, int lo, int hi);
void myMerge(vector<int>& v, vector<int>& aux, int lo, int mid, int hi);

void mergeSortNonRecursive(vector<int>& v);
void mergeSortNonRecursive(vector<int>& v, vector<int>& aux);

void selectionSort(vector<int>& v);
void recursiveSelectionSort(vector<int>& v);
void recursiveSelectionSort(vector<int>& v, int first, int last);

void showVector(vector<int> v, string caption = "");
void experiment01();
void experiment02();
void experiment03();


//Prototypes -------------------------------------------------------------------------
int main()
{

    //experiment01();   //Test ideas needed to construct the app
    experiment02();  //Benchmark mergesort and selection sort
    experiment03();  //Test the non-recursive mergesort algorithm

    cout << "\nAll done!\n";
}

void experiment03()
{
    //This array defines the size of the experimental unsorted samples
    vector<int> vSampleSize{ 10, 100, 1000, 10000, 100000, 1000000 };
    //vector<int> vSampleSize{ 10, };

    //Benchmark performance of mergesort and selection-sort on each sample
    for (auto N : vSampleSize) {
        //declare and populate the vector holding random data items
        vector<int> v(N);
        for (int i = 0; i < N; i++) {
            v[i] = rand() % 1000;
        }

        //Run the clock - Measure microseconds needed to sort the current sample
        auto start = high_resolution_clock::now();
        mergeSortNonRecursive(v);
        //selectionSort(v);
        //recursiveSelectionSort(v);            //will fail after 3k calls - stack overflow!
        auto stop = high_resolution_clock::now();

        //Determine duration of the sorting function working on the given sample
        auto duration = duration_cast<microseconds>(stop - start);

        //Show computational time required by the sample
        cout << "Vector size: " << setw(12) << left << N << "Elapsed Time: "
            << duration.count() << " microseconds" << endl;
    }

}


// ==============================================================================================
void experiment02()
{
    //This array defines the size of the experimental unsorted samples
    //vector<int> vSampleSize{ 10, };
    vector<int> vSampleSize{ 10, 100, 1000, 10000, 100000, 1000000 };

    //Benchmark performance of mergesort and selection-sort on each sample
    for (auto N : vSampleSize) {
        //declare and populate the vector holding random data items
        vector<int> v(N); 
        for (int i = 0; i < N; i++) {
            v[i] = rand() % 1000;
        }

        //Run the clock - Measure microseconds needed to sort the current sample
        auto start = high_resolution_clock::now();
        mergeSort(v);
        //selectionSort(v);
        //recursiveSelectionSort(v);            //will fail after 3k calls - stack overflow!
        auto stop = high_resolution_clock::now();

        //Determine duration of the sorting function working on the given sample
        auto duration = duration_cast<microseconds>(stop - start);

        //Show computational time required by the sample
        cout << "Vector size: " << setw(12) << left << N << "Elapsed Time: "
            << duration.count() << " microseconds" << endl;
        //showVector(v, "Sorted vector (mergesort): ");   //debugging
    }


}


//--------------------------------------------------------------------------------------------
void experiment01()
{
    using namespace std::chrono;
    // PRACTICE & EXPERIENCE TIP -------------------------------------------------------------
    // CAUTION - More than 3000 calls will result on "stack overflow" (too bad!). 
    //           For instance, try 4000 items! (it will not work!)
    //           The recursive version of selection sort is not recommended for large datasets.
    //           The iterative version is the best choice for large vectors.
    // -----------------------------------------------------------------------------------------

    int MAXSIZE = 3000;
    vector<int>v(MAXSIZE);
    for (int i = 0; i < MAXSIZE; i++) v[i] =(rand() % 1000);
    //showVector(v, "Original vector: ");
    auto start = high_resolution_clock::now();
    recursiveSelectionSort(v);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Test - Duration for MAXSIZE: " << MAXSIZE
        << " is " << duration.count() << " microsec.\n";

    //showVector(v, "Sorted vector: ");

    //Verify that the vector is indeed sorted!
    for (int i = 0; i < MAXSIZE-1; i++) {
        if (v[i] > v[i + 1]) cout << "OH NOOO! - Invalid order " << v[i] << endl;
    }
    cout << "\nAll done!\n";
}

//-------------------------------------------------------------------------------------
//SELECTION SORT
//Non-recursive implementation. Swap the smallest element found in the unsorted region 
//of the array with the first item of the unsorted portion. 
//-------------------------------------------------------------------------------------
void selectionSort(vector<int>& v)
{
    int n = v.size();  // number of elements to be sorted
    for (int first = 0; first < n - 1; first++) {
        //location of the smallest element
        int minIndex = first;
        //explore the vector looking for its smallest value
        for (int j = first + 1; j < n; j++) {
            if (v[j] < v[minIndex]) {
                minIndex = j; //correction. we found a smaller element
            }
        }
        //swap first and smallest elements
        if (minIndex != first) {
            swap(v[first], v[minIndex]);
        }
    }
}

//--------------------------------------------------------------------------------------
//SELECTION SORT.  Recursive version
//--------------------------------------------------------------------------------------
void recursiveSelectionSort(vector<int>& v) {
    //Helper function
    int first = 0; int last = v.size() - 1;
    recursiveSelectionSort(v, first, last);
}

// -------------------------------------------------------------------------------------
// The v region between first and last is unsorted
// Find the largest element in the unsorted region and move it to the last position
// Reduce the size of the unsorted region by one and repeat
// -------------------------------------------------------------------------------------
void recursiveSelectionSort(vector<int>& v, int first, int last)
{
    //Base case
    if (first >= last) return;

    //General recursion. Assume the first is the largest
    int maxPos = first;

    //Explore the unsorted portion to find the largest element
    for (int i = first + 1; i <= last; i++) {
        if (v[i] > v[maxPos]) maxPos = i;
    }

    //Exchange last and maximum element
    swap(v[maxPos], v[last]);

    //Reduce the size of the unsorted region. Repeat
    recursiveSelectionSort(v, first, last - 1);
}






//-------------------------------------------------------------------------------------
//MERGE SORT
//Split the workspace. Recursively sort each half. Merge the sorted segments
//-------------------------------------------------------------------------------------
void mergeSort(vector<int>& v, vector<int>& aux, int lo, int hi)
{
    // Base case testing
    if (hi <= lo) return;           //if the array has only one element, return

    // General recursion
    int mid = (lo + hi) / 2;        //mid point of the array

    //cout << "(" << lo << ", " << mid << "),  (" << mid + 1 << ", " << hi << ")" << endl; //debugging
    mergeSort(v, aux, lo, mid);         //sort the first half
    mergeSort(v, aux, mid + 1, hi);     //sort the second half
    //cout << "Merging (" << lo << ", " << mid << "),  (" << mid + 1 << ", " << hi << ")" << endl; //debugging
    myMerge(v, aux, lo, mid, hi);        //merge the two halves

}

//----------------------------------------------------------
void mergeSort(vector<int>& v)
{
    // Helper function.
    // Create a temporary vector called 'aux' to copy the original vector v.
    // (It will be used during the merge step.)
    vector<int> aux(v.size());

    mergeSort(v, aux, 0, v.size() - 1);
}


//-----------------------------------------------------------
void showVector(vector<int> v, string caption)
{
    cout << endl << caption << " ";
    for (auto element : v) { cout << element << ", "; }
    cout << endl;
}


//------------------------------------------------------------
void myMerge(vector<int>& v, vector<int>& aux, int lo, int mid, int hi)
{
    //cout << "Merge  [" << lo << ", " << mid << "]   [" << mid + 1 << ", " << hi << "]" << endl;  //debugging
    
    // copy segment v(lo,hi) into aux
    for (int k = lo; k <= hi; k++) aux[k] = v[k];

    // mixes array (lo,mid) (mid+1, hi)
    int i = lo;                             // index for the first half
    int j = mid + 1;                        // index for the second half

    for (int k = lo; k <= hi; k++) {
        if (i > mid) v[k] = aux[j++];       // if the first half is exhausted
        // copy the second half into v
        else if (j > hi) v[k] = aux[i++];   // if the second half is exhausted
        // copy the first half into v
        else if (aux[i] < aux[j])           // if the first half leader is smaller
            v[k] = aux[i++];                // copy the first half leader into v
        else                                // otherwise
            v[k] = aux[j++];                // copy the second half leader into v

    }
}

//-------------------------------------------------------------------------------------------

void mergeSortNonRecursive(vector<int>& v)
{
    // Helper function
    // Create a temporary vector called 'aux' to copy the original vector v.
    // (It will be used during the merge step.)
    vector<int> aux(v.size());
    mergeSortNonRecursive(v, aux);
}


//-------------------------------------------------------------------------------------------
void mergeSortNonRecursive(vector<int>& v, vector<int>& aux)
{
    //Bottom-up mergesort, combine groups of size: 1, 2, 4, 8, 16, ...
    int N = v.size();
    for (int sz = 1; sz < N; sz += sz) //sizes 1, 2, 4, 8, 16, ...
    {
        //cout << "sz: " << sz << endl;  //debugging  
        for (int lo = 0; lo < N - sz; lo += 2*sz) {
            long mid = lo + sz - 1;
            long hi = min(lo + 2*sz - 1, N - 1);
            myMerge(v, aux, lo, mid, hi);
        }
    }
    //showVector(v, "Sorted vector: ");   //debugging


}