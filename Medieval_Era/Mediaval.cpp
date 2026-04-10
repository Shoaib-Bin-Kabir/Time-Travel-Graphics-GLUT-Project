#include<windows.h>
#include<GL/glut.h>
#include <GL/glu.h>
#include<math.h>
#include<iostream>
#include <stdlib.h>
#include<stdio.h>
#define PI 3.1416
using namespace std;

float flagTime = 0.0f;
float windmillAngle = 0.0f;
float cartX = 0.0f;          // Horizontal movement of cart
bool cartMoving = false;     // Control flag

bool movingRight = true;
float cloudX = 0;
float cloudYOffset = 100.0f;
float rightSoldierX = 700.0f;
float soldierX = 300.0f;
float soldierSpeed = 0.2f;
bool rightSoldierMovingRight = false;
bool leftSoldierMovingRight = true;
float rightSoldierSpeed = 0.2f;
float birdGroup1X = -100.0f;
float birdGroup2X = -200.0f;
float wingFlapAngle = 0.0f;
float sunY = 275.0f;             // Start at ground level
float sunMaxY = 400.0f;          // Mid-sky target
bool sunRising = true;
float skyBlueFactor = 0.0f;      // 0 = white near bottom, increases to 1.0 (full blue)


void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 1000.0, 0.0, 500.0);
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

void drawTriangle(float cx, float cy, float size) {
    glBegin(GL_TRIANGLES);
        glVertex2f(cx, cy);
        glVertex2f(cx - size / 2, cy - size);
        glVertex2f(cx + size / 2, cy - size);
    glEnd();
}

void drawEllipse(float cx, float cy, float rx, float ry, int segments = 100) {
    glBegin(GL_POLYGON);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        float x = rx * cosf(angle);
        float y = ry * sinf(angle);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}
void drawSky() {
    glBegin(GL_QUADS);
        glColor3f(0.53f, 0.81f, 0.98f); // Top - Light sky blue
        glVertex2f(0, 500);
        glVertex2f(1000, 500);

        glColor3f(1.0f, 1.0f, 1.0f);   // Bottom - White
        glVertex2f(1000, 275);
        glVertex2f(0, 275);
    glEnd();
}



// Function to draw a simple V-shaped bird
void drawBird(float x, float y, float size = 10.0f) {
    float angle = sin(wingFlapAngle) * 15.0f; // Flap range: +/-15 degrees
    float radLeft = (45 + angle) * PI / 180.0f;   // Left wing: upward/downward
    float radRight = (135 - angle) * PI / 180.0f; // Right wing: mirror opposite

    glLineWidth(3.5f);
    glBegin(GL_LINES);
        // Left wing
        glVertex2f(x, y);
        glVertex2f(x + cos(radLeft) * size, y + sin(radLeft) * size);

        // Right wing
        glVertex2f(x, y);
        glVertex2f(x + cos(radRight) * size, y + sin(radRight) * size);
    glEnd();
    glLineWidth(1.0f);
}


// Function to draw a group of 3 birds
void drawBirdGroup(float startX, float startY) {
    drawBird(startX, startY);
    drawBird(startX + 20, startY + 5);
    drawBird(startX + 40, startY);
}

