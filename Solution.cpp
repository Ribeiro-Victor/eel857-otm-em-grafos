#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <type_traits>

using namespace std;

int getMaxElement(const std::vector<int> &vec)
{
    if (vec.empty())
    {
        throw std::runtime_error("O vetor está vazio");
    }

    return *std::max_element(vec.begin(), vec.end());
}

template <typename T>
double calculateAverage(const std::vector<T> &vec)
{
    static_assert(std::is_arithmetic<T>::value, "Apenas tipos aritméticos são suportados");

    if (vec.empty())
    {
        throw std::runtime_error("O vetor está vazio");
    }

    double sum = 0.0;
    for (const auto &value : vec)
    {
        sum += value;
    }

    return sum / vec.size();
}

template <typename T>
double calculateStandardDeviation(const std::vector<T> &vec)
{
    static_assert(std::is_arithmetic<T>::value, "Apenas tipos aritméticos são suportados");

    if (vec.size() <= 1)
    {
        throw std::runtime_error("Vetor com tamanho insuficiente para calcular desvio padrão");
    }

    double mean = calculateAverage(vec);
    double sumOfSquares = 0.0;

    for (const auto &value : vec)
    {
        sumOfSquares += std::pow(value - mean, 2);
    }

    return std::sqrt(sumOfSquares / (vec.size() - 1));
}

class SolutionItem
{

private:
    std::string name;
    int score;
    double duration;

public:
    SolutionItem(std::string name, int score, double duration)
        : name(name), score(score), duration(duration) {}

    std::string getName() const { return name; }
    int getScore() const { return score; }
    double getDuration() const { return duration; }

    void setName(const std::string &newName) { name = newName; }
    void setScore(double newScore) { score = newScore; }
    void setDuration(double newDuration) { duration = newDuration; }
};

class Solution
{
private:
    std::string name;
    std::string qty;
    int bestSolution;
    double avgSolution;
    double standardDeviation;
    double avgTime;
    double avgTimeStandardDeviation;

    std::list<SolutionItem> solutionItems;
    std::vector<int> scores;
    std::vector<double> durations;

public:
    explicit Solution(std::string name, std::string qty)
        : name(std::move(name)), qty(std::move(qty)), bestSolution(0), avgSolution(0), standardDeviation(0), avgTime(0), avgTimeStandardDeviation(0)
    {
    }

    Solution(std::string name, std::string qty, int bestSolution, double avgSolution, double standardDeviation, double avgTime, double avgTimeStandardDeviation)
        : name(name), qty(qty), bestSolution(bestSolution), avgSolution(avgSolution), standardDeviation(standardDeviation), avgTime(avgTime), avgTimeStandardDeviation(avgTimeStandardDeviation) {}

    std::string getName() const { return name; }
    std::string getQty() const { return qty; }
    int getBestSolution() const { return getMaxElement(getScores()); }
    double getAvgSolution() const { return calculateAverage(getScores()); }
    double getStandardDeviation() const { return calculateStandardDeviation(getScores()); }
    double getAvgTime() const { return calculateAverage(getDurations()); }
    double getAvgTimeStandardDeviation() const { return calculateStandardDeviation(getDurations()); }
    std::list<SolutionItem> getSolutionItems() const { return solutionItems; };
    std::vector<int> getScores() const
    {
        std::vector<int> scores;
        for (const auto &item : solutionItems)
        {
            scores.push_back(item.getScore());
        }
        return scores;
    };
    std::vector<double> getDurations() const
    {
        std::vector<double> durations;
        for (const auto &item : solutionItems)
        {
            durations.push_back(item.getDuration());
        }
        return durations;
    }

    void setName(const std::string &newName) { name = newName; }
    void setAvgSolution(double newAvgSolution) { avgSolution = newAvgSolution; }
    void setStandardDeviation(double newStandardDeviation) { standardDeviation = newStandardDeviation; }
    void setAvgTime(double newAvgTime) { avgTime = newAvgTime; }
    void setAvgTimeStandardDeviation(double newAvgTimeStandardDeviation) { avgTimeStandardDeviation = newAvgTimeStandardDeviation; }
    void addSolutionItem(const SolutionItem &item)
    {
        solutionItems.push_back(item);
    }

    void Display() const
    {
        std::cout << "Name: " << name << "\n"
                  << "Best Solution: " << bestSolution << "\n"
                  << "Average Solution: " << avgSolution << "\n"
                  << "Standard Deviation: " << standardDeviation << "\n"
                  << "Average Time: " << avgTime << "\n"
                  << "Average Time Standard Deviation: " << avgTimeStandardDeviation << std::endl;
    }

    void writeItemsToFile()
    {
        std::ofstream file(name + "_items.csv");
        file << "Item Name, Score, Duration\n";
        for (const auto &item : solutionItems)
        {
            file << item.getName() << ", " << item.getScore() << ", " << item.getDuration() << "\n";
        }
        file.close();
    }

    void writePropertiesToFile(std::ofstream &file)
    {
        file << name << ", " << qty << ", " << getBestSolution() << ", " << getAvgSolution() << ", " << getStandardDeviation() << ", "
             << getAvgTime() << ", " << getAvgTimeStandardDeviation() << "\n";
    }
};
