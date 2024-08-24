#ifndef SORT_HPP
#define SORT_HPP

#include <vector>
#include <algorithm>

// Quicksort Algorithm
// Array is sorted in-place
// A vector of integers is passed by reference
void quickSort(std::vector<int>& arr);

void removeLarge(std::vector<int>& arr, int largestSize);

#endif // SORT_HPP