void drawAllBirdGroups() {
    glColor3f(0.1f, 0.1f, 0.1f); // Dark birds
    drawBirdGroup(birdGroup1X, 450);
    drawBirdGroup(birdGroup2X, 430);
    drawBirdGroup(birdGroup2X-200, 410);
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

void drawground() {
    glBegin(GL_QUADS);
        glColor3f(0.76f, 0.60f, 0.42f); // Bottom - Sandy brown

        glVertex2f(0, 275);
        glVertex2f(1000, 275);

        glColor3f(0.42f, 0.56f, 0.14f); // Top - Olive drab green
        glVertex2f(1000, 0);
        glVertex2f(0, 0);
    glEnd();
}

void drawMountains() {
    glColor3f(0.3f, 0.4f, 0.f); // Dark gray silhouette

    glBegin(GL_POLYGON);
        glVertex2f(0, 275); // Left base

        // Left side: small and medium peaks
        glVertex2f(50, 320);
        glVertex2f(100, 275);
        glVertex2f(150, 350);
        glVertex2f(200, 275);
        glVertex2f(250, 325);
        glVertex2f(300, 275);
        glVertex2f(350, 340);
        glVertex2f(400, 275);
        glVertex2f(450, 300);

        // Middle smaller zigzags
        glVertex2f(500, 275);
        glVertex2f(550, 320);
        glVertex2f(600, 275);
        glVertex2f(650, 310);
        glVertex2f(700, 275);

        // Right side: only small peaks
        glVertex2f(750, 295);
        glVertex2f(800, 275);
        glVertex2f(850, 290);
        glVertex2f(900, 275);
        glVertex2f(950, 285);
        glVertex2f(1000, 275);

        glVertex2f(1000, 0); // Bottom right (fill)
        glVertex2f(0, 0);    // Bottom left
    glEnd();
}



void drawSunrise(float cx, float cy, float maxRadius) {
    for (float r = maxRadius; r > 0; r -= 2.0f) {
        float t = r / maxRadius; // gradient factor (1 to 0)
        glColor3f(1.0f, 0.4f * t + 0.6f, 0.0f); // From orange to bright yellow

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy); // center
        for (int i = 0; i <= 100; i++) {
            float angle = 2.0f * PI * i / 100; // Full 360° circle
            float x = cx + cosf(angle) * r;
            float y = cy + sinf(angle) * r;
            glVertex2f(x, y);
        }
        glEnd();
    }
}


void drawWavingFlag(float x, float y) {
    glBegin(GL_LINE_STRIP);
    for (float i = 0; i < 1; i += 0.05) {
        float wave = sin(i * 10 + glutGet(GLUT_ELAPSED_TIME) * 0.005) * 5;
        glVertex2f(x + i * 40, y + wave);
    }
    glEnd();
}


void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

