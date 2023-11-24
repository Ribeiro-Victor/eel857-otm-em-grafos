#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <bits/stdc++.h> // std::sort
#include <chrono>

struct item {
    int label;
    int value;
    int weight;
    std::vector<std::pair<int, int>> penalties; // (label, penalty)
};

struct file_records {
    int num_items;
    int knapsack_capacity;
    std::vector<item> items;
};


file_records read_entry_file(std::string);
void log_dataset(file_records*);

float calculate_penalties(std::vector<item>*, std::vector<int>*, int);

std::vector<int> constructive(file_records*);

int avaliate_solution(std::vector<int>*, file_records*);
std::vector<int> generate_neighbor(std::vector<int>*, file_records*);
// std::vector<int> simulated_annealing(file_records&);

#endif
