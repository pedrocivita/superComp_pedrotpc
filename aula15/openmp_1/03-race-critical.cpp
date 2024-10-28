#include<iostream>
#include<omp.h>
using namespace std;

int main(){

    int a = 0;
    #pragma omp parallel 
    {
        #pragma omp critical
        a = a + omp_get_thread_num();
    }
    cout << a << endl;


    return 0;
}