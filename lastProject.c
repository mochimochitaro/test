#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include "myShape.h"
#include "read_bitmap.h"
#define KEY_ESC 27
#define imageWidth 8
#define imageHeight 8

int ReadBitMapData();
void resetview(void);
void polarview(void);
void read_file1(void);
unsigned char* image;

float distance = 6.5, twist = 0.0, elevation = -45.0, azimuth = 30.0;
float nearw = 1.0, farw = 500.0, fovy = 60.0;

unsigned char image1[imageHeight][imageWidth][4];
unsigned char image2[imageHeight][imageWidth][4];
int xBegin = 0, yBegin = 0;

float diffuse1[] = { 0.0, 0.5, 1.0, 1.0 };
float specular1[] = { 0.8, 0.8, 0.8, 1.0 };
float ambient1[] = { 0.1, 0.1, 0.1, 1.0 };
float shininess1 = 128.0;
int countx, county = 0;
int count = 0;
int count2 = 0;
float point = 0.0; /*物体の回転角度*/
int hana = 0;

unsigned char wireFlag = GL_TRUE;
/*物体のソリッド←→ワイヤーフレーム表示を切り替えるためのフラッグ*/
float xOrig = 0.0, yOrig = 0.0;
/*物体のローカル座標原点、矢印キーで変化*/
float rColor = 1.0, gColor = 1.0, bColor = 1.0;
/*物体色、cキーでランダムに変化*/

