#include<windows.h>
#include<GL\glut.h>
#include <iostream>
#include <GL/glu.h>
#include<math.h>
#include <stdlib.h>
#include<stdio.h>
#define PI 3.1416
#define NUM_SAND_PARTICLES 5000
#define NUM_BIRDS 5
#define NUM_STARS 100
using namespace std;

float starX[NUM_STARS];
float starY[NUM_STARS];
bool blinkStar[NUM_STARS];
float starAlpha[NUM_STARS];


float sandX[NUM_SAND_PARTICLES];
float sandY[NUM_SAND_PARTICLES];
float sandRadius[NUM_SAND_PARTICLES];
float sandR[NUM_SAND_PARTICLES];
float sandG[NUM_SAND_PARTICLES];
float sandB[NUM_SAND_PARTICLES];
float birdX[NUM_BIRDS] = {1000, 1020, 1015, 1005, 1010};
float birdY[NUM_BIRDS] = {440, 455, 450, 465, 445};
float birdSpeed = 0.8f;
int birdFlapPhase = 0;

bool isDay = true;
float skyTransition = 0.0f; // 0.0 = day, 1.0 = night
float sunX = 850, sunY = 450;     // Start of day
float moonX = -100, moonY = 100;  // Moon hidden on left

float sunRadius = 45, moonRadius = 35;
bool transitioning = false;
float transitionSpeed = 0.01f;


GLuint staticSceneList;
bool initialized = false;

float cloudX = 0;
float cloudYOffset = 100.0f;
GLint i, j, k;
float wavePhase = 0.0f;
float waveSpeed = 0.1f;

float boat1X = -100, boat2X = 800;
float speed1 = 0.6f, speed2 = -0.5f;


void generateSandParticles() {
    for (int i = 0; i < NUM_SAND_PARTICLES; i++) {
        sandX[i] = (float)(rand() % 1000);
        sandY[i] = (float)(rand() % 200);
        sandRadius[i] = 0.3f + ((rand() % 100) / 100.0f) * 0.7f;

        float baseR = 0.90f, baseG = 0.78f, baseB = 0.60f;
        float variation = ((rand() % 6) - 3) / 255.0f;

        sandR[i] = baseR + variation;
        sandG[i] = baseG + variation;
        sandB[i] = baseB + variation;
    }
}
void initStars() {
    for (int i = 0; i < NUM_STARS; i++) {
        starX[i] = rand() % 1000;
        starY[i] = 300 + rand() % 200;  // upper sky only
        blinkStar[i] = rand() % 2;      // randomly blinking or not
        starAlpha[i] = 1.0f;
    }
}

void init(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Clear to white
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Enable transparency
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 1000.0, 0.0, 500.0);

    srand(2);  // Optional: ensures consistent results on startup
    generateSandParticles();
    initStars();
    for (int i = 0; i < NUM_BIRDS; i++) {
    birdX[i] = -200.0f - i * 150;  // Birds start off-screen, staggered
      }

}

void drawCircle(float cx, float cy, float radius, int segments = 100) {
    glBegin(GL_POLYGON);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}
void circle(GLdouble rad)
{
    GLint points = 50;
    GLdouble delTheta = (2.0 * PI) / (GLdouble)points;
    GLdouble theta = 0.0;

    glBegin(GL_POLYGON);
    {
        for( i = 0; i <=50; i++, theta += delTheta )
        {
            glVertex2f(rad * cos(theta),rad * sin(theta));
        }
    }
    glEnd();
}

void drawGround() {
    glColor3f(0.90f, 0.72f, 0.43f); // Sandy yellow

    glBegin(GL_POLYGON);

    // Bottom edge (fixed)
    glVertex2f(0, 0);
    glVertex2f(1000, 0);

    // Top edge (randomized bumps)
    for (int x = 10000; x >= 0; x -= 20) {
        float y = 200 + rand() % 4;  // height between 190 and 205
        glVertex2f(x, y);
    }

    glEnd();
}

