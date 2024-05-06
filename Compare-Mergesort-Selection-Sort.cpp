/*

S1201-Recursive-Sortmerge-App.cpp

The Sort-Merge algorithm, with its elegant recursive design,
is a prime example of the divide-and-conquer paradigm.
The algorithm consists of three steps:

Divide:
- Divide the array into smaller chunks recursively until each chunk has only one element.
- This is the base case.
 -For simplicity, let's assume we divide the array into halves.
Conquer:
- Sort each chunk using any sorting algorithm (typically merge sort).
Merge:
- Merge the sorted chunks back together into a single sorted array.

The key advantage of the Sort-Merge algorithm is its efficiency
in sorting large datasets that cannot fit into memory all at once.
Dividing the data into smaller chunks and sorting them individually
reduces the memory requirement. Additionally, the merge step is efficient
since it only requires comparing and merging already sorted lists.
*/

#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

void mymerge(vector<int>& v, vector<int>& aux, int lo, int hi);
void showVector(vector<int> v, string caption = "");
void msort(vector<int>& v);
void msort(vector<int>& v, vector<int>& aux, int lo, int hi);
void experiment01();
void experiment02();

void selectionSort(vector<int>& v)
{
    int n = v.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            swap(v[i], v[minIndex]);
        }
    }
}

int main()
{

    //experiment01();
    experiment02();

    cout << "\nAll done!\n";
}
// =========================================================
void experiment02()
{
    vector<int> sampleSizes{ 10, 100, 1000, 10000, 100000, 1000000 };
    for (auto N : sampleSizes) {
        vector<int> v(N); v.reserve(N);
        for (int i = 0; i < N; i++) v[i] = rand() % 1000;
        auto start = high_resolution_clock::now();
        msort(v);
        //selectionSort(v);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Vector size: " << setw(12) << left << N << "Time taken by function: "
            << duration.count() << " microseconds" << endl;
    }


}


//----------------------------------------------------------
void experiment01()
{
    vector<int> v{ 3, 2, 4, 5, 9, 1, 6, 7, 8 };
    showVector(v, "Original vector: ");

    //mymerge(v, aux, 0, v.size() - 1);
    //showVector(v, "Merged vector: ");

    msort(v);
    showVector(v, "Sorted vector: ");
}

void msort(vector<int>& v, vector<int>& aux, int lo, int hi)
{
    // Base case testing
    if (hi <= lo) return;           //if the array has only one element, return

    // General recursion
    int mid = (lo + hi) / 2;        //mid point of the array

    //cout << "(" << lo << ", " << mid << "),  (" << mid + 1 << ", " << hi << ")" << endl; //debugging
    msort(v, aux, lo, mid);         //sort the first half
    msort(v, aux, mid + 1, hi);     //sort the second half
    //cout << "Merging (" << lo << ", " << mid << "),  (" << mid + 1 << ", " << hi << ")" << endl; //debugging
    mymerge(v, aux, lo, hi);        //merge the two halves

}

//----------------------------------------------------------
void msort(vector<int>& v)
{
    // Create a temporary vector to copy the original vector v.
    // It will be used during the merge step.
    vector<int> aux(v.size());

    msort(v, aux, 0, v.size() - 1);
}


//-----------------------------------------------------------
void showVector(vector<int> v, string caption)
{
    cout << endl << caption << " ";
    for (auto element : v) { cout << element << ", "; }
    cout << endl;
}

//------------------------------------------------------------
void mymerge(vector<int>& v, vector<int>& aux, int lo, int hi)
{
    // aux has the same elements as v
    // The first and second half of aux are sorted
    // aux is merged back into v. Example:
    //   v = {2, 4, 5, 9, 1, 6, 7, 8} 
    // aux = {2, 4, 5, 9, 1, 6, 7, 8}
    // first  half of aux = {2, 4, 5, 9} (lo to mid)
    // second half of aux = {1, 6, 7, 8} (mid+1 to hi)
    // ----------------------------------------------------------

    for (int k = lo; k <= hi; k++) aux[k] = v[k]; // copy segment (lo,hi) of v into aux

    int mid = (lo + hi) / 2;                // mid point of the array (lo,mid) (mid+1, hi)
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