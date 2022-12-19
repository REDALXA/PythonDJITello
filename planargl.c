/////////////////////////////////////////////////////////////
/* Template OpengGL sengaja dibuat untuk kuliah robotik 
*  di Departemen Teknik Elektro
*  Bagi yang ingin memodifikasi untuk keperluan yang lain,
*  dipersilahkan dengan menuliskan acknowledgement pada
*    Dr. Abdul Muis, MEng.
*    Autonomous Control Electronics (ACONICS) Research Group
*    http://www.ee.ui.ac.id/aconics
*////////////////////////////////////////////////////////////

#include <stdio.h> 
#include <stdlib.h> 
#include <GL/glut.h> // Header File For The GLUT Library
#include <GL/gl.h> // Header File For The OpenGL32 Library
#include <GL/glu.h> // Header File For The GLu32 Library
#include <unistd.h> // Header file for sleeping.
#include <math.h> 
#include <fcntl.h>			/* File control definitions */
#include <errno.h>			/* Error number definitions */
#include <termios.h>		/* POSIX terminal control definitions */
#include <sys/time.h>
#include "planar.c"

/* ascii code for the escape key */
#define ESCkey	27

/* The number/handle of our GLUT window */
int window, wcam;  
FILE *fpt ;
 


/* To draw a quadric model */
GLUquadricObj *obj;

// ROBOT MODEL PARAMATER
#define Xoffset	0.0	
#define Yoffset	0.0
#define Zoffset	0.3
#define Ki 0.8
#define Kd 0.04
#define limit 6.2831

#define Link1 L1
#define Link2 L2

float tetha1= 0 * DTR;
float tetha2= 30 * DTR;
int linemode = 0 ;
float counter = 0.0 ;
float x=0.3;
float y=0.5;
float trajt = 8.500 ;
float dt = 0.100 ;
float t = 0.000 ;
float ex = 0.00 ;
float ey = 0.00 ;
float ex_old = 0.00 ;
float ey_old = 0.00 ;

float dx = 0.00 ;
float dy = 0.00;
float dq1 = 0.00 ;
float dq2 = 0.00 ;
float ddx = 0.00 ;
float ddy = 0.00 ;
float ddq1 = 0.00 ;
float ddq2 = 0.00 ;
float x_d = 0.00 ;
float y_d = 0.0 ;
float x_cmd = 0.00 ;
float y_cmd = 0.00 ;
float x_init = 0.30 ;
float y_init = 0.50 ;
float determinant = 0.00 ;
char debug=0;



void Sim_main(void); // Deklarasi lebih awal agar bisa diakses oleh fungsi sebelumnya
void display(void); // fungsi untuk menampilkan gambar robot / tampilan camera awal
void animate(void) ;
void control_robot(void);
void forward_kinematic(void);
void pd_control(void);
void inverse_jacobian(void);

/* define color */  
GLfloat green1[4]  ={0.8, 1.0, 0.8, 1.0};
GLfloat blue1[4]  ={0.1, 0.1, 1.0, 1.0};
GLfloat blue2[4]  ={0.2, 0.2, 1.0, 1.0};
GLfloat blue3[4]  ={0.3, 0.3, 1.0, 1.0};
GLfloat yellow1[4]={0.1, 0.1, 0.0, 1.0};
GLfloat yellow2[4]={0.2, 0.2, 0.0, 1.0};
GLfloat pink6[4] ={0.8, 0.55, 0.6, 1.0};
GLfloat yellow5[4]={0.8, 0.8, 0.0, 1.0};
GLfloat abu2[4]={0.5,0.5,0.5,1.0};
GLfloat gray1[4]  ={0.1, 0.1, 0.1, 1.0};
GLfloat gray2[4]  ={0.2, 0.2, 0.2, 1.0};
GLfloat gray3[4]  ={0.3, 0.3, 0.3, 1.0};
GLfloat gray4[4]  ={0.4, 0.4, 0.4, 1.0};
GLfloat gray5[4]  ={0.5, 0.5, 0.5, 1.0};
GLfloat gray6[4]  ={0.6, 0.6, 0.6, 1.0};
GLfloat gray7[4]  ={0.7, 0.7, 0.7, 1.0};
GLfloat gray8[4]  ={0.8, 0.8, 0.7, 1.0};
GLfloat gray9[4]  ={0.9, 0.9, 0.7, 1.0};


