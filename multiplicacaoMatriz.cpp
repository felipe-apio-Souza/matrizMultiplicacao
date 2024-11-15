#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <omp.h> 
#include <iomanip> 
using namespace std;
void inserirDados(int **matriz, int linha, int coluna);
void multiplicacaoMatrizSequencial(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoSequencial );
void multiplicacaoMatrizParallel(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoParalelizado );
int** criarMatrizDinamica(int linha, int coluna);
void deletaMatrizDinamica(int** matriz, int linha);
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

        inserirDados(matrizA, linhaA, colunaA);
        inserirDados(matrizB, linhaB, colunaB);
        multiplicacaoMatrizSequencial(matrizA, matrizB, matrizC, linhaA, colunaA, linhaB, colunaB, tempoSequencial);
        multiplicacaoMatrizParallel(matrizA, matrizB, matrizC, linhaA, colunaA, linhaB, colunaB, tempoParalelizado);

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

void multiplicacaoMatrizSequencial(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoSequencial ){
    double  comeco, fim, execucao;
    comeco = omp_get_wtime();
    for (int i = 0; i < linhaA; i++) {
        for (int j = 0; j < colunaB; j++) {
            int local_sum = 0;
            for (int k = 0; k < colunaA; k++) {
                local_sum += matrizA[i][k] * matrizB[k][j];
            }
            matrizC[i][j] = local_sum;
        }
    }
    fim = omp_get_wtime();
    tempoSequencial = fim - comeco; 
}

void multiplicacaoMatrizParallel(int **matrizA, int **matrizB, int **matrizC, int linhaA, int colunaA, int linhaB, int colunaB, double &tempoParalelizado ){
    double  comeco, fim, execucao;
    omp_set_num_threads(8);
    
    comeco = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < linhaA; i++) {
        for (int j = 0; j < colunaB; j++) {
            int local_sum = 0;
            for (int k = 0; k < colunaA; k++) {
                local_sum += matrizA[i][k] * matrizB[k][j];
            }
            matrizC[i][j] = local_sum;
        }
    }
    fim = omp_get_wtime();
    tempoParalelizado = fim - comeco;
}