unsigned char* r1, * g1, * b1;
int width, height;
FILE* fp1;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//    gluLookAt(0, -1, 3, 0, 0, 0, 0, 1, 0);
	polarview();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 3);
	glBegin(GL_QUADS);

	//背景画像（6面）
	glBegin(GL_QUADS);
	glTexCoord2f(0.35, 0.0); glVertex3f(-5, -5, -5);
	glTexCoord2f(0.35, 0.5); glVertex3f(-5, 5, -5);
	glTexCoord2f(0.0, 0.5); glVertex3f(5, 5, -5);
	glTexCoord2f(0.0, 0.0); glVertex3f(5, -5, -5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.95, 0.55); glVertex3f(-5, -5, 5);
	glTexCoord2f(0.95, 0.95); glVertex3f(-5, 5, 5);
	glTexCoord2f(0.72, 0.95); glVertex3f(5, 5, 5);
	glTexCoord2f(0.72, 0.55); glVertex3f(5, -5, 5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.4, 0.0); glVertex3f(-5, -5, -5);
	glTexCoord2f(0.7, 0.0); glVertex3f(-5, -5, 5);
	glTexCoord2f(0.7, 0.5); glVertex3f(5, -5, 5);
	glTexCoord2f(0.4, 0.5); glVertex3f(5, -5, -5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.4, 1.0); glVertex3f(-5, 5, -5);
	glTexCoord2f(0.4, 0.5); glVertex3f(-5, 5, 5);
	glTexCoord2f(0.7, 0.5); glVertex3f(5, 5, 5);
	glTexCoord2f(0.7, 1.0); glVertex3f(5, 5, -5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(1.0, 0.0); glVertex3f(5, -5, -5);
	glTexCoord2f(1.0, 0.5); glVertex3f(5, 5, -5);
	glTexCoord2f(0.7, 0.5); glVertex3f(5, 5, 5);
	glTexCoord2f(0.7, 0.0); glVertex3f(5, -5, 5);
	glEnd();

	glBegin(GL_QUADS);
	glTexCoord2f(0.35, 0.5); glVertex3f(-5, -5, -5);
	glTexCoord2f(0.35, 1.0); glVertex3f(-5, 5, -5);
	glTexCoord2f(0.0, 1.0); glVertex3f(-5, 5, 5);
	glTexCoord2f(0.0, 0.5); glVertex3f(-5, -5, 5);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse1);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular1);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient1);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess1);
	glEnable(GL_LIGHTING);

	glTranslatef(xOrig, yOrig, 0);

	//上半身
	glPushMatrix();
	glTranslated(0, 0, 0);
	glutSolidSphere(1.0, 100, 100);
	glPopMatrix();

	//頭
	glPushMatrix();
	glTranslated(0, 1, 0);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.5);
	glRotatef(-180.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.8, 2.0, 20, 3);/*先端の描画*/
	glPopMatrix();/*座標系の復元*/

	glBegin(GL_TRIANGLES); // 右面の四角形の定義とUV座標の設定
	glVertex3f(0.5, 0.5, -3.0);
	glVertex3f(-0.5, 0.5, -3.0);
	glVertex3f(0.0, 0.0, -2.2);
	glEnd();

	glPushMatrix();
	glTranslated(0.9, 0.5, 0);
	if (county % 2 == 1) {
		glRotatef(-30.0, 0.0, 0.0, 1.0);
	}
	else {
		glRotatef(-70, 0.0, 0.0, 1.0);
	}
	mySolidCylinder(0.5, 0.1, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.9, 0.5, 0);
	if (county % 2 == 1) {
		glRotatef(30.0, 0.0, 0.0, 1.0);
	}
	else {
		glRotatef(70.0, 0.0, 0.0, 1.0);
	}
	mySolidCylinder(0.5, 0.1, 30);
	glPopMatrix();

	float diffuse2[] = { 0.05, 0.05, 0.06, 1.0 };
	float specular2[] = { 0.18, 0.17, 0.22, 1.0 };
	float ambient2[] = { 0.33,0.32,0.34,1.0 };
	float shininess2 = 38.4;

	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular2);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient2);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess2);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glTranslated(-0.3, 1.1, 0.6);
	if (count == 1) {
		glRotatef(45.0, 0.0, 0.0, 1.0);
		glRotatef(-20.0, 1.0, 0.0, 0.0);
		mySolidCylinder(0.05, 0.2, 20); 
		glPopMatrix();

		glPushMatrix();
		glTranslated(-0.4, 1.1, 0.6);
		glRotatef(-45.0, 0.0, 0.0, 1.0);
		mySolidCylinder(0.05, 0.2, 20); /*2段目の描画*/
	}
	else if (yOrig <= 0.0) {
		glRotatef(-90.0, 0.0, 0.0, 1.0);
		glRotatef(20.0, 1.0, 0.0, 0.0);
		mySolidCylinder(0.05, 0.3, 20); /*2段目の描画*/
	}
	else {
		glutSolidSphere(0.1, 100, 100);
	}
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.3, 1.1, 0.6);
	if (count == 1) {
		glRotatef(-45.0, 0.0, 0.0, 1.0);
		glRotatef(-20.0, 1.0, 0.0, 0.0);
		mySolidCylinder(0.05, 0.2, 20); /*2段目の描画*/
		glPopMatrix();

		glPushMatrix();
		glTranslated(0.4, 1.1, 0.6);
		glRotatef(45.0, 0.0, 0.0, 1.0);
		mySolidCylinder(0.05, 0.2, 20); /*2段目の描画*/
	}
	else if (yOrig <= 0.0) {
		glRotatef(-90.0, 0.0, 0.0, 1.0);
		glRotatef(-20.0, 1.0, 0.0, 0.0);
		mySolidCylinder(0.05, 0.3, 20); /*2段目の描画*/
	}
	else {
		glutSolidSphere(0.1, 100, 100);
	}
	glPopMatrix();

	float diffuse3[] = { 0.7, 0.6, 0.2, 1.0 };
	float specular3[] = { 0.6, 0.5, 0.4, 1.0 };
	float ambient3[] = { 0.3, 0.2, 0.1, 1.0 };
	float shininess3 = 50.0;

	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular3);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient3);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess3);
	glEnable(GL_LIGHTING);

	//左脚（軸）
	glPushMatrix();
	glTranslatef(0.5, 0.0, 0.9);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	mySolidCylinder(0.05, 0.3, 20);
	glPopMatrix();

	//右脚（軸）
	glPushMatrix();
	glTranslatef(-0.5, 0.0, 0.9);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	mySolidCylinder(0.05, 0.3, 20);
	glPopMatrix();

	//右脚（指①）
	glPushMatrix();
	glTranslatef(-0.5, 0.2, 1.1);
	mySolidCylinder(0.05, 0.3, 20);
	glPopMatrix();

	//右脚（指②）
	glPushMatrix();
	glTranslatef(-0.4, 0.2, 1.1);
	glRotatef(-30.0, 0.0, 0.0, 1.0);
	mySolidCylinder(0.05, 0.3, 20);
	glPopMatrix();

	//右脚（指③）
	glPushMatrix();
	glTranslatef(-0.6, 0.2, 1.1);
	glRotatef(30.0, 0.0, 0.0, 1.0);
	mySolidCylinder(0.05, 0.3, 20);
	glPopMatrix();

	//左脚（指①）
	glPushMatrix();
	glTranslatef(0.5, 0.2, 1.1);
	mySolidCylinder(0.05, 0.3, 20);
	glPopMatrix();

	//左脚（指②）
	glPushMatrix();
	glTranslatef(0.6, 0.2, 1.1);
	glRotatef(-30.0, 0.0, 0.0, 1.0);
	mySolidCylinder(0.05, 0.3, 20);
	glPopMatrix();

	//左脚（指③）
	glPushMatrix();
	glTranslatef(0.4, 0.2, 1.1);
	glRotatef(30.0, 0.0, 0.0, 1.0);
	mySolidCylinder(0.05, 0.3, 20);
	glPopMatrix();

	//くちばし
	glPushMatrix();
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(1.0, 0.0, 0.0);
	glutSolidCone(0.5, 1.0, 20, 3);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glPopMatrix();

	glPushMatrix();

	float diffuse4[] = { 0.191,  0.073,   0.022,  1.0 };
	float specular4[] = { 0.703,   0.270,  0.082,  1.0 };
	float ambient4[] = { 0.256, 0.1376, 0.086,1.0 };
	float shininess4 = 12.8;
	//    gluLookAt(0, -1, 3, 0, 0, 0, 0, 1, 0);
	polarview();
	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse4);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular4);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient4);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess4);
	glEnable(GL_LIGHTING);

	//木①（小鳥が乗っているところ）
	glPushMatrix();
	glTranslated(0, -1, 0);
	glRotatef(-90.0, 0.0, 0.0, 1.0);
	mySolidCylinder(0.5, 10, 30);
	glPopMatrix();

	//木②（飾り）
	glPushMatrix();
	glTranslated(1.5, -1, 1.5);
	glRotatef(-90.0, 0.0, 0.0, 1.0);
	glRotatef(-30.0, 1.0, 0.0, 0.0);
	glRotatef(-30.0, 0.0, 1.0, 0.0);
	mySolidCylinder(0.2, 5, 30);
	glPopMatrix();

	float diffuse5[] = { 0.8, 0.0, 0.05, 1.0 };
	float specular5[] = { 0.8, 0.4, 0.4, 1.0 };
	float ambient5[] = { 0.8, 0.0, 0.0, 1.0 };
	float shininess5 = 30.0;

	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse5);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular5);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient5);
	glMaterialf(GL_FRONT, GL_SHININESS, shininess5);
	glEnable(GL_LIGHTING);

	//太陽（中心）
	glPushMatrix();
	glTranslated(-3, 2, -2);
	glutSolidSphere(1.0, 100, 100);
	glPopMatrix();

	//太陽（周り）
	glPushMatrix();
	glTranslatef(-2.0, 2.0, -2.5);
	glRotatef(-180.0, 0.0, 0.0, 1.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.3, 0.5, 20, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.5, 1.0, -2.5);
	glRotatef(-200.0, 0.0, 0.0, 1.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.3, 0.5, 20, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.5, 0.7, -2.0);
	glRotatef(230.0, 1.0, 0.0, 0.0);
	glRotatef(-180.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.3, 0.5, 20, 3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-4.0, 1.5, -1.5);
	glRotatef(200.0, 1.0, 0.0, 0.0);
	glRotatef(-160.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.3, 0.5, 20, 3);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glPopMatrix();

	glPushMatrix();
	polarview();
	glEnable(GL_DEPTH_TEST);

	glColor3f(1.0, 1.0, 1.0);

	//雲①
	glPushMatrix();
	glTranslated(-2, 2, -4);
	glutSolidSphere(1.0, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2, 1, -4);
	glutSolidSphere(1.0, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-3, 1.5, -4);
	glutSolidSphere(1.0, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1, 1.5, -4);
	glutSolidSphere(1.0, 100, 100);
	glPopMatrix();

	//雲②
	glPushMatrix();
	glTranslated(0.5, -3, 2);
	glutSolidSphere(0.5, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, -3, 2);
	glutSolidSphere(0.7, 100, 100);
	glPopMatrix();

	//雲③
	glPushMatrix();
	glTranslated(3, 0, -4);
	glutSolidSphere(1.2, 100, 100);
	glPopMatrix();


	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glPopMatrix();

	if (count == 1) {
		glPushMatrix();

		float diffuse6[] = { 0.8, 0.4, 0.05, 1.0 };
		float specular6[] = { 0.8, 0.4, 0.4, 1.0 };
		float ambient6[] = { 0.8, 0.4, 0.0, 1.0 };
		float shininess6 = 30.0;

		polarview();
		glEnable(GL_DEPTH_TEST);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse6);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular6);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient6);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess6);
		glEnable(GL_LIGHTING);

		if (count2 % 2 == 0) {
			//音符①
			glPushMatrix();
			glTranslatef(-0.5, 1.5, 2.5);
			mySolidCylinder(0.05, 0.8, 20);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-0.6, 1.0, 2.5);
			glutSolidSphere(0.15, 100, 100);
			glPopMatrix();

			count2++;
		}
		else {
			glPushMatrix();
			glTranslatef(-0.5, 2.0, 2.5);
			mySolidCylinder(0.05, 0.8, 20);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(-0.6, 1.5, 2.5);
			glutSolidSphere(0.15, 100, 100);
			glPopMatrix();

			count2++;
		}
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glPopMatrix();

		glPushMatrix();

		float diffuse7[] = { 0.0, 0.9, 0.8, 1.0 };
		float specular7[] = { 0.0, 0.9, 0.8, 1.0 };
		float ambient7[] = { 0.0, 0.9, 0.8, 1.0 };
		float shininess7 = 30.0;

		polarview();
		glEnable(GL_DEPTH_TEST);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse7);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular7);
		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient7);
		glMaterialf(GL_FRONT, GL_SHININESS, shininess7);
		glEnable(GL_LIGHTING);

		if (count2 % 2 == 0) {
			//音符②
			glPushMatrix();
			glTranslatef(2.0, 1.0, -1.0);
			mySolidCylinder(0.05, 0.8, 20);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.9, 0.5, -1.0);
			glutSolidSphere(0.15, 100, 100);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.6, 1.0, -1.0);
			mySolidCylinder(0.05, 0.8, 20);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.5, 0.5, -1.0);
			glutSolidSphere(0.15, 100, 100);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.8, 1.4, -1.0);
			glRotatef(-90.0, 0.0, 0.0, 1.0);
			mySolidCylinder(0.05, 0.5, 20);
			glPopMatrix();
		}
		else
		{
			glPushMatrix();
			glTranslatef(2.0, 1.5, -1.0);
			mySolidCylinder(0.05, 0.8, 20);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.9, 1.0, -1.0);
			glutSolidSphere(0.15, 100, 100);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.6, 1.5, -1.0);
			mySolidCylinder(0.05, 0.8, 20);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.5, 1.0, -1.0);
			glutSolidSphere(0.15, 100, 100);
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1.8, 1.9, -1.0);
			glRotatef(-90.0, 0.0, 0.0, 1.0);
			mySolidCylinder(0.05, 0.5, 20);
			glPopMatrix();

			glDisable(GL_LIGHTING);
			glDisable(GL_DEPTH_TEST);
			glPopMatrix();
		}

		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glPopMatrix();

	}
	if (hana == 1) {

		glPushMatrix();
		polarview();
		glEnable(GL_DEPTH_TEST);

		glColor3f(1.0, 0.5, 1.0);

		//花びら①
		glPushMatrix();
		glTranslated(1.5, point + 10.0, 2.0);
		glRotatef(-30.0, 0.0, 0.0, 1.0);
		mySolidCylinder(0.3, 0.01, 30);
		glPopMatrix();

		//花びら②
		glPushMatrix();
		glTranslated(-2.0, point + 7.0, -1.0);
		glRotatef(-30.0, 0.0, 0.0, 1.0);
		glRotatef(-30.0, 0.0, 1.0, 0.0);
		mySolidCylinder(0.3, 0.01, 30);
		glPopMatrix();

		//花びら③
		glPushMatrix();
		glTranslated(-1.0, point + 3.0, 1.0);
		glRotatef(-30.0, 0.0, 0.0, 1.0);
		glRotatef(-30.0, 0.0, 1.0, 0.0);
		mySolidCylinder(0.3, 0.01, 30);
		glPopMatrix();

		//花びら④
		glPushMatrix();
		glTranslated(2.0, point + 12.0, -2.0);
		glRotatef(-30.0, 0.0, 0.0, 1.0);
		glRotatef(-30.0, 0.0, 1.0, 0.0);
		mySolidCylinder(0.3, 0.01, 30);
		glPopMatrix();

		glDisable(GL_DEPTH_TEST);
		glPopMatrix();

	}
	glutSwapBuffers();
	glFlush();
}

