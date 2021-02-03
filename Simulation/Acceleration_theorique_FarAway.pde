//Permet de visualiser l'accélération maximale  theorique de la grue en fonction du couple moteur (l'accélération reel sera plus faible

int ecranx = 800;
int ecrany = 800;


float ax = ecranx/2;
float ay = ecrany/2;
float bx;
float by;
float l1 = min(ecranx/2, ecrany/2)-10;
float coupleMoteur = 4.5; // En N/m
float J = 2.73;
float K1 = coupleMoteur/(J*2);
int tpsini = 0;


void setup() {
  size(800, 800);
  stroke(255);
  tpsini = millis();
  print(K1);
}

void draw() {
  background(0);
  updateline();
  line(ax, ay, bx, by);
}
void updateline() {
  float tps = millis() - tpsini;
  bx=l1*cos(K1*tps*tps/1000000)+ax;
  by=l1*sin(K1*tps*tps/1000000)+ay;
}

void mousePressed() {
  tpsini = millis();
}
