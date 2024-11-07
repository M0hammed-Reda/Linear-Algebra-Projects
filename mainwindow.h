#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_solveButton_clicked();      // Slot connected to the solve button
    void updateTableSize();             // Slot to update the table size based on spin box values

private:
    Ui::MainWindow *ui;

    // Helper functions for Gaussian elimination
    void printMatrixToUI(const QVector<QVector<double>>& matrix, const QString& comment);
    void swapRows(QVector<QVector<double>>& matrix, int row1, int row2);
    void normalizeRow(QVector<QVector<double>>& matrix, int row, double pivot);
    void eliminateBelow(QVector<QVector<double>>& matrix, int row);
    void gaussianElimination(QVector<QVector<double>>& matrix, QVector<double>& solution);
    void backSubstitution(QVector<QVector<double>>& matrix, QVector<double>& solution);

    // Helper function to find the pivot row
    int findPivotRow(const QVector<QVector<double>>& matrix, int row, int col) {
        int pivotRow = row;
        for (int i = row + 1; i < matrix.size(); ++i) {
            if (qAbs(matrix[i][col]) > qAbs(matrix[pivotRow][col])) {
                pivotRow = i;
            }
        }
        return pivotRow;
    }
};

#endif // MAINWINDOW_H
