#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>

std::vector<double> normalize(std::vector<double>& v) {
    double sum = 0.0;
    size_t n = v.size();

    // Calcula a soma dos quadrados dos elementos em paralelo
    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < n; ++i) {
        sum += v[i] * v[i];
    }

    double magnitude = sqrt(sum);

    // Normaliza cada elemento em paralelo
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        v[i] /= magnitude;
    }

    return v;
}

int main() {
    std::vector<double> vec{1.0, 2.0, 3.0, 4.0};
    vec = normalize(vec);
    for (auto v : vec) {
        std::cout << v << " ";
    }
    std::cout << std::endl;
    return 0;
}
