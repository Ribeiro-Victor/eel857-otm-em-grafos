#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct file_records {
    int num_items;
    int knapsack_capacity;
    std::vector<int> value;
    std::vector<int> weight;
    std::vector<std::vector<std::pair<int, int>>> penalties; // Usando uma lista de adjacência para as penalidades
};

file_records read_entry_file(std::string);
void log_dataset(file_records);

#endif
