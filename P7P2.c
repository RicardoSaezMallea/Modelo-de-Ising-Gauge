#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define L 24

int red[L][L][L][3];
//[x][y][z][0] para arista x, [1] para y, [2] para z de la componente de espines en dirección positiva

double frand() {
    return (double)rand() / RAND_MAX;
}

void inicializarRedMin(){
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            for (int k = 0; k < L; k++) {
                for (int d = 0; d < 3; d++) {
                    red[i][j][k][d] = 1;
                }
            }
        }
    }
}

void inicializarRed() {
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            for (int k = 0; k < L; k++) {
                for (int d = 0; d < 3; d++) {
                    red[i][j][k][d] = (frand() < 0.5) ? -1 : 1;
                }
            }
        }
    }
}

void metropolis(int pasos, double beta) {
    for (int i = 0; i < pasos; i++) {
        int x = rand() % L;
        int y = rand() % L;
        int z = rand() % L;
        int d = rand() % 3;

        int energia_inicial = 0;

        if(d==0){
            energia_inicial -= energiaPlaqueta(x, y, z, 0);
            energia_inicial -= energiaPlaqueta(x, y, z, 1);
            energia_inicial -= energiaPlaqueta(x, y, (z-1+L)%L, 0);
            energia_inicial -= energiaPlaqueta(x, (y-1+L)%L, z, 1);
        }
        else if(d==1){
            energia_inicial -= energiaPlaqueta(x, y, z, 1);
            energia_inicial -= energiaPlaqueta(x, y, z, 2);
            energia_inicial -= energiaPlaqueta((x-1+L)%L, y, z, 1);
            energia_inicial -= energiaPlaqueta(x, y, (z-1+L)%L, 2);
        }
        else if(d==2){
            energia_inicial -= energiaPlaqueta(x, y, z, 2);
            energia_inicial -= energiaPlaqueta(x, y, z, 0);
            energia_inicial -= energiaPlaqueta((x-1+L)%L, y, z, 0);
            energia_inicial -= energiaPlaqueta(x, (y-1+L)%L, z, 2);
        }

        red[x][y][z][d] *= -1;

        int energia_final = 0;
        if(d==0){
            energia_final -= energiaPlaqueta(x, y, z, 0);
            energia_final -= energiaPlaqueta(x, y, z, 1);
            energia_final -= energiaPlaqueta(x, y, (z-1+L)%L, 0);
            energia_final -= energiaPlaqueta(x, (y-1+L)%L, z, 1);
        }
        else if(d==1){
            energia_final -= energiaPlaqueta(x, y, z, 1);
            energia_final -= energiaPlaqueta(x, y, z, 2);
            energia_final -= energiaPlaqueta((x-1+L)%L, y, z, 1);
            energia_final -= energiaPlaqueta(x, y, (z-1+L)%L, 2);
        }
        else if(d==2){
            energia_final -= energiaPlaqueta(x, y, z, 2);
            energia_final -= energiaPlaqueta(x, y, z, 0);
            energia_final -= energiaPlaqueta((x-1+L)%L, y, z, 0);
            energia_final -= energiaPlaqueta(x, (y-1+L)%L, z, 2);
        }

        int deltaE = (energia_final - energia_inicial);
        //printf("Delta E = %d: ",deltaE);

        if (deltaE <= 0 || frand() < exp(-deltaE * beta)) {
            //printf("Cambio aceptado!\n");
        }
        else {
            red[x][y][z][d] *= -1;
            //printf("Cambio rechazado!\n");
        }

    }
}


void TransfGauge(int pasos) {
    int x,y,z;

    for (int i = 0; i < pasos; i++)
        x = rand() % L;
        y = rand() % L;
        z = rand() % L;
        red[x][y][z][0]=-red[x][y][z][0];
        red[x][y][z][1]=-red[x][y][z][1];
        red[x][y][z][2]=-red[x][y][z][2];
        red[(x-1+L)%L][y][z][0]=-red[(x-1+L)%L][y][z][0];
        red[x][(y-1+L)%L][z][1]=-red[x][(y-1+L)%L][z][1];
        red[x][y][(z-1+L)%L][2]=-red[x][y][(z-1+L)%L][2];
}

int energiaRed(){
    int suma =0;
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < L; j++) {
            for (int k = 0; k < L; k++) {
                for (int d = 0; d < 3; d++) {
                    suma += energiaPlaqueta(i,j,k,d);
                }
            }
        }
    }
    return -suma;
}

