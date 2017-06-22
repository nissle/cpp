//
//  main.cpp
//  homework
//
//  Created by Ting Yan on 6/12/17.
//  ID: 010765513
//

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

// array size
const int SIZE = 55000;

// reads the file into the given array
// returns true if the operation succeeds
// return false if the file couldn't be opened or if the array is too small to sore the entire file.
template <class T>
bool loadFile ( const string & filename, T array[], int SIZE)
{
    //open file
    ifstream din;
    din.open(filename.c_str());
    
    // return false if the file is not opened
    if (!din)
    {
        cout << "Unable to read file." << endl;
        return false;
    }
    
    //read file into a array
    for (int i = 0; i < SIZE; i++)
    {
        din >> array[i];
        if (din.eof())
        {
            din.close();
            return true;
        }
    }
    cout << "Array is too small to store the entire file" << endl;
    din.close();
    return false;
    
}



// prints the given array
template <class T>
void printArray (const T array[],const int size)
{
    //use for loop to print the array
    for (int i = 0; i < size ; i ++)
        cout << array[i] << endl;
    cout << endl;
}

//prompt choicelist(sorting methods) for customer to choose and return choice
void sortChoice (int & choice)
{
    // choicelist with 8 different sorting methods
    string choiceList[8] = {"1: Baseline",
        "2: 3-way",
        "3: Median" ,
        "4: Hybrid",
        "5: 3-way + Median" ,
        "6: 3-way + Hybrid" ,
        "7: Median + Hybrid" ,
        "8: 3-way + Median + Hybrid"
    };
    
    //prompt choicelist and store choice
    cout << " Please select which method you want to use:" << endl;
    printArray(choiceList, 8);
    cin >> choice;
    
    //error checking for choice
    while ( !(choice > 0 && choice <= 8))
    {
        cout << "Please enter a number from 1 to 8" << endl;
        cout << "Please select which method you want to use:" << endl;
        printArray(choiceList, 8);
        
        cin >> choice;
    }
    
    cout << "You have chosen #" << choiceList[choice-1] << endl;
    
}

// insertion sort
template <class T>
void insertionSort(T data[], const int low, const int high)
{
    // Insert each element of unsorted list into sorted list
    for (int unsorted = low+1; unsorted <= high; unsorted++)
    {
        // Select unsorted value to be inserted
        T value = data[unsorted];
        int posn  = unsorted;
        
        // Make room for new data value
        while ((posn > 0) && (data[posn - 1] > value))
        {
            data[posn] = data[posn - 1];
            posn--;
        }
        
        // Put new value into array
        data[posn] = value;
    }
}

template <class T>
T median(const T a, const T b, const T c)
{
    if (a < b)
    {
        // partial order = a,b
        if (b<c) { return b;} // 2 comparisons: order is a,b,c
        else
        { // order is a,c,b or c,a,b
            if (a<c) { return c; } // order is a,c,b -- 3 comparisons
            else { return a;}     // order is c,a,b -- 3 comparisons
        }
    }
    
    else
    {
        // partial order = b,a
        if (c<b) { return b; } // 2 comparisons: order is c,b,a
        else
        {  // order is b,c,a or b,a,c
            if (c>a) { return a;} // order is b,a,c -- 3 comparisons
            else { return c; }   // order is b,c,a -- 3 comparisons
        }
    }
    
    
}

// Partition function used by baseline quicksort(use last element as pivot)
// and median quicksort (use middle element as pivot).
//----------------------------------------------------------------
template <class T>
void partition(T data[], const int low, const int high, int & mid)
{
    // Select pivot value
    //swap(data[high], data[pivotIndex]);
    T pivot = data[high];
    int left  = low;
    int right = high;
    
    // Partition array into two parts
    while (left < right)
    {
        // Scan left to right
        while ((left < right) && (data[left] < pivot))
            left++;
        
        // Scan right to left
        while ((left < right) && (data[right] >= pivot))
            right--;
        
        // Swap data values
        swap(data[left], data[right]);
    }
    
    // Swap pivot to mid
    mid        = left;
    data[high] = data[mid];
    data[mid]  = pivot;
}