void drawCastle() {
    float scale = 0.6f;
    float offsetX = 500;
    float offsetY = 200;

    // ===== Wall Base =====
    glColor3f(0.6f, 0.6f, 0.6f); // Gray stone
    glBegin(GL_POLYGON);
        glVertex2f(offsetX - 240 * scale, offsetY);
        glVertex2f(offsetX + 240 * scale, offsetY);
        glVertex2f(offsetX + 240 * scale, offsetY + 200 * scale);
        glVertex2f(offsetX - 240 * scale, offsetY + 200 * scale);
    glEnd();

    // ===== Left Tower =====
    glBegin(GL_POLYGON);
        glVertex2f(offsetX - 290 * scale, offsetY);
        glVertex2f(offsetX - 240 * scale, offsetY);
        glVertex2f(offsetX - 240 * scale, offsetY + 250 * scale);
        glVertex2f(offsetX - 290 * scale, offsetY + 250 * scale);
    glEnd();


    // ===== Right Tower =====
    glBegin(GL_POLYGON);
        glVertex2f(offsetX + 240 * scale, offsetY);
        glVertex2f(offsetX + 290 * scale, offsetY);
        glVertex2f(offsetX + 290 * scale, offsetY + 250 * scale);
        glVertex2f(offsetX + 240 * scale, offsetY + 250 * scale);
    glEnd();


    // ===== Central Gatehouse =====
    float gatehouseW = 120 * scale;
    float gatehouseH = 220 * scale;
    glBegin(GL_POLYGON);
        glVertex2f(offsetX - gatehouseW / 2, offsetY);
        glVertex2f(offsetX + gatehouseW / 2, offsetY);
        glVertex2f(offsetX + gatehouseW / 2, offsetY + gatehouseH);
        glVertex2f(offsetX - gatehouseW / 2, offsetY + gatehouseH);
    glEnd();

    // ===== Central Gatehouse Battlements =====
glColor3f(0.4f, 0.4f, 0.45f);
float battlementW = 15 * scale;
float battlementH = 15 * scale;
float spacing = 5 * scale;

float gatehouseTopY = offsetY + 220 * scale;
float gatehouseLeft = offsetX - 60 * scale;
float gatehouseRight = offsetX + 60 * scale;

// Draw 4 small battlements on gatehouse
for (float bx = gatehouseLeft + spacing; bx + battlementW <= gatehouseRight - spacing; bx += battlementW + spacing) {
    glBegin(GL_POLYGON);
        glVertex2f(bx, gatehouseTopY);
        glVertex2f(bx + battlementW, gatehouseTopY);
        glVertex2f(bx + battlementW, gatehouseTopY + battlementH);
        glVertex2f(bx, gatehouseTopY + battlementH);
    glEnd();
}


  // ===== Wall Battlements (excluding towers & gatehouse top) =====
  glColor3f(0.4f, 0.4f, 0.45f);
  battlementW = 15 * scale;
   battlementH = 15 * scale;
   spacing = 3 * scale;
  float wallLeft = offsetX - 240 * scale;
  float wallRight = offsetX + 240 * scale;
  float gateLeft = offsetX - gatehouseW / 2;
  float gateRight = offsetX + gatehouseW / 2;

// Draw battlements before gatehouse (extended)
  for (float bx = wallLeft+2 ; bx + battlementW <= gateLeft ; bx += battlementW + spacing) {
    glBegin(GL_POLYGON);
        glVertex2f(bx, offsetY + 200 * scale);
        glVertex2f(bx + battlementW, offsetY + 200 * scale);
        glVertex2f(bx + battlementW, offsetY + 200 * scale + battlementH);
        glVertex2f(bx, offsetY + 200 * scale + battlementH);
    glEnd();
  }

// Draw battlements after gatehouse (extended)
  for (float bx = gateRight+2 ; bx + battlementW <= wallRight ; bx += battlementW + spacing) {
    glBegin(GL_POLYGON);
        glVertex2f(bx, offsetY + 200 * scale);
        glVertex2f(bx + battlementW, offsetY + 200 * scale);
        glVertex2f(bx + battlementW, offsetY + 200 * scale + battlementH);
        glVertex2f(bx, offsetY + 200 * scale + battlementH);
    glEnd();
  }


    // ===== Gate (Arched) =====
    glColor3f(0.3f, 0.2f, 0.1f); // Wood
    drawEllipse(offsetX, offsetY + 80 * scale, 40 * scale, 40 * scale);
    glBegin(GL_POLYGON);
        glVertex2f(offsetX - 40 * scale, offsetY + 80 * scale);
        glVertex2f(offsetX + 40 * scale, offsetY + 80 * scale);
        glVertex2f(offsetX + 40 * scale, offsetY);
        glVertex2f(offsetX - 40 * scale, offsetY);
    glEnd();

    // ===== Flag Pole =====
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_LINES);
        glVertex2f(offsetX, offsetY + gatehouseH);
        glVertex2f(offsetX, offsetY + gatehouseH + 60 * scale);
    glEnd();

    // ===== Waving Flag =====
    glColor3f(0.8f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= 10; ++i) {
        float t = i / 10.0f;
        float wave = sin(t * 2 * PI + flagTime) * 5;
        glVertex2f(offsetX + t * 35 * scale, offsetY + gatehouseH + 45 * scale + wave);
        glVertex2f(offsetX + t * 35 * scale, offsetY + gatehouseH + 60 * scale + wave);
    }
    glEnd();
}


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

