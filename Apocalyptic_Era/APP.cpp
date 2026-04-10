#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <cstdlib>  // for rand()
#include <ctime>    // for seeding
#include <cmath>
#define PI 3.1416
#include <vector>
#include<algorithm>
using namespace std;

int temp[] = {5, 12, 27, 35, 46, 53, 67};
vector<int> flickerWindows(temp, temp + 7);
float tankX = 500;
float tankY = 60;
float tankSpeed = 2.0f;

float heliX = -100;  // start offscreen to the left
float heliY = 500;   // helicopter Y position in sky
float leftBuildingHeights[70];
float rightBuildingHeights[50];
float rotorAngle = 0.0f;
bool flickerState = true;
int flickerCounter = 0;
float carX = 50;           // initial X position
float carY = 314;          // lower deck Y position
float carAngle = 0.0f;     // rotation angle
float carSpeed = 0.0f;
bool carFalling = false;
bool carVisible = true;
bool showLightning = false;
int lightningTimer = 0;
const int SNOW_COUNT = 200;
float snowX[SNOW_COUNT];
float snowY[SNOW_COUNT];
float snowSpeed[SNOW_COUNT];
bool bulletActive = false;
float bulletX = 0;
float bulletY = 0;
float barrelAngle = 0.0f;  // 0 to 180 degrees

// === UTIL ===
void drawRect(float x, float y, float w, float h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
}

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

// === SKY ===
void drawSky() {
    if (showLightning) {
        glColor3f(0.9f, 0.9f, 1.0f);  // bright flash
        glBegin(GL_QUADS);
        glVertex2f(0, 600);
        glVertex2f(800, 600);
        glVertex2f(800, 300);
        glVertex2f(0, 300);
        glEnd();

        // Optional: draw a lightning bolt shape
        glColor3f(1.0f, 1.0f, 1.0f);  // pure white bolt
        glBegin(GL_TRIANGLES);
        glVertex2f(500, 580);
        glVertex2f(490, 530);
        glVertex2f(510, 530);
        glVertex2f(495, 530);
        glVertex2f(505, 480);
        glVertex2f(485, 480);
        glEnd();
    } else {
        // Normal sky
        glBegin(GL_QUADS);
        glColor3f(0.35f, 0.45f, 0.55f);
        glVertex2f(0, 600);
        glVertex2f(800, 600);
        glColor3f(0.2f, 0.25f, 0.3f);
        glVertex2f(800, 300);
        glVertex2f(0, 300);
        glEnd();
    }
}


// === GROUND + RIVER ===
void drawGroundAndRiver() {
    int segments = 100;
    float waveAmplitude = 8.0f;
    float waveFreq = 2 * PI / segments;

    // === LEFT GRASS ===
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float y = i * (300.0f / segments);
        float xOffset = waveAmplitude * sin(waveFreq * i);
        glColor3f(0.1f, 0.4f, 0.1f);
        glVertex2f(0, y);
        glVertex2f(350 + xOffset, y);
    }
    glEnd();

    // === RIVER ===
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float y = i * (300.0f / segments);
        float leftX = 350 + waveAmplitude * sin(waveFreq * i);
        float rightX = 450 + waveAmplitude * cos(waveFreq * i);
        glColor3f(0.1f, 0.2f, 0.5f);
        glVertex2f(leftX, y);
        glVertex2f(rightX, y);
    }
    glEnd();

    // === RIGHT GRASS ===
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float y = i * (300.0f / segments);
        float xOffset = waveAmplitude * cos(waveFreq * i);
        glColor3f(0.1f, 0.4f, 0.1f);
        glVertex2f(450 + xOffset, y);
        glVertex2f(800, y);
    }
    glEnd();
}


// === BUILDINGS (STATIC) ===
void initBuildings() {
    for (int i = 0; i < 12; ++i)
        leftBuildingHeights[i] = 95 + rand() % 80;
    for (int i = 0; i < 6; ++i)
        rightBuildingHeights[i] = 95 + rand() % 80;
}