void drawClouds() {
    // === Background + Foreground Cloud Pair 1 ===
    glColor3f(0.85, 0.85, 0.85);
    float baseX1 = cloudX * 0.6 + 100, baseY1 = 560 - cloudYOffset;
    glPushMatrix(); glTranslatef(baseX1, baseY1, 0); drawCircle(0, 0, 15); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX1 - 12, baseY1 - 4, 0); drawCircle(0, 0, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX1 + 12, baseY1 - 4, 0); drawCircle(0, 0, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX1, baseY1 - 8, 0); drawCircle(0, 0, 8); glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);
    float fgX1 = baseX1 + 25, fgY1 = baseY1 - 10;
    glPushMatrix(); glTranslatef(fgX1, fgY1, 0); drawCircle(0, 0, 18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1 - 15, fgY1 - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1 + 15, fgY1 - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1, fgY1 - 10, 0); drawCircle(0, 0, 10); glPopMatrix();

    // === Extra Foreground Cloud Below Pair 1 ===
    float fgX1b = baseX1 - 20, fgY1b = baseY1 - 50;
    glPushMatrix(); glTranslatef(fgX1b, fgY1b, 0); drawCircle(0, 0, 18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1b - 15, fgY1b - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1b + 15, fgY1b - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1b, fgY1b - 10, 0); drawCircle(0, 0, 10); glPopMatrix();

    // === Background + Foreground Cloud Pair 2 ===
    glColor3f(0.85, 0.85, 0.85);
    float baseX2 = cloudX * 0.6 + 350, baseY2 = 530 - cloudYOffset;
    glPushMatrix(); glTranslatef(baseX2, baseY2, 0); drawCircle(0, 0, 15); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX2 - 12, baseY2 - 4, 0); drawCircle(0, 0, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX2 + 12, baseY2 - 4, 0); drawCircle(0, 0, 10); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX2, baseY2 - 8, 0); drawCircle(0, 0, 8); glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);
    float fgX2 = baseX2 - 25, fgY2 = baseY2 + 10;
    glPushMatrix(); glTranslatef(fgX2, fgY2, 0); drawCircle(0, 0, 18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2 - 15, fgY2 - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2 + 15, fgY2 - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2, fgY2 - 10, 0); drawCircle(0, 0, 10); glPopMatrix();

    // === Extra Foreground Cloud Below Pair 2 ===
    float fgX2b = baseX2 + 20, fgY2b = baseY2 - 50;
    glPushMatrix(); glTranslatef(fgX2b, fgY2b, 0); drawCircle(0, 0, 18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2b - 15, fgY2b - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2b + 15, fgY2b - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2b, fgY2b - 10, 0); drawCircle(0, 0, 10); glPopMatrix();

    // === Foreground-Only Cloud 1 ===
    float fgX3 = cloudX + 550, fgY3 = 510 - cloudYOffset;
    glPushMatrix(); glTranslatef(fgX3, fgY3, 0); drawCircle(0, 0, 18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX3 - 15, fgY3 - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX3 + 15, fgY3 - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX3, fgY3 - 10, 0); drawCircle(0, 0, 10); glPopMatrix();

    // === Foreground-Only Cloud 2 ===
    float fgX4 = cloudX + 700, fgY4 = 540 - cloudYOffset;
    glPushMatrix(); glTranslatef(fgX4, fgY4, 0); drawCircle(0, 0, 18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX4 - 15, fgY4 - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX4 + 15, fgY4 - 5, 0); drawCircle(0, 0, 12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX4, fgY4 - 10, 0); drawCircle(0, 0, 10); glPopMatrix();
}
void drawShadedSun(float x, float y, float radius) {
    // Center sun color
    float centerR = 0.95f, centerG = 0.7f, centerB = 0.2f;  // Yellow-orange
    float outerR  = 0.95f, outerG = 0.7f, outerB = 0.2f;


    int rings = 30;

    for (int i = rings; i >= 1; i--) {
        float alpha = (float)i / rings;
        float r = radius * alpha;

        glColor4f(centerR, centerG, centerB, 0.1f * alpha);  // fade outward

        glBegin(GL_POLYGON);
        for (int j = 0; j <= 360; j++) {
            float theta = j * PI / 180.0f;
            glVertex2f(x + cos(theta) * r, y + sin(theta) * r);
        }
        glEnd();
    }

    // Draw solid center
    glColor3f(centerR, centerG, centerB);
    glBegin(GL_POLYGON);
    for (int j = 0; j <= 360; j++) {
        float theta = j * PI / 180.0f;
        glVertex2f(x + cos(theta) * radius * 0.6f, y + sin(theta) * radius * 0.6f);
    }
    glEnd();
}
void drawMoon(float x, float y, float radius) {
    // Moon core color (off-white)
    float centerR = 0.95f, centerG = 0.95f, centerB = 0.92f;

    int rings = 25;

    for (int i = rings; i >= 1; i--) {
        float alpha = (float)i / rings;
        float r = radius * alpha;

        glColor4f(centerR, centerG, centerB, 0.07f * alpha);  // transparent glow

        glBegin(GL_POLYGON);
        for (int j = 0; j <= 360; j++) {
            float theta = j * PI / 180.0f;
            glVertex2f(x + cos(theta) * r, y + sin(theta) * r);
        }
        glEnd();
    }


    glColor3f(centerR, centerG, centerB);
    glBegin(GL_POLYGON);
    for (int j = 0; j <= 360; j++) {
        float theta = j * PI / 180.0f;
        glVertex2f(x + cos(theta) * radius * 0.6f, y + sin(theta) * radius * 0.6f);
    }
    glEnd();
}
void drawDynamicSky() {
    // Interpolated sky color
    float topR = (1.0f - skyTransition) * 0.35f + skyTransition * 0.05f;
    float topG = (1.0f - skyTransition) * 0.65f + skyTransition * 0.05f;
    float topB = (1.0f - skyTransition) * 0.85f + skyTransition * 0.2f;

    float bottomR = (1.0f - skyTransition) * 0.98f + skyTransition * 0.6f;
    float bottomG = (1.0f - skyTransition) * 0.80f + skyTransition * 0.6f;
    float bottomB = (1.0f - skyTransition) * 0.69f + skyTransition * 0.6f;

    glBegin(GL_QUADS);
        glColor3f(topR, topG, topB);
        glVertex2f(0, 500);
        glVertex2f(1000, 500);

        glColor3f(bottomR, bottomG, bottomB);
        glVertex2f(1000, 200);
        glVertex2f(0, 200);
    glEnd();
}

void drawStars() {
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_STARS; i++) {
        glColor4f(1.0f, 1.0f, 1.0f, blinkStar[i] ? starAlpha[i] : 1.0f);
        glVertex2f(starX[i], starY[i]);
    }
    glEnd();
}

void drawSandParticles() {
    for (int i = 0; i < NUM_SAND_PARTICLES; i++) {
        glColor3f(sandR[i], sandG[i], sandB[i]);
        glPushMatrix();
        glTranslatef(sandX[i], sandY[i], 0);
        circle(sandRadius[i]);
        glPopMatrix();
    }
}



