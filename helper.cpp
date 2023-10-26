#include "helper.h"

using namespace std;

file_records read_entry_file(string filename){
    ifstream file(filename); 
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        exit (1);
    }

    int num_items, num_penalties, knapsack_capacity;
    file >> num_items >> num_penalties >> knapsack_capacity;

    vector<int> value(num_items);
    vector<int> weight(num_items);

    for (int i = 0; i < num_items; i++) 
        file >> value[i];

    for (int i = 0; i < num_items; i++) 
        file >> weight[i];

    vector<vector<pair<int, int>>> penalties(num_items);

    for (int i = 0; i < num_penalties; i++) {
        int temp1, penalidade, temp2, item_1, item_2; //temp1 e temp2 são descartados
        file >> temp1 >> penalidade >> temp2 >> item_1 >> item_2;
        penalties[item_1].push_back(make_pair(item_2, penalidade));
        penalties[item_2].push_back(make_pair(item_1, penalidade));
    }

    file.close();

    file_records result = {num_items, knapsack_capacity, value, weight, penalties};
    return result;
}

void log_dataset(file_records dataset){
    
    cout << "Number of itens: " << dataset.num_items << endl; 
    cout << "Knapsack capacity: " << dataset.knapsack_capacity << endl;

    cout << "=================VALUE=================" << endl;
    for (int i=0; i < dataset.num_items; i++)
        cout << "[" << i << "] = " << dataset.value[i] << endl; 
    cout << "=================WEIGHT=================" << endl;
    for (int i=0; i < dataset.num_items; i++)
        cout << "[" << i << "] = " << dataset.weight[i] << endl; 

    cout << "=================PENALTIES=================" << endl;
    for (int i = 0; i < dataset.num_items; i++) {
        cout << "Penalties for item " << i << " -> [";
        int count = 0;
        for (auto par : dataset.penalties[i]) {
            cout << "(" << par.first << ", " << par.second << ")";
            count ++;
            if (count < dataset.penalties[i].size())
                cout << ", ";
        }
        cout << "]" << endl;
    }
}