int energiaPlaqueta(int x, int y, int z, int d) {
    int suma = 0;
    if (d == 0) { // x
        //printf("%d %d %d %d\n",red[x][y][z][2],red[x][y][z][0],red[(x+1)%L][y][z][2],red[x][y][(z+1)%L][0]);
        suma = red[x][y][z][2]*red[x][y][z][0]*red[(x+1)%L][y][z][2]*red[x][y][(z+1)%L][0];
    }
    else if (d == 1) { // y
        //printf("%d %d %d %d\n",red[x][y][z][0],red[(x+1)%L][y][z][1],red[x][y][z][1],red[x][(y+1)%L][z][0]);
        suma = red[x][y][z][0]*red[(x+1)%L][y][z][1]*red[x][y][z][1]*red[x][(y+1)%L][z][0];
    }
    else if (d == 2) { // z
        //printf("%d %d %d %d\n",red[x][y][z][2],red[x][y][z][1],red[x][(y+1)%L][z][2],red[x][y][(z+1)%L][1]);
        suma = red[x][y][z][2]*red[x][y][z][1]*red[x][(y+1)%L][z][2]*red[x][y][(z+1)%L][1];
    }
    return suma;
}

int Loops(int n, int x, int y, int z, int d){
    if(n >= L){
        printf("Demasiado grande");
        return 0;
    }
    else{
        int i, energia = 1;
        if(d==0){
            for(i = 0; i<n; i++){
                energia *= red[x][y][(z+i)%L][2]*red[(x+i)%L][y][z][0]*red[(x+n)%L][y][(z+i)%L][2]*red[(x+i)%L][y][(z+n)%L][0];
            }
        }
        else if(d==1){
            for(i = 0; i<n; i++){
                energia *= red[(x+i)%L][y][z][0]*red[(x+n)%L][(y+i)%L][z][1]*red[x][(y+i)%L][z][1]*red[(x+i)%L][(y+n)%L][z][0];
            }
        }
        else if (d==2){
            for(i = 0; i<n; i++){
                energia *= red[x][y][(z+i)%L][2]*red[x][(y+i)%L][z][1]*red[x][(y+n)%L][(z+i)%L][2]*red[x][(y+i)%L][(z+n)%L][1];
            }
        }
    return energia;
    }
}

void LoopsHastaNmax (int nMin, int nMax, int *ELoops, int x, int y, int z, int d){
    int i;

    for(i=0; i<=nMax-nMin; i++){
        ELoops[i] = Loops(nMin+i,x,y,z,d);
    }
}

//Pruebas Inicializar Red
/*int main(){
    srand(time(NULL));
    FILE *f = fopen("InicializarRedTest.txt","w");
    inicializarRed();
    long long int E, i = 0, nPasos = 10000000;
    for(i = 0; i<nPasos; i++){
        inicializarRed();
        E = energiaRed();
        fprintf(f,"%d\n",E);
        if(i % (nPasos / 100) == 0){
            printf("Progreso: %.1f%%\n",((double)i * 100) /nPasos);
        }
    }
    fclose(f);
    return 0;
}*/

//Pruebas Gauge

//Pruebas Metrópolis

// //Plotear 1 energía en función de pasos
/*int main() {
    srand(time(NULL));
    inicializarRed();
    printf("Energía red: %d\n",energiaRed());
    int i = 0;
    double beta = 10.0;
    int nPasos = 100000;
    FILE *f = fopen("MetrópolisTest_beta10.0.txt","w");
    for (i = 0; i<nPasos; i++){
        fprintf(f,"%d\t %d\n", i, energiaRed());
        metropolis(1,beta);
        if(i % (nPasos / 10) == 0)printf("Progreso Termalizacion: %d%%\n", (1 * 100) / nPasos);
    }
    printf("Energía final: %d\n",energiaRed());
    fclose(f);
}*/

