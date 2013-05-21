/*
 enter		 oyuna baþla, devam et.
 UP, DOWN	 saðdaki oyuncu için hareket.
 W, S		 soldaki oyuncu için hareket.
 +			 artiya basildiðinde top hýzlanýr.
 -			 eksiye basýldýðýnda top yavaþlar.
*/

#include <GL\glut.h>
#include "glm.c"

GLint id;
GLMmodel* pmodel = NULL;
int font=(int)GLUT_BITMAP_TIMES_ROMAN_24;
static GLuint textures[3];
float top_x,top_y,top_vektor_x = 0.3,top_vektor_y = 0.12;
float engel1_y,engel2_y;
int skor1,skor2;
bool oyunBasladi = false;
int frame,time,timebase=0;
char s[50];
char skors[20];
GLfloat yaziRengi[] = {1,1,1,1.0};

void skorlariStringYap()
{
	sprintf(skors,"%d        -        %d",skor1,skor2);
}

void baslangicaAyarla()
{
	engel1_y = 50;
	engel2_y = 50;

	int i = rand() % 2;
	if(i == 0) 
		top_x = 97;
	else
		top_x = 3;
	top_y = 50;

	skorlariStringYap();
}

void ilerlet()
{
	top_x += top_vektor_x;
	top_y += top_vektor_y;
}

void oynat()
{
	if(top_x <= 2) //sola çarpmýþtýr.
	{
		if(engel1_y + 11 >= top_y && engel1_y -11 <= top_y)//doðru hamledir.
		{
			//topu geri sektir.
			top_vektor_x = -top_vektor_x;

			ilerlet();
		}
		else
		{
			//yanlýþ hamledir. Oyuncu2'ye sayý yaz.
			skor2++;
			if(skor2 >= 10) {oyunBasladi = false; skor1=0; skor2=0;}
			baslangicaAyarla();
			skorlariStringYap();
		}
	}
	else if(top_x >= 98)
	{
		if(engel2_y + 11 >= top_y && engel2_y -11 <= top_y)//doðru hamledir.
		{
			//topu geri sektir.
			top_vektor_x = -top_vektor_x;

			ilerlet();
		}
		else
		{
			//yanlýþ hamledir. Oyuncu1'e sayý yaz.
			skor1++;
			if(skor1 >= 10) {oyunBasladi = false; skor1=0; skor2=0;}
			baslangicaAyarla();
			skorlariStringYap();
		}
	}
	else if(top_y >= 98) //top yukarý çarpmýþtýr.
	{
		//aþaðýya sektir.
		top_vektor_y = -top_vektor_y;

		ilerlet();
	}
	else if(top_y <= 2) //top aþaðý çarpmýþtýr.
	{
		//yukarý sektir.
		top_vektor_y = -top_vektor_y;

		ilerlet();
	}
	else
	{
		//topun konumunu mevcut yönde deðiþtirmeye devam et.
		ilerlet();
	}
}

void ciz()
{
	GLfloat engelRengi[] = {0.4,0.4,0.4,1.0};

	glPushMatrix();
		glMaterialfv(GL_FRONT,GL_DIFFUSE,engelRengi);
		glTranslatef(-1,engel1_y,0.5);
		glScalef(2,20,1);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glMaterialfv(GL_FRONT,GL_DIFFUSE,engelRengi);
		glTranslatef(101,engel2_y,0.5);
		glScalef(2,20,1);
		glutSolidCube(1);
	glPopMatrix();

	if (!pmodel) {
		pmodel = glmReadOBJ("soccerball.obj");
		glmUnitize(pmodel);
		glmFacetNormals(pmodel);
		glmVertexNormals(pmodel, 90.0);
    }
    
	glPushMatrix();
		glTranslatef(top_x,top_y,0.5);
		glScalef(1,1,1);
		glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
}