void drawShortBuildings() {
    int windowIndex = 0;  // Global counter for window number
    float x = 80;

    // === LEFT SIDE BUILDINGS ===
    for (int i = 0; i < 12; ++i) {
        float bHeight = leftBuildingHeights[i];
        float bWidth = 25;
        drawRect(x, 300, bWidth, bHeight, 0.15f, 0.15f, 0.15f);

        // Windows
        float wx = x + 4;
        float wy = 360;
        float wSize = 5;
        float gap = 15;
        int rowCount = 0;

        while (wy + wSize + gap < 300 + bHeight - 10 && rowCount < 3) {
            for (int col = 0; col < 2; col++) {
                float xOffset = col * (wSize + 6);

                if (std::find(flickerWindows.begin(), flickerWindows.end(), windowIndex) != flickerWindows.end()) {
                    if (flickerState)
                        glColor3f(1.0f, 1.0f, 0.4f);  // yellow ON
                    else
                        glColor3f(0.4f, 0.4f, 0.4f);  // OFF (grey)
                } else {
                    glColor3f(0.4f, 0.4f, 0.4f);      // always grey
                }


                glBegin(GL_QUADS);
                    glVertex2f(wx + xOffset, wy);
                    glVertex2f(wx + xOffset + wSize, wy);
                    glVertex2f(wx + xOffset + wSize, wy + wSize);
                    glVertex2f(wx + xOffset, wy + wSize);
                glEnd();

                windowIndex++;
            }
            wy += wSize + gap;
            rowCount++;
        }

        x += 60;
    }

    // === RIGHT SIDE BUILDINGS ===
    x = 550;
    for (int i = 0; i < 6; ++i) {
        float bHeight = rightBuildingHeights[i];
        float bWidth = 25;
        drawRect(x, 300, bWidth, bHeight, 0.15f, 0.15f, 0.15f);

        float wx = x + 4;
        float wy = 360;
        float wSize = 5;
        float gap = 15;
        int rowCount = 0;

        while (wy + wSize + gap < 300 + bHeight - 10 && rowCount < 3) {
            for (int col = 0; col < 2; col++) {
                float xOffset = col * (wSize + 6);

                if (std::find(flickerWindows.begin(), flickerWindows.end(), windowIndex) != flickerWindows.end()) {
                    if (flickerState)
                        glColor3f(1.0f, 1.0f, 0.4f);  // yellow ON
                    else
                        glColor3f(0.4f, 0.4f, 0.4f);  // OFF (grey)
                } else {
                    glColor3f(0.4f, 0.4f, 0.4f);      // always grey
                }


                glBegin(GL_QUADS);
                    glVertex2f(wx + xOffset, wy);
                    glVertex2f(wx + xOffset + wSize, wy);
                    glVertex2f(wx + xOffset + wSize, wy + wSize);
                    glVertex2f(wx + xOffset, wy + wSize);
                glEnd();

                windowIndex++;
            }
            wy += wSize + gap;
            rowCount++;
        }

        x += 60;
    }
}


// === BRIDGE ===
void drawBridge() {
    const float deckR = 0.55f, deckG = 0.5f, deckB = 0.45f;
    const float pillarR = 0.3f, pillarG = 0.3f, pillarB = 0.3f;

    float deckTopY = 390;
    float deckBottomY = 310;
    float pillarBaseY = 190;
    float pillarWidth = 20;

    float baseHeight = 8.0f;
    float baseOffset = 4.0f;

    float leftPillars[] = {80, 180, 280};
    float rightPillars[] = {520, 620, 720};

    // Pillars and Bases
    for (int i = 0; i < 3; i++) {
        // Left base
        drawRect(leftPillars[i] - baseOffset, pillarBaseY - baseHeight,
                 pillarWidth + 2 * baseOffset, baseHeight,
                 pillarR * 0.8f, pillarG * 0.8f, pillarB * 0.8f);
        // Left pillar
        drawRect(leftPillars[i], pillarBaseY, pillarWidth,
                 deckBottomY - pillarBaseY,
                 pillarR, pillarG, pillarB);

        // Right base
        drawRect(rightPillars[i] - baseOffset, pillarBaseY - baseHeight,
                 pillarWidth + 2 * baseOffset, baseHeight,
                 pillarR * 0.8f, pillarG * 0.8f, pillarB * 0.8f);
        // Right pillar
        drawRect(rightPillars[i], pillarBaseY, pillarWidth,
                 deckBottomY - pillarBaseY,
                 pillarR, pillarG, pillarB);
    }

    float gapStart = 355, gapEnd = 510;

    // Decks
    glColor3f(deckR, deckG, deckB);
    glBegin(GL_QUADS); glVertex2f(0, deckTopY); glVertex2f(gapStart, deckTopY);
                       glVertex2f(gapStart, deckTopY - 6); glVertex2f(0, deckTopY - 6); glEnd();
    glBegin(GL_QUADS); glVertex2f(0, deckBottomY); glVertex2f(gapStart, deckBottomY);
                       glVertex2f(gapStart, deckBottomY - 6); glVertex2f(0, deckBottomY - 6); glEnd();

    glBegin(GL_QUADS); glVertex2f(gapEnd, deckTopY); glVertex2f(800, deckTopY);
                       glVertex2f(800, deckTopY - 6); glVertex2f(gapEnd, deckTopY - 6); glEnd();
    glBegin(GL_QUADS); glVertex2f(gapEnd, deckBottomY); glVertex2f(800, deckBottomY);
                       glVertex2f(800, deckBottomY - 6); glVertex2f(gapEnd, deckBottomY - 6); glEnd();

    // Trusses
    glLineWidth(4.0f);
    glColor3f(deckR, deckG, deckB);
    float yTop = deckTopY - 6, yBot = deckBottomY;

    glBegin(GL_LINES);
        // Left X pairs
        glVertex2f(85, yBot); glVertex2f(160, yTop);
        glVertex2f(85, yTop); glVertex2f(160, yBot);
        glVertex2f(185, yBot); glVertex2f(260, yTop);
        glVertex2f(185, yTop); glVertex2f(260, yBot);

        // Right X pairs
        glVertex2f(525, yBot); glVertex2f(600, yTop);
        glVertex2f(525, yTop); glVertex2f(600, yBot);
        glVertex2f(625, yBot); glVertex2f(700, yTop);
        glVertex2f(625, yTop); glVertex2f(700, yBot);
    glEnd();

    glLineWidth(1.0f);
}