void drawDottedLine(float x1, float y1, float x2, float y2, float spacing) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    float dist = sqrt(dx * dx + dy * dy);
    float steps = dist / spacing;
    float deltaX = dx / steps;
    float deltaY = dy / steps;

    glBegin(GL_POINTS);
    for (int i = 0; i < steps; i += 2) {
        float px = x1 + deltaX * i;
        float py = y1 + deltaY * i;
        glVertex2f(px, py);
    }
    glEnd();
}

void drawRandomDottedLayers(float baseX, float baseY, float width, float height, int minLines, int maxLines) {
    int lines = minLines + rand() % (maxLines - minLines + 1);
    for (int i = 0; i < lines; i++) {
        float frac = (float)i / lines;  // bottom to top
        float y = baseY + height * frac;
        float lineWidth = (width / 2.0f) * (1.0f - frac);  // wide at bottom, narrow at top
        float cx = baseX + width / 2.0f;

        // Denser spacing (2.0 or even 1.5 = tighter)
        drawDottedLine(cx - lineWidth, y, cx + lineWidth, y, 1.0f);
    }
}

void drawPyramidPerspective(float baseX, float baseY, float width, float height, float shadowOffsetX, float shadowOffsetY, int layers, float baseDip) {
    float centerX = baseX + width / 2.0f;



    // Front face
    glColor3f(0.84f, 0.72f, 0.42f);
    glBegin(GL_TRIANGLES);
        glVertex2f(baseX, baseY);
        glVertex2f(centerX, baseY + height);
        glVertex2f(baseX + width, baseY);
    glEnd();

    // Shadow face
    glColor3f(0.62f, 0.52f, 0.30f);
    glBegin(GL_TRIANGLES);
        glVertex2f(baseX + width, baseY);
        glVertex2f(centerX, baseY + height);
        glVertex2f(baseX + width + shadowOffsetX, baseY + shadowOffsetY);
    glEnd();



   glColor3f(0.58f, 0.48f, 0.30f);

    drawRandomDottedLayers(baseX, baseY, width, height, 45, layers);
}



// Simple Stylized Tree (non-palm)

void drawDesertTree(float x, float y, float scale) {
    float trunkHeight = 45 * scale;
    float trunkWidth = 8 * scale;

    // Trunk
    glColor3f(0.36f, 0.22f, 0.10f);
    glBegin(GL_POLYGON);
        glVertex2f(x - trunkWidth / 2, y);
        glVertex2f(x + trunkWidth / 2, y);
        glVertex2f(x + trunkWidth / 2, y + trunkHeight);
        glVertex2f(x - trunkWidth / 2, y + trunkHeight);
    glEnd();

    float crownY = y + trunkHeight;

    // Foliage - three overlapping ellipses
    glColor3f(0.13f, 0.5f, 0.13f);
    for (int i = -1; i <= 1; i++) {
        float offsetX = i * 10 * scale;
        float offsetY = (i == 0) ? 10 * scale : 0; // raise center ellipse
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j += 10) {
            float rad = j * PI / 180.0f;
            float cx = x + offsetX + cos(rad) * 18 * scale;
            float cy = crownY + offsetY + sin(rad) * 15 * scale;
            glVertex2f(cx, cy);
        }
        glEnd();
    }
}
// New Desert Tree Design: Layered canopy with branch structure

void drawStylizedDesertTree(float x, float y, float scale) {
    float trunkHeight = 50 * scale;
    float trunkWidth = 6 * scale;

    // Trunk
    glColor3f(0.35f, 0.2f, 0.1f);
    glBegin(GL_POLYGON);
        glVertex2f(x - trunkWidth / 2, y);
        glVertex2f(x + trunkWidth / 2, y);
        glVertex2f(x + trunkWidth / 2, y + trunkHeight);
        glVertex2f(x - trunkWidth / 2, y + trunkHeight);
    glEnd();

    float baseLeafY = y + trunkHeight;

    // Foliage - stacked horizontal leafy layers
    glColor3f(0.12f, 0.5f, 0.16f);
    for (int i = 0; i < 3; i++) {
        float yOffset = i * 10 * scale;
        float width = (30 - i * 5) * scale;
        float height = 10 * scale;

        glBegin(GL_POLYGON);
        for (int j = 0; j <= 360; j += 10) {
            float rad = j * PI / 180.0f;
            float cx = x + cos(rad) * width;
            float cy = baseLeafY + yOffset + sin(rad) * height;
            glVertex2f(cx, cy);
        }
        glEnd();
    }
}

