#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include <string>   
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "TPGM.h"


#define FLOYD 1
#define THRESHOLD 2
#define RANDOM 3
#define JARVIS 4
#define STUCKI 5
#define ATKINSON 6
#define BURKES 7
#define SIERRA_THREE 8
#define SIERRA_TWO 9
#define SIERRA_ONE 10


using namespace std;

void display_image(string image_name);
void floyd_steinberg(int rows, int cols, unsigned char **a);
void threshold(int rows, int cols, unsigned char **a, unsigned char threshold);
void random(int rows, int cols, unsigned char **a);
void dithering(string image_pgm, int type);
void jarvis_judice_ninke(int rows, int cols, unsigned char **a);
void stucki(int rows, int cols, unsigned char **a);
void atkinson(int rows, int cols, unsigned char **a);
void burkes(int rows, int cols, unsigned char **a);
void sierra_three(int rows, int cols, unsigned char **a);
void sierra_two(int rows, int cols, unsigned char **a);
void sierra_one(int rows, int cols, unsigned char **a);

int main(int argc, char **argv)
{ 
  string file = "x.pgm";
  display_image(file);     
  dithering(file, FLOYD);
  dithering(file, THRESHOLD);
  dithering(file, RANDOM);
  dithering(file, JARVIS);
  dithering(file, STUCKI);
  dithering(file, ATKINSON);
  dithering(file, BURKES);
  dithering(file, SIERRA_THREE);
  dithering(file, SIERRA_TWO);
  dithering(file, SIERRA_ONE);
    
    return 0;
}

void dithering(string image_pgm, int type){
  int rows, cols;           
  int max_color;
  int hpos;  
  string outfname = image_pgm;
  string method;
  

  if ( (hpos = readPGMB_header(image_pgm.c_str(), &rows, &cols, &max_color)) <= 0 )    exit(1);

  unsigned char **a = new unsigned char* [rows];
  a[0] = new unsigned char [rows*cols];
  for(int i = 1; i < rows; i++)
    a[i] = a[i-1] + cols;

  if( readPGMB_data(a[0], image_pgm.c_str(), hpos, rows, cols, max_color) == 0 )     exit(1);

  switch(type){
    case FLOYD:
      floyd_steinberg(rows, cols, a);
      method = "_floyd_steinberg.pgm";
      break;
    case THRESHOLD:
      threshold(rows, cols, a, 128);
      method = "_threshold.pgm";
      break;
    case RANDOM:
      random(rows, cols, a);
      method = "_random.pgm";
      break;
    case JARVIS:
      jarvis_judice_ninke(rows, cols, a);
      method = "_jarvis_judice_ninke.pgm";
      break;
    case STUCKI:
      stucki(rows, cols, a);
      method = "_stucki.pgm";
      break;
    case ATKINSON:
      atkinson(rows, cols, a);
      method = "_atkinson.pgm";
      break;
    case BURKES:
      burkes(rows, cols, a);
      method = "_burkes.pgm";
      break;
    case SIERRA_THREE:
      sierra_three(rows, cols, a);
      method = "_sierra_three.pgm";
      break;
    case SIERRA_TWO:
      sierra_two(rows, cols, a);
      method = "_sierra_two.pgm";
      break;
    case SIERRA_ONE:
      sierra_one(rows, cols, a);
      method = "_sierra_one.pgm";
      break;
  }
  
  outfname.replace(outfname.end()-4,outfname.end(),method);

  if( writePGMB_image(outfname.c_str(), a[0], rows, cols, 255) == 0)     exit(1);

  display_image(outfname);

  delete[] a[0]; delete[] a;
}

void display_image(string image_name){
    cv::Mat image = cv::imread(image_name);     
    cv::imshow(image_name, image);
    cv::waitKey(0);
}

void floyd_steinberg(int rows, int cols, unsigned char **a){
  int i,j;
  int old_pixel, new_pixel;
  int quant_error;
  float w[] = {7.0 / 16.0, 5.0 / 16.0, 3.0 / 16.0, 1.0/16.0};
  for (i=0; i< rows-1; i++){
    for (j=1; j< cols-1; j++){
      old_pixel = a[i][j];
      new_pixel = (a[i][j] > 128 ) ?  255 : 0;
      a[i][j] = new_pixel;
      quant_error = old_pixel - new_pixel;
      a[i][j+1] = a[i][j+1] + quant_error * w[0];
      a[i+1][j-1] = a[i+1][j-1] + quant_error * w[2];
      a[i+1][j] = a[i+1][j] + quant_error * w[1];
      a[i+1][j+1] = a[i+1][j+1] + quant_error * w[3];

    }
  }
  //boundary condidtions
  //last row
  for(j=0; j<cols;j++){
    a[rows-1][j] = (a[rows-1][j] > 128 ) ?  255 : 0;
  }
  //first and last columns
  for(i=0;i<rows;i++){
    a[i][0] = (a[i][0] > 128 ) ?  255 : 0;
    a[i][cols-1] = (a[i][cols-1] > 128 ) ?  255 : 0;
  }
}

