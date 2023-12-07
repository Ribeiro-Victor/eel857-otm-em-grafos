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

class Cluster {
    public:
        Cluster(std::vector<int> center): center_solution(center), volume(1), inefficiency_index(0) {}
        
        void add_solution(std::vector<int> solution) {
            solutions.push_back(solution);
            volume++;
        }

        // Incrementa o índice de ineficiência quando não há melhoria após uma busca local
        void update_inefficiency_index() {
            inefficiency_index++;
        }

        // Zera o índice de ineficiência após ele atingir o limite
        void reset_inefficiency_index() {
            inefficiency_index = 0;
        }

        // Apaga todas as soluções do cluster com exceção da central
        void clear_cluster() {
            solutions.clear();
            solutions.push_back(center_solution);
            volume = 1;
        }

        // Find

        // Getters
        std::vector<int> get_center_solution() {
            return center_solution;
        }

        int get_volume() const {
            return volume;
        }

        int get_inefficiency_index() {
            return inefficiency_index;
        }


        // Setters
        void set_center_solution(std::vector<int> solution) {
            center_solution = solution;
        }


    private:
        std::vector<int> center_solution;
        int volume;
        int inefficiency_index;
        std::vector<std::vector<int>> solutions;
};

file_records read_entry_file(std::string);
void log_dataset(file_records*);

float calculate_penalties(std::vector<item>*, std::vector<int>*, int);

std::vector<int> constructive(file_records*);

int avaliate_solution(std::vector<int>*, file_records*);
std::vector<int> generate_neighbor(std::vector<int>*, file_records*);
std::vector<int> simulated_annealing(int, int, int, double, int, int, int, file_records*);
std::unordered_map<std::string, std::string> read_config_file(const std::string&);

#endif