void  drawOneLine(double x1, double y1, double x2, double y2) 
   {glBegin(GL_LINES); glVertex3f((x1),(y1),0.0); glVertex3f((x2),(y2),0.0); glEnd();}
   
void  model_cylinder(GLUquadricObj * object, GLdouble lowerRadius,
  GLdouble upperRadius, GLdouble length, GLint res, GLfloat *color1, GLfloat *color2)
{
  glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);
    glTranslatef(0,0,-length/2);
	  gluCylinder(object, lowerRadius, upperRadius, length, 20, res);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
    gluDisk(object, 0.01, lowerRadius, 20, res); 
    glTranslatef(0, 0, length);
    gluDisk(object, 0.01, upperRadius, 20, res); 
  glPopMatrix();
}

void  model_box(GLfloat width, GLfloat depth, GLfloat height, GLfloat *color1, GLfloat *color2, GLfloat *color3, int color)
{
   width=width/2.0;depth=depth/2.0;height=height/2.0;
   glBegin(GL_QUADS);
// top
    if (color==1) 
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);
    glVertex3f(-width,-depth, height);
    glVertex3f( width,-depth, height);
    glVertex3f( width, depth, height);
    glVertex3f(-width, depth, height);
   glEnd();
   glBegin(GL_QUADS);
// bottom
    if (color==1) 
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color1);
    glVertex3f(-width,-depth,-height);
    glVertex3f( width,-depth,-height);
    glVertex3f( width, depth,-height);
    glVertex3f(-width, depth,-height);
   glEnd();
   glBegin(GL_QUAD_STRIP);
// sides
    if (color==1) 
	    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color2);
    glVertex3f(-width,-depth,height);
    glVertex3f(-width,-depth,-height);
    glVertex3f(width,-depth,height);
    glVertex3f(width,-depth,-height);
    glVertex3f(width,depth,height);
    glVertex3f(width,depth,-height);
    glVertex3f(-width,depth,height);
    glVertex3f(-width,depth,-height);
    glVertex3f(-width,-depth,height);
   glEnd();
}



void disp_floor(void)
{
  int i,j,flagc=1;

  glPushMatrix();
  
  GLfloat dx=4.5,dy=4.5;
  GLint amount=15;
  GLfloat x_min=-dx/2.0, x_max=dx/2.0, x_sp=(GLfloat) dx/amount, y_min=-dy/2.0, y_max=dy/2.0, y_sp=(GLfloat) dy/amount;

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green1);
  for(i = 0; i<=48; i++){
     drawOneLine(-2.4+0.1*i, -2.4,       -2.4+0.1*i,  2.4);
     drawOneLine(-2.4,       -2.4+0.1*i,  2.4,       -2.4+0.1*i);
  }

  glPopMatrix();
}

