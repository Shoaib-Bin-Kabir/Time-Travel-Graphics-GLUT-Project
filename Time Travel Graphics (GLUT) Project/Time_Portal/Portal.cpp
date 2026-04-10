#include <windows.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h>
#include <ctime>
#define PI 3.1416
#include <cstdlib>
using namespace std;


GLfloat ring1_angle = 0.0;
GLfloat ring2_angle = 0.0;
GLfloat ring3_angle = 0.0;
float cloudX = 0;
float starBrightness[30]; // for twinkling
float starX[30], starY[30];
float signBorderAlpha = 1.0f;
float humanX = 400;
float humanY = 0;
float humanAngle = 0.0f;
bool humanVisible = true;
float fadeAlpha = 0.0f;
bool startFade = false;
float humanSpeed = 0.0f;  // Default walking speed
float ufoX = 850;
float ufoY = 400;
bool ufoHovering = false;
bool ufoBeamOn = false;  // Toggle for beam
bool ufoOscillateRight = true;
bool gatewayOpened = false;
float ufoTargetX = 0.0f;  // will be set to humanX at runtime
float ufoTargetY = 410.0f;
bool abductionStarted = false;
bool humanBeingAbducted = false;
bool ufoDeparting = false;
bool ufoDriveMode = false;
bool ufoLightBlinkState = false;


void drawCircle(float radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float theta = 2.0f * PI * float(i) / 100;
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();
}


// Initialize stars once
void initStars() {
    srand(time(0));
    for (int i = 0; i < 30; i++) {
        starX[i] = rand() % 800;
        starY[i] = 400 + rand() % 200;
        starBrightness[i] = (rand() % 100) / 100.0f;
    }
}

// Sky
void drawSky() {
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.2);
    glVertex2f(0, 600);
    glVertex2f(800, 600);
    glColor3f(0.0, 0.0, 0.1);
    glVertex2f(800, 300);
    glVertex2f(0, 300);
    glEnd();
}

// Ground
void drawGround() {
    glBegin(GL_QUADS);
    glColor3f(0.1, 0.1, 0.15);
    glVertex2f(0, 300);
    glVertex2f(800, 300);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(800, 0);
    glVertex2f(0, 0);
    glEnd();
}

