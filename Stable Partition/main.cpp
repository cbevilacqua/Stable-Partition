// Stable Partition
// Authored by Cory Bevilacqua

// Stable partition implementation with O(1) constant memory overhead and O(n log n) runtime performance.
// Based on a combination of mergesort and quicksort methodologies, with unwound recursion for true
// constant memory overhead (according to the normal assumptions of the RAM model).

// To use, call function 'stablepartition' and pass the datatype T, a vector of T elements, and a
// boolean function that operates on one T element and returns whether it is in the 'true' or
// 'false' section of the partition (where the 'true' portion comes before the 'false' portion
// in the final partitioned vector).

// Example usage: stablepartition<int>(list, isEven);, where list is a vector of ints and isEven
// is a boolean function that accepts one int value as its function parameter.

//-----------------------------------------------------------------------------------------------------------------------

#include<iostream>
#include<vector>

using namespace std;

// Function prototypes for key stable partition components

template<typename T>
void stablepartition(std::vector<T>& list, bool (&test)(T));

template<typename T>
void merge(std::vector<T>& list, bool (&test)(T), int low, int middle, int high);

template<typename T>
void swap(std::vector<T>& list, int a, int b);

// Function prototypes for example boolean partition functions

bool isEven(int value);

bool firstHalf(char c);

//-----------------------------------------------------------------------------------------------------------------------

// Follows mergesort methodology of partitioning two elements, then partitioning 4 elements composed of 2
// already partitioned 2-element subsets, then 8, etc.

// Recursion is unwound and done 'in-order' to avoid having O(log n) recursive calls in-flight simultaneously
// (which would violate O(1) memory overhead goal).

// In normal case (i.e., both subsets being merged are of lengths that are the appropriate power of 2 for the
// current cycle), finding the middle element is obvious.

// For edge cases, the middle is found manually before passing the subsets to the merge function.
template<typename T>
void stablepartition(std::vector<T>& list, bool (&test)(T))
{
    int last = (int)list.size()-1;
    
    int low, middle, high;
    
	// for powers of two from 2 to 2N-1
	for (int i = 2; i < 2*(last+1); i*=2)
	{
		// for each subset of size i in the list
		for (int j = 0; j < last; j+=i)
		{
			// edge case, end of list does not contain a full i elements
            // different high, plus we must find the middle
			if ( (j+i-1) > last )
            {
                low = j;
                high = last;
                
                middle = -1;
                // find middle (where there is a 'true' element to the right of a 'false' element)
                // example: even element to the right of odd element when partioning function is isEven.
                for (int k = low; k < high; k++)
                {
                    if (!test(list[k]) && test(list[k+1]))
                    {
                        middle = k+1;
                        break;
                    }
                }
                
                if (middle != -1)
                    merge(list, test, low, middle, high);
                // if middle still = -1, subset is already partitioned correctly so no merge call necessary
            }
            
			// normal case
			else
            {
                low = j;
                high = j+i-1;
                middle = (low+high)/2+1;
                
                if (!test(list[middle-1]) && test(list[middle]))
                    merge(list, test, low, middle, high);
                // if first subset is all 'true' values OR second subset is all 'false' values, this subset is
                // already partioned correctly so no merge call is necessary
            }
		}
	}
}

// Function to merge two partitioned subsets into larger partioned subset, with O(n) runtime and O(1) memory overhead.

// Trivial in case where the number of 'false' values in subset 1 is equivalent to the number of 'true' values in subset 2:
// just perform that number of 1-1 swaps. Call that number k; the window created by the indexes used is of size 2k, & k swaps made.

// If not equivalent, perform a number of swaps equal to the smaller of those two numbers, and adjust the window size in
// the appropriate direction to create a new subproblem. This process is iterated as many times as necessary until both
// sides of the window are equivalent, and such a final case always occurs even if it is necessary to go down to a k=1 scenario.
// The total number of swaps is never more than the number of elements in the two subsets.
template<typename T>
void merge(std::vector<T>& list, bool (&test)(T), int low, int middle, int high)
{
    // define important indexes that we will use
    int correctBeforeHere = low;
    while(test(list[correctBeforeHere]))
        correctBeforeHere++;
    
    int correctFromHere = high;
    while(!test(list[correctFromHere]))
        correctFromHere--;
    correctFromHere++;
    
    int movingFrontier = middle;
    
    int swapIndex;
    
    // ends when an even number of values have been swapped in the most recent iteration
    while(correctBeforeHere != movingFrontier)
    {
        swapIndex = movingFrontier;
        
        while(swapIndex < correctFromHere)
        {
            if (correctBeforeHere == movingFrontier)
                movingFrontier=swapIndex;
            
            swap(list, correctBeforeHere, swapIndex);
            correctBeforeHere++;
            swapIndex++;
        }
    }
    
}

// Simple helper function, swaps two values in a vector
template<typename T>
void swap(std::vector<T>& list, int a, int b)
{
    T temp = list[a];
    list[a] = list[b];
    list[b] = temp;
}

// Example boolean function for passing to stablepartition, partitions based on whether an int is even or odd
bool isEven(int value)
{
    return !(value%2);
}

// Example boolean function for passing to stablepartition, partitions whether a char is in the first half
// of the alphabet (A-M) or the second half (N-Z). Assumes char passed is between 65 and 90, i.e., a capital letter.
// Not truly a part of this implementation, just an example function; correct error checking between your vector
// initialization and the boolean function used would be an important part of using this stablepartition implementation.
bool firstHalf(char c)
{
    if (c <= 'M')
        return true;
    else
        return false;
}

//-----------------------------------------------------------------------------------------------------------------------

int main()
{
    srand((unsigned int)time(NULL));
    
    // Example usage 1 - int vector, partition based on whether values are even or odd
    cout << "Partitioning of int vector, even | odd" << endl << endl;
    
    // Create test vector
    int size = 12;
    vector<int> list(size);
    
    // populate test vector with (pseudo)random ints between 0 and 99
    for (int i = 0; i < list.size(); i++)
        list[i] = rand() % 100;
    
    // print out starting vector for comparison with final partitioned vector
    cout << "Original vector" << endl;
    for (int i = 0; i < list.size(); i++)
		cout << list[i] << " ";
    cout << endl << endl;
    
    // run the even/odd partitioning function
    stablepartition<int>(list, isEven);
    
    // print out the new ordering to see that the even/odd partitioning has occurred
    cout << "Partitioned vector" << endl;
    for (int i = 0; i < list.size(); i++)
		cout << list[i] << " ";
    cout << endl << endl;
    
    //-----------------------------------------------------------------------------------------------------------------------
    
    // Example usage 2 - char vector, partition based on whether chars are in the first half of the alphabet
    // or the second.
    cout << "Partitioning of char vector, first half of alphabet | second half of alphabet" << endl << endl;
    
    int size2 = 15;
    vector<char> list2(size2);
    
    for (int i = 0; i < list2.size(); i++)
        list2[i] = rand() % 26 + 65;
    
    cout << "Original vector" << endl;
    for (int i = 0; i < list2.size(); i++)
        cout << list2[i] << " ";
    cout << endl << endl;
    
    stablepartition<char>(list2, firstHalf);
    
    cout << "Partitioned vector" << endl;
    for (int i = 0; i < list2.size(); i++)
        cout << list2[i] << " ";
    cout << endl << endl;
    
    cin.get();
    return 0;
}