void drawWindmill(float x, float y, float scale) {
    // Windmill tower
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_POLYGON);
        glVertex2f(x - 10 * scale, y);
        glVertex2f(x + 10 * scale, y);
        glVertex2f(x + 8 * scale, y + 60 * scale);
        glVertex2f(x - 8 * scale, y + 60 * scale);
    glEnd();

    // Windmill hub
    glColor3f(0.3f, 0.3f, 0.3f);
    drawCircle(x, y + 70 * scale, 5 * scale);

    // Windmill blades (rotating)
    glPushMatrix();
    glTranslatef(x, y + 70 * scale, 0);
    glRotatef(windmillAngle, 0, 0, 1);
    glColor3f(0.96f, 0.94f, 0.89f); // Linen-like off-white


    // Blade 1
    glBegin(GL_POLYGON);
        glVertex2f(0, 0);
        glVertex2f(0, 40 * scale);
        glVertex2f(5 * scale, 40 * scale);
        glVertex2f(5 * scale, 0);
    glEnd();

    // Blade 2
    glBegin(GL_POLYGON);
        glVertex2f(0, 0);
        glVertex2f(0, -40 * scale);
        glVertex2f(-5 * scale, -40 * scale);
        glVertex2f(-5 * scale, 0);
    glEnd();

    // Blade 3
    glBegin(GL_POLYGON);
        glVertex2f(0, 0);
        glVertex2f(40 * scale, 0);
        glVertex2f(40 * scale, 5 * scale);
        glVertex2f(0, 5 * scale);
    glEnd();

    // Blade 4
    glBegin(GL_POLYGON);
        glVertex2f(0, 0);
        glVertex2f(-40 * scale, 0);
        glVertex2f(-40 * scale, -5 * scale);
        glVertex2f(0, -5 * scale);
    glEnd();

    glPopMatrix();
}

void drawCustomFlag(float x, float y, float width, float height) {
    // === Dark Navy flag background ===
    glColor3f(0.0f, 0.1f, 0.4f); // Dark Navy
    glBegin(GL_POLYGON);
        glVertex2f(x, y);                          // Top-left
        glVertex2f(x + width, y);                 // Top-right
        glVertex2f(x + width, y - height * 0.6f); // Mid-right
        glVertex2f(x + width * 0.55f, y - height); // Bottom center-right
        glVertex2f(x + width * 0.5f, y - height * 0.8f); // Bottom tip
        glVertex2f(x + width * 0.45f, y - height); // Bottom center-left
        glVertex2f(x, y - height * 0.6f);         // Mid-left
    glEnd();

    // === Silver Emblem ===
    glColor3f(0.85f, 0.85f, 0.85f); // Silver

    // --- Vertical Bar ---
    float cx = x + width * 0.5f;
    float top = y - height * 0.3f;
    float bottom = y - height * 0.7f;
    float barWidth = width * 0.04f;

    glBegin(GL_POLYGON);
        glVertex2f(cx - barWidth / 2, bottom);
        glVertex2f(cx + barWidth / 2, bottom);
        glVertex2f(cx + barWidth / 2, top);
        glVertex2f(cx - barWidth / 2, top);
    glEnd();

    // --- Curved Horns ---
    glBegin(GL_LINE_STRIP);
    for (float angle = -1.0f; angle <= 1.0f; angle += 0.1f) {
        float px = cx + sin(angle) * width * 0.25f;
        float py = y - height * 0.45f + cos(angle) * height * 0.12f;
        glVertex2f(px, py);
    }
    glEnd();
}

void drawSimpleVeranda(float x, float y, float width, float height) {
    // Base rectangle (light color for veranda base)
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();

    // Railings (dark vertical lines)
    glColor3f(0.4f, 0.4f, 0.45f);
    float barWidth = 4.0f;
    float spacing = 10.0f;
    for (float i = spacing; i < width; i += spacing) {
        glBegin(GL_POLYGON);
            glVertex2f(x + i - barWidth / 2, y);
            glVertex2f(x + i + barWidth / 2, y);
            glVertex2f(x + i + barWidth / 2, y + height);
            glVertex2f(x + i - barWidth / 2, y + height);
        glEnd();
    }
}