void idle(void)/*イベントがなければidleがつねに実行される*/
{
	if (hana == 1)
	{
		point = fmod(point - 0.05, 15.0);/*回転角を０°～３６０°まで0.5°づつ増加させる）*/
		glutPostRedisplay();/*ディスプレイコールバックバック関数(display)を実行*/
	}
}

void makeImage(void)
{
	int i, j;
	//	printf("width, height = %d %d\n",width,height);
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			image[0 + 4 * (j + i * width)] = (unsigned char)r1[j + width * i];			// bitmap2に変えたり
			image[1 + 4 * (j + i * width)] = (unsigned char)g1[j + width * i];			// bitmap1の内容を変えてみる
			image[2 + 4 * (j + i * width)] = (unsigned char)b1[j + width * i];
			image[3 + 4 * (j + i * width)] = (unsigned char)50;
			//		printf(" %d %d\n", image[i+j*width+0*width*height] ,image[i+j*width+1*width*height]); 
		}
	}
}

void myKbd(unsigned char key, int x, int y)
{
	switch (key) {
	case KEY_ESC:
		exit(0);
	case 's':
		hana = 0;
		count = 1;
		break;
	case 'h':
		hana = 1;
		count = 0;
		break;
	}
	glutPostRedisplay();
}

void mySkey(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:/*左矢印キー*/
	/*矢印キーを押すと物体のローカル座標原点が変化する*/
		xOrig -= 0.1;
		hana = 0;
		count = 0;
		countx--;
		if (xOrig <= -2.0) xOrig = -2.0;
		break;
	case GLUT_KEY_RIGHT:/*右矢印キー*/
		hana = 0;
		count = 0;
		xOrig += 0.1;
		countx++;
		if (xOrig >= 2.0) xOrig = 2.0;
		break;
	case GLUT_KEY_UP:/*上矢印キー*/
		hana = 0;
		count = 0;
		yOrig += 0.1;
		county++;
		if (yOrig >= 2.5) yOrig = 2.5;
		break;
	case GLUT_KEY_DOWN:/*下矢印キー*/
		hana = 0;
		count = 0;
		yOrig -= 0.1;
		county--;
		if (yOrig <= 0.0) {
			yOrig = 0.0;
			county = 0;
		}
	}


	glutPostRedisplay();
}

