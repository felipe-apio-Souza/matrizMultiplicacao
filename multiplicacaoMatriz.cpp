#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h> 
#include <iomanip> // Para formatar a saída
using namespace std;
void inserirDados(int **matriz, int linha, int coluna);
void printMatriz(int **matriz, int linha, int coluna);
void multiplicacaoMatrizSequencial(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoSequencial );
void multiplicacaoMatrizParallel(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoParalelizado );
int** criarMatrizDinamica(int linha, int coluna);
void deletaMatrizDinamica(int** matriz, int linha);
void iniciarZerada(int** matriz, int linha, int coluna);
int main()
{
    double tempoSequencial, tempoParalelizado;
    int linhaA, colunaA, linhaB, colunaB;

    cout << "Linha da Matriz A: ";
    cin >> linhaA;
    cout << endl;

    cout << "Coluna da Matriz A: ";
    cin >> colunaA;
    cout << endl;

    cout << "Linha da Matriz B: ";
    cin >> linhaB;
    cout << endl;

    cout << "Coluna da Matriz B: ";
    cin >> colunaB;
    cout << endl;

     


    if(colunaA == linhaB){
        int** matrizA = criarMatrizDinamica(linhaA, colunaA);
        int** matrizB = criarMatrizDinamica(linhaB, colunaB);
        int** matrizC = criarMatrizDinamica(linhaA, colunaB);


        //cout << "Matrix A:" << endl;
        inserirDados(matrizA, linhaA, colunaA);
        //printMatriz(matrizA, linhaA, colunaA);
        //cout << "Matrix B:" << endl;
        inserirDados(matrizB, linhaB, colunaB);
        //printMatriz(matrizB, linhaB, colunaB);
        
        //cout << "Matrix C Sequencial:" << endl;
        multiplicacaoMatrizSequencial(matrizA, matrizB, matrizC, linhaA, colunaA, linhaB, colunaB, tempoSequencial);
        //printMatriz(matrizC, linhaA, colunaB);
        
        //cout << "Matrix C Paralelizada:" << endl;
        multiplicacaoMatrizParallel(matrizA, matrizB, matrizC, linhaA, colunaA, linhaB, colunaB, tempoParalelizado);
        //printMatriz(matrizC, linhaA, colunaB);

        cout << "Matriz A[" << linhaA << "][" << colunaA << "] * Matriz B[" << linhaB << "][" << colunaB << "] = Matriz C [" << linhaA << "][" << colunaB << "]" << endl;
        cout << "--Resultados Obtiditos--" << endl;
        
        
        
        double aceleracaoObtida = tempoSequencial / tempoParalelizado;
        
        
        

    cout << fixed << setprecision(3); 
    cout << "Com Sequencial   " 
     << (int)(tempoSequencial / 60) << "m " 
     << (int)(tempoSequencial) % 60 << "s "
     << fixed << setprecision(3) << (tempoSequencial * 1000) << "ms" << endl;

    cout << "Com Paralelizada " 
     << (int)(tempoParalelizado / 60) << "m " 
     << (int)(tempoParalelizado) % 60 << "s "
     << fixed << setprecision(3) << (tempoParalelizado * 1000) << "ms" << endl;


        cout << aceleracaoObtida << " vezes mais rapido que a sequencial" << endl;
        
        deletaMatrizDinamica(matrizA, linhaA);
        deletaMatrizDinamica(matrizB, linhaB);
        deletaMatrizDinamica(matrizC, linhaA);
    }else{
        cout << "nao atende aos requisitos para realizar a multiplicacao" << endl;
    }
    
    return 0;
}

int** criarMatrizDinamica(int linha, int coluna){
    int** matriz = new int*[linha];
    for(int i =0; i<linha; i++){
        matriz[i] = new int[coluna];
    }
    return matriz;
}

void deletaMatrizDinamica(int** matriz, int linha){
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

void printMatriz(int **matriz, int linha, int coluna){
    for(int i = 0; i < linha; i++){
        for(int j = 0; j < coluna; j++){
            cout << matriz[i][j] << "  ";
        }
        cout << endl;
    }
}

void multiplicacaoMatrizSequencial(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoSequencial ){
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

void multiplicacaoMatrizParallel(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoParalelizado ){
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

