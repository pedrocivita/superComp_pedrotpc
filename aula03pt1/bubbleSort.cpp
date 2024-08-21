#include <iostream>

void bubbleSort(int arr[], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j] > arr[j+1]) {
                std::swap(arr[j], arr[j+1]);
            }
        }
    }
}

int main() {
    int vetor[5];
    std::cout << "Digite 5 números: ";
    for(int i = 0; i < 5; i++) {
        std::cin >> vetor[i];
    }

    bubbleSort(vetor, 5);

    std::cout << "Vetor ordenado: ";
    for(int i = 0; i < 5; i++) {
        std::cout << vetor[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
