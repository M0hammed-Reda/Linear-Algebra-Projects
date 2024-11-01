#include <bits/stdc++.h>
using namespace std;

void fastio() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
}

// Takes a matrix as a parameter and print it.
void printMatrix(const vector<vector<double>>& matrix) {
    int rows = (int)matrix.size();
    int cols = (int)matrix[0].size();

    cout << '\n';
    for (int i = 0; i < rows; ++i) {
        cout << "| ";
        for (int j = 0; j < cols; ++j)
            cout << setw(10) << fixed << setprecision(3) << matrix[i][j] << ' ';
        cout << " |" << '\n';
    }
    cout << '\n';
}

// Takes the matrix and two rows to swap these two rows.
void swapRows(vector<vector<double>>& matrix, int row1, int row2) {
    if (row1 != row2) {
        swap(matrix[row1], matrix[row2]);
        cout << "R" << row1 + 1 << "  <--->  R" << row2 + 1 << ":" << '\n';
        printMatrix(matrix); // Print matrix after swap operation and print the notation of this operation.
    }
}

/* 
    This function normalize specific row (change the pivot to be leading 1).
    it takes the matrix and currant row and the pivot as a parameters
    to do the muiltiple operation on this row to make the pivot(leading) = 1.
*/
void normalizeRow(vector<vector<double>>& matrix, int row, double pivot) {
    int cols = (int)matrix[row].size();
    for (int i = 0; i < cols; ++i) // this loop iterate on all row start from col = 0, to last col.
        matrix[row][i] /= pivot; // and divide each element in this row on the pivot to make first non-zero number(leading) = 1.
    cout << "R " << row + 1 << " / " << pivot << " -> R " << row + 1 << ":" << '\n';
    printMatrix(matrix); // Print matrix after normalizing operation and print the notation of this operation.
}

/*
    This function make all numbers below the leading = 0.
    it takes the matrix and the currant row as a parameter and do some operation on it.
    
*/
void eliminateBelow(vector<vector<double>>& matrix, int row) {
    int rows = (int)matrix.size();
    int cols = (int)matrix[0].size();

    for (int i = row + 1; i < rows; ++i) {
        double factor = matrix[i][row] / matrix[row][row];
        cout << "-(" << factor << ") * R" << row + 1 << " + R" << i + 1 << " -> R" << i + 1 << ":" << '\n';
        for (int j = row; j < cols; ++j)
            matrix[i][j] -= factor * matrix[row][j];
        printMatrix(matrix);
    }
}

int findPivotRow(const vector<vector<double>>& matrix, int startRow, int col) {
    int rows = (int)matrix.size();
    int maxRow = startRow;
    double maxValue = fabs(matrix[startRow][col]);

    for (int i = startRow + 1; i < rows; ++i) {
        if (fabs(matrix[i][col]) > maxValue) {
            maxValue = fabs(matrix[i][col]);
            maxRow = i;
        }
    }
    return maxRow;
}

void backSubstitution(vector<vector<double>>& matrix, vector<double>& solution) {
    int n = matrix.size();
    solution.assign(n, 0);

    for (int i = n - 1; i >= 0; --i) {
        solution[i] = matrix[i].back();
        for (int j = i + 1; j < n; ++j)
            solution[i] -= matrix[i][j] * solution[j];
        solution[i] /= matrix[i][i];
    }

    cout << "Solution:" << '\n';
    for (int i = 0; i < n; ++i) {
        cout << "x" << i + 1 << " = " << solution[i] << '\n';
    }
}

void gaussianElimination(vector<vector<double>>& matrix, vector<double>& solution) {
    int rows = (int)matrix.size();

    for (int i = 0; i < rows; ++i) {
        int pivotRow = findPivotRow(matrix, i, i);
        swapRows(matrix, i, pivotRow);
        if (matrix[i][i] != 0)
            normalizeRow(matrix, i, matrix[i][i]);
        eliminateBelow(matrix, i);
    }
    backSubstitution(matrix, solution);
}

void RunSystem() {
    int numEquations, numVariables;
    cout << "Enter the number of variables: ";
    cin >> numVariables;
    cout << "\nEnter the number of equations: ";
    cin >> numEquations;

    vector<vector<double>> matrix(numEquations, vector<double>(numVariables + 1));
    for (int i = 0; i < numEquations; ++i)
        for (int j = 0; j <= numVariables; ++j)
            cin >> matrix[i][j];

    vector<double> solution(numVariables);
    cout << "\nInitial Matrix:" << '\n';
    printMatrix(matrix);
    gaussianElimination(matrix, solution);
}

int main() {
    fastio();
    RunSystem();
    return 0;
}
