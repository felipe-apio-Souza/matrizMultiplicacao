#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h> 

using namespace std;
void inserirDados(int **matrix, int linha, int coluna);
void printMatrix(int **matrix, int linha, int coluna);
void multiplicacaoMatrixSequencial(int **matrixA, int **matrixB, int **matrixC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoSequencial );
void multiplicacaoMatrixParallel(int **matrixA, int **matrixB, int **matrixC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoParalelizado );
int** criarMatrixDinamica(int linha, int coluna);
void deletaMatrixDinamica(int** matrix, int linha);
void iniciarZerada(int** matrix, int linha, int coluna);
int main()
{
    double tempoSequencial, tempoParalelizado;
    const int linhaA = 1000;
    const int colunaA = 1000;
    const int linhaB = 1000;
    const int colunaB = 1000;

    int** matrixA = criarMatrixDinamica(linhaA, colunaA);
    int** matrixB = criarMatrixDinamica(linhaB, colunaB);
    int** matrixC = criarMatrixDinamica(linhaA, colunaB);

    //cout << "Matrix A:" << endl;
    inserirDados(matrixA, linhaA, colunaA);
    //printMatrix(matrixA, linhaA, colunaA);
    //cout << "Matrix B:" << endl;
    inserirDados(matrixB, linhaB, colunaB);
    //printMatrix(matrixB, linhaB, colunaB);
    
    //cout << "Matrix C Sequencial:" << endl;
    multiplicacaoMatrixSequencial(matrixA, matrixB, matrixC, linhaA, colunaA, linhaB, colunaB, tempoSequencial);
    //printMatrix(matrixC, linhaA, colunaB);
    
    //cout << "Matrix C Paralelizada:" << endl;
    multiplicacaoMatrixParallel(matrixA, matrixB, matrixC, linhaA, colunaA, linhaB, colunaB, tempoParalelizado);
    //printMatrix(matrixC, linhaA, colunaB);

    cout << "--Resultados Obtiditos--" << endl;
    cout << "Matrix C Sequencial: " << tempoSequencial << endl;
    cout << "Matrix C Paralelizada: " << tempoParalelizado << endl;
    //1 / (b + (1-b)/n)
    double aceleracaoObtida = tempoSequencial / tempoParalelizado;
    //estou calculando com base no numero de cores do meu processador altere para o seu
    //double eficiencia = (aceleracaoObtida / 6) * 100;
    
    cout << aceleracaoObtida << " vezes mais rapido que a sequencial" << endl;
    //cout << eficiencia << "% do potencial paralelo" << endl;
    
    deletaMatrixDinamica(matrixA, linhaA);
    deletaMatrixDinamica(matrixB, linhaB);
    deletaMatrixDinamica(matrixC, linhaA);
    return 0;
}

int** criarMatrixDinamica(int linha, int coluna){
    int** matrix = new int*[linha];
    for(int i =0; i<linha; i++){
        matrix[i] = new int[coluna];
    }
    return matrix;
}

void deletaMatrixDinamica(int** matrix, int linha){
    for(int i = 0; i < linha; i++){
        delete[] matrix[i];
    }
    delete[] matrix;
}

void inserirDados(int** matrix, int linha, int coluna){
    srand(time(NULL));
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){
            matrix[i][j] = rand()%999;
        }
    }
}

void printMatrix(int **matrix, int linha, int coluna){
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){
            cout << matrix[i][j] << "  ";
        }
        cout << endl;
    }
}

void multiplicacaoMatrixSequencial(int **matrixA, int **matrixB, int **matrixC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoSequencial ){
    if(colunaA == linhaB){
        double  comeco, fim, execucao;
        iniciarZerada(matrixC, linhaA, colunaB);
        comeco = omp_get_wtime();
        for (int i = 0; i < linhaA; i++) {
            for (int j = 0; j < colunaB; j++) {
                for (int k = 0; k < colunaA; k++) {
                    matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
                }
            }
        }
        fim = omp_get_wtime();
        tempoSequencial = fim - comeco; 
    }else{
        cout << "nao atende aos requisitos para realizar a multiplicacao" << endl;
    }
}

void multiplicacaoMatrixParallel(int **matrixA, int **matrixB, int **matrixC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoParalelizado ){
    if(colunaA == linhaB){
        double  comeco, fim, execucao;
        #pragma omp parallel for
        iniciarZerada(matrixC, linhaA, colunaB);
        comeco = omp_get_wtime();
        #pragma omp parallel for
        for (int i = 0; i < linhaA; i++) {
            for (int j = 0; j < colunaA; j++) {
                for (int k = 0; k < colunaA; k++) {
                    matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
                }
            }
        }
        fim = omp_get_wtime();
        tempoParalelizado = fim - comeco;
    }else{
        cout << "nao atende aos requisitos para realizar a multiplicacao" << endl;
    }
}

void iniciarZerada(int** matrix, int linha, int coluna){
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){
            matrix[i][j] = 0;
        }
    }
}