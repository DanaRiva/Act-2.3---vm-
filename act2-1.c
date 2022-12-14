#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void img_create(){
  char org[17]; snprintf(org, 17, "input_%i.bmp", omp_get_thread_num()+1);
  char dst[16]; snprintf(dst, 16, "flip_%i.bmp", omp_get_thread_num()+1);

  FILE *fptr_r; fptr_r=fopen(org,"rb");
  FILE *fptr_w; fptr_w=fopen(dst,"wb");

  long ancho, alto;
  unsigned char r, g, b;
  unsigned char* ptr;

  unsigned char xx[54];
  int cuenta = 0;
  for(int i=0; i<54; i++){
    xx[i] = fgetc(fptr_r);
    fputc(xx[i], fptr_w);   //Copia cabecera a nueva img
  }

  ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
  alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

  ptr = (unsigned char*)malloc(alto*ancho*3*3*sizeof(unsigned char));

  while(!feof(fptr_r)){
    b = fgetc(fptr_r);
    g = fgetc(fptr_r);
    r = fgetc(fptr_r);

    ptr[cuenta] = b; //b
    ptr[cuenta+1] = g; //g
    ptr[cuenta+2] = r; //r

    cuenta +=3;
  }

  for(int i=0; i<alto-1; i++){
    for(int j=(ancho*3); j>2; j-=3){
      fputc(ptr[(ancho*i*3)+j+0+162], fptr_w);
      fputc(ptr[(ancho*i*3)+j+1+162], fptr_w);
      fputc(ptr[(ancho*i*3)+j+2+162], fptr_w);
    }
  }

  free(ptr);
  fclose(fptr_r);
  fclose(fptr_w);

  printf("Img #%i was succesfull!!!\n", omp_get_thread_num()+1);
}

int main(){
  double t1, t2; t1 = omp_get_wtime();

  int img_tests = 20; //Static number

  omp_set_num_threads(img_tests);
  #pragma omp parallel
    img_create();

  printf("\nImages: %i", img_tests);
  t2 = omp_get_wtime(); printf("\nTime: %lfs\n", t2-t1);
}
