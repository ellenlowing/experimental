import gab.opencv.*;

OpenCV opencv;
PImage gray_image;
float [] gray_vals;

int stride = 10;
int mode = 0;

String filename = "ellenloo.jpg";
String exportfilename = "ellenloo-pixelated-2.png";

void setup(){
  background(255);
  opencv = new OpenCV(this, filename);
  gray_image = opencv.getOutput();
  //size(gray_image.width * stride, gray_image.height * stride);
  size(1000, 1000);
  gray_image.loadPixels();
  gray_vals = new float [gray_image.width * gray_image.height];
  for(int i = 0; i < gray_image.height; i++){
    for(int j = 0; j < gray_image.width; j++){
      gray_vals[i * gray_image.width + j] = red(gray_image.pixels[i*gray_image.width + j]);
    }
  }
  noStroke();
  fill(0);
  for(int i = 0; i < height ; i+=stride){
    for(int j = 0; j < width ; j+=stride){
      float gray_val = gray_vals[i/stride * gray_image.width + j/stride];
      float diameter = gray_val / 255.0 * stride;
      if(mode == 0){
        diameter = stride - diameter;
      } else if (mode == 1){
        diameter = diameter;
      }
      ellipse(j, i, diameter, diameter);
    }
  }
  save(exportfilename);
}

void draw(){
  
  
}