void  lighting(void)
{

	GLfloat light_ambient[] =  {0.2, 0.2, 0.2, 1.0};
	GLfloat light_diffuse[] =  {0.4, 0.4, 0.4, 1.0};
	GLfloat light_specular[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat light_position[] = {2, 0.1, 7,1.0};
	GLfloat spot_direction[] = {0.0, -0.1, -1.0, 1.0};

	glClearColor(0.0, 0.0, 0.0, 0.0);     
  
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 40.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 4);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void disp_robot(void)
{
	
  glPushMatrix();
    model_box(0.3, 0.5, 0.05, gray8, gray7, gray6,1);
    glTranslatef(Xoffset, Yoffset, Zoffset/2);
    // Draw base
    model_cylinder(obj, 0.1, 0.1, Zoffset, 2, blue1, yellow2);
    // Menuju joint-1
    glTranslatef(0, 0, Zoffset/2);
    glRotatef(tetha1*RTD,0,0,1);
    glPushMatrix();
      // Gambar link1-1
      glRotatef(-90,1,0,0);
      glTranslatef(0,0,Link1/2);
      model_cylinder(obj, 0.03, 0.03, Link1, 2, pink6, yellow2);
    glPopMatrix();
    // Menuju joint-2
    glTranslatef(0,Link1, 0);
    glRotatef(tetha2*RTD,0,0,1);
    glPushMatrix();
      // Gambar link1-1
      glRotatef(-90,1,0,0);
      glTranslatef(0,0,Link2/2);
      model_cylinder(obj, 0.03, 0.03, Link2, 2, yellow5, yellow2);
    glPopMatrix();
  glPopMatrix();
  glPushMatrix();
    glTranslatef(Xoffset-0.16-0.15, Yoffset+0.23-0.09, Zoffset);
    double x=0;
    double y=0;
    double radius=0.1;
    double y1=0;
		double x1=0;
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green1);
		glBegin(GL_LINES);
		for(double angle=0.0f;angle<=(2.0f*3.14159);angle+=0.01f)
		{
			double x2=x-(radius*(float)sin((double)angle));
			double y2=y-(radius*(float)cos((double)angle));
			glVertex3f(x1,y1,0);
			y1=y2;
			x1=x2;
		}
    glEnd();
  glPopMatrix();


}

// Draw Object
void display(void)
{
//   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT) ; // Clear The Screen and The Depth Buffer 
   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT) ; // Clear The Screen and The Depth Buffer 
   //glLoadIdentity();  // Reset View
   disp_floor();
   
   disp_robot();

   /* since window is double buffered, 
      Swap the front and back buffers (used in double buffering). */
   glutSwapBuffers() ; 
   

}

void Sim_main(void)
{
	unsigned long Xr=0,Yr=0, Xg=0,Yg=0, Xb=0,Yb=0; // titik untuk menghitung sum
	int Nr=0, Ng=0, Nb=0;
	static unsigned int Rx,Ry, Gx,Gy, Bx,By; // untuk menyimpan hasil titik berat
	unsigned int i,j,k;
  glutSetWindow(window);
  // usleep(100);
  usleep(2000);
  control_robot();
  display();
  Retrieve_serial();
  	
	
}
void keyboard(unsigned char key, int i, int j)
{
	 switch(key){
      case ESCkey: exit(1); break;
      case 'w' : control_robot(); display();break;
      
   }
}
void trajectory_line(void) {
  if(counter < 0.5) {
    x_init = x ; y_init = y ;
    counter += 1 ;
    linemode=(linemode==1) ? 0:1;
    if(linemode==0) {
      x_cmd = x - 0.3 ; y_cmd = y + 0.2 ;
    }
    else {
      x_cmd = x + 0.3 ; y_cmd = y - 0.2 ;
    }
  }
  x_d=(x_cmd - x_init) *t/trajt + x_init ;
  y_d=(y_cmd - y_init) *t/trajt + y_init ;
}

void forward_kinematic(void) {
	x = Link1 * cos(tetha1) + Link2 * cos(tetha1 + tetha2);
	y = Link1 * sin(tetha1) + Link2 * sin(tetha1 + tetha2);
}

void pd_control(void) {
	ddx = Ki * ex + Kd * ((ex - ex_old) / dt) ;
	ddy = Ki * ey + Kd * ((ey - ey_old) / dt) ;
	}
	
