#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <imageprocessing.h>
#include <time.h>
#include <math.h>

#define N_PROCESSOS  3


void entorno_r(int k, int z, float *r, int w, int h, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += r[j*(w) + i];
    }
  }
  soma /= ( (2*(N+1)) * (2*(N+1) ) -1 );
  r[z*(w) + k] = soma;
}


void entorno_b(int k, int z, float *b, int w, int h, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += b[j*(w) + i];
    }
  }
  soma /= ( (2*(N+1)) * (2*(N+1) ) -1 );
  b[z*(w) + k] = soma;
}


void entorno_g(int k, int z, float *g, int w, int h, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += g[j*(w) + i];
    }
  }
  soma /= ( (2*(N+1)) * (2*(N+1) ) -1 );
  g[z*(w) + k] = soma;
}


float media(float array[]){
	float count = 0.0; 
	float count_final = 0.0;
	int o = 0;
	for(int k=0; k<202; k++){
		if(array[k] != 0.000000){
		//printf("%f, ", (array[k]/1000.0));
		count = count + (array[k]/1000.0);
		o++;
		}
	}
	count_final = count/o;
	printf("A media eh: %f s\n", count_final);
	return count_final;
}


float desvio(float array[], float a){

    float variacoes = 0;
    for (int i = 0; i < 202; i++) {
        if(array[i] != 0.000000){
        float v = (array[i]/1000.0) - a;
        variacoes += v * v;
        }
    }

    float sigma = sqrt(variacoes / 100);
    printf("Desvio padrao: d = %.6f s\n", sigma);
}





int main(){
float results[100];
    imagem img;
    img = abrir_imagem("data/cachorro.jpg");
    
    clock_t Ticks[2];
    
    int w = img.width;
    int h = img.height;
    
    int N = 5;

    pid_t filho[N_PROCESSOS];

    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANON;

    int *aux;
    aux = (int*) mmap(NULL, sizeof(int), protection, visibility, 0, 0);
  
    const int TAM = 100;
    float (*resultado)[TAM]; 
    resultado = mmap(0,100*TAM, protection, visibility, -1, 0);


    float *r, *g, *b;
    r = (float*) mmap(NULL, (w * h)*sizeof(float), protection, visibility, 0, 0);
    g = (float*) mmap(NULL, (w * h)*sizeof(float), protection, visibility, 0, 0);
    b = (float*) mmap(NULL, (w * h)*sizeof(float), protection, visibility, 0, 0);

    for(int i = 0; i < w; i++){
      for(int j = 0; j < h; j++){
        r[j*w + i] = img.r[j*w + i];
        g[j*w + i] = img.g[j*w + i];
        b[j*w + i] = img.b[j*w + i];
      }
    }




    /*Criar processos filhos*/
	(*aux) = 0;
	
	//inicio
	int q = 0;
	for(q=0; q<101; q++){
    Ticks[0] = clock();
    for(int k = 0; k < N_PROCESSOS ; k++){
        filho[k] = fork();
       
        if (filho[k]==0) {

            /* Processo filho(k) Opera */ 
            int i, j;
		    for (int i = k; i<(w); i += 3) {
		        for (int j = 0; j<(h); j++) {
		            if( (i >= N) && (j >= N) && ( (w) - i > N) && ( (h) - j > N) ){
		            //for(a=0; a<101; a++){
				//Ticks[0] = clock();
		                entorno_r(i, j, r, w, h, N);
		                entorno_b(i, j, b, w, h, N);
		                entorno_g(i, j, g, w, h, N);
		                //Ticks[1] = clock();    
				//double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
	  			//(*resultado)[(*aux)] = Tempo;	 
	  			//printf("%i: Tempo gasto: %g s.\n"), ((*aux)), (Tempo/1000.0);
	  			//(*aux)++;
	  			//}
		            }
		        }
		    }
            exit(0);
            	
	  	}    
        }
        
    


    for (int k = 0; k < N_PROCESSOS ; k++){
        waitpid (filho[k], NULL, 0);
    }
    
    
  
    Ticks[1] = clock();    
    double Tempo = (Ticks[1] - Ticks[0]) * 1000.0 / CLOCKS_PER_SEC;
    (*resultado)[q] = Tempo;
    
    	
    //printf("resultado: %f, \n", (Tempo/1000)); 
    //printf("vetor resultado: %f, \n", ((*resultado)[q]/1000)); 
    // printf("vetor resultado: %f, \n", (results[q]/1000)); 
    q++; 
    }
    //fim
     

 

    for(int i = 0; i < w; i++){
      for(int j = 0; j < h; j++){
        img.r[j*w + i] = r[j*w + i];
        img.g[j*w + i] = g[j*w + i];
        img.b[j*w + i] = b[j*w + i];
      }
    }

	
 
    salvar_imagem("cachorro-out-processos.jpg", &img);
    liberar_imagem(&img);
    
    float media_final = media(*resultado);
    float desvio_padrao = desvio(*resultado, media_final);
    //gcc -omain_p main_p.c imageprocessing.c  -I./ -lfreeimage -lm
    return 0;
}
