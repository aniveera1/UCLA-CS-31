//
//  array.cpp
//  array
//
//  Created by Anirudh Veeraragavan on 11/1/16.
//  Copyright © 2016 Anirudh Veeraragavan. All rights reserved.
//

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

// Various array manipulation functions
int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int countRuns(const string a[], int n);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int separate(string a[], int n, string separator);

// This value is returned whenever a function is given bad input
const int RET_BAD_FUNCTION_ARGUMENT = -1;

int main() {
}


// This function will append a string to each of the n elements of an array and return n
int appendToAll(string a[], int n, string value)
{
    // If n is 0 we will simply append the string onto no elements, thus n = 0 is not a bad argument
    if(n < 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    // Increments through the array adding the string until the indicated n element
    for(int pos = 0; pos < n; pos++)
        a[pos] += value;
    
    return n;
}


// This function will return the position of the first string in an array that is equal to target
int lookup(const string a[], int n, string target)
{
    // If n is 0 then the array will never equal target, thus we can return -1
    if(n <= 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    for(int pos = 0; pos < n; pos++)
        if(a[pos] == target)
            return pos;
    
    return RET_BAD_FUNCTION_ARGUMENT;
}


// This function will return the position of the first string in an array that is >= all the other strings in the array
int positionOfMax(const string a[], int n)
{
    // If n is 0 then the array has no strings, thus there can be no such string that is >= all other strings so we return -1
    if(n <= 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    int pos = 0;
    string comparisonString = a[pos];
    
    // We initialize the comparisonString to the first string, and every time there is a string that
    // is > comparisonString we change comparisonString to that string
    for(int i = 1; i < n; i++)
        if(a[i] > comparisonString)
        {
            comparisonString = a[i];
            pos = i;
        }
    return pos;
}


// This function will eliminate the item at position pos by copying all elements after it one place to the left
// and place the eliminated item into the last position
int rotateLeft(string a[], int n, int pos)
{
    // If n is 0 we get an empty array, and we cannot manipulate an empty array, so we return -1
    if(n <= 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    // If position is negative or greater than the size of the array, that makes no sense so we return -1
    if(pos < 0 || pos >= n)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    // This code essentially removes a string, shifts each string one place to the left, and then places the string in the last position
    string eliminatedString;
    int returnValue = pos;
    int tempValue = 0;
    
    eliminatedString = a[pos];
    a[pos] = "";
    
    while(pos < (n - 1))
    {
        tempValue = pos;
        pos++;
        a[tempValue] = a[pos];
    }
    
    a[n - 1] = eliminatedString;
    
    return returnValue;
}


// This function will return the number of sequences of one or more consecutive identical items in a string
int countRuns(const string a[], int n)
{
    // N is allowed to be 0 because that simply means an empty array, which has no sequences of any items, which returns 0
    if(n < 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    // This functions works through the variable sequenceCount, which is incremented every time the for loop encounters a new item
    int sequenceCount = 0;
    for(int pos = 0; pos < n;)
    {
        string comparisonString = a[pos];
        pos++;
        sequenceCount++;
        while(a[pos] == comparisonString)
            pos++;
    }
    return sequenceCount;
}


// This function will reverse the order of the elements of an array and return n
int flip(string a[], int n)
{
    // N is allowed to be 0 because that means an empty array, and the function will simply do nothing
    if(n < 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    string tempString;
    int midPoint = n / 2;
    
    // This function simply switches strings from one side of the midPoint to the other side until the for loop reaches the value of midPoint
    for(int i = 0; i < midPoint; i++)
    {
        tempString = a[i];
        a[i] = a[n - (i + 1)];
        a[n - (i + 1)] = tempString;
    }
    return n;
}


// This function will return the position of the first corresponding elements of two arrays that are not equal
int differ(const string a1[], int n1, const string a2[], int n2)
{
    // N is allowed to be 0 because that either means the arrays differ at the 0th position itself, or if both arrays are 0
    // then we return the smallest size of the array which is also 0
    if(n1 < 0 || n2 < 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    int sizeOfSmallerArray;
    if(n1 < n2)
        sizeOfSmallerArray = n1;
    else
        sizeOfSmallerArray = n2;
    
    int pos = 0;
    while(pos < sizeOfSmallerArray)
    {
        if(a1[pos] == a2[pos])
            pos++;
        else
            return pos;
    }
    return pos;
}


// This function will check if all of array a2 appears in array a1 and if it does, will return the position where this sequence starts
int subsequence(const string a1[], int n1, const string a2[], int n2)
{
    // If both n1 and n2 are 0 then the function is fine because a1 contains all of a2 starting from the 0th position
    if(n1 == 0 && n2 == 0)
        return 0;
    // However if a1 is 0, then it will never contain a2
    else if(n1 <= 0 || n2 < 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    // But if a1 is positive and a2 is 0, then a1 contains a2 starting at the 0th position
    else if(n2 == 0)
        return 0;
    
    // If n2 is larger than n1, then the first array cannot contain all of n2 so we return -1
    if(n2 > n1)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    int array1Pos = 0;
    int array2Pos = 0;
    
    int array2SequenceCount;
    int startingPosition;
    
    // The outer while loop goes through a1 and the inner while loops goes through a2 while keeping a
    // count of how many values it has gone through. If this count is equal to the indicated size of a2, then a2 is a subsequence of a1
    while(array1Pos < n1)
    {
        array2SequenceCount = 0;
        startingPosition = array1Pos;
        
        while(a2[array2Pos] == a1[array1Pos])
        {
            array1Pos++;
            array2Pos++;
            array2SequenceCount++;
            
            if(array2SequenceCount == n2)
                return startingPosition;
        }
        array1Pos++;
    }
    return RET_BAD_FUNCTION_ARGUMENT;
}


// This function will check if any of the elements in the first array are equal to at least one element in the second array
// and if so, will return the position of the first such element
int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
    // If a1 and a2 are both 0 arrays, then they are both equivalent starting from the 0th position
    if(n1 == 0 && n2 == 0)
        return 0;
    // However, if one is 0 while the other is not, then they are never equivalent
    else if(n1 <= 0 || n2 <= 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    // This function essentially increments through all of a2 at each position of a1 to see if there is ever a match
    for(int array1Pos = 0; array1Pos < n1;)
    {
        for(int array2Pos = 0; array2Pos < n2;)
        {
            if(a2[array2Pos] == a1[array1Pos])
                return array1Pos;
            else
                array2Pos++;
        }
        array1Pos++;
    }
    return RET_BAD_FUNCTION_ARGUMENT;
}


// This function will rearrange an array so that all elements < separator come first and then all elements > separator come after
int separate(string a[], int n, string separator)
{
    // N is allowed to be 0 because that means an empty array, and the function will simply do nothing
    if(n < 0)
        return RET_BAD_FUNCTION_ARGUMENT;
    
    string tempArray[50] = {};
    int firstHalfOfArray = 0;
    int secondHalfOfArray = n - 1;
    
    // Here we assign the strings to a temporary array in the correct position
    for(int j = 0; j < n; j++)
    {
        if(a[j]< separator)
        {
            tempArray[firstHalfOfArray] = a[j];
            firstHalfOfArray++;
        }
        else if(a[j] > separator)
        {
            tempArray[secondHalfOfArray] = a[j];
            secondHalfOfArray--;
        }
    }
    
    // We check if separator itself was in the original string, and if so, assign it to the middle position of the temporary array
    for(int i = 0; i < n; i++)
        if(a[i] == separator)
        {
            tempArray[firstHalfOfArray] = separator;
            firstHalfOfArray++;
        }
    
    // We assign the values of the temporary array back to the original array
    for(int i = 0; i < n; i++)
        a[i] = tempArray[i];
    
    // Increment through the original array till we reach the first value >= separator
    for(int i = 0; i < n; i++)
        if(!(a[i] < separator))
            return i;
    
    return n;
}