void drawBlockyStone(float x, float y, float scale) {
    // Base sand-tinted stone color
    float baseR = 0.78f, baseG = 0.66f, baseB = 0.45f;

    // Add subtle variation: between -0.02 to +0.02
    float variation = ((rand() % 4) ) / 10.0f;

    float r = baseR - variation;
    float g = baseG - variation;
    float b = baseB - variation;

    // Clamp to 0.0 - 1.0 range to avoid color errors
    r = fmin(fmax(r, 0.0f), 1.0f);
    g = fmin(fmax(g, 0.0f), 1.0f);
    b = fmin(fmax(b, 0.0f), 1.0f);

    glColor3f(r, g, b);

    glBegin(GL_POLYGON);
        glVertex2f(x - 8 * scale, y);
        glVertex2f(x + 10 * scale, y + 2 * scale);
        glVertex2f(x + 12 * scale, y + 10 * scale);
        glVertex2f(x + 9 * scale, y + 16 * scale);
        glVertex2f(x - 3 * scale, y + 18 * scale);
        glVertex2f(x - 10 * scale, y + 13 * scale);
        glVertex2f(x - 12 * scale, y + 5 * scale);
    glEnd();

    glColor3f(0.44f, 0.49f, 0.43f);  // darker outline
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
      glVertex2f(x - 8 * scale, y);
      glVertex2f(x + 10 * scale, y + 2 * scale);
      glVertex2f(x + 12 * scale, y + 10 * scale);
      glVertex2f(x + 9 * scale, y + 16 * scale);
      glVertex2f(x - 3 * scale, y + 18 * scale);
      glVertex2f(x - 10 * scale, y + 13 * scale);
      glVertex2f(x - 12 * scale, y + 5 * scale);
    glEnd();

}
void drawObelisk(float x, float y, float scale) {
    float baseWidth = 20 * scale;
    float height = 100 * scale;
    float topHeight = 15 * scale;

    // ===== Obelisk Main Body =====
    glColor3f(0.80f, 0.65f, 0.45f); // sandstone shade
    glBegin(GL_POLYGON);
        glVertex2f(x - baseWidth / 2, y);
        glVertex2f(x + baseWidth / 2, y);
        glVertex2f(x + baseWidth / 2 * 0.9f, y + height);
        glVertex2f(x - baseWidth / 2 * 0.9f, y + height);
    glEnd();

    // ===== Horizontal dotted lines (engraving lines) =====
    glColor3f(0.58f, 0.48f, 0.30f); // darker shade for line contrast
    int lines = 20 + rand() % 10; // 20–29 lines
    for (int i = 0; i < lines; ++i) {
        float frac = (float)i / lines;
        float lineY = y + height * frac;
        float widthAtY = baseWidth * (1.0f - 0.1f * frac); // tapering slightly at top
        drawDottedLine(x - widthAtY / 2, lineY, x + widthAtY / 2, lineY, 1.0f);
    }

    // ===== Pyramidion (top point) =====
    glColor3f(0.90f, 0.75f, 0.55f); // Light tip
    glBegin(GL_TRIANGLES);
        glVertex2f(x - baseWidth / 2 * 0.9f, y + height);
        glVertex2f(x + baseWidth / 2 * 0.9f, y + height);
        glVertex2f(x, y + height + topHeight);
    glEnd();

    // ===== Base Platform =====
    glColor3f(0.55f, 0.45f, 0.28f);
    glBegin(GL_POLYGON);
        glVertex2f(x - baseWidth * 0.7f, y - 8 * scale);
        glVertex2f(x + baseWidth * 0.7f, y - 8 * scale);
        glVertex2f(x + baseWidth * 0.6f, y);
        glVertex2f(x - baseWidth * 0.6f, y);
    glEnd();
}


