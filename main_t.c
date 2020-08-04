#include <imageprocessing.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


#define THREADS_MAX 3

imagem img;
 int N = 5;


void entorno_r(int k, int z, imagem img, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.r[j*img.width + i];
    }
  }
  soma /= 2*N*2*N;
  img.r[z*img.width + k] = soma;
}

void entorno_b(int k, int z, imagem img, int N){
  float soma= 0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.b[j*img.width + i];
    }
  }
  soma /= 2*N*2*N;
  img.b[z*img.width + k] = soma;
}

void entorno_g(int k, int z, imagem img, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.g[j*img.width + i];
    }
  }
  soma /= 2*N*2*N;
  img.g[z*img.width + k] = soma;
}

float media(float array[]){
	float count = 0.0; 
	float count_final = 0.0;
	for(int k=0; k<100; k++){
		count = count + (array[k]/1000.0);
	}
	count_final = count/100;
	printf("A media eh: %6f s\n", count_final);
	return count_final;
}


float desvio(float array[], float a){

    float variacoes = 0;
    for (int i = 0; i < 100; i++) {
        float v = (array[i]/1000.0) - a;
        variacoes += v * v;
    }

    float sigma = sqrt(variacoes / 100);
    printf("Desvio padrao: d = %.6f s\n", sigma);
}




void* funcao_thread(void *arg) {
  /* Inicializacao: guardar o proprio numero da thread */
  int *U = (int*)(arg);
  int M = (*U);

  while (1) {

    float soma = 0; 
    int i, j;
    for (int i = M; i<(img.width); i += 3) {
        for (int j = 0; j<(img.height); j++) {
            if( (i >= N) && (j >= N) && (img.width - i > N) && (img.height - j > N) ){
               entorno_r(i, j, img, N);
		entorno_g(i, j, img, N);
		entorno_b(i, j, img, N);
            }
        }
    }
    return NULL;
  }
    return NULL;
}


int main(){
    pthread_t threads[THREADS_MAX];
    int thread_id[THREADS_MAX];

img = abrir_imagem("data/cachorro.jpg");


/* Identificadores de thread */
    for (int i = 0; i < THREADS_MAX; i++) {
        thread_id[i] = i;
        }


      /* Disparando threads */
    for (int i=0; i < THREADS_MAX; i++) {
        pthread_create(&(threads[i]), NULL, funcao_thread, (void*) (&thread_id[i]));
    }

        /* Esperando threads */
    for (int i=0; i < THREADS_MAX; i++) {
        pthread_join(threads[i], NULL);
    }

    salvar_imagem("cachorro-out-thread.jpg", &img);
    liberar_imagem(&img);
    //gcc -omain_t main_t.c imageprocessing.c  -I./ -lfreeimage -lm -lpthread
    return 0;
}