// 3 way partition which divides array into three parts:
// data[low...l-1] contains all elements smaller than pivot
// data[l...r] contains all elements equal to pivot
// data[r+1...high] contains all elements larger than pivot
template <class T>
void partition3Way(T data[], const int low, const int high, int &l, int &r)
{
    //initiate all index indicators
    l = low, r = high;
    int i = low;
    
    //select pivot value v
    //swap(data[high], data[pivotIndex]);
    T pivot = data[high];
    
    while (i <= r)
    {
        // if data[i] is larger than pivot, move to the right side
        if (data[i] > pivot)
        {
            swap (data[i], data[r]);
            r--;
        }
        
        // if data[i] is smaller than pivot, move to the left side
        else if (data[i] < pivot)
        {
            swap (data[i], data[l]);
            i++;
            l++;
        }
        // if data[i] equals pivot, keep in the middle part
        else
        {
            i++;
        }
    }
}


// sort data[] according to choice of method
template <class T>
void sort(T data[], int low, int high, const int choice)
{
    // Check terminating condition
    if (low < high)
    {
        // when choice contains hybrid (choice 4,6,7,8) and the size(high - low) is smaller than 20
        // switch to insertion sort
        if ( ((choice == 4) || (choice == 6) ||(choice == 7) || (choice ==8)) && ((high - low) < 20) )
        {
            insertionSort (data, low, high);
        }
        
        // when choice does not contains hybrid (choice 1,2,3,5)
        // or choice contains hybrid but the array size is large (high - low is larger than 20).
        // use sorting as below
        else
        {
            // baseline Quicksort algorithm
            if ((choice == 1) || (choice == 4))
            {
                int mid = 0;
                // use last element as pivot in partition
                partition(data, low, high, mid);
                
                // Recursive calls to sort array
                sort(data, low, mid - 1, choice);
                sort(data, mid + 1, high, choice);
            }
            
            // 3 way Quicksort algorithm
            else if ( (choice == 2) || (choice == 6))
            {
                int l, r;
                // use last element as pivot in partition
                partition3Way(data, low, high, l, r);
                
                // Recursive calls to sort array
                sort(data, low, l-1,choice);
                sort(data, r+1, high,choice);
                
            }
            
            // Median Quicksort algorithm
            else if ((choice == 3) ||(choice == 7))
            {
                int center = (low + high) / 2;
                T pivot    = median(data[low], data[center], data[high]);
                if (pivot == data[low])
                    swap(data[low], data[high]);
                else if (pivot == data[center])
                    swap(data[center], data[high]);
                int mid = 0;
                // use middle element as pivot in partition
                partition(data, low, high, mid);
                
                // Recursive calls to sort array
                sort(data, low, mid - 1, choice);
                sort(data, mid + 1, high,choice);
            }
            
            // 3 way + median
            else if ((choice == 5) || (choice == 8))
            {
                
                int center = (low + high) / 2;
                T pivot    = median(data[low], data[center], data[high]);
                if (pivot == data[low])
                    swap(data[low], data[high]);
                else if (pivot == data[center])
                    swap(data[center], data[high]);
                int l, r;
                // use middle element as pivot in partition
                partition3Way(data, low, high, l, r);
                
                // Recursive calls to sort array
                sort(data, low, l-1,choice);
                sort(data, r+1, high,choice);
            }
        }
    }
}

// Returns true if the given array is in sorted order and false
// if it is not in sorted order. Ordering is defined by the result
// of the '>' operator.
template <class T>
bool isSorted(const T data[], const int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
        if (data[i] > data[i + 1])
            return false;
    }
    return true;
}


int main()
{
    // initial array and filename and read customer input into filename
    string filename;
    string array[SIZE] ;
    
    cout << "Please enter filename: " << endl;
    getline(cin,filename);
    
    // error handling if file cannot be load
    while (!loadFile(filename, array, SIZE))
    {
        cout << endl << "Please try again." << endl;
        cout << "Please enter filename: " << endl;
        getline(cin,filename);
    }
    
    // initial choice and prompt choice list
    // store input into choice
    int choice;
    sortChoice(choice);
    
    
    // count execute time for sorting array content
    clock_t start = clock();
    // do the sorting with the method customer chose
    sort(array, 0, SIZE-1, choice);
    clock_t end = clock();
    double runTime = (end - start) / (double) CLOCKS_PER_SEC;
    
    //print execute time
    cout << "Run time: (s)" << runTime << endl<< endl;
    
    // check if the array is sorted
    if( isSorted(array, SIZE == 1))
        cout << "Array is sorted" << endl;
    else cout <<"Array is not sorted" << endl;
    //delete [] array;
    
    return 0;
}

