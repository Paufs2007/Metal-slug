#include "raylib.h"
#include "resource_dir.h"

#define MAX_FRAME_SPEED 15
#define MIN_FRAME_SPEED  1

class player
{
public:
    int x;
    int y;
    int vx;
    int vy;
    bool canJump;

    void jump() {
        vy = 30;
        canJump = false;
    }
};

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Metal Slug");
    ToggleFullscreen();

    SearchAndSetResourceDir("resources");

    Texture p1 = LoadTexture("treure fons.png");
    Texture bg = LoadTexture("MetalSlug-Mission1.png");

    // Tamaño real del mundo (fondo escalado x5)
    const float bgScale = 5.0f;
    const int   worldWidth = (int)(bg.width * bgScale);
    const int   worldHeight = (int)(bg.height * bgScale);
    const int   FLOOR_Y = (int)(worldHeight * 0.7f);  // 3/10 desde abajo

    player p = { screenWidth / 2, FLOOR_Y, 0, 0, true };

    // --- Cámara 2D ---
    Camera2D camera = { 0 };
    camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f }; // centrada en pantalla
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Rectangle frameRec = { 0, 0, (float)p1.width / 9, (float)p1.height };
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 3;

    while (!WindowShouldClose())
    {
        // --- Animación ---
        framesCounter++;
        if (framesCounter >= (60 / framesSpeed))
        {
            framesCounter = 0;
            currentFrame++;
            if (currentFrame >= 9) currentFrame = 0;
            frameRec.x = (float)currentFrame * (float)p1.width / 9;
        }

        // --- Física ---
        p.x += p.vx;
        p.y -= p.vy;

        if (p.y < FLOOR_Y)
        {
            p.canJump = false;
            if (p.vy > -8) p.vy--;
        }
        else
        {
            p.y = FLOOR_Y;
            p.canJump = true;
            p.vy = 0;
        }

        // --- Movimiento horizontal ---
        if (IsKeyDown(KEY_D) && p.vx < 5 && !IsKeyDown(KEY_A)) p.vx++;
        else if (IsKeyDown(KEY_A) && p.vx > -5 && !IsKeyDown(KEY_D)) p.vx--;
        else if (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))               p.vx = 0;

        // --- Salto ---
        if (IsKeyPressed(KEY_W) && p.canJump) p.jump();

        // --- Límites del mundo (bordes del fondo) ---
        if (p.x < 0) { p.x = 0;          if (p.vx < 0) p.vx = 0; }
        if (p.x > worldWidth) { p.x = worldWidth;  if (p.vx > 0) p.vx = 0; }

        // --- Cámara sigue al jugador, clampeada al mundo ---
        camera.target = { (float)p.x, (float)FLOOR_Y };

        float halfW = screenWidth / 2.0f;
        float halfH = screenHeight / 2.0f;

        // Clamp horizontal: no mostrar fuera del fondo
        if (camera.target.x < halfW)              camera.target.x = halfW;
        if (camera.target.x > worldWidth - halfW) camera.target.x = worldWidth - halfW;
        // Clamp vertical
        if (camera.target.y < halfH)              camera.target.y = halfH;
        if (camera.target.y > worldHeight - halfH) camera.target.y = worldHeight - halfH;

        // --- Dibujo ---
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        // Fondo en el origen del mundo
        DrawTextureEx(bg, { 0, 0 }, 0.0f, bgScale, WHITE);

        // Jugador en su posición del mundo
        Vector2 position = { (float)p.x, (float)p.y };
        DrawTextureRec(p1, frameRec, position, WHITE);
        EndMode2D();

        EndDrawing();
    }

    UnloadTexture(p1);
    UnloadTexture(bg);
    CloseWindow();
    return 0;
}