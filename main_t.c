#include <imageprocessing.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#define THREADS_MAX 3

imagem img;
imagem img2;
 int N = 5;


void entorno_r(int k, int z, imagem img, imagem img2, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img2.r[j*img.width + i];
    }
  }
  soma /= 2*N*2*N;
  img.r[z*img.width + k] = soma;
}

void entorno_b(int k, int z, imagem img,imagem img2, int N){
  float soma= 0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img2.b[j*img.width + i];
    }
  }
  soma /= 2*N*2*N;
  img.b[z*img.width + k] = soma;
}

void entorno_g(int k, int z, imagem img,imagem img2, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img2.g[j*img.width + i];
    }
  }
  soma /= 2*N*2*N;
  img.g[z*img.width + k] = soma;
}

float media(float array[]){
	float count = 0.0; 
	float count_final = 0.0;
	for(int k=0; k<100; k++){
	//printf("%f, ", (array[k]/1000.0));
		count = count + (array[k]);
	}
	count_final = count/100;
	//printf("A media eh: %6f s\n", count_final);
	return count_final;
}


float desvio(float array[], float a){

    float variacoes = 0;
    for (int i = 0; i < 100; i++) {
        float v = (array[i]) - a;
        variacoes += v * v;
    }

    float sigma = sqrt(variacoes / 100);
    //printf("Desvio padrao: d = %.6f s\n", sigma);
}



float results[100];


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
		       entorno_r(i, j, img, img2, N);
			entorno_g(i, j, img, img2, N);
			entorno_b(i, j, img, img2, N);
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

struct timeval rt0, rt1, drt;

char input_nome_arquivo[40];
    char output_nome_arquivo[50] ;
    char *ptr;
    //printf("selecionar imagem:\n");
    scanf("%s", input_nome_arquivo);
   // printf("saida da imagem:\n");
    scanf("%s", output_nome_arquivo);
    img = abrir_imagem(input_nome_arquivo);
    img2 = abrir_imagem(input_nome_arquivo);
    
int q=0;
//inicio
for(q=0; q<101; q++){
    	  gettimeofday(&rt0, NULL);

/* Identificadores de thread */
    for (int i = 0; i < THREADS_MAX; i++) {
        thread_id[i] = i;
        }

//

      /* Disparando threads */
    for (int i=0; i < THREADS_MAX; i++) {
        pthread_create(&(threads[i]), NULL, funcao_thread, (void*) (&thread_id[i]));
    }

        /* Esperando threads */
    for (int i=0; i < THREADS_MAX; i++) {
        pthread_join(threads[i], NULL);
    }
    
    //fim
    gettimeofday(&rt1, NULL);
	  timersub(&rt1, &rt0, &drt);
	  double Tempo = (double) drt.tv_usec / 1000000 + (double) drt.tv_sec ;
	  results[q] = Tempo;
	  //printf ("Total time = %f seconds\n", Tempo);
	}    

    strtok_r(input_nome_arquivo, "/", &ptr);
    strcat(output_nome_arquivo, ptr);

 
    salvar_imagem(output_nome_arquivo, &img);
     liberar_imagem(&img2);
    
    float media_final = media(results);
    float desvio_padrao = desvio(results, media_final);
    //gcc -omain_t main_t.c imageprocessing.c  -I./ -lfreeimage -lm -lpthread
    
    
  //printf ( "# x \t y \t z \t t\n" );
  
  
  printf ( "thread %f %f\n",  media_final, desvio_padrao);

    
    
    return 0;
}
