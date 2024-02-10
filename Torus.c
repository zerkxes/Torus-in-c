#include <math.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#define theta_spacing 0.07
#define phi_spacing 0.02
#define screen_width 32
#define screen_height 32

#define R1 1
#define R2 2
#define K2 25
#define K1 (screen_width * K2 * 3 / (8 * (R1 + R2)))

char luminance_char[] = ".,-~:;=!*#$@";

void draw(float A, float B) {
  float cosA = cos(A);
  float cosB = cos(B);
  float sinA = sin(A);
  float sinB = sin(B);

  char output[screen_width][screen_height];
  float zBuffer[screen_width][screen_height];

  for (int i = 0; i < screen_width; i++) {
    for (int j = 0; j < screen_height; j++) {
      output[i][j] = ' ';
      zBuffer[i][j] = 0;
    }
  }

  for (float theta = 0.0; theta < 2 * M_PI; theta += theta_spacing) {
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    for (float phi = 0.0; phi < 2 * M_PI; phi += phi_spacing) {
      float cosPhi = cos(phi);
      float sinPhi = sin(phi);

      float circleX = R2 + R1 * cosTheta;
      float circleY = R1 * sinTheta;

      float x = circleX * (cosB * cosPhi + sinA * sinB * sinPhi) -
                circleY * cosA * sinB;
      float y = circleX * (cosPhi * sinB - cosB * sinA * sinPhi) +
                circleY * cosA * cosB;
      float z = K2 + cosA * circleX * sinPhi + circleY * sinA;

      float obz = 1 / z;

      int xProj = (int)(screen_width / 2.0 + K1 * obz * x);
      int yProj = (int)(screen_height / 2.0 - K1 * obz * y);

      float Lumi = cosPhi * cosTheta * sinB - cosA * cosTheta * sinPhi -
                   sinA * sinTheta +
                   cosB * (cosA * sinTheta - cosTheta * sinA * sinPhi);

      if (Lumi > 0) {
        if (obz > zBuffer[xProj][yProj]) {
          zBuffer[xProj][yProj] = obz;
          int lumi_index = (int)(Lumi * 8);
          output[xProj][yProj] = luminance_char[lumi_index];
        }
      }
    }
  }

  printf("\x1b[H");
  for (int j = 0; j < screen_height; j++) {
    for (int i = 0; i < screen_width; i++) {
      putchar(output[i][j]);
    }
    putchar('\n');
  }
}

int main(void) {
  for (float i = 0, j = 0;; i += 0.0004, j += 0.0002) {
    draw(i, j);
    // printf("\x1b[2J");
  }
  // printf("\x1b[2J");
  return 0;
}