void drawWindow(float x, float y, float width, float height, float r, float g, float b) {
    float radiusX = width / 2;
    float radiusY = width / 4;

    // === Fill color for window body ===
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);                 // Bottom left
        glVertex2f(x + width, y);         // Bottom right
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();


    glColor3f(0.2f, 0.2f, 0.2f); // Black outline for ellipse
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i++) {
        float theta = i * PI / 180.0f;
        float ex = x + radiusX + cos(theta) * radiusX;
        float ey = y + height + sin(theta) * radiusY;
        glVertex2f(ex, ey);
    }
    glEnd();




}
void drawStoneRoad() {
    // ======== Stone Road ========
    glColor3f(0.4f, 0.4f, 0.4f); // Dark gray stones

    // Main horizontal road in front of castle and houses
    glBegin(GL_POLYGON);
        glVertex2f(0, 140);
        glVertex2f(1000, 140);
        glVertex2f(1000, 83);
        glVertex2f(0, 83);
    glEnd();

    // Vertical road to castle gate
    glBegin(GL_POLYGON);
        glVertex2f(470, 140);
        glVertex2f(530, 140);
        glVertex2f(530, 200);
        glVertex2f(470, 200);
    glEnd();

    // ======== Stone Slabs on Horizontal Road ========
    glColor3f(0.3f, 0.3f, 0.3f); // Slightly darker for tiles
    for (int i = 0; i < 20; ++i) {
        float x = 10 + i * 50;
        glBegin(GL_LINE_LOOP);
            glVertex2f(x, 83);
            glVertex2f(x + 40, 83);
            glVertex2f(x + 40, 140);
            glVertex2f(x, 140);
        glEnd();
    }

    // ======== Stone Slabs on Vertical Road ========
    for (int i = 0; i < 3; ++i) {
        float y = 140 + i * 20;
        glBegin(GL_LINE_LOOP);
            glVertex2f(470, y);
            glVertex2f(530, y);
            glVertex2f(530, y + 15);
            glVertex2f(470, y + 15);
        glEnd();
    }
}

void drawCobblestoneRoad() {
    // ======== Cobblestone Base ========
    glColor3f(0.45f, 0.45f, 0.45f); // Slightly lighter gray

    // Main horizontal cobblestone road
    glBegin(GL_POLYGON);
        glVertex2f(0, 140);
        glVertex2f(1000, 140);
        glVertex2f(1000, 83);
        glVertex2f(0, 83);
    glEnd();

    // Vertical road to castle gate
    glBegin(GL_POLYGON);
        glVertex2f(470, 140);
        glVertex2f(530, 140);
        glVertex2f(530, 200);
        glVertex2f(470, 200);
    glEnd();

    // ======== Cobblestone Grid - Horizontal ========
    glColor3f(0.3f, 0.3f, 0.3f); // Stone outlines
    int rowCount = 3;
    int colCount = 25;
    float startY = 83;
    float brickH = (140 - 83) / (float)rowCount;
    float brickW = 1000 / (float)colCount;

    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < colCount; ++j) {
            float x = j * brickW;
            float y = startY + i * brickH;
            glBegin(GL_LINE_LOOP);
                glVertex2f(x, y);
                glVertex2f(x + brickW - 2, y);
                glVertex2f(x + brickW - 2, y + brickH - 2);
                glVertex2f(x, y + brickH - 2);
            glEnd();
        }
    }

    // ======== Cobblestone Grid - Vertical ========
    int vRow = 3;
    int vCol = 3;
    float vertX = 470;
    float vertY = 140;
    float vW = 60;
    float vH = 15;

    for (int i = 0; i < vRow; ++i) {
        for (int j = 0; j < vCol; ++j) {
            float x = vertX + j * (vW / vCol);
            float y = vertY + i * (vH + 3);
            glBegin(GL_LINE_LOOP);
                glVertex2f(x, y);
                glVertex2f(x + (vW / vCol) - 2, y);
                glVertex2f(x + (vW / vCol) - 2, y + vH);
                glVertex2f(x, y + vH);
            glEnd();
        }
    }
}