void drawTrees() {
    struct Tree {
        float x;
        float r, g, b;
    } trees[] = {
        {70, 0.0f, 0.5f, 0.0f},   // dark green
        {130, 0.3f, 0.8f, 0.3f},  // light green
        {190, 0.0f, 0.5f, 0.0f},
        {600, 0.3f, 0.8f, 0.3f},
        {660, 0.0f, 0.5f, 0.0f},
        {720, 0.3f, 0.8f, 0.3f},
        {780, 0.0f, 0.5f, 0.0f}
    };

    for (int i = 0; i < 7; i++) {
        float x = trees[i].x;
        float r = trees[i].r, g = trees[i].g, b = trees[i].b;

        // Trunk
        glColor3f(0.4f, 0.2f, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(x - 2, 300);
        glVertex2f(x + 2, 300);
        glVertex2f(x + 2, 315);
        glVertex2f(x - 2, 315);
        glEnd();

        // Leaves - 3 stacked triangles
        glColor3f(r, g, b);
        glBegin(GL_TRIANGLES);
        glVertex2f(x, 335);
        glVertex2f(x - 15, 315);
        glVertex2f(x + 15, 315);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(x, 345);
        glVertex2f(x - 12, 325);
        glVertex2f(x + 12, 325);
        glEnd();

        glBegin(GL_TRIANGLES);
        glVertex2f(x, 355);
        glVertex2f(x - 9, 335);
        glVertex2f(x + 9, 335);
        glEnd();
    }
}

void drawBush(float x, float y) {
    glColor3f(0.0f, 0.5f, 0.0f);  // dark green

    // Middle blade (vertical)
    glBegin(GL_QUADS);
    glVertex2f(x - 2, y);
    glVertex2f(x + 2, y);
    glVertex2f(x + 1, y + 20);
    glVertex2f(x - 1, y + 20);
    glEnd();

    // Left blade (leans left)
    glBegin(GL_QUADS);
    glVertex2f(x - 4, y);
    glVertex2f(x - 1, y);
    glVertex2f(x - 5, y + 16);
    glVertex2f(x - 8, y + 16);
    glEnd();

    // Right blade (leans right)
    glBegin(GL_QUADS);
    glVertex2f(x + 1, y);
    glVertex2f(x + 4, y);
    glVertex2f(x + 10, y + 14);
    glVertex2f(x + 7, y + 14);
    glEnd();
}




void drawBushes() {
    // Left side
    drawBush(60, 100);
    drawBush(100, 150);
    drawBush(140, 100);
    drawBush(180, 120);
    drawBush(220, 140);
    drawBush(260, 123);
    drawBush(300, 145);

    int off=80;
    drawBush(60,  100+off);
    drawBush(100, 150+off);
    drawBush(140, 100+off);
    drawBush(180, 120+off);
    drawBush(220, 140+off);
    drawBush(260, 123+off);
    drawBush(300, 145+off);
    drawBush(300, 165+off);
    drawBush(250, 165+off);
    drawBush(250, 80);
    drawBush(130, 57);
    drawBush(60, 72);


    // Right side
    drawBush(540, 100);
    drawBush(580, 150);
    drawBush(620, 100);
    drawBush(660, 120);
    drawBush(700, 140);
    drawBush(740, 123);
    drawBush(780, 145);

    drawBush(540, 100+off);
    drawBush(580, 150+off);
    drawBush(620, 100+off);
    drawBush(660, 120+off);
    drawBush(700, 140+off);
    drawBush(740, 123+off);
    drawBush(780, 145+off);
    drawBush(700, 80);
    drawBush(540, 77);
    drawBush(480, 65);


}



// Road
void drawRoad() {
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_POLYGON);
    glVertex2f(280, 0);
    glVertex2f(520, 0);
    glVertex2f(440, 300);
    glVertex2f(360, 300);
    glEnd();
}

void drawSignBoard() {
    float signX = 640, signY = 180;           // X remains constant, Y controls full sign position
    float width = 150, height = 35;
    float poleHeight = 85;                    // fixed height for the post

    // Sign post (starts from signY - poleHeight to signY)
    glColor3f(0.3, 0.3, 0.3); // dark grey
    glBegin(GL_QUADS);
    glVertex2f(signX + width / 2 - 2, signY - poleHeight);
    glVertex2f(signX + width / 2 + 2, signY - poleHeight);
    glVertex2f(signX + width / 2 + 2, signY);
    glVertex2f(signX + width / 2 - 2, signY);
    glEnd();

    // Signboard panel (sits on top of post)
    glColor3f(0.0, 0.0, 0.0); // black
    glBegin(GL_QUADS);
    glVertex2f(signX, signY);
    glVertex2f(signX + width, signY);
    glVertex2f(signX + width, signY + height);
    glVertex2f(signX, signY + height);
    glEnd();

    // Twinkling red border using alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0, 0.0, 0.0, signBorderAlpha); // twinkling red
    glLineWidth(3.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(signX, signY);
    glVertex2f(signX + width, signY);
    glVertex2f(signX + width, signY + height);
    glVertex2f(signX, signY + height);
    glEnd();
    glDisable(GL_BLEND);


    // Warning text
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2f(signX + 6, signY + 20);
    const char* line1 = "Warning! Don't enter.";
    for (int i = 0; line1[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line1[i]);
    }

    glRasterPos2f(signX + 6, signY + 8);
    const char* line2 = "You'll be lost in time";
    for (int i = 0; line2[i] != '\0'; i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, line2[i]);
    }
}



