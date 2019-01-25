
int cols, rows;
int scl = 20;
int w, h;
int x = 0;
int y = 0;
float flying = 0;

float[][] terrain;

PFont cursive;
PGraphics pg;
String s = "Maryknoll Convent School";

void setup() {
  size(600, 600, P3D);
  w = 600;
  h = 600;
  cols = w / scl;
  rows = h / scl;
  terrain = new float[cols][rows];
  cursive = createFont("SchoolScriptDashed.ttf", 48);
  pg = createGraphics(800, 800);
}


void draw() {
  background(255);
  
  // Updating 2D noise data
  float yoff = flying;
  for(int y = 0; y < rows; y++) {
    float xoff = 0;
    for(int x = 0; x < cols; x++) {
      terrain[x][y] = map(noise(xoff, yoff), 0, 1, -300, 300);
      xoff += 0.1;
    }
    yoff+= 0.1;
  }
  
   
  // Updating 2D sine wave
  //for(int y = 0; y < rows; y++) {
  //  for(int x = 0; x < cols; x++) {
  //    terrain[x][y] = twod_sine(x, y, 0.4, 1., 0.8, 0.2);
  //  }
  //}
  
     
  noStroke();
  noFill();
  
  // draw text as texture
  pg.beginDraw();
  pg.textAlign(CENTER);
  pg.background(255);
  pg.fill(0);
  pg.textFont(cursive);
  pg.text(s, pg.width/2, pg.height/2);
  pg.endDraw();
  
  textureMode(NORMAL);
  for(int y = 0; y < rows-1; y++) {
    beginShape(TRIANGLE_STRIP);
    texture(pg);      // uncomment to show texture map
    //stroke(0);      // uncomment to show just terrain (geometry)
    for(int x = 0; x < cols; x++) {
      vertex(x * scl, y * scl, terrain[x][y], float(x) / float(cols-1), float(y) / float(rows-1));
      vertex(x * scl, (y+1) * scl, terrain[x][y+1], float(x) / float(cols-1), float(y+1) / float(rows-1));
    }
    endShape();
  }
  
  flying -= .01;
  //saveFrame("frames/####.png");
}

// amplitude: define no. of peaks (amp = 1 means 5 peaks)
// speed: define speed of wave travel
float twod_sine(int row_index, int row_number, float amplitudex, float amplitudey, float speedx, float speedy) {
  float sinex = sin((millis()) * PI / 1000.0 * speedx + row_index) * amplitudex / 2;
  float siney = sin((millis()) * PI / 1000.0 * speedy + row_number) * amplitudey / 2;
  float sine = sinex + siney;
  float mapped_sine = (sine - (-1)) * (50 - (-50)) / (1 - ( - 1)) - 50;
  float int_sine = mapped_sine;
  return int_sine;
}