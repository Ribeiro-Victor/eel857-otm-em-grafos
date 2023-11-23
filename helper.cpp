#include "helper.h"

using namespace std;

file_records read_entry_file(string filename){
    ifstream file(filename); 
    if (!file.is_open()) {
        cerr << "Erro ao abrir o arquivo." << endl;
        exit (1);
    }

    // Read first line
    int num_items, num_penalties, knapsack_capacity;
    file >> num_items >> num_penalties >> knapsack_capacity;

    // Initialize items vector
    vector<item> items(num_items);
    // Define the items' labels, values and weights
    for (int i = 0; i < num_items; i++) {
        file >> items[i].value;
        items[i].label = i;
    }
    for (int i = 0; i < num_items; i++) 
        file >> items[i].weight;

    // Initialize penalties map for each item
    for (int i = 0; i < num_penalties; i++) {
        int temp1, penalty, temp2, item_1, item_2; //temp1 e temp2 são descartados
        file >> temp1 >> penalty >> temp2 >> item_1 >> item_2;
        
        
        items[item_1].penalties[&items[item_2]] = penalty;
        items[item_2].penalties[&items[item_1]] = penalty;
    }

    file.close();

    file_records result = {num_items, knapsack_capacity, items};
    return result;
}

void log_dataset(file_records* dataset){
    
    cout << "Number of itens: " << dataset->num_items << endl; 
    cout << "Knapsack capacity: " << dataset->knapsack_capacity << endl;

    cout << "=================VALUE=================" << endl;
    for (int i=0; i < dataset->num_items; i++)
        cout << "[" << i << "] = " << dataset->items[i].value << endl; 
    cout << "=================WEIGHT=================" << endl;
    for (int i=0; i < dataset->num_items; i++)
        cout << "[" << i << "] = " << dataset->items[i].weight << endl; 

    cout << "=================PENALTIES=================" << endl;
    for (int i = 0; i < dataset->num_items; i++) {
        cout << "Penalties for item " << i << " -> [";
        int count = 0;
        for (auto pair : dataset->items[i].penalties) {
            cout << "(" << pair.first->label << ", " << pair.second << ")";
            count ++;
            if (count < dataset->items[i].penalties.size())
                cout << ", ";
        }
        cout << "]" << endl;
    }
    cout << "============================================" << endl;
}

float calculate_penalties(vector<item>* items, vector<int>* solution, int item_index) {
    // If solution is empty
    if (item_index == 0) {
        cout<<"        Solution is empty. Penalty = 0."<<endl;
        return 0;
    }

    // Initialize variables
    map<item*, int> penalties = items->at(item_index).penalties;
    float sum = 0;
    cout<<items->at(item_index).label<<endl;
    
    // If the item has no pairs with penalties
    if (penalties.size()==0)
        cout<<"        Item "<<item_index<<" doesn't have penalty pairs."<<endl;
    
    for (auto pair: penalties) {
        cout<<pair.first->label<<endl;
        if (solution->at(pair.first->label) == 0)
            cout<<"        Item "<<pair.first->label<<" is NOT in the solution."<<endl;
        else {
            cout<<"        Item "<<pair.first->label<<" is in the solution and it's penalty with item"<<item_index<<" is: "<<pair.second<<endl;
            sum += pair.second;
        }
        cout<<"Debug"<<endl;
        
    }
    return sum;
}

bool compare(item& a, item& b) {
    float score_a = static_cast<float>(a.value)/a.weight;
    float score_b = static_cast<float>(b.value)/b.weight;

    return score_a > score_b;
}

vector<int> constructive(file_records* dataset){
    // Iitialize solution vector
    vector<int> solution(dataset->num_items, 0);
    int current_knapsack_weight = 0;

    // Log
    cout<<"Items: "<<dataset->num_items<<endl;
    cout<<"Knapsack capacity: "<<dataset->knapsack_capacity<<endl;

    // 
    vector<item> items = dataset->items;
    sort(items.begin(), items.end(), compare);

    // Calculate ratio value/weight
    for (int i=0; i<items.size(); i++) {
        // If the item fits in the knapsack
        cout<<"Item: "<<i<<endl;
        cout<<"    Current knapsack weight: "<<current_knapsack_weight<<endl;
        cout<<"    Item weight: "<<items[i].weight<<endl;
        if(current_knapsack_weight + items[i].weight <= dataset->knapsack_capacity) {
            // Calculate the item score
            float score = (items[i].value - calculate_penalties(&items, &solution, i))/items[i].weight;
            cout<<"    Calculate item "<<i<<" score:"<<score<<endl;
            
            // Include item in the solution
            if (score > 0) { //TODO: Incluir mais condições
                cout<<"    Include item "<<i<<" in the solution."<<endl;
                solution[items[i].label] = 1;
                current_knapsack_weight += items[i].weight;
            }
            // Print solution
            cout<<"    Solution = [";
            for (int i=0; i<solution.size(); i++)
                cout<<solution[i];
            cout<<"]"<<endl;
        }
    }
    return solution;
}

int avaliation(vector<int>* solution, file_records* dataset) {
    cout<<"Avaliation"<<endl;
    int sum = 0;
    for (int i=0; i < solution->size(); i++) {
        if (solution->at(i) == 1) {
            sum += dataset->items[i].value - calculate_penalties(&dataset->items, solution, i);
        }
    }

    cout<<"Score: "<<sum<<endl;
    return sum;
}