// Ring Drawing
void drawfRing(GLfloat outerR, GLfloat innerR, int segments, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        float angle = PI * i / (float)segments;
        float x_outer = cos(angle) * outerR;
        float y_outer = sin(angle) * outerR;
        float x_inner = cos(angle) * innerR;
        float y_inner = sin(angle) * innerR;
        glVertex2f(x_outer, y_outer);
        glVertex2f(x_inner, y_inner);
    }
    glEnd();
}

// Portal

void drawPortal() {
    glPushMatrix();
    glTranslatef(400, 350, 0); // Portal center

    glPushMatrix();
    glRotatef(ring1_angle, 0, 0, 1);
    drawfRing(70, 60, 50, 0.0, 0.5, 1.0); // Was 100–90
    glPopMatrix();

    glPushMatrix();
    glRotatef(-ring2_angle, 0, 0, 1);
    drawfRing(58, 50, 50, 1.0, 0.0, 1.0); // Was 85–75
    glPopMatrix();

    glPushMatrix();
    glRotatef(ring3_angle, 0, 0, 1);
    drawfRing(45, 38, 50, 1.0, 0.5, 0.0); // Was 70–60
    glPopMatrix();

    glPopMatrix();
}

// Moon (static)
void drawMoon() {
    glColor3f(1.0, 1.0, 0.8);
    float cx = 80, cy = 520, radius = 30;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++) {
        float angle = 2 * PI * i / 100;
        glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
    }
    glEnd();
}

//  Static Stars (twinkle with varying alpha)
void drawStars() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPointSize(4.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < 30; i++) {
        glColor4f(1.0, 1.0, 1.0, starBrightness[i]);
        glVertex2f(starX[i], starY[i]);
    }
    glEnd();
    glDisable(GL_BLEND);
}


void drawClouds() {
    // === Background + Foreground Cloud Pair 1 ===
    glColor3f(0.85, 0.85, 0.85);
    float baseX1 = cloudX * 0.6 + 100, baseY1 = 560;
    glPushMatrix(); glTranslatef(baseX1, baseY1, 0); drawCircle(15); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX1 - 12, baseY1 - 4, 0); drawCircle(10); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX1 + 12, baseY1 - 4, 0); drawCircle(10); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX1, baseY1 - 8, 0); drawCircle(8); glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);
    float fgX1 = baseX1 + 25, fgY1 = baseY1 - 10;
    glPushMatrix(); glTranslatef(fgX1, fgY1, 0); drawCircle(18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1 - 15, fgY1 - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1 + 15, fgY1 - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1, fgY1 - 10, 0); drawCircle(10); glPopMatrix();

    // === Extra Foreground Cloud Below Pair 1 ===
    float fgX1b = baseX1 - 20, fgY1b = baseY1 - 50;
    glPushMatrix(); glTranslatef(fgX1b, fgY1b, 0); drawCircle(18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1b - 15, fgY1b - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1b + 15, fgY1b - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX1b, fgY1b - 10, 0); drawCircle(10); glPopMatrix();

    // === Background + Foreground Cloud Pair 2 ===
    glColor3f(0.85, 0.85, 0.85);
    float baseX2 = cloudX * 0.6 + 350, baseY2 = 530;
    glPushMatrix(); glTranslatef(baseX2, baseY2, 0); drawCircle(15); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX2 - 12, baseY2 - 4, 0); drawCircle(10); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX2 + 12, baseY2 - 4, 0); drawCircle(10); glPopMatrix();
    glPushMatrix(); glTranslatef(baseX2, baseY2 - 8, 0); drawCircle(8); glPopMatrix();

    glColor3f(1.0, 1.0, 1.0);
    float fgX2 = baseX2 - 25, fgY2 = baseY2 + 10;
    glPushMatrix(); glTranslatef(fgX2, fgY2, 0); drawCircle(18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2 - 15, fgY2 - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2 + 15, fgY2 - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2, fgY2 - 10, 0); drawCircle(10); glPopMatrix();

    // === Extra Foreground Cloud Below Pair 2 ===
    float fgX2b = baseX2 + 20, fgY2b = baseY2 - 50;
    glPushMatrix(); glTranslatef(fgX2b, fgY2b, 0); drawCircle(18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2b - 15, fgY2b - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2b + 15, fgY2b - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX2b, fgY2b - 10, 0); drawCircle(10); glPopMatrix();

    // === Foreground-Only Cloud 1 ===
    float fgX3 = cloudX + 550, fgY3 = 510;
    glPushMatrix(); glTranslatef(fgX3, fgY3, 0); drawCircle(18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX3 - 15, fgY3 - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX3 + 15, fgY3 - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX3, fgY3 - 10, 0); drawCircle(10); glPopMatrix();

    // === Foreground-Only Cloud 2 ===
    float fgX4 = cloudX + 700, fgY4 = 540;
    glPushMatrix(); glTranslatef(fgX4, fgY4, 0); drawCircle(18); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX4 - 15, fgY4 - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX4 + 15, fgY4 - 5, 0); drawCircle(12); glPopMatrix();
    glPushMatrix(); glTranslatef(fgX4, fgY4 - 10, 0); drawCircle(10); glPopMatrix();
}