// === HELICOPTER ===

void drawHelicopter(float x, float y) {
    // === Body (scaled up slightly) ===
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_POLYGON);
        glVertex2f(x, y);
        glVertex2f(x + 32, y);
        glVertex2f(x + 36, y - 8);
        glVertex2f(x - 4, y - 8);
    glEnd();

    // === Tail Boom (scaled) ===
    glBegin(GL_QUADS);
        glVertex2f(x - 25, y - 5);
        glVertex2f(x - 25, y - 2);
        glVertex2f(x, y - 2);
        glVertex2f(x, y - 5);
    glEnd();

    // === Skids (scaled)
    glBegin(GL_LINES);
        glVertex2f(x + 5, y - 13); glVertex2f(x + 30, y - 13);  // main bar
        glVertex2f(x + 10, y - 13); glVertex2f(x + 10, y - 9);  // left leg
        glVertex2f(x + 25, y - 13); glVertex2f(x + 25, y - 9);  // right leg
    glEnd();

       // === Rotors (rotating X shape) ===
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(x + 12, y + 6, 0);  // center of rotation (pivot)
    glRotatef(rotorAngle, 0, 0, 1);  // rotate around Z axis

    glBegin(GL_LINES);
        glVertex2f(-18, 0); glVertex2f(18, 0);   // horizontal blade
        glVertex2f(0, -18); glVertex2f(0, 18);   // vertical blade
    glEnd();

    glPopMatrix();

}

void drawCar(float x, float y, float angle) {
    if (!carVisible) return;

    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle, 0, 0, 1);

    // Car body
    glColor3f(0.7f, 0.1f, 0.1f);  // red
    glBegin(GL_QUADS);
        glVertex2f(-20, 0);
        glVertex2f(20, 0);
        glVertex2f(20, 10);
        glVertex2f(-20, 10);
    glEnd();

    // Roof
    glColor3f(0.5f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-12, 10);
        glVertex2f(12, 10);
        glVertex2f(8, 18);
        glVertex2f(-8, 18);
    glEnd();

    // Wheels
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(-15, -3);
        glVertex2f(-9, -3);
        glVertex2f(-9, 0);
        glVertex2f(-15, 0);

        glVertex2f(9, -3);
        glVertex2f(15, -3);
        glVertex2f(15, 0);
        glVertex2f(9, 0);
        glEnd();

    glPopMatrix();
}
void drawBuilding(float x, float y) {
    // Main structure
    drawRect(x - 50, y - 60, 150, 100, 0.25f, 0.25f, 0.25f);

    // Door
    drawRect(x + 20, y-60, 20, 30, 0.1f, 0.1f, 0.1f);

    // Flat roof with slight overhang to the right
    glColor3f(0.3f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
        glVertex2f(x - 75, y + 40);    // left end
        glVertex2f(x + 125, y + 40);   // right end (overhangs past wall)
        glVertex2f(x + 125, y + 55);   // roof height
        glVertex2f(x - 75, y + 55);
    glEnd();
}



void drawOilBarrel(float x, float y) {
    // Barrel
    drawRect(x, y, 10, 20, 0.3f, 0.0f, 0.0f);

    // X mark
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x + 10, y + 20);
        glVertex2f(x + 10, y);
        glVertex2f(x, y + 20);
    glEnd();
}

