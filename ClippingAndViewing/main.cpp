///author: shuhan mirza
//
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include<bits/stdc++.h>

using namespace std;

struct Point {
	GLint x;
	GLint y;
};


struct Color {
	GLfloat r;
	GLfloat g;
	GLfloat b;
};

typedef float matrix3x3 [3][3];
typedef float matrix1x3 [1][3];

#define PI 3.14159265

int sizei = 600;
float sizef = 600.0;
float midf = sizef/2;
float midi = sizei/2;

int polygonX[100];
int polygonY[100];
int numPolygon;

int Xmin,Ymin,Xmax,Ymax;
double line[100][4];
int regionCode[100][2];
int visible[100];
int numLine;

bool needToClip;

Color red = {1.0,0.0,0.0};
Color white = {1.0,1.0,1.0};
Color green = {0.0,1.0,0.0};
Color blue = {0.0,0.0,1.0};

float enPixel(float x)
{
  float p = (x/midf);
  return p;
}

int dePixel(float x)
{
  float p = (x*midf);

  return (int)p;
}
Color getPixel(GLint x, GLint y) {
	Color color;
	glReadPixels(x+midi,y+midi,1,1, GL_RGB, GL_FLOAT, &color);
	return color;
}
void setPixel(float x,float y, Color color) {
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_POINTS);
        glVertex2f(x, y);
	glEnd();
	glFlush();
}

void drawAxis()
{
    glColor3f(1.0,1.0,0.0);
    glBegin(GL_LINES);
        glVertex2f(enPixel(0),enPixel(-midi));
        glVertex2f(enPixel(0),enPixel(midi));

        glVertex2f(enPixel(-midi),enPixel(0));
        glVertex2f(enPixel(midi),enPixel(0));
   glEnd();
    glFlush();
}

void drawLine(int x0,int y0,int x1 , int y1 , Color color)
{
    glColor3f(color.r,color.g,color.b);
    glBegin(GL_LINES);
        glVertex2f(enPixel(x0),enPixel(y0));
        glVertex2f(enPixel(x1),enPixel(y1));
   glEnd();
   glFlush();
}

void drawLines()
{
    int i;

    for(i = 0 ; i < numLine ; i++)
    {
        if(visible[i] != 0)
        drawLine(line[i][0],line[i][1],line[i][2],line[i][3],red);
    }
}

void clip()
{
    int i,j;
    double m,tmp;
    char c = 'A';
    int danger = 0;

    if(needToClip == false) return;

    for(i = 0 ; i < numLine ; i++,c+=2)
    {
       if(visible[i] != 1) continue;
       danger = 0;
       if(line[i][0] == line[i][2]) danger++;

       if(danger == 0) m = (double)((line[i][1] - line[i][3]) / (line[i][0] - line[i][2]));
       ///
       if(regionCode[i][0] & 1)
         {
            if(danger == 0) tmp = line[i][1] + m*(Xmin - line[i][0]);
            else tmp = Ymax;
            line[i][1] =  tmp;
            line[i][0] = Xmin;

            cout << c <<"`( " << line[i][0] << "," << line[i][1] << " )\n";
         }


        else if(regionCode[i][0] & 2)
         {
            if(danger == 0) tmp = line[i][1] + m*(Xmax - line[i][0]);
            else tmp == Ymax;
            line[i][1] =  (int)tmp;
            line[i][0] = Xmax;

            cout << c <<"`( " << line[i][0] << "," << line[i][1] << " )\n";
         }

        else if(regionCode[i][0] & 4)
         {
            if(danger == 0)tmp = (double)(line[i][0] + (Ymin - line[i][1])/m);
            else tmp = line[i][0];
            line[i][0] =  (int)tmp;
            line[i][1] = Ymin;

            cout << c <<"`( " << line[i][0] << "," << line[i][1] << " )\n";
         }



        else if(regionCode[i][0] & 8)
         {
            if(danger == 0) tmp = (double)(line[i][0] + (Ymax - line[i][1])/m);
            tmp = line[i][0];
            line[i][0] =  (int)tmp;
            line[i][1] = Ymax;

            cout << c <<"`( " << line[i][0] << "," << line[i][1] << " )\n";
         }



    ///

        if(regionCode[i][1] & 1)
         {
            if(danger == 0)tmp = line[i][3] + m*(Xmin - line[i][2]);
            else tmp = Ymax;
            line[i][3] =  (int)tmp;
            line[i][2] = Xmin;

            cout << (char)(c+1) <<"`( " << line[i][2] << "," << line[i][3] << " )\n";
         }

        else if(regionCode[i][1] & 2)
         {
            if(danger == 0)tmp = line[i][3] + m*(Xmax - line[i][2]);
            else tmp = Ymax;
            line[i][3] =  (int)tmp;
            line[i][2] = Xmax;

            cout << (char)(c+1) <<"`( " << line[i][2] << "," << line[i][3] << " )\n";
         }


         else if(regionCode[i][1] & 4)
         {
            if(danger == 0)tmp = (double)(line[i][2] + (Ymin - line[i][3])/m);
            else tmp = line[i][2];

            line[i][2] =  (int)tmp;
            line[i][3] = Ymin;

            cout << (char)(c+1) <<"`( " << line[i][2] << "," << line[i][3] << " )\n";
         }

         else if(regionCode[i][1] & 8)
         {
            if(danger == 0)tmp = (double)(line[i][2] + (Ymax - line[i][3])/m);
            else tmp = line[i][2];

            line[i][2] =  (int)tmp;
            line[i][3] = Ymax;

            cout << (char)(c+1) <<"`( " << line[i][2] << "," << line[i][3] << " )\n";
         }
    }
}

