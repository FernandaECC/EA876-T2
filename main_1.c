#include <imageprocessing.h>
#include <cronometro_1.h>
#include <time.h>
#include <malloc.h>
#include <pthread.h>
#include <math.h>


//Usando tempo REAL


float results[100];
typedef struct {
  int *x;
  int N;
  int result;
} threadargs;

void *blur(void *args) {
  imagem img;
  img = abrir_imagem("data/cachorro.jpg");
  unsigned int tmp;

  float alpha = 0.998;

  for (int i=0; i<(img.width); i++) {
    for (int j=0; j<(img.height); j++) {
      /* Ganho no canal R */
      tmp = img.r[j*img.width + i] * 2;
      if (tmp > 255) tmp=255;
      img.r[j*img.width + i] = tmp;

      /* Reducao no canal B */
      img.b[j*img.width + i] /= 2;

      /* Blur exponencial no canal G */
      if (i!=0) {
        img.g[j*img.width + i] = (1-alpha)*img.g[j*img.width + i] +(alpha)* img.g[j*img.width + i -1];
      }

    }
  }
  salvar_imagem("cachorro-out.jpg", &img);
  liberar_imagem(&img);
return NULL;
}

float media(float array[]){
	float count = 0.0; 
	float count_final = 0.0;
	for(int k=0; k<100; k++){
		count = count + array[k];
	}
	count_final = count/100;
	printf("A media eh: %f\n", count_final);
	return count_final;
}


float desvio(float array[], float a){


    float variacoes = 0;
    for (int i = 0; i < 100; i++) {
        float v = array[i] - a;
        variacoes += v * v;
    }

    float sigma = sqrt(variacoes / 100);
    printf("Desvio padrao: d = %.2f\n", sigma);
}



int main() {
  threadargs f;
 int z = 0; 
 float coletora = 0;
  //blur;
  //coletora = medir_tempo(blur, &f);
  //printf("%f\n", coletora);
 
  while(z < 101){
  coletora = medir_tempo(blur, &f);
  results[z] = coletora;
  //printf("%f\n", results[z]);
  z++;
  } 
  float media_final = media(results);
  float desvio_padrao = desvio(results, media_final);
//gcc -omain main.c imageprocessing.c cronometro.c -I./ -lfreeimage
  return 0;
}