void drawSignboard(float x, float y) {
    // Sign pole
    drawRect(x + 18, y - 30, 4, 30, 0.4f, 0.4f, 0.4f);

    // Signboard
    drawRect(x, y, 80, 30, 0.8f, 0.2f, 0.2f);

    // Text
    glColor3f(1.0f, 1.0f, 0.0f);
    glRasterPos2f(x + 3, y + 18);
    const char* msg = "Warning!";
    for (const char* c = msg; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);

    glRasterPos2f(x + 3, y + 10);
    const char* msg2 = "Radioactivity";
    for (const char* c = msg2; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);
}


void drawDeadBody(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    glRotatef(-90, 0, 0, 1);  // rotate clockwise to lie flat

    // Move figure origin to “feet on ground”
    glTranslatef(0, 80, 0);

    // === Head (black)
    glColor3f(0.0f, 0.0f, 0.0f);
    glPushMatrix();
        glTranslatef(0, 34, 0);
        drawCircle(8);
    glPopMatrix();

    // === Torso (purple)
    glColor3f(0.4f, 0.0f, 0.0f); // purple shirt
    glBegin(GL_QUADS);
        glVertex2f(-6, -8);
        glVertex2f(6, -8);
        glVertex2f(6, 24);
        glVertex2f(-6, 24);
    glEnd();

    // === Arms
    glColor3f(0.9f, 0.7f, 0.6f); // same skin color
    glBegin(GL_LINES);
        glVertex2f(-6, 18); glVertex2f(-12, 6);   // left arm
        glVertex2f(6, 18);  glVertex2f(12, 6);    // right arm
    glEnd();

    // === Legs
    glBegin(GL_LINES);
        glVertex2f(-4, -8); glVertex2f(-10, -24); // left leg
        glVertex2f(4, -8);  glVertex2f(10, -24);  // right leg
    glEnd();

    glPopMatrix();
}

void drawSnow() {
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < SNOW_COUNT; i++) {
        glBegin(GL_QUADS);
        float size = 2.0f;
        glVertex2f(snowX[i], snowY[i]);
        glVertex2f(snowX[i] + size, snowY[i]);
        glVertex2f(snowX[i] + size, snowY[i] + size);
        glVertex2f(snowX[i], snowY[i] + size);
        glEnd();
    }
}

void drawTank(float x, float y) {
    // Tank Body
    glColor3f(0.3f, 0.2f, 0.0f);  // army green
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 100, y);
        glVertex2f(x + 100, y + 30);
        glVertex2f(x, y + 30);
    glEnd();

    // Tank Turret
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
        glVertex2f(x + 35, y + 30);
        glVertex2f(x + 65, y + 30);
        glVertex2f(x + 65, y + 50);
        glVertex2f(x + 35, y + 50);
    glEnd();

    // Tank Barrel (rotating with angle)
    glLineWidth(8.0f);
    glColor3f(0.0f, 0.0f, 0.0f);

    float barrelLength = 50.0f;
    float angleRad = barrelAngle * PI / 180.0f;
    float bx = x + 50;       // base
    float by = y + 50;
    float ex = bx + cos(angleRad) * barrelLength;
    float ey = by + sin(angleRad) * barrelLength;

    glBegin(GL_LINES);
        glVertex2f(bx, by);
        glVertex2f(ex, ey);
    glEnd();
    glLineWidth(1.0f);



    // Wheels
    glColor3f(0.1f, 0.1f, 0.1f);
    for (int i = 0; i < 5; ++i) {
        float cx = x + 10 + i * 20;
        float cy = y;
        float radius = 6.0f;
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f(cx, cy);
            for (int j = 0; j <= 20; ++j) {
                float angle = 2 * PI * j / 20;
                glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
            }
        glEnd();
    }
}

void drawBullet() {
    if (!bulletActive) return;

    glLineWidth(6.0f);
    float angleRad = barrelAngle * PI / 180.0f;
    glColor3f(1.0f, 1.0f, 0.0f);  // yellow bullet
    glBegin(GL_LINES);
        glVertex2f(bulletX, bulletY);
        glVertex2f(bulletX + cos(angleRad) * 10, bulletY + sin(angleRad) * 10);
    glEnd();

}