void initTexture3(void)
{
	unsigned char* image3[1];
	int imageHeightb, imageWidthb;

	makeImage();

	glBindTexture(GL_TEXTURE_2D, 3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void myInit(char* progname)
{
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(800, 800);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow(progname);
	glClearColor(0.5, 1.0, 1.0, 1.0);
	glutKeyboardFunc(myKbd);
	glutSpecialFunc(mySkey);
	initTexture3();
	glEnable(GL_LIGHT0);
}


//void myReshape(int width, int height)
//{
//	//gluLookAt(0.0, 10.0, 10.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0);
//	float aspect = (float)width / (float)height;
//	glViewport(0, 0, width, height);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(50.0, aspect, 1.0, 10.0);
//	glMatrixMode(GL_MODELVIEW);
//}

void myReshape(int width, int height)
{
	float aspect = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, nearw, farw);
	glMatrixMode(GL_MODELVIEW);
}


void polarview(void)
{
	glTranslatef(0.0, 0.0, -distance);
	glRotatef(-twist, 0.0, 0.0, 1.0);
	glRotatef(-elevation, 1.0, 0.0, 0.0);
	glRotatef(-azimuth, 0.0, 1.0, 0.0);
}


int main(int argc, char** argv)
{
	fp1 = fopen("sky.bmp", "rb");
	printf(" file = %s\n", argv[1]);
	if (fp1 == 0) { // ファイルを開くのに失敗
		fprintf(stderr, " File open error : %s\n", argv[1]);
		exit(-1);
	}
	else { // こちらは成功
		fprintf(stderr, "File open success : %s\n", argv[1]);
	}
	read_file1();

	glutInit(&argc, argv);
	myInit(argv[0]);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
	return(0);
}

void read_file1() {
	int i, j, k;
	unsigned int width1, height1;
	unsigned short color;
	unsigned int dumlong;
	unsigned char buf10[10];
	unsigned char buf2[2];
	unsigned char buf20[20];
	unsigned int image_start_point, header_size;
	unsigned char pad[4];

	/* １．ヘッダ部の読み取り
		  1.1  ファイルヘッダ　14バイトの読み取り */

		  // バイナリのファイルを読み取るときは　fread 関数を使います。
		  //                     書き込むときは　fwrite関数を使います。
		  // アスキーのファイルを読み取るときは　fgets関数を使います。
		  //  　　　　　　　　　 書き込むときは　fprintf だったかな、を使います。
		  //
	fread(buf10, sizeof(unsigned char), 10, fp1);  // 10byte
	fread(&image_start_point, sizeof(unsigned int), 1, fp1); // 14byte
	fprintf(stderr, " image start point = %d\n", image_start_point);
	//  1.2 画像ヘッダの読み取り　（image_start_point - 14）バイト
	//  (1) ヘッダサイズ
	fread(&header_size, sizeof(unsigned int), 1, fp1); //18byte
	fprintf(stderr, " header_size = %ld\n", header_size);
	if (image_start_point == (14 + header_size)) {
		fprintf(stderr, " header size OK! \n");
	}
	else {
		fprintf(stderr, " header size error \n");
		exit(-1);
	}
	//  (2) 画像サイズ　（幅と高さ）
	fread(&width1, sizeof(unsigned int), 1, fp1); // 22byte
	fread(&height1, sizeof(unsigned int), 1, fp1); //26byte
	fprintf(stderr, " Width, Height = %d %d\n", width1, height1);

	r1 = (unsigned char*)malloc(width1 * height1);
	g1 = (unsigned char*)malloc(width1 * height1);
	b1 = (unsigned char*)malloc(width1 * height1);
	image = (unsigned char*)malloc(4 * width1 * height1);

	width = width1;
	height = height1;

	//  (3) ２バイト飛ばし（プレーン数）
	fread(buf2, sizeof(char), 2, fp1); //28byte
	//  (4) Color サイズ　（このプログラムは24bit true color のみ）
	fread(&color, sizeof(unsigned short), 1, fp1); //30byte
	if (color == 24) {  // 24bit true color
		fprintf(stderr, " It's 24bit true color. OK! \n");
	}
	else {
		fprintf(stderr, " Oh! No. It's not 24bit color. I hate it.\n");
		exit(-1);
	}
	//  (5) 圧縮形式
	fread(&dumlong, sizeof(unsigned int), 1, fp1); //34byte
	if (dumlong == 0) {  // 無圧縮
		fprintf(stderr, " It's non compress data. OK! \n");
	}
	else {
		fprintf(stderr, " Oh! No. Why did you compress the image ? %d\n", dumlong);
		exit(-1);
	}

	fread(buf20, sizeof(char), 20, fp1);

	k = 0;
	for (j = 0; j < height1; j++) {
		for (i = 0; i < width1; i++) {
			fread(&b1[k], sizeof(unsigned char), 1, fp1);
			fread(&g1[k], sizeof(unsigned char), 1, fp1);
			fread(&r1[k], sizeof(unsigned char), 1, fp1);
			k++;
		}
		if ((3 * width) % 4 != 0) fread(pad, sizeof(unsigned char), (4 - (3 * width) % 4), fp1);
	}
}
