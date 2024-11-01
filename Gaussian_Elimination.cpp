#include <bits/stdc++.h>
using namespace std;

// Function to print the matrix
// Takes the matrix as a constant reference parameter to avoid copying
void printMatrix(const vector<vector<double>>& matrix) {
    int rows = (int)matrix.size();   // Number of rows in the matrix
    int cols = (int)matrix[0].size(); // Number of columns in the matrix

    cout << '\n';
    for (int i = 0; i < rows; ++i) {
        cout << "| ";
        for (int j = 0; j < cols; ++j)
            cout << setw(10) << matrix[i][j] << ' '; // Print each element with fixed spacing for alignment
        cout << " |" << '\n';
    }
    cout << '\n';
}

// Swaps two rows in the matrix
// Takes the matrix by reference, and indices of the two rows to swap
void swapRows(vector<vector<double>>& matrix, int row1, int row2) {
    if (row1 != row2) { // Only swap if the rows are different
        swap(matrix[row1], matrix[row2]); // Swap entire rows
        cout << "R" << row1 + 1 << "  <--->  R" << row2 + 1 << ":" << '\n';
        printMatrix(matrix); // Print matrix after swap operation
    }
}

/* 
    Normalize a specific row to make the pivot element 1.
    - Parameters:
        - matrix: the matrix to normalize, passed by reference
        - row: the current row to normalize
        - pivot: the pivot element (leading element) in the current row
    - Explanation:
        This function divides each element in the row by the pivot, turning the pivot into 1.
*/
void normalizeRow(vector<vector<double>>& matrix, int row, double pivot) {
    int cols = (int)matrix[row].size(); // Number of columns in the current row
    for (int i = 0; i < cols; ++i)      // Iterate over each column in the row
        matrix[row][i] /= pivot;         // Divide each element by the pivot
    cout << "R " << row + 1 << " / " << pivot << " -> R " << row + 1 << ":" << '\n';
    printMatrix(matrix); // Print matrix after normalization
}

/*
    Eliminate elements below the pivot in a given column.
    - Parameters:
        - matrix: the matrix to perform elimination on, passed by reference
        - row: the pivot row, above which elimination will be performed
    - Explanation:
        This function eliminates the elements below the pivot in a specific column by
        subtracting a multiple of the pivot row from each row below it.
*/
void eliminateBelow(vector<vector<double>>& matrix, int row) {
    int rows = (int)matrix.size();   // Number of rows in the matrix
    int cols = (int)matrix[0].size(); // Number of columns in the matrix

    for (int i = row + 1; i < rows; ++i) { // Iterate over rows below the pivot row
        double factor = matrix[i][row] / matrix[row][row]; // Calculate the elimination factor
        // Print the row operation being performed
        cout << "-(" << factor << ") * R" << row + 1 << " + R" << i + 1 << " -> R" << i + 1 << ":" << '\n';
        for (int j = row; j < cols; ++j)
            matrix[i][j] -= factor * matrix[row][j]; // Subtract scaled pivot row from the current row
        printMatrix(matrix); // Print matrix after each elimination step
    }
}

/*
    Finds the row with the largest absolute value in the given column (partial pivoting).
    Only returns a different row if the pivot element in the current row is zero.
    Parameters:
        - matrix: the matrix to search for the pivot, passed by constant reference
        - startRow: starting row index to search for pivot
        - col: column index where the pivot element is located
    Returns:
        - The index of the row with the largest pivot element in the specified column if the current pivot is zero,
            Otherwise, the startRow is returned as the pivot row.
*/
int findPivotRow(const vector<vector<double>>& matrix, int startRow, int col) {
    int rows = (int)matrix.size();

    // Check if the pivot element in the start row is non-zero
    if (matrix[startRow][col] != 0)
        return startRow; // No need to swap; return current row as pivot

    // Proceed to find the row with the largest absolute value if the pivot element is zero
    int maxRow = startRow;
    double maxValue = 0.0;
    for (int i = startRow; i < rows; ++i) {
        if (fabs(matrix[i][col]) > maxValue) { // Find row with largest absolute value in column
            maxValue = fabs(matrix[i][col]);
            maxRow = i;
        }
    }
    return maxRow;
}

/* 
    Performs back substitution to solve for variables after Gaussian elimination
    Parameters:
        - matrix: augmented matrix in row echelon form
        - solution: vector to store the calculated solution for each variable
*/
void backSubstitution(vector<vector<double>>& matrix, vector<double>& solution) {
    int n = matrix.size();
    solution.assign(n, 0); // Initialize solution vector with zeros

    for (int i = n - 1; i >= 0; --i) { // Start from the last row moving upwards
        solution[i] = matrix[i].back(); // Start with the constant term from the augmented matrix
        for (int j = i + 1; j < n; ++j)
            solution[i] -= matrix[i][j] * solution[j]; // Substitute known values from the solution
        solution[i] /= matrix[i][i]; // Divide by the coefficient of the variable to isolate it
    }

    // Output the solution for each variable
    cout << "Solution:" << '\n';
    for (int i = 0; i < n; ++i)
        cout << "x" << i + 1 << " = " << solution[i] << '\n';
}

/*
    Gaussian Elimination function that performs row operations
    Parameters:
        - matrix: augmented matrix representing the system of linear equations
        - solution: vector to store the solution after elimination and back substitution
*/
void gaussianElimination(vector<vector<double>>& matrix, vector<double>& solution) {
    int rows = (int)matrix.size();
    // This loop converts the augmented matrix to row echelon form.
    for (int i = 0; i < rows; ++i) {
        int pivotRow = findPivotRow(matrix, i, i); // Find the pivot row for partial pivoting
        swapRows(matrix, i, pivotRow);             // Swap the current row with the pivot row
        if (matrix[i][i] != 0)                     // Normalize the row if the pivot element is non-zero
            normalizeRow(matrix, i, matrix[i][i]);
        eliminateBelow(matrix, i);                 // Eliminate elements below the pivot
    }
    backSubstitution(matrix, solution); // Perform back substitution to get the solution
}

//The main function is to set up and solve the system of linear equations.
// Allows user input for the number of equations, variables, and augmented matrix elements.
void RunSystem() {
    int numEquations, numVariables;
    cout << "Enter the number of Variables: ";
    cin >> numVariables;
    cout << "\nEnter the number of Equations: ";
    cin >> numEquations;

    cout << "\nEnter the Augmented Matrix:\n";
    vector<vector<double>> matrix(numEquations, vector<double>(numVariables + 1));
    for (int i = 0; i < numEquations; ++i)
        for (int j = 0; j <= numVariables; ++j)
            cin >> matrix[i][j]; // Input the augmented matrix elements

    vector<double> solution(numVariables);
    cout << "\nInitial Matrix:" << '\n';
    printMatrix(matrix); // Print initial matrix for reference
    gaussianElimination(matrix, solution); // Perform Gaussian elimination and solve
}

int main() {
    RunSystem();
    return 0;
}