void buildRegionCode()
{
    int i;
    char c = 'A';
    needToClip = false;

    for ( i = 0 ; i < numLine ; i++ )
    {
        int sum = 0;
        int x = line[i][0];
        int y = line[i][1];

        if(x<Xmin)
            sum+=1;
        if(x>Xmax)
            sum+=2;
        if(y<Ymin)
            sum+=4;
        if(y>Ymax)
            sum+=8;

        regionCode[i][0] = sum;

        cout << c << "( " << bitset<4>(sum) << " )\n";
        c++;

        sum = 0;
        x = line[i][2];
        y = line[i][3];

        if(x < Xmin)
            sum+=1;
        if(x > Xmax)
            sum+=2;
        if(y < Ymin)
            sum+=4;
        if(y > Ymax)
            sum+=8;

        regionCode[i][1] = sum;

        cout << c << "( " << bitset<4>(sum) << " )\n\n";
        c++;

        if((regionCode[i][0] & regionCode[i][1]) == 0)
        {
            if(regionCode[i][0] == 0 && regionCode[i][1] == 0)
                visible[i] = 2; //window er vitore
            else
            {
                needToClip = true;
                visible[i] = 1;
            }
        }
        else
        {
            visible[i] = 0;
        }
    }
}

static void display(void)
{
  int x;
  int y = 0;
  drawAxis();
     ///draw window

    drawLine(Xmin,Ymin,Xmin,Ymax,white);
    drawLine(Xmin,Ymin,Xmax,Ymin,white);
    drawLine(Xmax,Ymax,Xmin,Ymax,white);
    drawLine(Xmax,Ymax,Xmax,Ymin,white);

  drawLines();
  cout << "\n\nregion codes\n";
  buildRegionCode();
}

static void on(int button ,int state ,int x, int y)
{
    if(state == 1) ///clicked
    {
      if(needToClip)
      {
            cout <<"Intersection points\n";
            clip();
      }

      glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      display();
    }
    return;
}

void initGL()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glPointSize(1.0);
}

void userInput()
{
    int i;
    char c;

    cout << "Give Clipping Window Coordinates Xmin & Ymin & Xmax & Ymax\n";
    cin >> Xmin >> Ymin >> Xmax >> Ymax;

    cout << "Give number of Lines\n";
    cin >> numLine;

    c = 'A';
    for(i = 0 ; i < numLine ; i++,c++)
    {
        cout << "Give coordinates of " << c;
        c++;
        cout << c << " ---- X1 & Y1 & X2 & Y2 " << endl;
        cin >> line[i][0] >> line[i][1] >> line[i][2] >> line[i][3] ;
    }

    memset(visible,1,sizeof visible);
    //memset(regionCode,1,sizeof regionCode);
}

void manualInput()
{
    Xmin = 20;
    Ymin = 20;
    Xmax = 250;
    Ymax = 250;

    numLine = 1;

    line[0][0] = -50;
    line[0][1] = -40;
    line[0][2] = 300;
    line[0][3] = 170;

    line[1][0] = -40;
    line[1][1] = -100;
    line[1][2] = 50;
    line[1][3] = 100;

    line[2][0] = -100;
    line[2][1] = -100;
    line[2][2] = -100;
    line[2][3] = 100;

    line[3][0] = -10;
    line[3][1] = -10;
    line[3][2] = 10;
    line[3][3] = 10;

    line[4][0] = -100;
    line[4][1] = -100;
    line[4][2] = 30;
    line[4][3] = 50;

    memset(visible,1,sizeof visible);
    //memset(regionCode,1,sizeof regionCode);
}

int main(int argc, char *argv[])
{
    int i;

    glutInit(&argc, argv);
    glutInitWindowSize(sizei,sizei);
    glutInitWindowPosition(10,10);
    //glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("View and Clip");

    //userInput();
    manualInput();

    initGL();
    glutDisplayFunc(display);
    glutMouseFunc(on); ///mouse click hole on() call hobe

    glutMainLoop();

    return EXIT_SUCCESS;
}