void threshold(int rows, int cols, unsigned char **a, unsigned char threshold){
  int i,j;   
  for (i=0; i< rows; i++){
    for (j=0; j< cols; j++){
      a[i][j] = (a[i][j] > threshold ) ?  255 : 0;   
    }
  }
}

void random(int rows, int cols, unsigned char **a){
  int i,j;
  srand (time(NULL));
  for (i=0; i< rows; i++){
    for (j=0; j< cols; j++){
      a[i][j] = (a[i][j] > (rand() % 255) ) ?  255 : 0;   
    }
  }
}

void jarvis_judice_ninke(int rows, int cols, unsigned char **a){
  int i,j;
  int old_pixel, new_pixel;
  int quant_error;
  float w[] = {7.0 / 48.0, 5.0 / 48.0, 3.0 / 48.0, 1.0/48.0};
  for (i=0; i< rows-2; i++){
    for (j=2; j< cols-2; j++){
      old_pixel = a[i][j];
      new_pixel = (a[i][j] > 128 ) ?  255 : 0;
      a[i][j] = new_pixel;
      quant_error = old_pixel - new_pixel;
      a[ i ][j+1] = a[ i ][j+1] + quant_error * w[0];
      a[ i ][j+2] = a[ i ][j+2] + quant_error * w[1];
      a[i+1][j-2] = a[i+1][j-2] + quant_error * w[2];
      a[i+1][j-1] = a[i+1][j-1] + quant_error * w[1];
      a[i+1][ j ] = a[i+1][ j ] + quant_error * w[0];
      a[i+1][j+1] = a[i+1][j+1] + quant_error * w[1];
      a[i+1][j+2] = a[i+1][j+2] + quant_error * w[2];
      a[i+2][j-2] = a[i+2][j-2] + quant_error * w[3];
      a[i+2][j-1] = a[i+2][j-1] + quant_error * w[2];
      a[i+2][ j ] = a[i+2][ j ] + quant_error * w[1];
      a[i+2][j+1] = a[i+2][j+1] + quant_error * w[2];
      a[i+2][j+2] = a[i+2][j+2] + quant_error * w[3];
    }
  }
  //boundary condidtions
  //last two rows
  for(j=0; j<cols;j++){
    a[rows-2][j] = (a[rows-2][j] > 128 ) ?  255 : 0;
    a[rows-1][j] = (a[rows-1][j] > 128 ) ?  255 : 0;
  }
  //first, second, last and one before last columns
  for(i=0;i<rows;i++){
    a[i][0] = (a[i][0] > 128 ) ?  255 : 0;
    a[i][1] = (a[i][1] > 128 ) ?  255 : 0;
    a[i][cols-2] = (a[i][cols-2] > 128 ) ?  255 : 0;
    a[i][cols-1] = (a[i][cols-1] > 128 ) ?  255 : 0;
  }
}

void stucki(int rows, int cols, unsigned char **a){
  int i,j;
  int old_pixel, new_pixel;
  int quant_error;
  float w[] = {8.0 / 42.0, 4.0 / 42.0, 2.0 / 42.0, 1.0/42.0};
  for (i=0; i< rows-2; i++){
    for (j=2; j< cols-2; j++){
      old_pixel = a[i][j];
      new_pixel = (a[i][j] > 128 ) ?  255 : 0;
      a[i][j] = new_pixel;
      quant_error = old_pixel - new_pixel;
      a[ i ][j+1] = a[ i ][j+1] + quant_error * w[0];
      a[ i ][j+2] = a[ i ][j+2] + quant_error * w[1];
      a[i+1][j-2] = a[i+1][j-2] + quant_error * w[2];
      a[i+1][j-1] = a[i+1][j-1] + quant_error * w[1];
      a[i+1][ j ] = a[i+1][ j ] + quant_error * w[0];
      a[i+1][j+1] = a[i+1][j+1] + quant_error * w[1];
      a[i+1][j+2] = a[i+1][j+2] + quant_error * w[2];
      a[i+2][j-2] = a[i+2][j-2] + quant_error * w[3];
      a[i+2][j-1] = a[i+2][j-1] + quant_error * w[2];
      a[i+2][ j ] = a[i+2][ j ] + quant_error * w[1];
      a[i+2][j+1] = a[i+2][j+1] + quant_error * w[2];
      a[i+2][j+2] = a[i+2][j+2] + quant_error * w[3];
    }
  }
  //boundary condidtions
  //last two rows
  for(j=0; j<cols;j++){
    a[rows-2][j] = (a[rows-2][j] > 128 ) ?  255 : 0;
    a[rows-1][j] = (a[rows-1][j] > 128 ) ?  255 : 0;
  }
  //first, second, last and one before last columns
  for(i=0;i<rows;i++){
    a[i][0] = (a[i][0] > 128 ) ?  255 : 0;
    a[i][1] = (a[i][1] > 128 ) ?  255 : 0;
    a[i][cols-2] = (a[i][cols-2] > 128 ) ?  255 : 0;
    a[i][cols-1] = (a[i][cols-1] > 128 ) ?  255 : 0;
  }
}