void drawAbandonedBuilding(float x, float y, float width, float height, float r, float g, float b) {
    // Main building
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // Chimney
    glBegin(GL_QUADS);
    glColor3f(r * 0.7f, g * 0.7f, b * 0.7f);  // slightly darker
    glVertex2f(x + width * 0.8f, y + height);
    glVertex2f(x + width * 0.83f, y + height);
    glVertex2f(x + width * 0.83f, y + height + 10);
    glVertex2f(x + width * 0.8f, y + height + 10);
    glEnd();

    // Door
    glColor3f(0.1f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(x + width * 0.4f, y);
    glVertex2f(x + width * 0.6f, y);
    glVertex2f(x + width * 0.6f, y + height * 0.45f);
    glVertex2f(x + width * 0.4f, y + height * 0.45f);
    glEnd();

    // Windows (same size and height)
    float winY1 = y + height * 0.6f;
    float winY2 = y + height * 0.75f;
    float winW = width * 0.08f;  // window width

    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    // Left window
    glVertex2f(x + width * 0.1f, winY1);
    glVertex2f(x + width * 0.1f + winW, winY1);
    glVertex2f(x + width * 0.1f + winW, winY2);
    glVertex2f(x + width * 0.1f, winY2);

    // Middle window
    glVertex2f(x + width * 0.46f, winY1);
    glVertex2f(x + width * 0.46f + winW, winY1);
    glVertex2f(x + width * 0.46f + winW, winY2);
    glVertex2f(x + width * 0.46f, winY2);

    // Right window
    glVertex2f(x + width * 0.75f, winY1);
    glVertex2f(x + width * 0.75f + winW, winY1);
    glVertex2f(x + width * 0.75f + winW, winY2);
    glVertex2f(x + width * 0.75f, winY2);
    glEnd();
}


void drawHuman() {
    if (!humanVisible) return;

    glPushMatrix();

    glTranslatef(humanX, humanY, 0);  // ← use actual humanX!


    if (humanY >= 300)             // only rotate once inside inner portal ring
        glRotatef(humanAngle, 0, 0, 1);

    glTranslatef(0, 80, 0);        // feet to ground

    // Head
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(0, 34, 0); // move head upwards
    drawCircle(8);
    glPopMatrix();

    // Body (torso)
    glColor3f(0.2f, 0.2f, 0.8f); // blue shirt
    glBegin(GL_QUADS);
    glVertex2f(-6, -8);
    glVertex2f(6, -8);
    glVertex2f(6, 24);
    glVertex2f(-6, 24);
    glEnd();

      // Arms
    glColor3f(0.9f, 0.7f, 0.6f);
    glBegin(GL_LINES);
    // Left arm
    glVertex2f(-6, 18); glVertex2f(-12, 6);
    // Right arm
    glVertex2f(6, 18);  glVertex2f(12, 6);
    glEnd();


    // Legs
    glColor3f(0.9f, 0.7f, 0.6f); // dark pants
    glBegin(GL_LINES);
    glVertex2f(-4, -8); glVertex2f(-10, -24);
    glVertex2f(4, -8);  glVertex2f(10, -24);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - fadeAlpha);

    glDisable(GL_BLEND);
    glPopMatrix();
}

