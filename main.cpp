#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <iomanip>
#include <mutex>


using namespace std;


// Create a structure to hold the data for each thread to make its calculation
struct ThreadData {
    int row;
    int col;
    const vector<vector<int>>* A;
    const vector<vector<int>>* B;
    vector<vector<int>>* C;
    mutex* mtx;
};




// Function to compute the element of the result matrix
void calculateElement(ThreadData data) {
    int a = data.A->size();
    int b = data.B->size();
    int sum = 0;


    // Multiply row A by column B
    for (int i = 0; i < a; ++i) {
        sum += (*data.A)[data.row][i] * (*data.B)[i][data.col];
    }


    // Lock the mutex to safely update the result matrix
    lock_guard<mutex> lock(*data.mtx);
    (*data.C)[data.row][data.col] = sum;
}


// Function to print the matrix
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int elem : row) {
            cout << setw(4) << elem << " ";
        }
        cout << endl;
    }
    cout << endl;
}


int main() {
    // Get matrix parameters
    int p, q, r;
    std::cout << "Enter the dimensions of the matrices (p, q, r): ";
    std::cin >> p >> q >> r;


    // End program if input is invalid
    if (p >= 100 || q >= 100 || r >= 100) {
        std::cout << "Matrix dimensions should be less than 100." << std::endl;
        return 1;
    }


    // Create random generator for 0-9
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);


    // Create matrix A (p x q)
    vector<vector<int>> A(p, vector<int>(q));
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < q; j++) {
            A[i][j] = dis(gen);
        }
    }


    // Create matrix B (q x r)
    vector<vector<int>> B(q, vector<int>(r));
    for (int i = 0; i < q; i++) {
        for (int j = 0; j < r; j++) {
            B[i][j] = dis(gen);
        }
    }
    
    // Create matrix C (p x r) with all zeroes
    vector<vector<int>> C(p, vector<int>(r, 0));


    vector<thread> threads;
    mutex mtx;


    // Spawn threads for each element and execute at once
    for (int i = 0; i < p; i++) {
        for (int j = 0; j < r; j++) {
            ThreadData data = {i, j, &A, &B, &C, &mtx};
            threads.emplace_back(calculateElement, data);
        }
    }


    // Wait for all threads to finish to avoid conflicts
    for (auto& t : threads) {
        t.join();
    }


    cout << "Matrix A:" << endl;
    printMatrix(A);


    cout << "Matrix B:" << endl;
    printMatrix(B);


    cout << "Matrix C (Result):" << endl;
    printMatrix(C);
    
    return 0;
}