#include "tsp_genetic_algorithm.hpp"
#include "sort.hpp"
#include <iostream>
#include <algorithm>
#include <limits>
#include <random>
#include <cstdlib> 
#include <time.h> 
#include <functional>
#include <unordered_set>

using namespace std;

TSPGenetic::TSPGenetic(const vector<vector<double>>& cityGraph_, int numberOfCities_)
    : cityGraph(cityGraph_), numberOfCities(numberOfCities_) {}

double TSPGenetic::computeShortestTour() {
    double shortestDistance = numeric_limits<double>::max();
    int popSize = 10 * numberOfCities;
    int numGenerat = 100 * numberOfCities;
    srand(time(0));

    vector<int> tour(numberOfCities);
    vector<int> tourOrder(popSize);
    vector<int> badTours(popSize);
    
    vector<vector<int>> allTours(popSize);
    for (int i = 0; i < popSize; i++) {
        for (int j = 0; j < numberOfCities; ++j) {
            tour[j] = j;
        }
        shuffle(tour.begin(), tour.end(), default_random_engine(rand()));
        allTours[i] = tour;
    }

    for (int k = 0; k < numGenerat; k++) {
        tournament(popSize, allTours, tourOrder, badTours);
    }

    for (int i = 0; i < popSize; i++) {
        double distance = computeTourDistance(allTours[i]);
        if (distance < shortestDistance) {
            shortestDistance = distance;
            bestTour = allTours[i];
        } 
    }
    
    return shortestDistance;
}

void TSPGenetic::tournament(int popSize, vector<vector<int>>& allTours, vector<int>& tourOrder, vector<int>& badTours) {
    double crossRate = 0.9;
    double mutatRate = 0.3;

    tourOrder.resize(popSize);
    badTours.resize(popSize);
    for (int i = 0; i < popSize; ++i) {
        tourOrder[i] = i;
    }
    
    shuffle(tourOrder.begin(), tourOrder.end(), default_random_engine(rand()));
    double dis1;
    double dis2;
    for (int j = 0; j < popSize / 2; j++) {
        dis1 = computeTourDistance(allTours[tourOrder[j*2]]);
        dis2 = computeTourDistance(allTours[tourOrder[j*2+1]]);
        if(dis1 > dis2) {
            tourOrder[j*2] = popSize;
            badTours[j] = tourOrder[j*2];
        }
        else if (dis1 < dis2) {
            tourOrder[j*2+1] = popSize;
            badTours[j] = tourOrder[j*2+1];
        }
        else {
            badTours[j] = popSize;
        }
    }
    quickSort(tourOrder);
    removeLarge(tourOrder, popSize);
    shuffle(tourOrder.begin(), tourOrder.end(), default_random_engine(rand()));
    int tourOrderSize = tourOrder.size();

    quickSort(badTours);
    removeLarge(badTours, popSize);
    shuffle(badTours.begin(), badTours.end(), default_random_engine(rand()));

    vector<vector<int>> newAllTours(popSize);
    int numChildren = 0;

    default_random_engine generator(rand());
    uniform_int_distribution<int> distribution(1,10);
    auto chance = bind(distribution, generator);
    double crossChance = chance() / 10.0;
    bool muteChance = false;
    if(chance() / 10.0 <= mutatRate) {
        muteChance = true;
    }
    
    for (int i = 0; i < tourOrderSize/2; i++) {
        if (crossChance <= crossRate) {
            newAllTours[numChildren] = makeChild(allTours[tourOrder[i*2]], allTours[tourOrder[i*2+1]], muteChance);
            numChildren++;
        }
        crossChance = chance() / 10.0; 
    }

    shuffle(tourOrder.begin(), tourOrder.end(), default_random_engine(rand()));

    if (tourOrderSize < popSize - numChildren) {
        for (int i = 0; i < tourOrderSize; i++) {
            newAllTours[numChildren + i] = allTours[tourOrder[i]];
        }
        for (int i = tourOrderSize + numChildren; i < popSize; i++) {
            newAllTours[i] = allTours[badTours[i-tourOrderSize-numChildren]];
        }
    }
    else {
        for (int i = 0; i < popSize - numChildren; i++) {
            newAllTours[numChildren + i] = allTours[tourOrder[i]];
        }
    }
    newAllTours.swap(allTours);
}

vector<int> TSPGenetic::makeChild(const vector<int>& par1, const vector<int>& par2, bool muteChance) {
    vector<int> child(numberOfCities);
    for (int i = 0; i < numberOfCities; ++i) {
        child[i] = -1;
    }

    default_random_engine generator(rand());
    uniform_int_distribution<int> distribution(1,numberOfCities);
    auto chance = bind(distribution, generator);
    int subStart = chance()-1;
    int subLength = chance();
    int subEnd = subStart + subLength;

    unordered_set<int> unusable;
    vector<int> usable;

    if (subEnd <= numberOfCities) {
        for (int i = subStart; i < subEnd; ++i) {
            child[i] = par1[i];
            unusable.insert(par1[i]);
        }
    }
    else {
        subEnd -= numberOfCities;
        for (int i = subStart; i < numberOfCities; i++) {
            child[i] = par1[i];
            unusable.insert(par1[i]);
        }
        for (int i = 0; i < subEnd; i++) {
            child[i] = par1[i];
            unusable.insert(par1[i]);
        }
    }

    for (int i = 0; i < numberOfCities; i++) {
        if (unusable.find(par2[i]) == unusable.end()) {
            usable.push_back(par2[i]);
        }
    }

    int usableLength = usable.size();
    
    for (int i = 0; i < usableLength; i++) {
        for (int j = 0; j < numberOfCities; j++) {
            if (child[j] < 0) {
                swap(child[j], usable[i]);
                break;
            }
        }
    }

    //MUTATION
    if (muteChance) {
        int a = chance() - 1;
        int b = chance() - 1;
        swap(child[a], child[b]);
    }

    return child;
}

double TSPGenetic::computeTourDistance(const vector<int>& tour) {
    double distance = 0.0;
    for (int i = 0; i < numberOfCities - 1; ++i) {
        distance += cityGraph[tour[i]][tour[i + 1]];
    }
    distance += cityGraph[tour[numberOfCities - 1]][tour[0]];
    return distance;
}

const vector<int>& TSPGenetic::getBestTour() const {
    return bestTour;
}