void drawRiver() {
   glColor3f(0.09f, 0.35f, 0.72f);  // Rich blue with a Nile tone

    glBegin(GL_POLYGON);

    // Bottom edge of river (fixed)
    glVertex2f(0, 0);


    // Top edge with sine wave animation
    for (int x = 0; x <= 1000; x += 10) {
        float y = 80 + 5 * sinf((x * 0.02f) + wavePhase); // 80 is base height
        glVertex2f(x, y);
    }

     glVertex2f(1000, 0);

    glEnd();
}
void drawCactus(float x, float y, float scale) {
    // Body color
    glColor3f(0.0f, 0.6f, 0.2f);  // Cactus green

    // --- Main vertical lobe (central oval) ---
    glBegin(GL_POLYGON);
    for (int angle = 0; angle <= 360; angle++) {
        float rad = angle * PI / 180.0f;
        float cx = cos(rad) * 20 * scale;
        float cy = sin(rad) * 35 * scale;
        glVertex2f(x + cx, y + cy + 35 * scale);  // raise vertically
    }
    glEnd();

    // --- Side lobe (right arm) ---
    glBegin(GL_POLYGON);
    for (int angle = 0; angle <= 360; angle++) {
        float rad = angle * PI / 180.0f;
        float cx = cos(rad) * 15 * scale;
        float cy = sin(rad) * 20 * scale;
        glVertex2f(x + 20 * scale + cx, y + 40 * scale + cy);  // offset to right + up
    }
    glEnd();


    // --- Yellow thorns ---
    glColor3f(1.0f, 1.0f, 0.0f);
    for (int i = 0; i < 25; i++) {
        float tx = x + (rand() % 40 - 20) * scale;
        float ty = y + (rand() % 70) * scale;
        float length = 5 * scale;

        glBegin(GL_LINES);
            glVertex2f(tx, ty);
            glVertex2f(tx + cos(i) * length, ty + sin(i) * length);
        glEnd();
    }
}
void drawStylizedSphinx(float x, float y, float scale) {
    // === COLORS ===
    glColor3f(0.80f, 0.62f, 0.35f); // Body color

    // === BODY BASE ===
    glBegin(GL_POLYGON);
        glVertex2f(x - 30 * scale, y);
        glVertex2f(x + 30 * scale, y);
        glVertex2f(x + 30 * scale, y + 20 * scale);
        glVertex2f(x - 30 * scale, y + 20 * scale);
    glEnd();

    // === LEFT SHOULDER ===
    glBegin(GL_POLYGON);
        glVertex2f(x - 30 * scale, y + 20 * scale);
        glVertex2f(x - 15 * scale, y + 40 * scale);
        glVertex2f(x - 10 * scale, y + 30 * scale);
        glVertex2f(x - 25 * scale, y + 20 * scale);
    glEnd();

    // === RIGHT SHOULDER ===
    glBegin(GL_POLYGON);
        glVertex2f(x + 30 * scale, y + 20 * scale);
        glVertex2f(x + 15 * scale, y + 40 * scale);
        glVertex2f(x + 10 * scale, y + 30 * scale);
        glVertex2f(x + 25 * scale, y + 20 * scale);
    glEnd();

    // === CHEST CONNECTOR ===
    glBegin(GL_POLYGON);
        glVertex2f(x - 15 * scale, y + 20 * scale);
        glVertex2f(x + 15 * scale, y + 20 * scale);
        glVertex2f(x + 10 * scale, y + 40 * scale);
        glVertex2f(x - 10 * scale, y + 40 * scale);
    glEnd();

    // === HEAD (trapezoid) ===
    glColor3f(0.76f, 0.58f, 0.32f);
    glBegin(GL_POLYGON);
        glVertex2f(x - 15 * scale, y + 40 * scale);
        glVertex2f(x + 15 * scale, y + 40 * scale);
        glVertex2f(x + 10 * scale, y + 60 * scale);
        glVertex2f(x - 10 * scale, y + 60 * scale);
    glEnd();

    // === HEADPIECE / EAR FLAPS ===
    glColor3f(0.55f, 0.4f, 0.2f);  // darker gold
    glBegin(GL_POLYGON);  // Left ear
        glVertex2f(x - 15 * scale, y + 40 * scale);
        glVertex2f(x - 20 * scale, y + 48 * scale);
        glVertex2f(x - 15 * scale, y + 52 * scale);
        glVertex2f(x - 10 * scale, y + 60 * scale);
    glEnd();

    glBegin(GL_POLYGON);  // Right ear
        glVertex2f(x + 15 * scale, y + 40 * scale);
        glVertex2f(x + 20 * scale, y + 48 * scale);
        glVertex2f(x + 15 * scale, y + 52 * scale);
        glVertex2f(x + 10 * scale, y + 60 * scale);
    glEnd();

    // === FACE DETAILS ===
    glColor3f(0.2f, 0.1f, 0.0f);  // face lines
    glBegin(GL_LINES); // Eyes
        glVertex2f(x - 6 * scale, y + 52 * scale);
        glVertex2f(x - 2 * scale, y + 52 * scale);
        glVertex2f(x + 2 * scale, y + 52 * scale);
        glVertex2f(x + 6 * scale, y + 52 * scale);
    glEnd();

    glBegin(GL_LINES); // Nose
        glVertex2f(x, y + 52 * scale);
        glVertex2f(x, y + 48 * scale);
    glEnd();

    // === HIEROGLYPHS - CHEST ===
    glColor3f(0.25f, 0.15f, 0.0f);
    for (int i = -2; i <= 2; ++i) {
        float hx = x + i * 5 * scale;
        float hy = y + 25 * scale;
        glBegin(GL_LINES);
            glVertex2f(hx, hy);
            glVertex2f(hx, hy + 5 * scale);
            glVertex2f(hx - 2 * scale, hy + 3 * scale);
            glVertex2f(hx + 2 * scale, hy + 3 * scale);
        glEnd();
    }

    // === HIEROGLYPHS - TORSO ===
    for (int i = -2; i <= 2; ++i) {
        float hx = x + i * 5 * scale;
        float hy = y + 5 * scale;
        glBegin(GL_LINES);
            glVertex2f(hx, hy);
            glVertex2f(hx, hy + 4 * scale);
            glVertex2f(hx - 1.5f * scale, hy + 2.5f * scale);
            glVertex2f(hx + 1.5f * scale, hy + 2.5f * scale);
        glEnd();
    }


    glEnd();
}

void drawPapyrusBoat(float x, float y, float scale) {
    // Hull base (papyrus golden)
    glColor3f(0.85f, 0.65f, 0.35f);
    glBegin(GL_POLYGON);
        glVertex2f(x - 40 * scale, y);           // back end
        glVertex2f(x - 30 * scale, y + 12 * scale);
        glVertex2f(x + 30 * scale, y + 12 * scale);
        glVertex2f(x + 40 * scale, y);           // front end
        glVertex2f(x + 35 * scale, y - 6 * scale);
        glVertex2f(x - 35 * scale, y - 6 * scale);
    glEnd();

    // Central floor shading
    glColor3f(0.75f, 0.55f, 0.28f);
    glBegin(GL_POLYGON);
        glVertex2f(x - 28 * scale, y + 3 * scale);
        glVertex2f(x + 28 * scale, y + 3 * scale);
        glVertex2f(x + 24 * scale, y + 8 * scale);
        glVertex2f(x - 24 * scale, y + 8 * scale);
    glEnd();

    // Papyrus flares (left and right)
    glColor3f(0.78f, 0.58f, 0.25f);
    glBegin(GL_TRIANGLES); // front
        glVertex2f(x + 40 * scale, y);
        glVertex2f(x + 50 * scale, y + 15 * scale);
        glVertex2f(x + 30 * scale, y + 15 * scale);
    glEnd();
    glBegin(GL_TRIANGLES); // back
        glVertex2f(x - 40 * scale, y);
        glVertex2f(x - 50 * scale, y + 15 * scale);
        glVertex2f(x - 30 * scale, y + 15 * scale);
    glEnd();

    // === CANOPY FRAME (POLES) ===
    glColor3f(0.3f, 0.2f, 0.1f);  // dark wood
    glBegin(GL_LINES);
        glVertex2f(x - 20 * scale, y + 12 * scale);
        glVertex2f(x - 20 * scale, y + 30 * scale);
        glVertex2f(x + 20 * scale, y + 12 * scale);
        glVertex2f(x + 20 * scale, y + 30 * scale);
    glEnd();

    // === CANOPY ROOF ===
    glColor3f(0.92f, 0.85f, 0.6f);  // light fabric
    glBegin(GL_POLYGON);
        glVertex2f(x - 20 * scale, y + 30 * scale);
        glVertex2f(x + 20 * scale, y + 30 * scale);
        glVertex2f(x + 18 * scale, y + 35 * scale);
        glVertex2f(x - 18 * scale, y + 35 * scale);
    glEnd();

    // === CARGO BOX ===
    glColor3f(0.65f, 0.45f, 0.25f);  // darker wood
    glBegin(GL_POLYGON);
        glVertex2f(x - 5 * scale, y + 12 * scale);
        glVertex2f(x + 5 * scale, y + 12 * scale);
        glVertex2f(x + 4 * scale, y + 20 * scale);
        glVertex2f(x - 4 * scale, y + 20 * scale);
    glEnd();

    // === ROPE LINES ===
    glColor3f(0.4f, 0.25f, 0.1f);
    glBegin(GL_LINES);
        glVertex2f(x - 20 * scale, y + 30 * scale);
        glVertex2f(x, y + 20 * scale);
        glVertex2f(x + 20 * scale, y + 30 * scale);
        glVertex2f(x, y + 20 * scale);
    glEnd();
}


