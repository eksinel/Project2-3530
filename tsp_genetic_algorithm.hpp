#ifndef TSP_GENETIC_HPP
#define TSP_GENETIC_HPP

#include <vector>

class TSPGenetic {
public:
    TSPGenetic(const std::vector<std::vector<double>>& cityGraph_, int numberOfCities_);

    double computeShortestTour();
    const std::vector<int>& getBestTour() const;

private:
    const std::vector<std::vector<double>>& cityGraph;
    int numberOfCities;
    std::vector<int> bestTour;

    void tournament(int popSize, std::vector<std::vector<int>>& allTours, std::vector<int>& tourOrder, std::vector<int>& badTours);
    double computeTourDistance(const std::vector<int>& tour);
    std::vector<int> makeChild(const std::vector<int>& par1, const std::vector<int>& par2, bool muteChance);
};

#endif // TSP_GENETIC_HPP
