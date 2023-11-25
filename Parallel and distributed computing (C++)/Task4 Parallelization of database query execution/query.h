#ifndef DATABASEQUERIES_QUERY_H
#define DATABASEQUERIES_QUERY_H

#include <vector>
#include <functional>
#include <atomic>

template<typename row_t>
using predicate_t = std::function<bool(const row_t &)>;



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    unsigned int row_count = data_table.size();
    bool is_satisfied = false;

    #pragma omp parallel for
    for (unsigned int i = 0; i < row_count; i++) {
        #pragma omp cancellation point for
        auto& row = data_table[i];
        bool all_predicates_satisfied = true;

        for (auto& predicate : predicates) {
            if (!predicate(row)) {
                all_predicates_satisfied = false;
                break;
            }
        }

        if (all_predicates_satisfied) {
            #pragma omp atomic write
            is_satisfied = true;
            #pragma omp cancel for
        }
    }

    return is_satisfied;
}

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    /*bool found = false;
    #pragma omp parallel for
    for (int i = 0; i < predicates.size(); ++i) {
        #pragma omp cancellation point for
        auto& predicate = predicates[i];
        for (auto& row : data_table) {
            if (found)
                break;
            else if (predicate(row)) {
                #pragma omp atomic write
                found = true;
                #pragma omp cancel for
            }
        }
    }
    return found;*/

    unsigned int row_count = data_table.size();
    bool is_satisfied = false;

    #pragma omp parallel for
    for (unsigned int i = 0; i < row_count; i++) {
        #pragma omp cancellation point for
        auto& row = data_table[i];
        bool found_predicate = false;

        for (auto& predicate : predicates) {
            if (predicate(row)) {
                found_predicate = true;
                break;
            }
        }

        if (found_predicate) {
            #pragma omp atomic write
            is_satisfied = true;
            #pragma omp cancel for
        }
    }

    return is_satisfied;
}


#endif //DATABASEQUERIES_QUERY_H
