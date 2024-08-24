#include "sort.hpp"

int partition(std::vector<int>& arr, int front, int end) {
    int pos = front;
    for (int i = front; i < end; i++) {
        if (arr.at(i) < arr.at(end)) {
            std::swap(arr[i], arr[pos]);
            pos++;
        }
    }
    std::swap(arr[end], arr[pos]);
    return pos;
};

void quickSortRecurs(std::vector<int>& arr, int front, int end) {
    if (front < end) {
        int pos = partition(arr, front, end);
        quickSortRecurs(arr, front, pos-1);
        quickSortRecurs(arr, pos+1, end);
    }
};

void removeLarge(std::vector<int>& arr, int largestSize) {
    for (int i = arr.size()-1; i >= 0; i--) {
        if (arr[i] >= largestSize) {
            arr.pop_back();
        }
        else {
            break;
        }
    }
}

void quickSort(std::vector<int>& arr) 
{
    int end = arr.size()-1;
    int pos = partition(arr, 0, end);
    quickSortRecurs(arr, 0, pos-1);
    quickSortRecurs(arr, pos+1, end);
}