void drawCart(float cartX) {
    glPushMatrix();
    glTranslatef(cartX, 0.0f, 0.0f); // Move cart horizontally

    // === Wooden Cart Body ===
    glColor3f(0.4f, 0.3f, 0.2f); // Light brown
    glBegin(GL_POLYGON);
        glVertex2f(80, 90);
        glVertex2f(140, 90);
        glVertex2f(140, 110);
        glVertex2f(80, 110);
    glEnd();

    // === Side Supports ===
    glColor3f(0.4f, 0.2f, 0.1f); // Darker wood
    glBegin(GL_LINES);
        glVertex2f(80, 110); glVertex2f(80, 120);
        glVertex2f(140, 110); glVertex2f(140, 120);
    glEnd();

    // === Cart Handle ===
    glBegin(GL_LINES);
        glVertex2f(140, 90); glVertex2f(155, 80);
    glEnd();

    // === Wheels ===
    glColor3f(0.1f, 0.1f, 0.1f); // Black
    drawCircle(90, 85, 8);
    drawCircle(130, 85, 8);

    glPopMatrix();
}

void drawWoodenHouse() {

// ========== Wooden House 1 ==========
    float x = 50, y = 150, scale = 1.0f;

    // House base (wood)
    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 60 * scale, y);
        glVertex2f(x + 60 * scale, y + 40 * scale);
        glVertex2f(x, y + 40 * scale);
    glEnd();

    // Roof
    glColor3f(0.4f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x - 5 * scale, y + 40 * scale);
        glVertex2f(x + 65 * scale, y + 40 * scale);
        glVertex2f(x + 30 * scale, y + 65 * scale);
    glEnd();

    // Roof texture lines
    glColor3f(0.4f, 0.0f, 0.0f);
    for (int i = 0; i < 5; ++i) {
        glBegin(GL_LINES);
            glVertex2f(x + 5 * i * scale, y + 40 * scale);
            glVertex2f(x + 30 * scale, y + 65 * scale);
        glEnd();
    }

    // Plank texture on base
    glColor3f(0.3f, 0.2f, 0.1f);
    for (int i = 1; i < 6; ++i) {
        glBegin(GL_LINES);
            glVertex2f(x + i * 10 * scale, y);
            glVertex2f(x + i * 10 * scale, y + 40 * scale);
        glEnd();
    }

    // ========== Wooden House 2 ==========
    x = 120; y = 150; scale = 0.9f;

    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 60 * scale, y);
        glVertex2f(x + 60 * scale, y + 40 * scale);
        glVertex2f(x, y + 40 * scale);
    glEnd();

    glColor3f(0.4f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x - 5 * scale, y + 40 * scale);
        glVertex2f(x + 65 * scale, y + 40 * scale);
        glVertex2f(x + 30 * scale, y + 65 * scale);
    glEnd();

    glColor3f(0.4f, 0.0f, 0.0f);
    for (int i = 0; i < 5; ++i) {
        glBegin(GL_LINES);
            glVertex2f(x + 5 * i * scale, y + 40 * scale);
            glVertex2f(x + 30 * scale, y + 65 * scale);
        glEnd();
    }

    glColor3f(0.3f, 0.2f, 0.1f);
    for (int i = 1; i < 6; ++i) {
        glBegin(GL_LINES);
            glVertex2f(x + i * 10 * scale, y);
            glVertex2f(x + i * 10 * scale, y + 40 * scale);
        glEnd();
    }

 // ========== Wooden House 3 ==========
    x = 850; y = 200; scale = 1.1f;

    glColor3f(0.5f, 0.35f, 0.05f);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 60 * scale, y);
        glVertex2f(x + 60 * scale, y + 40 * scale);
        glVertex2f(x, y + 40 * scale);
    glEnd();

    glColor3f(0.4f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x - 5 * scale, y + 40 * scale);
        glVertex2f(x + 65 * scale, y + 40 * scale);
        glVertex2f(x + 30 * scale, y + 65 * scale);
    glEnd();

    glColor3f(0.4f, 0.0f, 0.0f);
    for (int i = 0; i < 5; ++i) {
        glBegin(GL_LINES);
            glVertex2f(x + 5 * i * scale, y + 40 * scale);
            glVertex2f(x + 30 * scale, y + 65 * scale);
        glEnd();
    }

    glColor3f(0.3f, 0.2f, 0.1f);
    for (int i = 1; i < 6; ++i) {
        glBegin(GL_LINES);
            glVertex2f(x + i * 10 * scale, y);
            glVertex2f(x + i * 10 * scale, y + 40 * scale);
        glEnd();
    }

}

