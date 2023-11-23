#include "helper.h"

int main() {

    // Read files
    file_records dataset = read_entry_file("sum_instances/500/kpf_1_sum.txt");
    // file_records dataset = read_entry_file("sum_instances/teste_entrada.txt");
    log_dataset(&dataset);
    
    // Generate first solution
    std::vector<int> solution = constructive(&dataset);
    
    // Avaliate solution
    int score = avaliation(&solution, &dataset);

    // // Print solution result
    // std::cout<<"Total Value: "<<score<<"$"<<std::endl;

    return 0;
}