void drawSailBarge(float x, float y, float scale) {
    // Hull
    glColor3f(0.65f, 0.45f, 0.25f);  // wood tone
    glBegin(GL_POLYGON);
        glVertex2f(x - 40 * scale, y);
        glVertex2f(x - 35 * scale, y + 10 * scale);
        glVertex2f(x + 35 * scale, y + 10 * scale);
        glVertex2f(x + 40 * scale, y);
        glVertex2f(x + 35 * scale, y - 5 * scale);
        glVertex2f(x - 35 * scale, y - 5 * scale);
    glEnd();

    // Sail mast
    glColor3f(0.3f, 0.2f, 0.1f);
    glBegin(GL_LINES);
        glVertex2f(x, y + 10 * scale);
        glVertex2f(x, y + 40 * scale);
    glEnd();

    // Sail
    glColor3f(1.0f, 0.9f, 0.6f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y + 40 * scale);
        glVertex2f(x - 20 * scale, y + 15 * scale);
        glVertex2f(x + 20 * scale, y + 15 * scale);
    glEnd();

    // Cabin in center
    glColor3f(0.6f, 0.4f, 0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(x - 10 * scale, y + 10 * scale);
        glVertex2f(x + 10 * scale, y + 10 * scale);
        glVertex2f(x + 8 * scale, y + 20 * scale);
        glVertex2f(x - 8 * scale, y + 20 * scale);
    glEnd();
}

void drawBird(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f); // Black bird

    glBegin(GL_POLYGON);
        glVertex2f(-25, -2);  // Tail
        glVertex2f(-20, 2);
        glVertex2f(-12, -3);

        glVertex2f(-5, -2);
        glVertex2f(5, 1);
        glVertex2f(15, 0);   // Front
        glVertex2f(20, -1);  // Beak

        // Wings
        glVertex2f(13, 5);
        glVertex2f(9, 9);
        glVertex2f(6, 5);

        glVertex2f(-4, 7);
        glVertex2f(-9, 12);
        glVertex2f(-7, 6);
    glEnd();

    glPopMatrix();
}


void drawBirdCurveLeft(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_POLYGON);
        glVertex2f(-18, -2);
        glVertex2f(-30, -10);
        glVertex2f(-20, -12);
        glVertex2f(-10, -6);
        glVertex2f(0, 0);
        glVertex2f(10, -6);
        glVertex2f(20, -12);
        glVertex2f(30, -10);
        glVertex2f(18, -2);
    glEnd();

    glPopMatrix();
}
void drawBirdCurveRight(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, 1.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    glBegin(GL_POLYGON);
        glVertex2f(-18, -2);
        glVertex2f(-30, -10);
        glVertex2f(-20, -12);
        glVertex2f(-10, -6);
        glVertex2f(0, 0);
        glVertex2f(10, -6);
        glVertex2f(20, -12);
        glVertex2f(30, -10);
        glVertex2f(18, -2);
    glEnd();

    glPopMatrix();
}

void drawEgyptianFigure(float x, float y, float scale) {
    glLineWidth(2.0f);

    // === BODY (robe) ===
    glColor3f(0.95f, 0.95f, 0.9f);  // Linen White

    glBegin(GL_QUADS);
        glVertex2f(x - 10 * scale, y);
        glVertex2f(x + 10 * scale, y);
        glVertex2f(x + 10 * scale, y + 30 * scale);
        glVertex2f(x - 10 * scale, y + 30 * scale);
    glEnd();

    // === HEAD (solid circle) ===
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i++) {
        float theta = i * PI / 180.0f;
        glVertex2f(x + 10 * scale * cos(theta), y + 40 * scale + 10 * scale * sin(theta));
    }
    glEnd();
    // === HEADBAND / HEADDRESS STRIPES ===
glColor3f(0.0f, 0.0f, 1.0f);  // royal blue

// Horizontal headband
glBegin(GL_QUADS);
    glVertex2f(x - 10 * scale, y + 45 * scale);
    glVertex2f(x + 10 * scale, y + 45 * scale);
    glVertex2f(x + 10 * scale, y + 42 * scale);
    glVertex2f(x - 10 * scale, y + 42 * scale);
glEnd();