void createDl()
{
	id = glGenLists(1);

	glNewList(id,GL_COMPILE);
		/*GLfloat masaRengi[] = {1,1,1,1};
		glMaterialfv(GL_FRONT,GL_DIFFUSE,masaRengi);*/

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glPushMatrix();
			glBegin(GL_POLYGON);
				glTexCoord2f(0.0, 1.0);
				glVertex3f(0,0,0);
				glTexCoord2f(1.0, 1.0);
				glVertex3f(100,0,0);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(100,100,0);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(0,100,0);
			glEnd();
			/*glTexCoord2f(0.0, 1.0);
			glTexCoord2f(1.0, 1.0);
			glTexCoord2f(1.0, 0.0);
			glTexCoord2f(0.0, 0.0);
			glTranslatef(50,engel2_y,-0.5);
			glScalef(100,100,0.5);
			glutSolidCube(1);*/
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	glEndList();
}

void renderBitmapString(float x, float y, void *font,char *string)
{  
  char *c;
  
  glRasterPos2f(x, y);
  
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void setOrthographicProjection() {
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
       gluOrtho2D(0, 900, 0, 600);
    glScalef(1, -1, 1);
    glTranslatef(0, -600, 0);
    glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void init()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//doku
	unsigned char* image = NULL;
    int iheight, iwidth;
    glGenTextures(1, textures);

    image = glmReadPPM("resim/futbol.ppm", &iwidth, &iheight);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iwidth, iheight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glBindTexture( GL_TEXTURE_2D, 0 );

	//nate-robins örneklerinden materyal ve ýþýk özellikleri:
	float no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	float mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
	float mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
	float mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
	float mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lightColor[] = {1,1,1,0};
	GLfloat lightPosition[] = {0,-10,20,0};

	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightColor);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel (GL_FLAT);

	glEnable(GL_DEPTH_TEST);

	//kamera
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();

	glViewport(0, 0, 900, 600);
	//glOrtho(-60,60,-60,60,-60,60);
	gluPerspective(100,1.0,1.0,100.0);
	gluLookAt(0,0,50,
			0,0,0,
			0,1,0);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	baslangicaAyarla();
}

void mydisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		glTranslatef(-50,-55,0);
		glCallList(id);
		ciz();
	glPopMatrix();

	if(oyunBasladi){
		oynat();
	}

	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"PONG By Abbas FPS:%4.2f",frame*1000.0/(time-timebase));
		glutSetWindowTitle(s);
		timebase = time;
		frame = 0;
	}
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,yaziRengi);
    setOrthographicProjection();
		glPushMatrix();
		glLoadIdentity();
		renderBitmapString(380,35,(void *)font,skors);
    glPopMatrix();
    resetPerspectiveProjection();
	
	glutSwapBuffers();
}

void processSpecialKeys(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_DOWN: if(engel2_y - 10 > 0) engel2_y -= 1;
			break; 
		case GLUT_KEY_UP: if( engel2_y + 10 < 100) engel2_y += 1;
			break;
   }
}

void klavye(unsigned char key, int x, int y)
{
	switch (key) {
		case 's':
		case 'S': if(engel1_y - 10 > 0) engel1_y -= 1;
			break;
		case 'w':
		case 'W': if( engel1_y + 10 < 100) engel1_y += 1;
			break;
		case 13: oyunBasladi = true;
			break;
		case '+': top_vektor_x *= 1.1; top_vektor_y *= 1.1;
			break;
		case '-': top_vektor_x /= 1.1; top_vektor_y /= 1.1;
			break;
   }
}

int main(int argc,char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_ACCUM);
	glutInitWindowSize (900, 600);
	glutCreateWindow (argv[0]);

	init();
	createDl();
	glutDisplayFunc(mydisplay);
	glutIdleFunc(mydisplay);
	glutSpecialFunc(processSpecialKeys);
	glutKeyboardFunc(klavye);

	glutMainLoop();
	return 0;
}