#include <iostream>
#include <vector>
#include <omp.h>

void processImage(std::vector<std::vector<int>>& image) {
    size_t rows = image.size();
    size_t cols = image[0].size();

    // Paraleliza os loops aninhados usando collapse(2)
    #pragma omp parallel for collapse(2)
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            image[i][j] = (image[i][j] * 2) % 256;
        }
    }
}

int main() {
    std::vector<std::vector<int>> img{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    processImage(img);
    for (auto& row : img) {
        for (auto& col : row) {
            std::cout << col << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}
