#include <stdio.h>
// C program to implement recursive Binary Search
// this was taken from geeksforgeeks.com I didn't feel
//like making the sorting algorithm myself
// A recursive binary search function. It returns
// location of x in given array arr[l..r] is present,
// otherwise -1
int binarySearch(int arr[], int l, int r, int x)
{
    if (r >= l) {
        int mid = l + (r - l) / 2;

        // If the element is present at the middle
        // itself
        if (arr[mid] == x)
            return mid;

        // If element is smaller than mid, then
        // it can only be present in left subarray
        if (arr[mid] > x)
            return binarySearch(arr, l, mid - 1, x);

        // Else the element can only be present
        // in right subarray
        return binarySearch(arr, mid + 1, r, x);
    }

    // We reach here when element is not
    // present in array
    return -1;
}

// use this to create an array of linearly interpolated intergers
// from min to max where size is the size of the array
// *array is a pointer to the array that will store the linear interpolation
int * linInterp(int min,int max,int size,int *array){
  int step=(max-min)/size;
  int i;
  for(i=0; i<=size; i++){
    *(array+i)=min+step*i;
  }
  return array;
}