// Side hanging stripes (symbolic of cloth flaps)
glBegin(GL_QUADS);
    glVertex2f(x - 10 * scale, y + 40 * scale);
    glVertex2f(x - 6 * scale, y + 40 * scale);
    glVertex2f(x - 6 * scale, y + 25 * scale);
    glVertex2f(x - 10 * scale, y + 25 * scale);

    glVertex2f(x + 6 * scale, y + 40 * scale);
    glVertex2f(x + 10 * scale, y + 40 * scale);
    glVertex2f(x + 10 * scale, y + 25 * scale);
    glVertex2f(x + 6 * scale, y + 25 * scale);
glEnd();

// Cobra emblem (very simplified)
glColor3f(1.0f, 0.84f, 0.0f); // gold
glBegin(GL_TRIANGLES);
    glVertex2f(x, y + 53 * scale);           // top
    glVertex2f(x - 2 * scale, y + 48 * scale);
    glVertex2f(x + 2 * scale, y + 48 * scale);
glEnd();

    // === LEGS (rectangles) ===
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        // Left leg
        glVertex2f(x - 8 * scale, y);
        glVertex2f(x - 3 * scale, y);
        glVertex2f(x - 3 * scale, y - 20 * scale);
        glVertex2f(x - 8 * scale, y - 20 * scale);

        // Right leg
        glVertex2f(x + 3 * scale, y);
        glVertex2f(x + 8 * scale, y);
        glVertex2f(x + 8 * scale, y - 20 * scale);
        glVertex2f(x + 3 * scale, y - 20 * scale);
    glEnd();

    // === ARMS (rectangles) ===
    glBegin(GL_QUADS);
        // Left arm
        glVertex2f(x - 10 * scale, y + 25 * scale);
        glVertex2f(x - 20 * scale, y + 10 * scale);
        glVertex2f(x - 18 * scale, y + 8 * scale);
        glVertex2f(x - 8 * scale, y + 23 * scale);

        // Right arm
        glVertex2f(x + 10 * scale, y + 25 * scale);
        glVertex2f(x + 20 * scale, y + 10 * scale);
        glVertex2f(x + 18 * scale, y + 8 * scale);
        glVertex2f(x + 8 * scale, y + 23 * scale);
    glEnd();

    // === CROOK STAFF ===
    glColor3f(0.3f, 0.1f, 0.3f);  // golden
    glBegin(GL_LINES);
        glVertex2f(x + 25 * scale, y + 10 * scale);
        glVertex2f(x + 25 * scale, y + 55 * scale);
    glEnd();

    // Crook hook (curved top)
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 180; i++) {
        float angle = i * PI / 180.0f;
        float r = 5 * scale;
        glVertex2f(x + 25 * scale + r * cos(angle), y + 55 * scale + r * sin(angle));
    }
    glEnd();
}

void drawCamel(float x, float y, float scale) {
    glColor3f(0.76f, 0.45f, 0.20f);// sandy brown camel color

    // === BODY ===
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 60 * scale, y);
        glVertex2f(x + 60 * scale, y + 30 * scale);
        glVertex2f(x, y + 30 * scale);
    glEnd();

    // === HUMP ===
    glBegin(GL_QUADS);
        glVertex2f(x + 10 * scale, y + 30 * scale);
        glVertex2f(x + 20 * scale, y + 30 * scale);
        glVertex2f(x + 20 * scale, y + 40 * scale);
        glVertex2f(x + 10 * scale, y + 40 * scale);
    glEnd();

    // === NECK ===
    glBegin(GL_QUADS);
        glVertex2f(x + 50 * scale, y + 30 * scale);
        glVertex2f(x + 55 * scale, y + 30 * scale);
        glVertex2f(x + 55 * scale, y + 60 * scale);
        glVertex2f(x + 50 * scale, y + 60 * scale);
    glEnd();

    // === HEAD ===
    glBegin(GL_QUADS);
        glVertex2f(x + 55 * scale, y + 50 * scale);
        glVertex2f(x + 65 * scale, y + 50 * scale);
        glVertex2f(x + 65 * scale, y + 60 * scale);
        glVertex2f(x + 55 * scale, y + 60 * scale);
    glEnd();

    // === LEGS ===
    float legWidth = 5 * scale;
    float legHeight = 25 * scale;

    // Front-left
    glBegin(GL_QUADS);
        glVertex2f(x + 5 * scale, y);
        glVertex2f(x + 10 * scale, y);
        glVertex2f(x + 10 * scale, y - legHeight);
        glVertex2f(x + 5 * scale, y - legHeight);
    glEnd();

    // Front-right
    glBegin(GL_QUADS);
        glVertex2f(x + 20 * scale, y);
        glVertex2f(x + 25 * scale, y);
        glVertex2f(x + 25 * scale, y - legHeight);
        glVertex2f(x + 20 * scale, y - legHeight);
    glEnd();

    // Back-left
    glBegin(GL_QUADS);
        glVertex2f(x + 40 * scale, y);
        glVertex2f(x + 45 * scale, y);
        glVertex2f(x + 45 * scale, y - legHeight);
        glVertex2f(x + 40 * scale, y - legHeight);
    glEnd();

    // Back-right
    glBegin(GL_QUADS);
        glVertex2f(x + 55 * scale, y);
        glVertex2f(x + 60 * scale, y);
        glVertex2f(x + 60 * scale, y - legHeight);
        glVertex2f(x + 55 * scale, y - legHeight);
    glEnd();
}

