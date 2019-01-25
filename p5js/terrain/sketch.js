let angle = 0;
let cursive;
let graphics;

let maryknoll;
var maryknoll_complete = "Maryknoll Convent School                     ";
var maryknoll_string_size;
var maryknoll_string;
var string_index;

var t;
var ry = [0, 0, 0, 0, 0];
var ty = [0, 0, 0, 0, 0];

var cols,rows;
var scl = 20;
var w, h;

var flying = 0;

var terrain = [];

function preload() {
  cursive = loadFont('assets/fonts/SchoolScriptDashed.ttf');
}


function setup() {
  createCanvas(800, 800, WEBGL);
  // createCanvas(800, 600);
  string_index = 0;
  maryknoll_string_size = maryknoll_complete.length;
  maryknoll_string = "";
  maryknoll = createGraphics(800, 800);
  maryknoll.fill(255);
  maryknoll.textFont(cursive);
  maryknoll.textAlign(CENTER);
  maryknoll.textSize(64);
  t = 0;
  w = 900;
  h = 900;
  cols = w / scl;
  rows = h / scl;

  for(var y = 0; y < w; y++) {
    var x_val = [];
    for(var x = 0; x < h; x++) {
      x_val.push(0);
    }
    terrain.push(x_val);
  }
  // noStroke();
  var yoff;
  for(var y = 0; y < rows; y++) {
    var xoff = 0;
    for(var x = 0; x < cols; x++) {
      terrain[x][y] = map(noise(xoff, yoff), 0, 1, 0, 150);
      xoff += 0.1;
    }
    yoff += 0.1;
  }
}

function draw() {
  background(0);
  orbitControl();
  drawTerrain();
  t++;
  angle+= 0.03;
}

function drawMaryknoll() {
  maryknoll.clear();
  maryknoll.text(maryknoll_string, 300, 300);
  if(t % 50 == 0) {
    for(var i = 0; i < 5; i++) {
      ry[i] = random(0, PI/2);
      ty[i] = random(-200, 200);
    }
  }
  for(var i = 0; i < 5; i++) {
    rotateY(degrees(ry[i]));
    translate(0, ty[i], 0);
    plane(800, 600);
    translate(0, -ty[i], 0);
    rotateY(-degrees(ry[i]));
  }
  texture(maryknoll);
  scale(0.6, 1, 1);
  angle += 0.03;
  if(t % 4 == 0) {
    if(string_index === maryknoll_string_size) {
      maryknoll_string = "";
      string_index = 0;
    }
    else {
      maryknoll_string += maryknoll_complete[string_index];
      string_index ++;
    }
  }
}

function drawTerrain() {


  maryknoll.text("maryknoll", 300, 300);
  texture(maryknoll);
  flying -= 0.01;
  var yoff = flying;
  for (var y = 0; y < rows; y++) {
    var xoff = 0;
    for (var x = 0; x < cols; x++) {
      terrain[x][y] = map(noise(xoff, yoff), 0, 1, -100, 100);
      xoff += 0.2;
    }
    yoff += 0.2;
  }

  // translate(0, 50);
  // rotateX(-PI/3);
  translate(-w*.5, -h/2);

  for (var y = 0; y < rows-1; y++) {

    beginShape(LINES);
    fill(255);
    for (var x = 0; x < cols; x++) {

      push();
      translate(x*scl, y*scl, terrain[x][y]);
      vertex(x*scl, y*scl, terrain[x][y], map(x, 0, cols, 0, 1), map(y, 0, rows, 0, 1));
      vertex(x*scl, (y+1)*scl, terrain[x][y+1], map(x, 0, cols, 0, 1), map(y+1, 0, rows, 0, 1));
      if(x>0) {
      vertex(x*scl, y*scl, terrain[x][y], map(x, 0, cols, 0, 1), map(y, 0, rows, 0, 1));
      vertex((x-1)*scl, y*scl, terrain[x-1][y], map(x-1, 0, cols, 0, 1), map(y, 0, rows, 0, 1));
      }
      pop();


    }
    endShape();

  }
}