void drawUFO(float x, float y) {
    // DRAW BEAM FIRST (BEHIND HUMAN)
    if (ufoBeamOn) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 1.0f, 0.7f, 0.6f);  // semi-transparent yellow
        glBegin(GL_TRIANGLES);
        glVertex2f(x, y - 10);
        glVertex2f(x - 30, y - 130);
        glVertex2f(x + 30, y - 130);
        glEnd();
        glDisable(GL_BLEND);
    }

    // DRAW UFO BODY ABOVE
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++) {
        float theta = 2 * PI * i / 100;
        glVertex2f(x + cos(theta) * 40, y + sin(theta) * 10);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 50; i++) {
        float theta = PI * i / 50;
        glVertex2f(x + cos(theta) * 20, y + 10 + sin(theta) * 20);
    }
    glEnd();
}



void drawFadeOverlay() {
    if (fadeAlpha > 0.0f) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, fadeAlpha);

        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(800, 0);
        glVertex2f(800, 600);
        glVertex2f(0, 600);
        glEnd();

        glDisable(GL_BLEND);
    }
}



// Display
void portalDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawSky();
    drawStars();
    drawMoon();
    drawClouds();
    drawPortal();
    drawGround();
    drawBushes();
    drawTrees();
    drawRoad();
    drawSignBoard();
    drawAbandonedBuilding(170, 180, 80, 75, 0.25f, 0.4f, 0.25f);
    drawAbandonedBuilding(35, 150, 160, 120, 0.5f, 0.2f, 0.3f);
    drawAbandonedBuilding(660, 300, 55, 35, 0.4f, 0.3f, 0.2f);
    drawHuman();
    drawUFO(ufoX, ufoY);
    drawFadeOverlay();
    glutSwapBuffers();
}


void update(int value) {
    ring1_angle += 5.0;
    ring2_angle += 7.0;
    ring3_angle += 10.0;

    cloudX += 1;
    if (cloudX > 900) cloudX = -200;

    // Human walking upward (normal movement)
    if (humanVisible) {
        humanY += humanSpeed;

        if (humanY >= 290 && humanY <= 315)
            humanAngle += 0.9f;

        if (humanY > 315) {
            humanVisible = false;
            startFade = true;
        }
    }

    // Portal fade
    if (startFade && fadeAlpha < 1.0f) {
        fadeAlpha += 0.01f;
        if (fadeAlpha > 1.0f) {
            fadeAlpha = 1.0f;



        }
    }

    // Star twinkle
    for (int i = 0; i < 30; i++)
        starBrightness[i] = 0.5f + (rand() % 50) / 100.0f;
    signBorderAlpha = 0.5f + (rand() % 50) / 100.0f;

    // === UFO Logic ===
    if (abductionStarted && !ufoDeparting) {
        // Step 1: move vertically until beam bottom aligns with human feet
        if (fabs(ufoY - ufoTargetY) > 1.0f) {
            ufoY += (ufoY < ufoTargetY) ? 1.0f : -1.0f;
        }
        // Step 2: then move horizontally to align with human X
        else if (fabs(ufoX - ufoTargetX) > 1.0f) {
            ufoX += (ufoX < ufoTargetX) ? 1.5f : -1.5f;
        }
        // Step 3: abduct (make human disappear instantly)
        else {
            humanVisible = false;
            ufoDeparting = true;
        }
    }

    // Step 4: UFO departs to right
    if (ufoDeparting) {
        ufoX += 2.0f;
        if (ufoX > 900) {
            ufoBeamOn = false;
            ufoDeparting = false;
            abductionStarted = false;
        }
    }

    // Step 5: Idle UFO approach + hover if nothing triggered yet
    if (!abductionStarted && !ufoDeparting) {
        if (!ufoHovering) {
            ufoX -= 1.0f;
            if (ufoX <= 150) {
                ufoX = 150;
                ufoHovering = true;
            }
        } else {
            if (ufoOscillateRight) {
                ufoX += 0.5f;
                if (ufoX >= 160) ufoOscillateRight = false;
            } else {
                ufoX -= 0.5f;
                if (ufoX <= 140) ufoOscillateRight = true;
            }
        }
    }

    // Blink UFO beam during drive mode
    if (ufoDriveMode) {
        static int blinkCounter = 0;
        blinkCounter++;
        if (blinkCounter > 15) {
            ufoLightBlinkState = !ufoLightBlinkState;
            ufoBeamOn = ufoLightBlinkState;
            blinkCounter = 0;
        }
    }


    // Trigger next frame
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}