void atkinson(int rows, int cols, unsigned char **a){
  int i,j;
  int old_pixel, new_pixel;
  int quant_error;
  float w = 1.0 / 8;
  for (i=0; i< rows-2; i++){
    for (j=1; j< cols-2; j++){
      old_pixel = a[i][j];
      new_pixel = (a[i][j] > 128 ) ?  255 : 0;
      a[i][j] = new_pixel;
      quant_error = old_pixel - new_pixel;
      a[ i ][j+1] = a[ i ][j+1] + quant_error * w;
      a[ i ][j+2] = a[ i ][j+2] + quant_error * w;
      a[i+1][j-1] = a[i+1][j-1] + quant_error * w;
      a[i+1][ j ] = a[i+1][ j ] + quant_error * w;
      a[i+1][j+1] = a[i+1][j+1] + quant_error * w;
      a[i+2][ j ] = a[i+2][ j ] + quant_error * w;
    }
  }
  //boundary condidtions
  //last two rows
  for(j=0; j<cols;j++){
    a[rows-2][j] = (a[rows-2][j] > 128 ) ?  255 : 0;
    a[rows-1][j] = (a[rows-1][j] > 128 ) ?  255 : 0;
  }
  //first, last and one before last columns
  for(i=0;i<rows;i++){
    a[i][0] = (a[i][0] > 128 ) ?  255 : 0;
    a[i][cols-2] = (a[i][cols-2] > 128 ) ?  255 : 0;
    a[i][cols-1] = (a[i][cols-1] > 128 ) ?  255 : 0;
  }
}

void burkes(int rows, int cols, unsigned char **a){
  int i,j;
  int old_pixel, new_pixel;
  int quant_error;
  float w[] = {8.0 / 32.0, 4.0 / 32.0, 2.0 / 32.0};
  for (i=0; i< rows-1; i++){
    for (j=2; j< cols-2; j++){
      old_pixel = a[i][j];
      new_pixel = (a[i][j] > 128 ) ?  255 : 0;
      a[i][j] = new_pixel;
      quant_error = old_pixel - new_pixel;
      a[ i ][j+1] = a[ i ][j+1] + quant_error * w[0];
      a[ i ][j+2] = a[ i ][j+2] + quant_error * w[1];
      a[i+1][j-2] = a[i+1][j-2] + quant_error * w[2];
      a[i+1][j-1] = a[i+1][j-1] + quant_error * w[1];
      a[i+1][ j ] = a[i+1][ j ] + quant_error * w[0];
      a[i+1][j+1] = a[i+1][j+1] + quant_error * w[1];
      a[i+1][j+2] = a[i+1][j+2] + quant_error * w[2];
    }
  }
  //boundary condidtions
  //last row
  for(j=0; j<cols;j++){
    a[rows-1][j] = (a[rows-1][j] > 128 ) ?  255 : 0;
  }
  //first, second, last and one before last columns
  for(i=0;i<rows;i++){
    a[i][0] = (a[i][0] > 128 ) ?  255 : 0;
    a[i][1] = (a[i][1] > 128 ) ?  255 : 0;
    a[i][cols-2] = (a[i][cols-2] > 128 ) ?  255 : 0;
    a[i][cols-1] = (a[i][cols-1] > 128 ) ?  255 : 0;
  }
}

