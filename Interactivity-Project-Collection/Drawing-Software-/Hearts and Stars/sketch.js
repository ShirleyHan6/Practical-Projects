//Declare variables
var N = 300;
var icm;
icm = 4 / N;
var x, y1, y2;
var col = {
  r: 0,
  g: 0,
  b: 0
};
var draw_layer;
var sliderD;
var sliderW;
var sliderC;
//a simple brush drawing ellipses
var switchBrush1 = false;
//drawing stars
var switchBrush2 = false;
//drawing hearts
var switchBrush3 = false;

function setup() {
  createCanvas(600, 600);
  background(255, 182, 193);
  angleMode(RADIANS);
  colorMode(RGB);
  draw_layer = createGraphics(600, 600);
  draw_layer.noStroke();
  
  //create a reset button
  var button = createButton("reset");
  button.doubleClicked(resetSketch);
  
  //create a slider for scaling the hearts
  sliderD = createSlider(0.3, 1, 0.7, 0.1);
  sliderD.input(draw);
  //create a slider for adjusting the lineWidth
  sliderW = createSlider(2,10,6,1);
  sliderW.input(draw);
}

function resetSketch() {
  background(255, 182, 193);
  angleMode(RADIANS);
  draw_layer = createGraphics(600, 600);
  draw_layer.noStroke();
}

function draw() {
  image(draw_layer, 0, 0);
}

//To draw the shape of heart
function heart(co_x, co_y) {
  var lineWidth = sliderW.value();
  beginShape();
  for (var x0 = -2; x0 <= 2;) {
    x = x0 * 20 + co_x;
    y1 = sqrt(2 * sqrt(x0 * x0) - x0 * x0) * 20 + co_y;
    y2 = -2.14 * sqrt(sqrt(2) - sqrt(abs(x0))) * 20 + co_y;
    stroke(123,104,238);
    strokeWeight(lineWidth);
    point(x, y1);
    point(x, y2);
    x0 = x0 + icm;
  }
  endShape(CLOSE);
}


//To draw the shape of stars
function star(x, y, radius1, radius2, npoints) {
  var angle = TWO_PI / npoints;
  var halfAngle = angle/2.0;
  beginShape();
  for (var a = 0; a < TWO_PI; a += angle) {
    var sx = x + cos(a) * radius2;
    var sy = y + sin(a) * radius2;
    stroke(123,104,238);
    vertex(sx, sy);
    sx = x + cos(a+halfAngle) * radius1;
    sy = y + sin(a+halfAngle) * radius1;
    vertex(sx, sy);
  }
  endShape(CLOSE);
}

function mousePressed() {
  if (mouseX <= 600 && mouseY <= 600) {
    if (switchBrush1 == true) {
      r = map(mouseX, 0, width, 20, 60);
      g = map(mouseY, 0, height, 40, 80);
      b = map(mouseY / mouseX, 0, width / height, 100, 140);
      c1 = draw_layer.color(r, g, b);
      draw_layer.fill(c1);
      draw_layer.ellipse(mouseX / 2, mouseY / 2, 4, 4);
    } else if (switchBrush2 == true) {
      //push();
      fill(0,191,255);
      translate(mouseX, mouseY);
      rotate(frameCount / -100.0);
      star(0, 0, 6, 14, 5);
      //pop();
    } else if(switchBrush3 == true) {
      var origin = createVector(mouseX, mouseY);
      var lineCount = random(3, 6);
      var one_angle = 2 * PI / lineCount;
      for (var i = 0; i < lineCount; i = i + 1) {
        push();
        d = sliderD.value();
        translate(width / 2, height / 2);
        rotate(i * one_angle, origin);
        scale(d);
        heart(mouseX, mouseY);
        pop();
      }
    }
  }
}

function mouseDragged()
{
    if (mouseX <= 600 && mouseY <= 600) {
    if (switchBrush1 == true) {
      r = map(mouseX, 0, width, 20, 60);
      g = map(mouseY, 0, height, 40, 80);
      b = map(mouseY / mouseX, 0, width / height, 100, 140);
      c1 = draw_layer.color(r, g, b);
      draw_layer.fill(c1);
      draw_layer.ellipse(mouseX / 2, mouseY / 2, 4, 4);
    } else if (switchBrush2 == true) {
      //push();
      fill(0,191,255);
      translate(mouseX, mouseY);
      rotate(frameCount / -100.0);
      star(0, 0, 6, 14, 5);
      //pop();
    } else if(switchBrush3 == true) {
      var origin = createVector(width/2, height/2);
      var lineCount = random(3, 6);
      var one_angle = 2 * PI / lineCount;
      for (var i = 0; i < lineCount; i = i + 1) {
        push();
        d = sliderD.value();
        translate(width / 2, height / 2);
        rotate(i * one_angle, origin);
        scale(d);
        heart(mouseX, mouseY);
        pop();
      }
    }
  }
}

function keyTyped() {
  if (key === '0') {
    switchBrush1 = true;
    switchBrush2 = false;
    switchBrush3 = false;
  } else if (key === '1') {
    switchBrush1 = false;
    switchBrush2 = true;
    switchBrush3 = false;
  } else if (key === '2') {
    switchBrush1 = false;
    switchBrush2 = false;
    switchBrush3 = true;
  }
}