void drawSoldier(float x, float y, float scale) {
    glColor3f(0.1f, 0.1f, 0.1f); // Soldier color

    // --- Legs ---
    glBegin(GL_POLYGON); // Left leg
        glVertex2f(x - 4 * scale, y);
        glVertex2f(x - 1 * scale, y);
        glVertex2f(x - 1 * scale, y + 15 * scale);
        glVertex2f(x - 4 * scale, y + 15 * scale);
    glEnd();
    glBegin(GL_POLYGON); // Right leg
        glVertex2f(x + 1 * scale, y);
        glVertex2f(x + 4 * scale, y);
        glVertex2f(x + 4 * scale, y + 15 * scale);
        glVertex2f(x + 1 * scale, y + 15 * scale);
    glEnd();

    // --- Body ---
    glBegin(GL_POLYGON);
        glVertex2f(x - 6 * scale, y + 15 * scale);
        glVertex2f(x + 6 * scale, y + 15 * scale);
        glVertex2f(x + 6 * scale, y + 35 * scale);
        glVertex2f(x - 6 * scale, y + 35 * scale);
    glEnd();

    // --- Arms ---
    glBegin(GL_LINES); // Left arm to shield
        glVertex2f(x - 6 * scale, y + 32 * scale);
        glVertex2f(x - 12 * scale, y + 28 * scale);
    glEnd();
    glBegin(GL_LINES); // Right arm to spear
        glVertex2f(x + 6 * scale, y + 32 * scale);
        glVertex2f(x + 9 * scale, y + 50 * scale);
    glEnd();

    // --- Head (under helmet) ---
    drawCircle(x, y + 41 * scale, 4 * scale); // Head

    // --- Helmet ---
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 180; i += 10) {
        float rad = i * PI / 180.0f;
        glVertex2f(x + cos(rad) * 6 * scale, y + 45 * scale + sin(rad) * 6 * scale);
    }
    glEnd();

    // --- Shield ---
    drawEllipse(x - 12 * scale, y + 28 * scale, 4 * scale, 9 * scale);

    // --- Spear ---
    glBegin(GL_LINES);
        glVertex2f(x + 9 * scale, y + 28 * scale);
        glVertex2f(x + 9 * scale, y + 70 * scale);
    glEnd();
    drawTriangle(x + 9 * scale, y + 70 * scale, 6 * scale);
}


