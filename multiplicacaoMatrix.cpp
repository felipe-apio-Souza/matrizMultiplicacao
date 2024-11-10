#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h> 
#include <iomanip> // Para formatar a saída
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
     int linhaA = 1000;
     int colunaA = 1000;
     int linhaB = 1000;
     int colunaB = 1000;


    if(colunaA == linhaB){
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
        
        
        
        double aceleracaoObtida = tempoSequencial / tempoParalelizado;
        
        
        

        cout << fixed << setprecision(3); 
        cout << "Com Sequencial   " << fixed << setprecision(3) 
        << (int)(tempoSequencial / 60) << "m " 
        << (int)(tempoSequencial) % 60 << "s" << endl;

        cout << "Com Paralelizada " << fixed << setprecision(3) 
        << (int)(tempoParalelizado / 60) << "m " 
        << (int)(tempoParalelizado) % 60 << "s" << endl;

        cout << aceleracaoObtida << " vezes mais rapido que a sequencial" << endl;
        
        deletaMatrixDinamica(matrixA, linhaA);
        deletaMatrixDinamica(matrixB, linhaB);
        deletaMatrixDinamica(matrixC, linhaA);
    }else{
        cout << "nao atende aos requisitos para realizar a multiplicacao" << endl;
    }
    
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
    double  comeco, fim, execucao;
    iniciarZerada(matrixC, linhaA, colunaB);
    comeco = omp_get_wtime();
    for (int i = 0; i < linhaA; i++) {
        for (int j = 0; j < colunaB; j++) {
            int local_sum = 0;
            for (int k = 0; k < colunaA; k++) {
                local_sum += matrixA[i][k] * matrixB[k][j];
                //matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
            matrixC[i][j] = local_sum;
        }
    }
    fim = omp_get_wtime();
    tempoSequencial = fim - comeco; 
}

void multiplicacaoMatrixParallel(int **matrixA, int **matrixB, int **matrixC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoParalelizado ){
    double  comeco, fim, execucao;
    //omp_set_num_threads(12);
    //#pragma omp parallel num_threads(12); 
    
    iniciarZerada(matrixC, linhaA, colunaB);
    comeco = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < linhaA; i++) {
        for (int j = 0; j < colunaB; j++) {
            int local_sum = 0;
            for (int k = 0; k < colunaA; k++) {
                local_sum += matrixA[i][k] * matrixB[k][j];
                //matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
            matrixC[i][j] = local_sum;
        }
    }
    fim = omp_get_wtime();
    tempoParalelizado = fim - comeco;
}

void iniciarZerada(int** matrix, int linha, int coluna){
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){
            matrix[i][j] = 0;
        }
    }
}

