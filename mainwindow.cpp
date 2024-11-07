#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidget>
#include <QVector>
#include <QString>
#include <QDebug>

// Constructor for MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the spin boxes to the slot to update table dimensions
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateTableSize()));
    connect(ui->spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(updateTableSize()));

    // Set initial size for tableWidget (3x4)
    ui->spinBox->setValue(3);   // Default rows
    ui->spinBox_2->setValue(4);  // Default columns

    updateTableSize();
}

// Slot function to update the table size
void MainWindow::updateTableSize()
{
    int rows = ui->spinBox->value();
    int cols = ui->spinBox_2->value();

    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(cols);

    // Initialize each cell with zero as default if it's empty
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!ui->tableWidget->item(i, j)) {
                ui->tableWidget->setItem(i, j, new QTableWidgetItem("0"));
            }
        }
    }
}

// Destructor for MainWindow
MainWindow::~MainWindow()
{
    delete ui;
}

// Function to print the matrix to QTableWidget with a given comment
void MainWindow::printMatrixToUI(const QVector<QVector<double>>& matrix, const QString& comment) {
    // Append comment to output area
    ui->outputTextEdit->append(comment + "\n");

    // Print matrix in text format
    for (const auto& row : matrix) {
        QString rowStr = "| ";
        for (const auto& elem : row) {
            rowStr += QString::number(elem, 'f', 5) + " ";
        }
        rowStr += " |";
        ui->outputTextEdit->append(rowStr);
    }
    ui->outputTextEdit->append("\n");
}

// Swap two rows and log the operation
void MainWindow::swapRows(QVector<QVector<double>>& matrix, int row1, int row2) {
    if (row1 != row2) {
        std::swap(matrix[row1], matrix[row2]);
        printMatrixToUI(matrix, QString("Swapped R%1 with R%2").arg(row1 + 1).arg(row2 + 1));
    }
}

// Normalize a specific row and log the operation
void MainWindow::normalizeRow(QVector<QVector<double>>& matrix, int row, double pivot) {
    int cols = matrix[row].size();
    for (int i = 0; i < cols; ++i)
        matrix[row][i] /= pivot;
    printMatrixToUI(matrix, QString("Normalized R%1 by dividing by %2").arg(row + 1).arg(pivot));
}

// Eliminate elements below the pivot in a given column and log the operation
void MainWindow::eliminateBelow(QVector<QVector<double>>& matrix, int row) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = row + 1; i < rows; ++i) {
        double factor = matrix[i][row] / matrix[row][row];
        for (int j = row; j < cols; ++j)
            matrix[i][j] -= factor * matrix[row][j];
        printMatrixToUI(matrix, QString("Eliminated below pivot in R%1 by factor %2").arg(i + 1).arg(factor));
    }
}

// Perform Gaussian elimination and log each step
void MainWindow::gaussianElimination(QVector<QVector<double>>& matrix, QVector<double>& solution) {
    int rows = matrix.size();

    for (int i = 0; i < rows; ++i) {
        int pivotRow = findPivotRow(matrix, i, i); // Find pivot row
        swapRows(matrix, i, pivotRow);             // Swap current row with pivot row
        if (matrix[i][i] != 0)                     // Normalize if pivot is non-zero
            normalizeRow(matrix, i, matrix[i][i]);
        eliminateBelow(matrix, i);                 // Eliminate elements below the pivot
    }
    backSubstitution(matrix, solution);            // Perform back substitution
}

// Back substitution to find solution after Gaussian elimination
void MainWindow::backSubstitution(QVector<QVector<double>>& matrix, QVector<double>& solution) {
    int n = matrix.size();
    solution.resize(n, 0);

    for (int i = n - 1; i >= 0; --i) {
        solution[i] = matrix[i].back();
        for (int j = i + 1; j < n; ++j)
            solution[i] -= matrix[i][j] * solution[j];
        solution[i] /= matrix[i][i];
    }

    // Output the solution
    ui->outputTextEdit->append("Solution:");
    for (int i = 0; i < n; ++i)
        ui->outputTextEdit->append(QString("x%1 = %2").arg(i + 1).arg(solution[i]));
}

// Function called when the solve button is clicked
void MainWindow::on_solveButton_clicked() {
    // Read input matrix from QTableWidget
    int rows = ui->tableWidget->rowCount();
    int cols = ui->tableWidget->columnCount();

    // Parse the input matrix into a QVector
    QVector<QVector<double>> matrix;
    for (int i = 0; i < rows; ++i) {
        QVector<double> row;
        for (int j = 0; j < cols; ++j) {
            QTableWidgetItem* item = ui->tableWidget->item(i, j);
            if (!item || item->text().isEmpty()) {
                row.append(0.0);
            } else {
                row.append(item->text().toDouble());
            }
        }
        matrix.append(row);
    }

    // Check for empty input
    if (matrix.isEmpty() || matrix[0].isEmpty()) {
        ui->outputTextEdit->setText("Invalid input. Please check your matrix.");
        return;
    }

    // Clear previous output
    ui->outputTextEdit->clear();

    // Print the matrix for debugging purposes
    printMatrixToUI(matrix, "Initial Matrix:");

    QVector<double> solution;

    // Perform Gaussian elimination
    gaussianElimination(matrix, solution);
}
