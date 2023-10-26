#include "helper.h"

int main() {

    file_records dataset = read_entry_file("sum_instances/500/kpf_1_sum.txt");
    log_dataset(dataset);

    return 0;
}