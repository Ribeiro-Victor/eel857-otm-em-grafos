#include "helper.h"

using namespace std;

bool DEBUG_ENABLED = false;

void debug_msg(const string& message){
    if (DEBUG_ENABLED)
        cout << message << endl;
}

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
        items[item_1].penalties.push_back(make_pair(item_2, penalty));
        items[item_2].penalties.push_back(make_pair(item_1, penalty));
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
            cout << "(" << pair.first << ", " << pair.second << ")";
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
        debug_msg("        Solution is empty. Penalty = 0.");
        return 0;
    }

    // Initialize variables
    vector<pair<int, int>> penalties = items->at(item_index).penalties;
    float sum = 0;
    debug_msg(to_string(items->at(item_index).label));
    
    // If the item has no pairs with penalties
    if (penalties.size()==0)
        debug_msg("        Item " + to_string(item_index) + " doesn't have penalty pairs.");
    
    for (auto pair: penalties) {
        debug_msg(to_string(pair.first));
        if (solution->at(pair.first) == 0)
            debug_msg("        Item " + to_string(pair.first) + " is NOT in the solution.");
        else {
            debug_msg("        Item " + to_string(pair.first) + " is in the solution and it's penalty with item "
            + to_string(item_index) + " is: " + to_string(pair.second));
            sum += pair.second;
        }
        
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
    debug_msg("********** CONSTRUCTIVE FUNCTION **********");
    debug_msg("Items: " + to_string(dataset->num_items));
    debug_msg("Knapsack capacity: " + dataset->knapsack_capacity);

    // 
    vector<item> items = dataset->items;
    sort(items.begin(), items.end(), compare);

    // Calculate ratio value/weight
    for (int i=0; i<items.size(); i++) {
        // If the item fits in the knapsack
        debug_msg("Item: " + to_string(i));
        debug_msg("    Current knapsack weight: " + to_string(current_knapsack_weight));
        debug_msg("    Item weight: " + to_string(items[i].weight));
        if(current_knapsack_weight + items[i].weight <= dataset->knapsack_capacity) {
            // Calculate the item score
            float score = (items[i].value - calculate_penalties(&items, &solution, i))/items[i].weight;
            debug_msg("    Calculate item " + to_string(i) + " score:" + to_string(score));
            
            // Include item in the solution
            if (score > 0) { //TODO: Incluir mais condições
                debug_msg("    Include item " + to_string(i) + " in the solution.");
                solution[items[i].label] = 1;
                current_knapsack_weight += items[i].weight;
            }
            // Print solution
            if(DEBUG_ENABLED){
                cout<<"    Solution = [";
                for (int i=0; i<solution.size(); i++)
                    cout<<solution[i];
                cout<<"]"<<endl;
            }
        }
    }
    return solution;
}

int avaliate_solution(vector<int>* solution, file_records* dataset) {
    debug_msg("********** AVALIATION FUNCTION **********");
    int sum = 0;
    for (int i=0; i < solution->size(); i++) {
        if (solution->at(i) == 1) {
            int total_penalty = 0;
            for (auto pair: dataset->items[i].penalties){
                if (pair.first < i && solution->at(pair.first) == 1)
                    total_penalty += pair.second;
            }
            sum += dataset->items[i].value - total_penalty;
        }
    }
    return sum;
}

int get_solution_weight(vector<int>* solution, file_records* dataset){
    int total_weight = 0;
    for(int i = 0; i < dataset->num_items; i++){
        total_weight += solution->at(i) * dataset->items[i].weight;
    }
    return total_weight;
} 

vector<int> generate_neighbor(vector<int>* solution, file_records* dataset){
    //Generate a viable solution by swapping two itens in backpack
    debug_msg("********** GENERATE NEIGHBOR FUNCTION **********");
    srand((unsigned) time(NULL));
    int max_iter = 10000;

    int n = solution->size();
    vector<int> new_solution;
    new_solution = *solution;

    int iter = 0;
    while(iter < max_iter){
        int i = rand() % n, j = rand() % n;
        if(solution->at(i) != solution->at(j)){ //Check if we're swapping itens
            new_solution = *solution;
            new_solution[i] = !new_solution[i];
            new_solution[j] = !new_solution[j];


            if(get_solution_weight(&new_solution, dataset) <= dataset->knapsack_capacity){ //Check if new solution is viable
                int old_score = avaliate_solution(solution, dataset);
                int new_score = avaliate_solution(&new_solution, dataset);
                if (old_score < new_score)// Check if new solution is better
                    return new_solution;
            }else{
                new_solution = *solution;
            }
        }
        iter++;
    }

    return new_solution;

}