#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <imageprocessing.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>

#define N_PROCESSOS  3

//int w, int h,
void entorno_r(int k, int z, float *r, imagem img, int w, int h, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.r[j*(w) + i];
    }
  }
  soma /= ( (2*(N+1)) * (2*(N+1) ) -1 );
  r[z*(w) + k] = soma;
}


void entorno_b(int k, int z, float *b, imagem img, int w, int h, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.b[j*(w) + i];
    }
  }
  soma /= ( (2*(N+1)) * (2*(N+1) ) -1 );
  b[z*(w) + k] = soma;
}


void entorno_g(int k, int z, float *g, imagem img, int w, int h, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.g[j*(w) + i];
    }
  }
  soma /= ( (2*(N+1)) * (2*(N+1) ) -1 );
  g[z*(w) + k] = soma;
}


float media(float array[]){
	float count = 0.0; 
	float count_final = 0.0;
	int o = 0;
	for(int k=0; k<101; k++){
		//printf("%f, ", (array[k]/1000.0));
		count = count + (array[k]);
		
		}
	
	count_final = count/100;
	//printf("A media eh: %f s\n", count_final);
	return count_final;
}


float desvio(float array[], float a){

    float variacoes = 0;
    for (int i = 0; i < 101; i++) {
        float v = (array[i]) - a;
        variacoes += v * v;
        
    }

    float sigma = sqrt(variacoes / 100);
    //printf("Desvio padrao: d = %.6f s\n", sigma);
}





int main(){
    float results[100];
    imagem img;
    //img = abrir_imagem("data/cachorro.jpg");
    char input_nome_arquivo[40];
    char output_nome_arquivo[50] ;
    char *ptr;
    //printf("selecionar imagem:\n");
    scanf("%s", input_nome_arquivo);
    //printf("saida da imagem:\n");
    scanf("%s", output_nome_arquivo);
    img = abrir_imagem(input_nome_arquivo);
    
   struct timeval rt0, rt1, drt;
    
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




   
	
	//inicio
int q = 0;
for(q=0; q<101; q++){
    gettimeofday(&rt0, NULL);
    
    /*Criar processos filhos*/
    for(int k = 0; k < N_PROCESSOS ; k++){
        filho[k] = fork();
       
        if (filho[k]==0) {

            /* Processo filho(k) Opera */ 
            int i, j;
		    for (int i = k; i<(w); i += 3) {
		        for (int j = 0; j<(h); j++) {
		            if( (i >= N) && (j >= N) && ( (w) - i > N) && ( (h) - j > N) ){

		                entorno_r(i, j, r, img, w, h, N);
		                entorno_b(i, j, b, img, w, h, N);
		                entorno_g(i, j, g, img, w, h, N);
		            }
		        }
		    }
            exit(0);
            	
	  	}    
        }
        
    


    for (int k = 0; k < N_PROCESSOS ; k++){
        waitpid (filho[k], NULL, 0);
    }
    
    
  gettimeofday(&rt1, NULL);
	  timersub(&rt1, &rt0, &drt);
	  double Tempo = (double) drt.tv_usec / 1000000 + (double) drt.tv_sec ;
	   (*resultado)[q] = Tempo;
	  //printf ("Total time = %f seconds\n", Tempo);
    
    
    if(q==0){
        for(int i = 0; i < w; i++){
      for(int j = 0; j < h; j++){
        img.r[j*w + i] = r[j*w + i];
        img.g[j*w + i] = g[j*w + i];
        img.b[j*w + i] = b[j*w + i];
      }
    }
    
    }
    
    }
    //fim
     



    strtok_r(input_nome_arquivo, "/", &ptr);
    strcat(output_nome_arquivo, ptr);

 
    salvar_imagem(output_nome_arquivo, &img);
    liberar_imagem(&img);
    
    float media_final = media(*resultado);
    float desvio_padrao = desvio(*resultado, media_final);
    
    

  //plotar o grafico
   //printf ( "# x \t y \t z \t t\n" );
  
  
   printf ( "proc %f %f\n",  media_final, desvio_padrao);
 
    
    //gcc -omain_p main_p.c imageprocessing.c  -I./ -lfreeimage -lm
    return 0;
}
