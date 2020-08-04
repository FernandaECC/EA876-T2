#include <imageprocessing.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void entorno_r(int k, int z, imagem img,int w, int h, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.r[j*w + i];
    }
  }
  soma /= 2*N*2*N;
  img.r[z*w + k] = soma;
}

void entorno_b(int k, int z, imagem img,int w, int h, int N){
  float soma= 0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.b[j*w + i];
    }
  }
  soma /= 2*N*2*N;
  img.b[z*w + k] = soma;
}

void entorno_g(int k, int z, imagem img,int w, int h, int N){
  float soma=0; 
  int i, j;
  for(i = (k-N); i < (k+N); i++){
    for(j = (z-N); j < (z+N); j++){
      soma += img.g[j*w + i];
    }
  }
  soma /= 2*N*2*N;
  img.g[z*w + k] = soma;
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








int main() {
  imagem img;
  
  img = abrir_imagem("data/cachorro.jpg");
  
  int N = 5;
  clock_t Ticks[2];
    
  int w = img.width;
  int h = img.height;
  int tam = w*h;
  float results[100];
  int aux = 0;  
    for(aux; aux<101; aux++){ 
    	  Ticks[0] = clock();
	  for (int i=0; i<(w); i++) {
	    for (int j=0; j<(h); j++) {
	      if( (i>=N) && (j>=N) && (w - i > N) && (h - j > N) ){
		entorno_r(i, j, img, w, h, N);
		entorno_g(i, j, img, w,  h, N);
		entorno_b(i, j, img, w, h, N);	 
	      }
	    }
	  }
	  Ticks[1] = clock();
	  double Tempo = (Ticks[1] - Ticks[0]) *1000.0  / CLOCKS_PER_SEC;
	  printf("%i: %7f s \n", aux, (Tempo/1000.0));
	  results[aux] = Tempo;
	}	  

  salvar_imagem("cachorro-out-linear.jpg", &img);
  liberar_imagem(&img);
  
  
  float media_final = media(results);
  float desvio_padrao = desvio(results, media_final);
  
  return 0;
}
//gcc -omain main.c imageprocessing.c  -I./ -lfreeimage -lm