// //Test de termalización
/*int main(){
    srand(time(NULL));
    double beta = 0.68, beta_max=0.86;
    int nPasos = 1000000, i = 0, E1, E2;
    char filename[100];
    int intervalo = 5000, nMedidas = nPasos/intervalo;
    while (beta<=beta_max){
        inicializarRedMin();
        E1 = energiaRed();
        sprintf(filename,"MetropolisTest_beta%.2f.txt",beta);
        FILE *f = fopen(filename,"w");
        printf("Fichero beta = %lf\n",beta);
        for(i = 0; i<nMedidas; i++){
            metropolis(intervalo,beta);
            E2 = energiaRed();
            fprintf(f,"%d \t%d\n",intervalo*i,E2-E1);
            E1 = E2;
            if(i % (nMedidas / 10) == 0)printf("Progreso Termalizacion: %d%%\n", (i * 100) / nMedidas);
        }
        beta = beta+0.02;
    }
}*/

//Prueba energía plaqueta:
/*int main(){
    int x,y,z,d,E[L][L][L][3],i,j,k,r;
    inicializarRed();
    x = 1, y = 1, z= 1;
    d=2;
    for(i=0; i<L; i++){
        for(j=0; j<L; j++){
            for(k=0; k<L; k++){
                for(r=0; r<3; r++){
                    E[i][j][k][r] = energiaPlaqueta(i,j,k,r);
                }
            }
        }
    }

    red[x][y][z][d] *=-1;

    for(i=0; i<L; i++){
        for(j=0; j<L; j++){
            for(k=0; k<L; k++){
                for(r=0; r<3; r++){
                    if(E[i][j][k][r] != energiaPlaqueta(i,j,k,r))
                        printf("x=%d\t y=%d\t z=%d\t d=%d\n",i,j,k,r);
                }
            }
        }
    }
}*/

//Pruebas Loops

/*int main (){
    srand(time(NULL));
    inicializarRed();
    int x, y, z, d;
    x = rand() % L;
    y = rand() % L;
    z = rand() % L;
    d = rand() % 3;
    printf("%d\n",energiaPlaqueta(x,y,z,d));
    printf("%d\n",Loops(1,x,y,z,d));
    printf("%d\n",Loops(2,x,y,z,d));
    printf("%d\n",Loops(3,x,y,z,d));
    printf("%d\n",Loops(4,x,y,z,d));
    printf("%d\n",Loops(5,x,y,z,d));
    printf("%d\n",Loops(6,x,y,z,d));
    printf("%d\n",Loops(7,x,y,z,d));
    printf("%d\n",Loops(8,x,y,z,d));
    printf("%d\n",Loops(9,x,y,z,d));
    printf("%d\n",Loops(10,x,y,z,d));
    return 0;
}*/


//Pruebas Loops

/*int main(){
    srand(time(NULL));
    int nMin=1, nMax=10, ELoops[10], i, x, y, z, d;

    inicializarRed();
    x = rand() % L;
    y = rand() % L;
    z = rand() % L;
    d = rand() % 3;

    LoopsHastaNmax(nMin,nMax,ELoops,x,y,z,d);

    for(i=0; i<nMax; i++){
        printf("n=%d \t Loop=%d\n", i+nMin, ELoops[i]);
    }

    return 0;
}*/

//Ley del área y del perímetro
int main(){
    srand(time(NULL));
    int nMin = 1, nMax = 10;
    int control = nMax-nMin+1;
    int ELoops[control], i, j, x, y, z, d;
    int nRedes = 100000;
    int N = L*L*L*3;
    int tot = N*control;
    double avg[control], std[control];
    int suma[control];
    double beta = 0.74;
    char filename[100];
    FILE *f;
    sprintf(filename,"LoopsRed%d_beta%.2f.txt",L,beta);
    f = fopen(filename,"w");

    for(i=0; i<nRedes; i++){

        for(j=0; j<control; j++){
            suma[j] = 0;
        }

        inicializarRed();
        metropolis(500000,beta);

        for(x=0; x<L; x++){
            for(y=0; y<L; y++){
                for(z=0; z<L; z++){
                    for(d=0; d<3; d++){
                        LoopsHastaNmax(nMin,nMax,ELoops,x,y,z,d);

                        for(j=0; j<control; j++){
                            suma[j] += ELoops[j];
                        }
                    }
                }
            }
        }
        for(j=0; j<control; j++){
            avg[j] = ((double) suma[j])/((double) N);
            std[j] = sqrt((1.0 - avg[j]*avg[j]));
            fprintf(f,"%d\t %d\t %.15lf\t %.15lf\n",i+1,j+nMin,avg[j],std[j]);
        }
        if(i % (nRedes / 1000) == 0)printf("Progreso: %lf%%\n", ((double) i * 100) / nRedes);
    }
    fclose(f);
}