void createStaticScene() {
    staticSceneList = glGenLists(1);
    glNewList(staticSceneList, GL_COMPILE);

    drawGround();
    drawSandParticles();
    drawStylizedDesertTree(70, 200, 0.5f);
    drawDesertTree(90, 200, 0.5f);
    drawBlockyStone(90, 195, 0.5f);
    drawBlockyStone(69, 197, 0.4f);
    drawBlockyStone(78, 192, 0.6f);
    drawPyramidPerspective(120, 200, 60, 25, 7, 7, 6, 4);
    drawCamel(80, 170, 0.6f);
    drawEgyptianFigure(70, 170, 0.4f);

    drawStylizedDesertTree(520, 195, 0.4f);
    drawStylizedDesertTree(500, 200, .6f);
    drawPyramidPerspective(250, 173, 250, 130, 28, 26, 40, 6);
    drawPyramidPerspective(110, 140, 270, 145, 32, 28, 30, 8);

    drawObelisk(910, 160, 1.4f);
    drawObelisk(850, 175, 1.3f);
    drawDesertTree(945, 200, 1.0f);
    drawBlockyStone(950, 190, 1.0f);
    drawStylizedDesertTree(910, 200, 0.9f);
    drawStylizedSphinx(650, 165, 1.5f);
    drawPyramidPerspective(620, 145, 340, 155, 35, 30, 25, 6);
    drawCactus(100, 100, 0.5f);
    drawCactus(150, 100, 0.6f);

    drawCactus(900, 100, 0.5f);
    drawCactus(850, 100, 0.6f);

     drawEgyptianFigure(430, 165, 0.5f);
     drawCamel(450, 160, 0.6f);



    drawEgyptianFigure(540, 200, 0.3f);
    drawCamel(570, 200, 0.4f);
    drawEgyptianFigure(560, 190, 0.3f);








    glEndList();
    initialized = true;
}

void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
    drawDynamicSky();

     drawShadedSun(sunX, sunY, sunRadius);
    drawMoon(moonX, moonY, moonRadius);
     drawClouds();
    if (skyTransition >= 0.3f) {
    drawStars();
                             }
    if (!initialized)
        createStaticScene();

    glCallList(staticSceneList);




    drawRiver();

    drawPapyrusBoat(boat1X, 60, 1.0f);
    drawSailBarge(boat2X, 20, 1.0f);
    for (int i = 0; i < NUM_BIRDS; i++) {
    float x = birdX[i];
    float y = birdY[i];
    float scale = 0.4f;

    switch (birdFlapPhase) {
        case 0: drawBirdCurveRight(x, y, scale); break;
        case 1: drawBirdCurveLeft(x, y, scale); break;
        case 2: drawBirdCurveLeft(x, y, scale); break;
        case 3: drawBirdCurveRight(x, y, scale); break;
    }
}




    glutSwapBuffers(); // Replaces glFlush()
}

void update(int value) {
    wavePhase += waveSpeed;
    if (wavePhase > 2 * PI) wavePhase -= 2 * PI;


    if (transitioning) {
    if (isDay) {
        skyTransition += transitionSpeed;

        // move sun to (1100, 100)
        sunX += (1100 - 850) * transitionSpeed;
        sunY += (100 - 350) * transitionSpeed;

        // move moon to (300, 350)
        moonX += (150 - (-100)) * transitionSpeed;
        moonY += (450 - 100) * transitionSpeed;

        if (skyTransition >= 1.0f) {
            skyTransition = 1.0f;
            isDay = false;
            transitioning = false;
        }

    } else {
        skyTransition -= transitionSpeed;

        // move sun back to (850, 350)
        sunX -= (1100 - 850) * transitionSpeed;
        sunY -= (100 - 350) * transitionSpeed;

        // move moon back to (-100, 100)
        moonX -= (150 - (-100)) * transitionSpeed;
        moonY -= (450 - 100) * transitionSpeed;

        if (skyTransition <= 0.0f) {
            skyTransition = 0.0f;
            isDay = true;
            transitioning = false;
        }
    }
}

    boat1X += speed1;
    boat2X += speed2;

    if (boat1X > 1100) boat1X = -200;
    if (boat2X < -200) boat2X = 1200;
    cloudX += 1;
    if (cloudX > 900) cloudX = -200;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}




void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':  // move boat1 left
            boat1X -= 5.0f;
            break;
        case 'd':  // move boat1 right
            boat1X += 5.0f;
            break;
        case 'j':  // move boat2 left
            boat2X -= 5.0f;
            break;
        case 'l':  // move boat2 right
            boat2X += 5.0f;
            break;
        case 'n':  // Toggle day/night
            if (!transitioning) transitioning = true;
            break;

        case '1':
            glutDestroyWindow(glutGetWindow());
            system("APP.exe");
            break;
        case '2':
            glutDestroyWindow(glutGetWindow());
            system("Egypt.exe");
            break;
        case '3':
            glutDestroyWindow(glutGetWindow());
            system("Mediaval.exe");
            break;
        case '4':
            glutDestroyWindow(glutGetWindow());
            system("Portal.exe");
            break;




    }

    // Optional clamping
    if (boat1X < 0) boat1X = 0;
    if (boat1X > 1000) boat1X = 1000;
    if (boat2X < 0) boat2X = 0;
    if (boat2X > 1000) boat2X = 1000;



    glutPostRedisplay();  // Update screen
}



int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Enable double buffering
    glutInitWindowPosition(20, 20);
    glutInitWindowSize(1300, 800);
    glutCreateWindow("Ancient Egypt Scene");

    // Register callbacks
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    init();

    cout<<"      "<<endl;
    cout<<"       "<<endl;
    printf("=== Ancient Egypt Scene Controls ===\n");
    printf("Boat 1 Movement: 'a' = Left, 'd' = Right\n");
    printf("Boat 2 Movement: 'j' = Left, 'l' = Right\n");
    printf("Toggle Day/Night: 'n'\n");
    printf("====================================\n");

    glutMainLoop();
}


