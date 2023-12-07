#include "helper.h"

using namespace std;

bool DEBUG_ENABLED = false;

void debug_msg(const string &message)
{
    if (DEBUG_ENABLED)
        cout << message << endl;
}

file_records read_entry_file(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Erro ao abrir o arquivo." << endl
             << filename << endl;
        exit(1);
    }

    // Read first line
    int num_items, num_penalties, knapsack_capacity;
    file >> num_items >> num_penalties >> knapsack_capacity;

    // Initialize items vector
    vector<item> items(num_items);
    // Define the items' labels, values and weights
    for (int i = 0; i < num_items; i++)
    {
        file >> items[i].value;
        items[i].label = i;
    }
    for (int i = 0; i < num_items; i++)
        file >> items[i].weight;

    // Initialize penalties map for each item
    for (int i = 0; i < num_penalties; i++)
    {
        int temp1, penalty, temp2, item_1, item_2; // temp1 e temp2 são descartados
        file >> temp1 >> penalty >> temp2 >> item_1 >> item_2;
        items[item_1].penalties.push_back(make_pair(item_2, penalty));
        items[item_2].penalties.push_back(make_pair(item_1, penalty));
    }

    file.close();
    file_records result = {num_items, knapsack_capacity, items};
    return result;
}

unordered_map<string, string> read_config_file(const string &filename)
{
    unordered_map<string, string> config;

    ifstream configFile(filename);

    if (!configFile.is_open())
    {
        cerr << "Error opening config file: " << filename << endl;
        return config;
    }

    string line;
    while (getline(configFile, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        istringstream iss(line);
        string key, value;
        if (getline(iss, key, '=') && getline(iss, value))
        {
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            config[key] = value;
        }
    }

    configFile.close();

    return config;
}

void log_dataset(file_records *dataset)
{

    cout << "Number of itens: " << dataset->num_items << endl;
    cout << "Knapsack capacity: " << dataset->knapsack_capacity << endl;

    cout << "=================VALUE=================" << endl;
    for (int i = 0; i < dataset->num_items; i++)
        cout << "[" << i << "] = " << dataset->items[i].value << endl;
    cout << "=================WEIGHT=================" << endl;
    for (int i = 0; i < dataset->num_items; i++)
        cout << "[" << i << "] = " << dataset->items[i].weight << endl;

    cout << "=================PENALTIES=================" << endl;
    for (int i = 0; i < dataset->num_items; i++)
    {
        cout << "Penalties for item " << i << " -> [";
        int count = 0;
        for (auto pair : dataset->items[i].penalties)
        {
            cout << "(" << pair.first << ", " << pair.second << ")";
            count++;
            if (count < dataset->items[i].penalties.size())
                cout << ", ";
        }
        cout << "]" << endl;
    }
    cout << "============================================" << endl;
}

float calculate_penalties(vector<item> *items, vector<int> *solution, int item_index)
{
    // If solution is empty
    if (item_index == 0)
    {
        debug_msg("        Solution is empty. Penalty = 0.");
        return 0;
    }

    // Initialize variables
    vector<pair<int, int>> penalties = items->at(item_index).penalties;
    float sum = 0;
    debug_msg(to_string(items->at(item_index).label));

    // If the item has no pairs with penalties
    if (penalties.size() == 0)
        debug_msg("        Item " + to_string(item_index) + " doesn't have penalty pairs.");

    for (auto pair : penalties)
    {
        debug_msg(to_string(pair.first));
        if (solution->at(pair.first) == 0)
            debug_msg("        Item " + to_string(pair.first) + " is NOT in the solution.");
        else
        {
            debug_msg("        Item " + to_string(pair.first) + " is in the solution and it's penalty with item " + to_string(item_index) + " is: " + to_string(pair.second));
            sum += pair.second;
        }
    }
    return sum;
}

bool compare(item &a, item &b)
{
    float score_a = static_cast<float>(a.value) / a.weight;
    float score_b = static_cast<float>(b.value) / b.weight;

    return score_a > score_b;
}

vector<int> constructive(file_records *dataset)
{
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
    for (int i = 0; i < items.size(); i++)
    {
        // If the item fits in the knapsack
        debug_msg("Item: " + to_string(i));
        debug_msg("    Current knapsack weight: " + to_string(current_knapsack_weight));
        debug_msg("    Item weight: " + to_string(items[i].weight));
        if (current_knapsack_weight + items[i].weight <= dataset->knapsack_capacity)
        {
            // Calculate the item score
            float score = (items[i].value - calculate_penalties(&items, &solution, i)) / items[i].weight;
            debug_msg("    Calculate item " + to_string(i) + " score:" + to_string(score));

            // Include item in the solution
            if (score > 0)
            { // TODO: Incluir mais condições
                debug_msg("    Include item " + to_string(i) + " in the solution.");
                solution[items[i].label] = 1;
                current_knapsack_weight += items[i].weight;
            }
            // Print solution
            if (DEBUG_ENABLED)
            {
                cout << "    Solution = [";
                for (int i = 0; i < solution.size(); i++)
                    cout << solution[i];
                cout << "]" << endl;
            }
        }
    }
    return solution;
}

int avaliate_solution(vector<int> *solution, file_records *dataset)
{
    debug_msg("********** EVALUATION FUNCTION **********");
    int sum = 0;
    for (int i = 0; i < solution->size(); i++)
    {
        if (solution->at(i) == 1)
        {
            int total_penalty = 0;
            for (auto pair : dataset->items[i].penalties)
            {
                if (pair.first < i && solution->at(pair.first) == 1)
                    total_penalty += pair.second;
            }
            sum += dataset->items[i].value - total_penalty;
        }
    }
    return sum;
}

int get_solution_weight(vector<int> *solution, file_records *dataset)
{
    int total_weight = 0;
    for (int i = 0; i < dataset->num_items; i++)
    {
        total_weight += solution->at(i) * dataset->items[i].weight;
    }
    return total_weight;
}

vector<int> generate_neighbor(vector<int> *solution, file_records *dataset)
{
    // Generate a viable solution by swapping two itens in backpack
    debug_msg("********** GENERATE NEIGHBOR FUNCTION **********");
    srand((unsigned)time(NULL));
    int max_iter = 10000;

    int n = solution->size();
    vector<int> new_solution;
    new_solution = *solution;

    int iter = 0;
    while (iter < max_iter)
    {
        int i = rand() % n, j = rand() % n;
        if (solution->at(i) != solution->at(j))
        { // Check if we're swapping itens
            new_solution = *solution;
            new_solution[i] = !new_solution[i];
            new_solution[j] = !new_solution[j];

            if (get_solution_weight(&new_solution, dataset) <= dataset->knapsack_capacity)
            { // Check if new solution is viable
                int old_score = avaliate_solution(solution, dataset);
                int new_score = avaliate_solution(&new_solution, dataset);
                if (old_score < new_score) // Check if new solution is better
                    return new_solution;
            }
            else
            {
                new_solution = *solution;
            }
        }
        iter++;
    }
    return new_solution;
}

// vector<int> generate_neighbor_destroying(vector<int> *solution, file_records *dataset)
// {
//     // Generate a viable solution by removing 5 to 25% itens in backpack
//     debug_msg("********** GENERATE NEIGHBOR FUNCTION **********");
//     srand((unsigned)time(NULL));
//     int max_iter = 10000;

//     int n = solution->size();
//     vector<int> new_solution;
//     new_solution = *solution;

//     vector<int> indicesInSolution;
//     vector<int> indicesOutSolution;
//     for (int i = 0; i < solution->size(); ++i)
//     {
//         if (solution->at(i) == 1)
//         {
//             indicesInSolution.push_back(i);
//         }
//         else
//         {
//             indicesOutSolution.push_back(i);
//         }
//     }

//     // Determine the percentage of items to remove (between 5% and 10%)
//     double minPercentage = 0.01;
//     double maxPercentage = 0.01;
//     double range = maxPercentage - minPercentage;
//     double removalPercentage = minPercentage + static_cast<double>(rand()) / RAND_MAX * range;
//     int numToRemove = static_cast<int>(removalPercentage * indicesInSolution.size());

//     // Generate random indices to remove
//     set<int> indicesToRemove;
//     while (indicesToRemove.size() < numToRemove && !indicesInSolution.empty())
//     {
//         int randomIndex = rand() % indicesInSolution.size();
//         auto insertResult = indicesToRemove.insert(indicesInSolution[randomIndex]);
//         // Check if the insertion actually occurred (not a duplicate)
//         if (insertResult.second)
//             indicesInSolution.erase(indicesInSolution.begin() + randomIndex);
//     }

//     // Remove items at the selected indices
//     for (int index : indicesToRemove)
//     {
//         new_solution[index] = 0;
//     }

//     // Determine the percentage of items to insert
//     double insertPercentage = minPercentage + static_cast<double>(rand()) / RAND_MAX * range;
//     int numToInsert = static_cast<int>(insertPercentage * indicesOutSolution.size());

//     // Generate random indices to insert
//     set<int> indicesToInsert;
//     while (indicesToInsert.size() < numToInsert && !indicesOutSolution.empty())
//     {
//         int randomIndex = rand() % indicesOutSolution.size();
//         auto insertResult = indicesToInsert.insert(indicesOutSolution[randomIndex]);
//         // Check if the insertion actually occurred (not a duplicate)
//         if (insertResult.second)
//             indicesOutSolution.erase(indicesOutSolution.begin() + randomIndex);
//     }

//     // Insert items at the selected indices
//     for (int index : indicesToInsert)
//     {
//         new_solution[index] = 1;
//     }

//     // vector<item> items = dataset->items;
//     // sort(items.begin(), items.end(), compare);

//     // int current_knapsack_weight = get_solution_weight(&new_solution, dataset);
//     // // Calculate ratio value/weight
//     // for (int i=0; i<items.size(); i++) {
//     //     if(indicesToRemove.find(items[i].label) == indicesToRemove.end()){ // If item was not removed from backpack
//     //         if(current_knapsack_weight + items[i].weight <= dataset->knapsack_capacity) {
//     //             // Calculate the item score
//     //             float score = (items[i].value - calculate_penalties(&items, &new_solution, i))/items[i].weight;

//     //             // Include item in the solution
//     //             if (score > 0) { //TODO: Incluir mais condições
//     //                 new_solution[items[i].label] = 1;
//     //                 current_knapsack_weight += items[i].weight;
//     //             }

//     //         }
//     //     }
//     // }

//     // cout << "old solution:" << endl;
//     // for (int i=0; i<solution->size(); i++)
//     //     cout<<solution->at(i);
//     // cout<<"]"<<endl;

//     //  cout << "new solution:" << endl;
//     // for (int i=0; i<new_solution.size(); i++)
//     //     cout<<new_solution[i];
//     // cout<<"]"<<endl;

//     return new_solution;
// }

// vector<int> simulated_annealing(int AS_max, int T_end, int t_0, double alpha, file_records* dataset){
//     vector<int> curr_solution = constructive(dataset); // curr_solution = s'
//     cout<<"Initial solution: R$"<< avaliate_solution(&curr_solution, dataset) <<endl;
//     vector<int> best_solution = curr_solution; // best_solution = s*

//     int iter = 0;
//     int t = t_0;
//     vector<int> new_solution;
//     srand((unsigned) time(NULL));
//     while (t > T_end){
//         while (iter < AS_max){
//             iter ++;
//             new_solution = generate_neighbor(&curr_solution, dataset); // new_solution = s''
//             // new_solution = generate_neighbor_destroying(&curr_solution, dataset);
//             int old_score = avaliate_solution(&curr_solution, dataset);
//             int new_score = avaliate_solution(&new_solution, dataset);
//             int delta = new_score - old_score;
//             if (delta > 0){
//                 curr_solution = new_solution;
//                 int best_score = avaliate_solution(&best_solution, dataset);
//                 if (new_score > best_score){
//                     best_solution = curr_solution;
//                     cout << "Found better solution: R$" << best_score << endl;
//                 }
//             }else{
//                 double rand_between_0_1 = static_cast<double>(rand()) / RAND_MAX;
//                 if( rand_between_0_1 < exp( -delta/t ) )
//                     curr_solution = new_solution;
//             }
//         }
//         t = alpha * t;
//         iter = 0;
//     }

//     return best_solution;
// }

// vector<int> simulated_annealing(int AS_max, int T_end, int t_0, double alpha, file_records *dataset)
// {
//     vector<int> curr_solution = constructive(dataset); // curr_solution = s'
//     cout << "Initial solution: R$" << avaliate_solution(&curr_solution, dataset) << endl;
//     vector<int> best_solution = curr_solution; // best_solution = s*

//     int iter = 0;
//     int t = t_0;
//     vector<int> new_solution;
//     srand((unsigned)time(NULL));
//     while (t > T_end)
//     {
//         while (iter < AS_max)
//         {
//             iter++;
//             new_solution = generate_neighbor(&curr_solution, dataset); // new_solution = s''
//             int old_score = avaliate_solution(&curr_solution, dataset);
//             int new_score = avaliate_solution(&new_solution, dataset);
//             int delta = new_score - old_score;
//             if (delta > 0)
//             {
//                 curr_solution = new_solution;
//                 int best_score = avaliate_solution(&best_solution, dataset);
//                 if (new_score > best_score)
//                 {
//                     best_solution = curr_solution;
//                     // cout << "Found better solution: R$" << new_score << endl;
//                 }
//             }
//             else
//             {
//                 double rand_between_0_1 = static_cast<double>(rand()) / RAND_MAX;
//                 if (rand_between_0_1 < exp(-delta / t))
//                     curr_solution = new_solution;
//             }
//         }
//         t = alpha * t;
//         iter = 0;
//     }

//     return best_solution;
// }

vector<int> greedy_fill(vector<int> *solution, file_records *dataset)
{
    bool item_added;

    do
    {
        item_added = false;
        int best_value_increase = 0;
        int best_item_index = -1;

        for (int i = 0; i < solution->size(); i++)
        {
            if (solution->at(i) == 0)
            {
                vector<int> potential_solution = *solution;
                potential_solution[i] = 1;

                if (get_solution_weight(&potential_solution, dataset) <= dataset->knapsack_capacity)
                {
                    int current_value = avaliate_solution(solution, dataset);
                    int new_value = avaliate_solution(&potential_solution, dataset);
                    int value_increase = new_value - current_value;

                    if (value_increase > best_value_increase)
                    {
                        best_value_increase = value_increase;
                        best_item_index = i;
                    }
                }
            }
        }

        if (best_item_index != -1)
        {
            solution->at(best_item_index) = 1;
            item_added = true;
        }

    } while (item_added);

    return *solution;
}

void remove_elements(vector<int> *solution, int number_of_elements)
{
    vector<int> indices_with_items;

    for (int i = 0; i < solution->size(); i++)
    {
        if (solution->at(i) == 1)
        {
            indices_with_items.push_back(i);
        }
    }

    for (int i = 0; i < number_of_elements; i++)
    {
        if (!indices_with_items.empty())
        {
            int random_index = rand() % indices_with_items.size();
            int item_to_remove = indices_with_items[random_index];

            solution->at(item_to_remove) = 0;

            indices_with_items.erase(indices_with_items.begin() + random_index);
        }
    }
}

vector<int> local_search(vector<int>* solution, file_records* dataset) {
    int n = solution->size();
    int a, b, c, d, a_total_penalty, b_total_penalty = 0;
    vector<int> new_solution = *solution;
    

    // Randomly removes an item from the solution
    a = rand() % n;
    new_solution.erase(new_solution.begin() + a);
    
    // Calculates weight
    int original_solution_weight = get_solution_weight(solution, dataset);
    int current_space = original_solution_weight - dataset->items[a].weight;
    
    // Randomly chooses two items to swap.
    while(solution->at(a) == solution->at(b)) {
        a = rand() % n;
        b = rand() % n;
    }

    // For each 'a' penalty pair
    for (auto pair: dataset->items[a].penalties){
        // If the item is in the solution
        // and the item is not 'b'
        if (solution->at(pair.first) == 1 && pair.first != b)
            // Increment a_total_penalty
            a_total_penalty += pair.second;
    }

    // For each 'b' penalty pair
    for (auto pair: dataset->items[b].penalties){
        // If the item is in the solution
        // and the item is not 'a'
        if (solution->at(pair.first) == 1 && pair.first != a)
            // Increment b_total_penalty
            b_total_penalty += pair.second;
    }

    // If it's worth it
    if (a_total_penalty > b_total_penalty) {
        // Make swap
        new_solution[a] = !new_solution[a];
        new_solution[b] = !new_solution[b];
    }
    
    return new_solution;
}

// Função para calcular a Hamming Distance entre dois vetores binários
int hamming_distance(std::vector<int> *v1, std::vector<int> *v2) {
    // Verificar se os vetores têm o mesmo tamanho
    if (v1->size() != v2->size()) {
        std::cerr << "Erro: Os vetores devem ter o mesmo tamanho." << std::endl;
        return -1;  // Valor de retorno indicando erro
    }

    int distance = 0;

    // Calcular a Hamming Distance
    for (size_t i = 0; i < v1->size(); ++i) {
        if (v1[i] != v2[i]) {
            distance++;
        }
    }

    return distance;
}

// Função para encontrar o índice do vetor com a menor Hamming Distance em relação a curr_solution
int findClosestSolutionIndex(vector<Cluster> *clusters, vector<int> *curr_solution) {
    int minDistance = std::numeric_limits<int>::max();
    int closestSolutionIndex = -1;
    vector<int> center_solution;


    // Iterar sobre todos os clusters
    for (int i = 0; i < clusters->size(); ++i) {
        center_solution = clusters->at(i).get_center_solution();
        int distance = hamming_distance(&center_solution, curr_solution);

        // Atualizar o índice mais próximo se encontrarmos uma distância menor
        if (distance < minDistance) {
            minDistance = distance;
            closestSolutionIndex = static_cast<int>(i);
        }
    }

    return closestSolutionIndex;
}

vector<int> simulated_annealing(int AS_max, int T_end, int t_0, double alpha, int cluster_volume_threshold, int max_number_of_clusters, int max_inefficiency_index, file_records *dataset)
{
    vector<int> curr_solution = constructive(dataset); // curr_solution = s'
    cout << "Initial solution: R$" << avaliate_solution(&curr_solution, dataset) << endl;
    vector<int> best_solution = curr_solution; // best_solution = s*

    int reheatLimit = 3;
    int reheatCounter = 0;
    int iter = 0;
    int t = t_0;
    int old_score, new_score = 0;
    int i = 0;
    vector<int> new_solution;
    vector<Cluster> clusters;
    vector<int> center_solution;
    srand((unsigned)time(NULL));

    while (t > T_end)
    {
        while (iter < AS_max)
        {
            iter++;
            new_solution = generate_neighbor(&curr_solution, dataset);
            old_score = avaliate_solution(&curr_solution, dataset);
            new_score = avaliate_solution(&new_solution, dataset);
            int delta = new_score - old_score;
            if (delta > 0)
            {
                curr_solution = new_solution;
                int best_score = avaliate_solution(&best_solution, dataset);
                if (new_score > best_score)
                {
                    // cout << "New Solution: " << new_score << endl;
                    best_solution = curr_solution;
                }
            }
            else
            {
                double rand_between_0_1 = static_cast<double>(rand()) / RAND_MAX;
                if (rand_between_0_1 < exp(-delta / t))
                    curr_solution = new_solution;
            }
        }

        if (t - T_end <= 100 && reheatCounter < reheatLimit)
        {

            remove_elements(&curr_solution, 3);

            curr_solution = greedy_fill(&curr_solution, dataset);

            t = t_0 * 0.8 - (reheatCounter * 100);

            reheatCounter++;
        }
        else
        {
            t = alpha * t;
            iter = 0;
        }

        // Cluster é uma tupla (ci, vi, ri)
        // ci (center) repesenta a melhor solução no cluster i
        // vi (volume) indica o número de soluções atribuídas a um cluster i,
        // ri (inefficiency index) está  relacionado ao numero de vezes que o cluster i esteve promissor mas gerou uma
        // melhora a partir de uma busca local nele.

        // Uma solução é gerada e atribuída ao cluster mais similar i considerando a métrica de similaridade.
        // Cluster i se torna promissor quando um volume vi atinge o threshold lambda, e uma perturbação é aplicada ao
        // centro ci quando uma busca local roda 'mi' vezes sem gerar melhoria.
        
        // gamma: Número de clusters
        // lambda: Limite de volume do cluster
        // mi: limite de ineficiência do cluster
        
        // Se o número máximo de clusters ainda não foi atingido
        if (clusters.size() < max_number_of_clusters) {// 15 if clt < γ then
            // Criar novo cluster com a solução
            // 16 clt ← clt + 1; vclt ← vclt + 1; cclt ← s;
            Cluster cluster(curr_solution);
            clusters.push_back(cluster);
        } else { // 17 else
            // Incluir solução num cluster seguindo critérios de semelhança
            // Se um cluster for promissor, aplicar perturbação ou busca local em ci (centro)
            // 18 i← min [Hi(s)]; vi ← vi + 1; ci ← Best(ci, s);
            i = findClosestSolutionIndex(&clusters, &curr_solution);
            clusters[i].add_solution(curr_solution);
            if (clusters[i].get_volume() == cluster_volume_threshold) {// 19 if vi = λ then
                // 20 vi ← 1;
                clusters[i].clear_cluster();
                center_solution = clusters[i].get_center_solution();
                if (clusters[i].get_inefficiency_index() == max_inefficiency_index) {// 21 if ri = μ then
                    // 22 ri ← 0; ci ← ψ(ci);
                    clusters[i].reset_inefficiency_index();
                    clusters[i].set_center_solution(generate_neighbor(&center_solution, dataset));
                } else {// 23 else
                    // 24 ci ← Local Search(ci);
                    clusters[i].set_center_solution(local_search(&center_solution, dataset));
                    // 25 if ci is improved then ri ← 0; else ri ← ri + 1; end
                    if (avaliate_solution(&center_solution, dataset) > new_score) {
                        clusters[i].reset_inefficiency_index();
                    } else {
                        clusters[i].update_inefficiency_index();
                    }
                }// 26 end
                // 27 Update the best solution found (s*, ci);
                best_solution = center_solution;
            } // 28 end
        } // 29 end

    }

    return best_solution;
}

