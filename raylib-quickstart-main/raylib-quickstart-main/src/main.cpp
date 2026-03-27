#include "raylib.h"
#include "resource_dir.h"

#define MAX_FRAME_SPEED 15
#define MIN_FRAME_SPEED  1
Sound soundArray[10];
Music musicArray[10];
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
    //-----------------------------CHANGE ------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    InitWindow(1280, 720, "Metal Slug");
    SetWindowMinSize(800, 450);
    //-----------------------------CHANGE ------------------------------------------

    
    int screenWidth2 = GetScreenWidth();
    int screenHeight2 = GetScreenHeight();

    SearchAndSetResourceDir("resources");

    //Audio
    InitAudioDevice();
    //Sound
    soundArray[0] = LoadSound("sexy_death.mp3");
    soundArray[1] = LoadSound("vaca.mp3");
    //Music
    musicArray[0] = LoadMusicStream("bo.mp3");
    musicArray[0].looping = true;
    float pitch = 0.5f;

    PlayMusicStream(musicArray[0]);

    Texture p1 = LoadTexture("treure fons.png");
    Texture bg = LoadTexture("MetalSlug-Mission1.png");
    Texture e1 = LoadTexture("hud.png");

    // Tamaño real del mundo (fondo escalado x5)
    const float bgScale = 5.0f;
    const int   worldWidth = (int)(bg.width * bgScale);
    const int   worldHeight = (int)(bg.height * bgScale);
    const int   FLOOR_Y = (int)(worldHeight * 0.7f);  // 3/10 desde abajo

    player p = { screenWidth2 / 2, FLOOR_Y, 0, 0, true };

    // --- Cámara 2D ---
    Camera2D camera = { 0 };
    camera.offset = { screenWidth2 / 2.0f, screenHeight2 / 2.0f }; // centrada en pantalla
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

        if (IsKeyPressed(KEY_SPACE))
            PlaySound(soundArray[0]);
        
        if (IsKeyPressed(KEY_V)) 
            PlaySound(soundArray[1]);

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
        camera.target.x = (float)p.x;         //-----------------------------REMOVE CURRENT LINE FOR THESE TWO ONE ------------------------------------------
        camera.target.y = (float)FLOOR_Y - 100; // ----------------------------------------------------------------------------------------------------------
        float halfW = screenWidth2 / 2.0f;
        float halfH = screenHeight2 / 2.0f;

        // Clamp horizontal: no mostrar fuera del fondo
        if (camera.target.x < halfW)              camera.target.x = halfW;
        if (camera.target.x > worldWidth - halfW) camera.target.x = worldWidth - halfW;

        //-----------------------------REMOVE VERTICAL CLAMP ------------------------------------------

        UpdateMusicStream(musicArray[0]);

        // --- Dibujo ---
        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera);
        // Fondo en el origen del mundo
        Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
        Rectangle dest = { 0, 0, bg.width * bgScale, bg.height * bgScale };

        DrawTexturePro(bg, src, dest, { 0,0 }, 0.0f, WHITE);

        // Jugador en su posición del mundo
        Vector2 position = { (float)p.x, (float)p.y };
        DrawTextureRec(p1, frameRec, position, WHITE);
        EndMode2D();

        EndDrawing();
    }
    CloseAudioDevice();
    UnloadTexture(p1);
    UnloadTexture(bg);
    UnloadTexture(e1);
    CloseWindow();
    return 0;
}