void inverse_jacobian(void) {
  determinant = Link1 * Link2 * sin(tetha2) ;
  if(determinant < 0.001) {
    determinant = 0.003 ;
  }
  ddq1 = Link2 * cos(tetha1 + tetha2) * ddy - ddx * Link2 * sin(tetha1 + tetha2) ;
  ddq2 = ddx * (Link1 * sin(tetha1) + Link2 * sin(tetha1 + tetha2))- ddy * (Link1 * cos(tetha1) + Link2 * cos(tetha1 + tetha2));
  ddq1 = ddq1 / determinant ;
  ddq2 = ddq2 / determinant ;

	}

void control_robot(void) {
  printf("t:%.2f , q1 :%.2f , q2 :%.2f , x :%.2f , y :%.2f , cmdx :%.2f , cmdy :%.2f\n",t,tetha1,tetha2,x,y,x_cmd,y_cmd);
  fprintf(fpt,"%.2f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f\n",t,tetha1,tetha2,x,y,x_cmd,y_cmd);
  forward_kinematic();
  trajectory_line();
  ex_old = ex ;
  ey_old = ey ;
  ex = x_d - x ; ey = y_d - y ;
  pd_control();
  inverse_jacobian();
  float v1 = 1.2*ddq1 ; float v2 = 1.2*ddq2 ;
  dq1 = dq1 +(2.083*v1 - 2.71 * dq1)*dt ;
  dq2 = dq2 +(2.083*v2 - 2.71 * dq2)*dt ;
  // if(dq1 > 0.2) { dq1 = 0.2 ;}
  //if(dq1 < -0.2) {dq1 = -0.2 ;}
  //if(dq2 > 0.2) {dq2 = 0.2 ;}
  //if(dq2 < -0.2) {q2 = -0.2 ;}
  tetha1 = tetha1 + dq1 * dt ;
  tetha2 = tetha2 + dq2 * dt ;
  //while(tetha1 > limit) {tetha1 -= limit;} 
  //while(tetha1 < -limit) {tetha1 += limit;}
  //while(tetha2 > limit) {tetha2 -= limit;}
  //while(tetha2 < -limit) {tetha2 += limit;}
  t = t + dt ;
  counter = counter + dt ;
  if(counter >= trajt) {counter = 0.0 ;}
}



void init(void) 
{ 
   obj = gluNewQuadric(); 
   /* Clear background to (Red, Green, Blue, Alpha) */
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
   glEnable(GL_DEPTH_TEST); // Enables Depth Testing
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0, 2, 0.2, 8);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0.3, 0.0, 1.5,  -0.1, 0.0, 0.4,  0.0, 0.0, 1.0); 
	 lighting();
	 
   /* When the shading model is GL_FLAT only one colour per polygon is used, 
      whereas when the shading model is set to GL_SMOOTH the colour of 
      a polygon is interpolated among the colours of its vertices.  */
   glShadeModel(GL_SMOOTH) ; 

   glutDisplayFunc (&display) ;
   glutKeyboardFunc(&keyboard);

}

// Main Program
int main(int argc, char** argv)
{
 // Initialize GLUT
   /* Initialize GLUT state - glut will take any command line arguments 
      see summary on OpenGL Summary */  
   glutInit (&argc, argv);
   
   // Berikut jika ingin menggunakan serial port
   fd = open_port();
   init_port(fd);

   /* Select type of Display mode:   
      Double buffer 
      RGBA color
      Alpha components supported 
      Depth buffer */  
   //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );
   /* set a 400 (width) x 400 (height) window and its position */
   glutInitWindowSize(800,400);	
   glutInitWindowPosition (40, 100);

   /* Open a window */  
   window = glutCreateWindow ("Simple Window");
   fpt = fopen("Robotika1.csv" ,"w+");
   fprintf(fpt,"t , q1 , q2 , x , y , cmdx , cmdy \n");

   /* Initialize our window. */
   init() ;
   init_robot();

   /* Register the function to do all our OpenGL drawing. */
   glutIdleFunc(&Sim_main); // fungsi untuk simulasi utama

   /* Start Event Processing Engine */ 
   glutMainLoop () ;
   return 0 ;
}           