void sierra_three(int rows, int cols, unsigned char **a){
  int i,j;
  int old_pixel, new_pixel;
  int quant_error;
  float w[] = {5.0 / 32.0, 4.0 / 32.0, 3.0 / 32.0, 2.0/32.0};
  for (i=0; i< rows-2; i++){
    for (j=2; j< cols-2; j++){
      old_pixel = a[i][j];
      new_pixel = (a[i][j] > 128 ) ?  255 : 0;
      a[i][j] = new_pixel;
      quant_error = old_pixel - new_pixel;
      a[ i ][j+1] = a[ i ][j+1] + quant_error * w[0];
      a[ i ][j+2] = a[ i ][j+2] + quant_error * w[2];
      a[i+1][j-2] = a[i+1][j-2] + quant_error * w[3];
      a[i+1][j-1] = a[i+1][j-1] + quant_error * w[1];
      a[i+1][ j ] = a[i+1][ j ] + quant_error * w[0];
      a[i+1][j+1] = a[i+1][j+1] + quant_error * w[1];
      a[i+1][j+2] = a[i+1][j+2] + quant_error * w[3];
      a[i+2][j-1] = a[i+2][j-1] + quant_error * w[3];
      a[i+2][ j ] = a[i+2][ j ] + quant_error * w[2];
      a[i+2][j+1] = a[i+2][j+1] + quant_error * w[3];
    }
  }
  //boundary condidtions
  //last two rows
  for(j=0; j<cols;j++){
    a[rows-2][j] = (a[rows-2][j] > 128 ) ?  255 : 0;
    a[rows-1][j] = (a[rows-1][j] > 128 ) ?  255 : 0;
  }
  //first, second, last and one before last columns
  for(i=0;i<rows;i++){
    a[i][0] = (a[i][0] > 128 ) ?  255 : 0;
    a[i][1] = (a[i][1] > 128 ) ?  255 : 0;
    a[i][cols-2] = (a[i][cols-2] > 128 ) ?  255 : 0;
    a[i][cols-1] = (a[i][cols-1] > 128 ) ?  255 : 0;
  }
}

void sierra_two(int rows, int cols, unsigned char **a){
  int i,j;
  int old_pixel, new_pixel;
  int quant_error;
  float w[] = {4.0 / 16.0, 3.0 / 16.0, 2.0/16.0, 1.0 / 16.0};
  for (i=0; i< rows-1; i++){
    for (j=2; j< cols-2; j++){
      old_pixel = a[i][j];
      new_pixel = (a[i][j] > 128 ) ?  255 : 0;
      a[i][j] = new_pixel;
      quant_error = old_pixel - new_pixel;
      a[ i ][j+1] = a[ i ][j+1] + quant_error * w[0];
      a[ i ][j+2] = a[ i ][j+2] + quant_error * w[1];
      a[i+1][j-2] = a[i+1][j-2] + quant_error * w[3];
      a[i+1][j-1] = a[i+1][j-1] + quant_error * w[2];
      a[i+1][ j ] = a[i+1][ j ] + quant_error * w[1];
      a[i+1][j+1] = a[i+1][j+1] + quant_error * w[2];
      a[i+1][j+2] = a[i+1][j+2] + quant_error * w[3];
    }
  }
  //boundary condidtions
  //last two rows
  for(j=0; j<cols;j++){
    a[rows-1][j] = (a[rows-1][j] > 128 ) ?  255 : 0;
  }
  //first, second, last and one before last columns
  for(i=0;i<rows;i++){
    a[i][0] = (a[i][0] > 128 ) ?  255 : 0;
    a[i][1] = (a[i][1] > 128 ) ?  255 : 0;
    a[i][cols-2] = (a[i][cols-2] > 128 ) ?  255 : 0;
    a[i][cols-1] = (a[i][cols-1] > 128 ) ?  255 : 0;
  }
}

void sierra_one(int rows, int cols, unsigned char **a){
  int i,j;
  int old_pixel, new_pixel;
  int quant_error;
  float w[] = {2.0 / 4.0, 1.0 / 4.0};
  for (i=0; i< rows-1; i++){
    for (j=1; j< cols-1; j++){
      old_pixel = a[i][j];
      new_pixel = (a[i][j] > 128 ) ?  255 : 0;
      a[i][j] = new_pixel;
      quant_error = old_pixel - new_pixel;
      a[ i ][j+1] = a[ i ][j+1] + quant_error * w[0];
      a[i+1][j-1] = a[i+1][j-1] + quant_error * w[1];
      a[i+1][ j ] = a[i+1][ j ] + quant_error * w[1];
    }
  }
  //boundary condidtions
  //last two rows
  for(j=0; j<cols;j++){
    a[rows-2][j] = (a[rows-2][j] > 128 ) ?  255 : 0;
    a[rows-1][j] = (a[rows-1][j] > 128 ) ?  255 : 0;
  }
  //first, second, last and one before last columns
  for(i=0;i<rows;i++){
    a[i][0] = (a[i][0] > 128 ) ?  255 : 0;
    a[i][cols-1] = (a[i][cols-1] > 128 ) ?  255 : 0;
  }
}