var canvas;
var grid_size;
var mouseMovedCounter, mouseMovedFlag, maxMouseMovedCounter;
var maxVerticalLines;
var xOffset, yOffset, maxOffset, offset;
var prevTop, prevBottom, prevLeft, prevRight;

function setup() {
  canvas = createCanvas(windowWidth, windowHeight);
  canvas.position(0, 0);
  canvas.style('z-index', '-1');
  background(0);
  maxVerticalLines = 15;      // number of vertical lines is fixed
  grid_size = windowWidth / maxVerticalLines;
  mouseMovedFlag = 0;
  mouseMovedCounter = 0;
  maxMouseMovedCounter = 20; // number of glitch ticks
  maxOffset = 1;
  offset = maxOffset;
  prevTop = 0;
  prevBottom = 0;
  prevLeft = 0;
  prevRight = 0;
  stroke(255);
}

function draw() {
  // clear draw from previous frame
  background(0);

  // get neighboring 4 lines based on mouse location
  var top = grid_size * Math.floor(mouseY / grid_size); // 0
  var bottom = top + grid_size + 3; // 1
  var left = grid_size * Math.floor(mouseX / grid_size); // 2
  var right = left + grid_size + 3; // 3

  // check if neighboring lines change
  if(top === prevTop && bottom == prevBottom && left == prevLeft && right == prevRight) {
    // turn on glitch if neighboring lines dont change (mouse stays)
    if(mouseMovedFlag == 0) mouseMovedFlag = 1;
    if(mouseMovedFlag == 1) {
      if(mouseMovedCounter < maxMouseMovedCounter) {
        mouseMovedCounter++;
        offset = map(mouseMovedCounter, 0, maxMouseMovedCounter, maxOffset, 0);
      }
    }
  } else {
    // reset all if neighboring lines change
    mouseMovedFlag = 0;
    mouseMovedCounter = 0;
    offset = maxOffset;
  }

  // vertical lines
  // from top to bottom
  if(mouseMovedFlag == 1 && mouseMovedCounter < maxMouseMovedCounter) {
    xOffset = 0;
    for(var x = grid_size; x < windowWidth; x+=grid_size) {
      for(var y = 0; y < windowHeight; y++) {
        if(x >= left && x <= right && y >= top && y <= bottom) {
          if(int(random(0, 10)) == 1) {
            xOffset += random(-offset, offset);
          }
          point(x + xOffset, y);
        } else {
          point(x, y);
        }
      }
    }

    // from left to right
    yOffset = 0;
    for(var y = grid_size; y < windowHeight; y+=grid_size) {
      for(var x = 0; x < windowWidth; x++) {
        if(x >= left && x <= right && y >= top && y <= bottom) {
          if(int(random(0, 10)) == 1) {
            yOffset += random(-offset, offset);
          }
          point(x, y + yOffset);
        } else {
          point(x, y);
        }
      }
    }
  } else {
    for(var x = grid_size; x < windowWidth; x+=grid_size) {
      for(var y = 0; y < windowHeight; y++) {
        point(x, y);
      }
    }

    // from left to right
    yOffset = 0;
    for(var y = grid_size; y < windowHeight; y+=grid_size) {
      for(var x = 0; x < windowWidth; x++) {
        point(x, y);
      }
    }
  }

  // store previous neighboring lines if change
  if(mouseMovedFlag == 0){
    prevTop = top;
    prevBottom = bottom;
    prevLeft = left;
    prevRight = right;
  }

}

function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
  grid_size = windowWidth / maxVerticalLines;
}