void display(void)
{

    drawSky();


    drawSunrise(900.0f, sunY, 30.0f);
    drawClouds();
    drawMountains();
    drawAllBirdGroups();
    drawground();
    drawCastle();
    drawWoodenHouse();
    drawDesertTree(205, 150, 1.2f);

    drawDesertTree(800, 200, 1.5f);

    drawSimpleVeranda(370, 270, 40, 30); // Left
    drawSimpleVeranda(590, 270, 40, 30); // Right

    drawWindmill(100, 240, 1.5f); // Position and scale can be adjusted
    drawCustomFlag(485, 332, 27, 50);  // Near castle top
    drawCobblestoneRoad();
    drawSoldier(soldierX, 140, 1.0f);
    drawSoldier(rightSoldierX, 140, 1.0f);


    drawWindow(333, 300, 15, 30, 0.2f, 0.2f, 0.2f);
    drawWindow(652, 300, 15, 30, 0.2f, 0.2f, 0.2f);

    drawCart(cartX);


    glFlush();

}


void timer(int value) {
    flagTime += 0.1f;           // Increment wave offset
    glutPostRedisplay();        // Redraw the screen
    glutTimerFunc(16, timer, 0); // Call again ~60 FPS (1000ms/60 ≈ 16ms)
    windmillAngle += 2.0f; // Adjust speed as needed
    if (windmillAngle > 360.0f) windmillAngle -= 360.0f;
    if (cartMoving) {
    cartX += 2.0f;
    if (cartX > 1000) cartX = -150; // Reset to left side
        }
    cloudX += 1;
    if (cloudX > +900) cloudX = -200;

    birdGroup1X += 1.2f;
    if (birdGroup1X > 1000) birdGroup1X = -60;

    birdGroup2X += 0.8f;
    if (birdGroup2X > 1000) birdGroup2X = -100;
    wingFlapAngle += 0.2f;
    if (wingFlapAngle > 360.0f) wingFlapAngle -= 360.0f;

        // Sunrise animation
    if (sunRising && sunY < sunMaxY) {
        sunY += 0.3f;
        if (sunY >= sunMaxY) {
            sunY = sunMaxY;
            sunRising = false; // Stop rising
        }
    }



}

void updateSoldier() {
    // Left soldier movement (100-pixel range centered at 300)
    if (leftSoldierMovingRight) {
        soldierX += soldierSpeed;
        if (soldierX >= 350.0f) leftSoldierMovingRight = false;
    } else {
        soldierX -= soldierSpeed;
        if (soldierX <= 250.0f) leftSoldierMovingRight = true;
    }

    // Right soldier movement
    if (rightSoldierMovingRight) {
        rightSoldierX += rightSoldierSpeed;
        if (rightSoldierX >= 750.0f) rightSoldierMovingRight = false;
    } else {
        rightSoldierX -= rightSoldierSpeed;
        if (rightSoldierX <= 650.0f) rightSoldierMovingRight = true;
    }

    glutPostRedisplay();
}



void keyboard(unsigned char key, int x, int y) {
    if (key == '3') {
        glutPostRedisplay(); }
    else if (key == 'w' || key == 'W') {
        cartMoving = true;
    } else if (key == 'q' || key == 'Q') {
        cartMoving = false;
      }
      else {
    switch (key) {
        case '1':
            glutDestroyWindow(glutGetWindow());
            system("APP.exe");
            break;
        case '2':
            glutDestroyWindow(glutGetWindow());
            system("Egypt.exe");
            break;
        case '6':
            glutDestroyWindow(glutGetWindow());
            system("Mediaval.exe");
            break;
        case '4':
            glutDestroyWindow(glutGetWindow());
            system("Portal.exe");
            break;
    }
}


}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(1500, 800);
    glutCreateWindow("Mountain side greenish village");
    init();
    glutDisplayFunc(display);
    cout<<"      "<<endl;
    cout<<"       "<<endl;
    cout<<"==Mountain side greenish village=="<<endl;
    cout<<"Press W to move the cart"<<endl;
    cout<<"Press Q to stop the cart"<<endl;

    glutTimerFunc(0, timer, 0);
    glutIdleFunc(updateSoldier);
    glutKeyboardFunc(keyboard);
    glutMainLoop();



}
