#include "helper.h"

using namespace std;

int main() {

    // Read files
    unordered_map<string, string> config = read_config_file("config.txt");
    file_records dataset = read_entry_file(config["dataset_file"]);
    
    log_dataset(&dataset);
    
    // Generate first solution
    // vector<int> solution = constructive(&dataset);
    
    // Avaliate solution
    // int score = avaliate_solution(&solution, &dataset);

    // // Print solution result
    // cout<<"Initial solution: R$"<<score<<endl;
    
    // vector<int> neighbor = generate_neighbor(&solution, &dataset);
    // for(int i=0; i < 100; i++){
    //     cout << "Neighbor Value: R$" << avaliate_solution(&neighbor, &dataset) << endl;
    //     neighbor = generate_neighbor(&neighbor, &dataset);
    // }
    int AS_max = stoi(config["AS_max"]);
    int t_0 = stoi(config["t_0"]);
    int T_end = stoi(config["T_end"]);
    double alpha = stod(config["alpha"]);
    vector<int> final_solution = simulated_annealing(AS_max, T_end, t_0, alpha, &dataset);
    cout << "Final solution: R$" << avaliate_solution(&final_solution, &dataset) << endl;
    
    // Generate first solution
    // vector<int> solution = constructive(&dataset);
    // cout << "primeira solução: " << avaliate_solution(&solution, &dataset) << endl;
    // vector<int> new_sol = generate_neighbor_destroying(&solution, &dataset);
    // cout << "vizinho destruindo x%: " << avaliate_solution(&new_sol, &dataset) << endl;

    return 0;
}