#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h> 
#include <iomanip> // Para formatar a saída
using namespace std;
void inserirDados(int **matriz, int linha, int coluna);
void printMatrix(int **matriz, int linha, int coluna);
void multiplicacaoMatrixSequencial(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoSequencial );
void multiplicacaoMatrixParallel(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoParalelizado );
int** criarMatrixDinamica(int linha, int coluna);
void deletaMatrixDinamica(int** matriz, int linha);
void iniciarZerada(int** matriz, int linha, int coluna);
int main()
{
    double tempoSequencial, tempoParalelizado;
    int linhaA, colunaA, linhaB, colunaB;

    cout << "Tamanho da Matriz: ";
    cin >> linhaA;
    cout << endl;

    colunaA = linhaA;
    colunaB = linhaA;
    linhaB = linhaA;
     


    if(colunaA == linhaB){
        int** matrizA = criarMatrixDinamica(linhaA, colunaA);
        int** matrizB = criarMatrixDinamica(linhaB, colunaB);
        int** matrizC = criarMatrixDinamica(linhaA, colunaB);


        //cout << "Matrix A:" << endl;
        inserirDados(matrizA, linhaA, colunaA);
        //printMatrix(matrixA, linhaA, colunaA);
        //cout << "Matrix B:" << endl;
        inserirDados(matrizB, linhaB, colunaB);
        //printMatrix(matrixB, linhaB, colunaB);
        
        //cout << "Matrix C Sequencial:" << endl;
        multiplicacaoMatrixSequencial(matrizA, matrizB, matrizC, linhaA, colunaA, linhaB, colunaB, tempoSequencial);
        //printMatrix(matrixC, linhaA, colunaB);
        
        //cout << "Matrix C Paralelizada:" << endl;
        multiplicacaoMatrixParallel(matrizA, matrizB, matrizC, linhaA, colunaA, linhaB, colunaB, tempoParalelizado);
        //printMatrix(matrixC, linhaA, colunaB);

        cout << "Matriz A[" << linhaA << "][" << colunaA << "] * Matriz B[" << linhaB << "][" << colunaB << "] = Matriz C [" << linhaA << "][" << colunaB << "]" << endl;
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
        
        deletaMatrixDinamica(matrizA, linhaA);
        deletaMatrixDinamica(matrizB, linhaB);
        deletaMatrixDinamica(matrizC, linhaA);
    }else{
        cout << "nao atende aos requisitos para realizar a multiplicacao" << endl;
    }
    
    return 0;
}

int** criarMatrixDinamica(int linha, int coluna){
    int** matriz = new int*[linha];
    for(int i =0; i<linha; i++){
        matriz[i] = new int[coluna];
    }
    return matriz;
}

void deletaMatrixDinamica(int** matriz, int linha){
    for(int i = 0; i < linha; i++){
        delete[] matriz[i];
    }
    delete[] matriz;
}

void inserirDados(int** matriz, int linha, int coluna){
    srand(time(NULL));
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){
            matriz[i][j] = rand()%999;
        }
    }
}

void printMatrix(int **matriz, int linha, int coluna){
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){
            cout << matriz[i][j] << "  ";
        }
        cout << endl;
    }
}

void multiplicacaoMatrixSequencial(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoSequencial ){
    double  comeco, fim, execucao;
    iniciarZerada(matrizC, linhaA, colunaB);
    comeco = omp_get_wtime();
    for (int i = 0; i < linhaA; i++) {
        for (int j = 0; j < colunaB; j++) {
            int local_sum = 0;
            for (int k = 0; k < colunaA; k++) {
                local_sum += matrizA[i][k] * matrizB[k][j];
                //matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
            matrizC[i][j] = local_sum;
        }
    }
    fim = omp_get_wtime();
    tempoSequencial = fim - comeco; 
}

void multiplicacaoMatrixParallel(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoParalelizado ){
    double  comeco, fim, execucao;
    //omp_set_num_threads(12);
    //#pragma omp parallel num_threads(12); 
    
    iniciarZerada(matrizC, linhaA, colunaB);
    comeco = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < linhaA; i++) {
        for (int j = 0; j < colunaB; j++) {
            int local_sum = 0;
            for (int k = 0; k < colunaA; k++) {
                local_sum += matrizA[i][k] * matrizB[k][j];
                //matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
            }
            matrizC[i][j] = local_sum;
        }
    }
    fim = omp_get_wtime();
    tempoParalelizado = fim - comeco;
}

void iniciarZerada(int** matriz, int linha, int coluna){
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){
            matriz[i][j] = 0;
        }
    }
}
