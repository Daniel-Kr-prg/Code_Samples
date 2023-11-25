#include "sort.h"
#include <iostream>

const int SIZE_TO_GO_SEQ = 50;

/*#pragma omp parallel for
for (int i = 0; i < vector_to_sort.size(); i++) {
    auto str = vector_to_sort[i];
    buckets[mappingFunction(str->at(string_lengths - 1))].push_back(str);
}*/

/*std::vector<std::string*> vector_new;
for (auto& bucket : buckets) {
    vector_new.insert(vector_new.end(), bucket.begin(), bucket.end());
}
vector_to_sort = &vector_new;*/



// implementace vaseho radiciho algoritmu. Detalnejsi popis zadani najdete v "sort.h"
void radix_par(std::vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
               unsigned long alphabet_size, unsigned long string_lengths) 
{

    std::vector<std::vector<std::string*>> buckets(alphabet_size);
    for (auto str : vector_to_sort) 
    {
        buckets[mappingFunction((*str).at(0))].push_back(str);
    }

    #pragma omp parallel
    #pragma omp single
    {
        for (unsigned long i = 0; i < alphabet_size; ++i) 
        {
            std::vector<std::string*>& bucket = buckets[i];
            if (!bucket.empty()) 
            {
                #pragma omp task shared(bucket)
                radix_with_index(bucket, mappingFunction, alphabet_size, string_lengths, 1);
            }
        }
        #pragma omp taskwait
    }

    vector_to_sort.clear();
    for (auto& bucket : buckets)
    {
        vector_to_sort.insert(vector_to_sort.end(), bucket.begin(), bucket.end());
    }

}

void radix_with_index(std::vector<std::string*>& vector_to_sort, const MappingFunction& mappingFunction,
    unsigned long alphabet_size, unsigned long string_lengths, unsigned long current_idx) {
    std::vector<std::vector<std::string*>> buckets(alphabet_size);
    for (auto str : vector_to_sort) {
        buckets[mappingFunction((*str).at(current_idx))].push_back(str);
    }
    if (current_idx < string_lengths-1)
    {
        #pragma omp taskgroup
        {
            for (unsigned long i = 0; i < alphabet_size; ++i) {
                std::vector<std::string*>& bucket = buckets[i];
                if (!bucket.empty()) {
                    if (bucket.size() < SIZE_TO_GO_SEQ)
                    {
                        radix_with_index(bucket, mappingFunction, alphabet_size, string_lengths, current_idx+1);
                    }
                    else
                    {
                        #pragma omp task shared(bucket)
                        radix_with_index(bucket, mappingFunction, alphabet_size, string_lengths, current_idx + 1);
                    }
                }
            }
        }
    }

    vector_to_sort.clear();
    for (auto& bucket : buckets) {
        vector_to_sort.insert(vector_to_sort.end(), bucket.begin(), bucket.end());
    }

}