void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'q': // Stop
        case 'Q':
            humanSpeed = 0.0f;
            break;
        case 'w': // Normal walk
        case 'W':
            humanSpeed = 0.5f;
            break;
        case 'e': // Fast walk
        case 'E':
            humanSpeed += 1.0f;
            break;
        case 'r': // Slow walk
        case 'R':
            humanSpeed -= 0.2f;
            break;

        case 'T':
        case 't':
            ufoBeamOn = !ufoBeamOn;  // toggle light beam
            break;
        case 'y':
        case 'Y':
        if (!abductionStarted && ufoHovering && humanVisible) {
            ufoTargetX = humanX;       // go to human's X
            ufoTargetY = humanY + 180;  // align beam with human feet (not UFO body)
            abductionStarted = true;
            ufoBeamOn = true;
            ufoHovering = false;
            break;
        }

        case 'o':
        case 'O':
            if (ufoHovering) {
                ufoDriveMode = true;
                ufoBeamOn = true;  // Start with light on
            }
            break;
        case 'p':
        case 'P':
            ufoDriveMode = false;
            ufoBeamOn = false;
            ufoX = 150; // Reset to hover X
            ufoHovering = true;
            break;

        case 'j':
        case 'J':
            if (ufoDriveMode) ufoX -= 5.0f;
            break;
        case 'k':
        case 'K':
            if (ufoDriveMode) ufoX += 5.0f;
            break;
        case 'i':
        case 'I':
            if (ufoDriveMode) ufoY += 5.0f;
            break;
        case 'n':
        case 'N':
            if (ufoDriveMode) ufoY -= 5.0f;
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




        case 27:
            exit(0);
            break;
    }
}



// Init
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
    initStars();
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(1300,850);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Time Portal");


    cout << " \n";
    cout << "\n";
    cout << "==Time Portal==\n";
    cout << "Press Q to stop human\n";
    cout << "Press W to move Again\n";
    cout << "Press E to move Human faster\n";
    cout << "Press R to move Human slower\n";
    cout << "Press T to turn on/off UFO light\n";
    cout << "Press Y to Abduct Human\n";
    cout << "Press O to turn on UFO drive mode\n";
    cout << "Press I to go up\n";
    cout << "Press N to go down\n";
    cout << "Press J to go left\n";
    cout << "Press K to go right\n";
    cout << "Press P to turn off drive mode\n";
    cout << " \n";
    cout << "\n";
    cout << "Press 1 for Traveling Post Appocalyptic Era\n";
    cout << "Press 2 for Traveling Egyptian Era\n";
    cout << "Press 3 for Traveling Medieval Era\n";
    cout << "Press 4 for Traveling Time Portal\n";



    init();
    glutDisplayFunc(portalDisplay);
    glutTimerFunc(0, update, 0);
    glutKeyboardFunc(handleKeypress);
    glutMainLoop();
    return 0;
}
