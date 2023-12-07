#include "helper.h"
#include "Solution.cpp"

using namespace std;

int main()
{

    // Read files
    unordered_map<string, string> config = read_config_file("config.txt");

    std::initializer_list<std::string> files_500 = {"sum_instances/500/kpf_1_sum.txt", "sum_instances/500/kpf_2_sum.txt", "sum_instances/500/kpf_3_sum.txt", "sum_instances/500/kpf_4_sum.txt", "sum_instances/500/kpf_5_sum.txt", "sum_instances/500/kpf_6_sum.txt", "sum_instances/500/kpf_7_sum.txt", "sum_instances/500/kpf_8_sum.txt", "sum_instances/500/kpf_9_sum.txt", "sum_instances/500/kpf_10_sum.txt"};
    std::initializer_list<std::string> files_700 = {"sum_instances/700/kpf_1_sum.txt", "sum_instances/700/kpf_2_sum.txt", "sum_instances/700/kpf_3_sum.txt", "sum_instances/700/kpf_4_sum.txt", "sum_instances/700/kpf_5_sum.txt", "sum_instances/700/kpf_6_sum.txt", "sum_instances/700/kpf_7_sum.txt", "sum_instances/700/kpf_8_sum.txt", "sum_instances/700/kpf_9_sum.txt", "sum_instances/700/kpf_10_sum.txt"};
    std::initializer_list<std::string> files_800 = {"sum_instances/800/kpf_1_sum.txt", "sum_instances/800/kpf_2_sum.txt", "sum_instances/800/kpf_3_sum.txt", "sum_instances/800/kpf_4_sum.txt", "sum_instances/800/kpf_5_sum.txt", "sum_instances/800/kpf_6_sum.txt", "sum_instances/800/kpf_7_sum.txt", "sum_instances/800/kpf_8_sum.txt", "sum_instances/800/kpf_9_sum.txt", "sum_instances/800/kpf_10_sum.txt"};
    std::initializer_list<std::string> files_1000 = {"sum_instances/1000/kpf_1_sum.txt", "sum_instances/1000/kpf_2_sum.txt", "sum_instances/1000/kpf_3_sum.txt", "sum_instances/1000/kpf_4_sum.txt", "sum_instances/1000/kpf_5_sum.txt", "sum_instances/1000/kpf_6_sum.txt", "sum_instances/1000/kpf_7_sum.txt", "sum_instances/1000/kpf_8_sum.txt", "sum_instances/1000/kpf_9_sum.txt", "sum_instances/1000/kpf_10_sum.txt"};

    std::map<std::string, std::list<std::string>> dict;

    // // Adicionando dados ao dicionário
    dict["500"].insert(dict["500"].end(), files_500.begin(), files_500.end());
    dict["700"].insert(dict["700"].end(), files_700.begin(), files_700.end());
    dict["800"].insert(dict["800"].end(), files_800.begin(), files_800.end());
    dict["1000"].insert(dict["1000"].end(), files_1000.begin(), files_1000.end());

    std::list<Solution> solutions;

    for (auto &pair : dict)
    {

        // Solution sol(pair.first);

        std::vector<double> durations;
        std::vector<int> scores;
        int counter = 1;

        for (auto &path : pair.second)
        {
            int repeatCounter = 1;
            Solution solution(pair.first + "_" + std::to_string(counter), pair.first); // 500_1, 500_2
            std::cout << std::endl;
            file_records dataset = read_entry_file(path);
            int AS_max = stoi(config["AS_max"]);
            int t_0 = stoi(config["t_0"]);
            int T_end = stoi(config["T_end"]);
            double alpha = stod(config["alpha"]);

            while (repeatCounter <= 10)
            {
                auto start = std::chrono::high_resolution_clock::now();
                vector<int> final_solution = simulated_annealing(AS_max, T_end, t_0, alpha, &dataset);
                auto finish = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> duration = finish - start;

                int final_score = avaliate_solution(&final_solution, &dataset);

                SolutionItem solutionItem(pair.first + "_" + std::to_string(counter) + "_" + std::to_string(repeatCounter), final_score, duration.count());

                solution.addSolutionItem(solutionItem);

                cout << "Final solution: R$" << final_score << endl;

                repeatCounter++;
            }
            solutions.push_back(solution);
            counter++;
        }
    }

    std::ofstream solutionsFile("solutions_summary.csv");
    solutionsFile << "Name, Qty, Best Solution, Average Solution, Standard Deviation, Average Time, Average Time SD\n";
    for (auto &solution : solutions)
    {
        solution.writePropertiesToFile(solutionsFile);
    }
    solutionsFile.close();

    return 0;
}