// === DISPLAY ===
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawSky();
    drawGroundAndRiver();
    drawShortBuildings();
    drawBridge();
    drawHelicopter(heliX, heliY);
    drawCar(carX, carY, carAngle);
    drawCar(carX, carY, carAngle);
    drawBuilding(120, 90);  // Example coordinates
    drawOilBarrel(240, 40);
    drawOilBarrel(260, 40);
    drawOilBarrel(280, 40);
    drawSignboard(280, 70);
    drawDeadBody(200, 20);  // adjust coordinates to place near bridge or road
    drawSnow();
    drawTank(tankX, tankY);
    drawBullet();

    glutSwapBuffers();
}

// === ANIMATION ===
void update(int value) {
    heliX += 1.5f;
    if (heliX > 900) heliX = -100;

    rotorAngle += 10.0f;
    if (rotorAngle > 360.0f) rotorAngle -= 360.0f;

    flickerCounter++;
    if (flickerCounter >= 15) {
        flickerState = !flickerState;
        flickerCounter = 0;
    }

    // Lightning trigger randomly every ~300 frames
    if (!showLightning && rand() % 200 == 0) {
        showLightning = true;
        lightningTimer = 5;  // show for 5 frames
    }

    // Count down lightning
    if (showLightning) {
        lightningTimer--;
        if (lightningTimer <= 0)
            showLightning = false;
    }

    // Car logic
    if (carVisible && !carFalling) {
        carX += carSpeed;
        if (carX >= 360) {  // bridge end before river gap
            carFalling = true;
            carSpeed = 0.0f;
        }
    }
    if (carFalling) {
        carAngle += 2.0f;
        carY -= 2.5f;
        if (carY < 260) {  // fell below river
            carVisible = false;
            carFalling = false;
            carX = 50;   // reset position if needed
            carY = 305;
            carAngle = 0;
        }
    }

    // Update snow positions
    for (int i = 0; i < SNOW_COUNT; i++) {
        snowY[i] -= snowSpeed[i];
        if (snowY[i] < 0) {
            snowY[i] = 600;
            snowX[i] = rand() % 800;
        } }

    if (bulletActive) {
        float angleRad = barrelAngle * PI / 180.0f;
        bulletX += cos(angleRad) * 5.0f;
        bulletY += sin(angleRad) * 5.0f;

        if (bulletX > 820 || bulletX < 0 || bulletY > 620 || bulletY < 0)
            bulletActive = false;
    }





    glutPostRedisplay();
    glutTimerFunc(16, update, 0);


}

void handleKeypress(unsigned char key, int x, int y) {
    if (key == 'w' || key == 'W') {
        carSpeed = 1.2f;
    } else if (key == 'q' || key == 'Q') {
        carSpeed = 0.0f;
    }
    else if (key == 'e' || key == 'E') {
    tankX -= tankSpeed;
    }
    else if (key == 'r' || key == 'R') {
        tankX += tankSpeed; }

    else if (key == 'f' || key == 'F') {
        if (!bulletActive) {
            bulletActive = true;
            float angleRad = barrelAngle * PI / 180.0f;
            bulletX = tankX + 50 + cos(angleRad) * 50;
            bulletY = tankY + 50 + sin(angleRad) * 50;
        }
    }

    else if (key == 's' || key == 'S') {
    if (barrelAngle < 180.0f)
        barrelAngle += 5.0f;  // rotate right
    }
    else if (key == 'd' || key == 'D') {
        if (barrelAngle > 0.0f)
            barrelAngle -= 5.0f;  // rotate left
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
        case '3':
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


// === INIT ===
void init() {
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 800, 0, 600);

    srand(time(0));        // random heights
    initBuildings();       // set building heights only once

    // Initialize snow
    for (int i = 0; i < SNOW_COUNT; i++) {
        snowX[i] = rand() % 800;
        snowY[i] = 300 + rand() % 300;
        snowSpeed[i] = 0.5f + static_cast<float>(rand()) / RAND_MAX;
    }
}

// === MAIN ===
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    cout<<"      "<<endl;
    cout<<"       "<<endl;
    cout<<("=== Appocalyptic World ===\n");
    cout<<"Press W to start car"<<endl;
    cout<<"Press Q to stop car"<<endl;
    cout<<"Press E to move the tank to left"<<endl;
    cout<<"Press R to move the tank to right"<<endl;
    cout<<"Press F to shoot from tank"<<endl;
    cout<<"Press S to move the tank barrel to left"<<endl;
    cout<<"Press D to move the tank barrel to right"<<endl;

    glutInitWindowSize(1300, 900);
    glutCreateWindow("Post-Apocalyptic Bridge + River");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutTimerFunc(0, update, 0);
    glutMainLoop();


    return 0;
}
