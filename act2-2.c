#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void img_create(int n){
//Img named as thread number created
char org[16]; snprintf(org, 16, "input_%i.bmp", n);
char dst[16]; snprintf(dst, 16, "blur_%i.bmp", n);
//Open file
FILE *fptr_r; fptr_r=fopen(org,"rb");
FILE *fptr_w; fptr_w=fopen(dst,"wb");
long ancho, alto;
unsigned char r, g, b;
unsigned char* ptr;
//BMP bitmap signature
unsigned char xx[54];
for(int i=0; i<54; i++){
xx[i] = fgetc(fptr_r);
fputc(xx[i], fptr_w);
}
//Read width(bytes 18-21) && height(bytes 22-25)
ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

ptr = (unsigned char*)malloc((alto*2*3+ancho*2*3+4*3*3+alto*ancho*3)*sizeof(unsigned char));
for(int i=0; i<(alto*ancho*3)+87; i+=3){
b = fgetc(fptr_r);
g = fgetc(fptr_r);
r = fgetc(fptr_r);
//Convert to gray scale
unsigned char pixel = 0.21*r+0.72*g+0.07*b;
ptr[i] = pixel; //b
ptr[i+1] = pixel; //g
ptr[i+2] = pixel; //r
}
//Goes through matrix
for(int i=0; i<alto-1; i++){
for(int j=(ancho*3); j>2; j-=3){
r = g = b = 0;
//Visit every pixel around (total of 49 on 7*7)
for(int k=-3; k<=3; k++)
for(int l=-3; l<=3; l++){
//Sum of pixel average
r += ptr[(ancho*i*3+l)+j+0+162+(k*3)]/49;
g += ptr[(ancho*i*3+l)+j+1+162+(k*3)]/49;
b += ptr[(ancho*i*3+l)+j+2+162+(k*3)]/49;
}
fputc(r, fptr_w);
fputc(g, fptr_w);
fputc(b, fptr_w);
}
}
//Close images and destroy vars
free(ptr);
fclose(fptr_r);
fclose(fptr_w);
printf("Img #%i was succesfull!!!\n", n);
}

int main(){
double t1, t2, time;
int img_tests = 20;
omp_set_num_threads(img_tests);
t1 = omp_get_wtime();
#pragma omp parallel
img_create(omp_get_thread_num()+1);
t2 = omp_get_wtime();
time = t2-t1;
printf("Time: %lf\n", time);
}
