#include "helper.h"

using namespace std;

int main() {

    // Read files
    file_records dataset = read_entry_file("sum_instances/500/kpf_1_sum.txt");
    // file_records dataset = read_entry_file("sum_instances/teste_entrada.txt");
    log_dataset(&dataset);
    
    // Generate first solution
    vector<int> solution = constructive(&dataset);
    
    // Avaliate solution
    int score = avaliate_solution(&solution, &dataset);

    // // Print solution result
    cout<<"Total Value: R$"<<score<<endl;
    
    vector<int> neighbor = generate_neighbor(&solution, &dataset);
    for(int i=0; i < 10; i++){
        cout << "Neighbor Value: R$" << avaliate_solution(&neighbor, &dataset) << endl;
        neighbor = generate_neighbor(&neighbor, &dataset);
    }
    
    
    return 0;
}