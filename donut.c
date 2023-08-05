#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <Windows.h>

//this version does not require a math library but i have not studied it yet
/*
#define R(mul, shift, x, y)            \
    _ = x;                             \
    x -= mul * y >> shift;             \
    y += mul * _ >> shift;             \
    _ = 3145728 - x * x - y * y >> 11; \
    x = x * _ >> 10;                   \
    y = y * _ >> 10;

int8_t b[1760], z[1760];

int main()
{
    int sA = 1024, cA = 0, sB = 1024, cB = 0, _;
    for (;;)
    {
        memset(b, 32, 1760);  // text buffer
        memset(z, 127, 1760); // z buffer
        int sj = 0, cj = 1024;
        for (int j = 0; j < 90; j++)
        {
            int si = 0, ci = 1024; // sine and cosine of angle i
            for (int i = 0; i < 324; i++)
            {
                int R1 = 1, R2 = 2048, K2 = 5120 * 1024;

                int x0 = R1 * cj + R2,
                    x1 = ci * x0 >> 10,
                    x2 = cA * sj >> 10,
                    x3 = si * x0 >> 10,
                    x4 = R1 * x2 - (sA * x3 >> 10),
                    x5 = sA * sj >> 10,
                    x6 = K2 + R1 * 1024 * x5 + cA * x3,
                    x7 = cj * si >> 10,
                    x = 40 + 30 * (cB * x1 - sB * x4) / x6,
                    y = 12 + 15 * (cB * x4 + sB * x1) / x6,
                    N = (-cA * x7 - cB * ((-sA * x7 >> 10) + x2) - ci * (cj * sB >> 10) >> 10) - x5 >> 7;

                int o = x + 80 * y;
                int8_t zz = (x6 - K2) >> 15;
                if (22 > y && y > 0 && x > 0 && 80 > x && zz < z[o])
                {
                    z[o] = zz;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
                R(5, 8, ci, si) // rotate i
            }
            R(9, 7, cj, sj) // rotate j
        }
        for (int k = 0; 1761 > k; k++)
            putchar(k % 80 ? b[k] : 10);
        R(5, 7, cA, sA);
        R(5, 8, cB, sB);
        system("cls");  //clear the screen
    }
    return 0;
}*/

#define DISP_WIDTH 60   //width of the display 
#define DISP_HEIGHT 20  //height of the display
#define PI 3.14 //pi

int k;
float thetaSpacing = 0.07;
float phiSpacing = 0.02;
int distanceFromScreen = 8; //how far away the donut is rendered

double sin(), cos();
int main()
{
    float A = 0, B = 0; //initialise the multipliers for rotations - these represent the axes about which the donut spins
    float z[DISP_WIDTH * DISP_HEIGHT];  //z buffer
    char b[DISP_WIDTH * DISP_HEIGHT];   //the donut
    system("cls");  //clear the screen
    while(1) {
        //empty the arrays
        memset(b, 32, DISP_WIDTH * DISP_HEIGHT);
        memset(z, 0, DISP_WIDTH * DISP_HEIGHT * 4); //this is the z-buffer - map the values over each other multiple times times

        //for each point on the donut
        for (float j = 0; j < 2 * PI; j += thetaSpacing)
            for (float i = 0; i < 2 * PI; i += phiSpacing)
            {
                //create the donut
                float c = sin(i), 
                    d = cos(j), 
                    e = sin(A), 
                    f = sin(j), 
                    g = cos(A), 
                    h = d + 3, //the constant here will scale the donut
                    D = 1 / (c * h * e + f * g + distanceFromScreen), 
                    l = cos(i), 
                    m = cos(B), 
                    n = sin(B),
                    t = c * h * g - f * e;

                //finding the x and y values - the constants are half the size of the display
                int x = (DISP_WIDTH / 2) + 30 * D * (l * h * m - t * n),
                    y = (DISP_HEIGHT / 2) + 15 * D * (l * h * n + t * m), 
                    
                    o = x + DISP_WIDTH * y, //convert x and y into points in the array - the multiple is the width of the display
                    N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);  //lighting

                if (DISP_HEIGHT > y && y > 0 && x > 0 && DISP_WIDTH > x && D > z[o])    //if the coordinate is within the bounds of the viewable space
                {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        printf("\x1b[H");   //put the cursor back to the start of the screen to overwrite whats there
        //80 characters per line
        for (k = 0; k < ((DISP_HEIGHT * DISP_WIDTH) + 1); k++)
            putchar(k % DISP_WIDTH ? b[k] : 10);
        A += 0.04;
        B += 0.02;
    }
    return 0;
}