#include "raylib.h"
#include "resource_dir.h"
#include <string>
#include <raymath.h>
using namespace std;
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
    int facing = 1; // 1 = right, -1 = left
    int facingy = 1; // 1 = up, -1 = down


    void jump() {
        vy = 30;
        canJump = false;
    }
};
//timer
struct Timer
{
    float lifetime;
};

void startTimer (Timer* timer, float lifetime)
{
    if (timer != NULL)
    {
        timer->lifetime = lifetime;
    }
}


void updatetimer(Timer* timer)
{
    if (timer != NULL && timer->lifetime > 0)
    {
        timer->lifetime -= GetFrameTime();
    }
}

bool TimerDone (Timer* timer)
{
    if (timer != NULL)
    {
        return timer->lifetime <= 0;
    }
    return true;
}

//BULLETS
struct Bullet {
    float x, y;
    float vx;
    float vy;
    bool active;
};

int main()
{
    //-----------------------------CHANGE ------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    InitWindow(1500, 1080, "Metal Slug"); // 960 720
    SetWindowMinSize(800, 450);
    //-----------------------------CHANGE ------------------------------------------

    int vpunts = 0;
    int screenWidth2 = GetScreenWidth();
    int screenHeight2 = GetScreenHeight();
        
    SearchAndSetResourceDir("resources");

    //Audio
    InitAudioDevice();
    //Sound
    soundArray[0] = LoadSound("sexy_death.mp3");
    soundArray[1] = LoadSound("vaca.mp3");
    soundArray[2] = LoadSound("pipa.mp3");
    //Music
    musicArray[0] = LoadMusicStream("bo.mp3");
    musicArray[0].looping = true;
    float pitch = 0.5f;

    PlayMusicStream(musicArray[0]);

    Texture p1 = LoadTexture("p1idle.png");
    Texture p1cap = LoadTexture("capquiet.png");
    Texture p1dretacorrentcames = LoadTexture("camescorrent.png");
    Texture bg = LoadTexture("MetalSlug-Mission1.png");
    Texture bullet = LoadTexture("bullet.png");
    if (bullet.id == 0) TraceLog(LOG_ERROR, "Failed to load bullet.png");

    //timer

    int timerlife = 453;

    Timer vidaTimer = { 0 };

    startTimer(&vidaTimer, timerlife);
    
    // Tama�o real del mundo (fondo escalado x5)
    const float bgScale = 5.0f;
    const int   worldWidth = (int)(bg.width * bgScale);
    const int   worldHeight = (int)(bg.height * bgScale);
    int   FLOOR_Y = 1300; //1300 - 780

    player p = { 400, FLOOR_Y +1 , 0, 0, true };

    // --- C�mara 2D ---
    Camera2D camera = { 0 };
    camera.offset = { 400, 440 }; // centrada en pantalla
    camera.rotation = 0.0f;
    camera.zoom = 0.85f;

    Rectangle frameReccap = { 0, 0, (float)p1cap.width / 4, (float)p1cap.height };
    Rectangle frameRecdretacorrent = { 0, 0, (float)p1dretacorrentcames.width / 12, (float)p1dretacorrentcames.height };
    Rectangle frameRecidle = { 0, 0, (float)p1.width / 4, (float)p1.height };
    int currentFrameidle = 0;
    int currentFramcorrer = 0;
    int framesCounter = 0;
    int framesSpeed = 3;    

    //BULLETS
    const int MAX_BULLETS = 20000;
    Bullet bullets[MAX_BULLETS] = {};

    while (!WindowShouldClose())
    {
        // --- Fullscreen ---
        if (IsKeyPressed(KEY_F11))
        {
            ToggleFullscreen();
        }
        
        //timer

        updatetimer(&vidaTimer);

        // --- Animaci�n ---
        framesCounter++;
        if (framesCounter >= (60 / framesSpeed))
        {
            framesCounter = 0;

            currentFrameidle++;
            if (currentFrameidle >= 4) currentFrameidle = 0;
            
            currentFramcorrer++;
            if (currentFramcorrer >= 12) currentFramcorrer = 0;

            frameRecidle.x = (float)currentFrameidle * (float)p1.width / 4;
            
            frameReccap.x = (float)currentFrameidle * (float)p1cap.width / 4;

            frameRecdretacorrent.x = (float)currentFramcorrer * (float)p1dretacorrentcames.width / 12;
        }

        if (IsKeyPressed(KEY_SPACE))
            PlaySound(soundArray[0]);
        
        if (IsKeyPressed(KEY_V)) 
            PlaySound(soundArray[1]);

        if (IsKeyPressed(KEY_F))
            PlaySound(soundArray[2]);

        // --- F�sica ---
        p.x += p.vx;
        p.y -= p.vy;

        if (p.y < FLOOR_Y)
        {
            if(p.canJump != false && p.y > FLOOR_Y-10) p.y = FLOOR_Y;
            if (p.vy > -8) p.vy--;
        }
        else
        {
            p.y = FLOOR_Y;
            p.canJump = true;
            p.vy = 0;
        }

        // --- Movimiento horizontal ---
        if (IsKeyDown(KEY_D) && p.vx < 5 && !IsKeyDown(KEY_A)) {
            p.vx++;
            p.facing = 1;
        }
        else if (IsKeyDown(KEY_A) && p.vx > -5 && !IsKeyDown(KEY_D)) {
            p.vx--;
            p.facing = -1;
        }
        else if (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) p.vx = 0;

        // --- FLOOR_Y ---
        if (p.x > 0) FLOOR_Y = 1220;
        if (p.x > 3380) FLOOR_Y = 1380;
        if (p.x > 8900) FLOOR_Y = p.x*-0.8+8500;
        if (p.x > 9050) FLOOR_Y = 1260;
        if (p.x > 9400) FLOOR_Y = 1380;
        if (p.x > 10100) FLOOR_Y = 1220;
        if (p.x > 10250) FLOOR_Y = 1020;
        if (p.x > 10700) FLOOR_Y = 1380;
        if (p.x > 16300) FLOOR_Y = p.x*-0.8+14420;
        if (p.x > 16550) FLOOR_Y = 1180; 
        if (p.x > 16750) FLOOR_Y = p.x*-0.6+11230;
        if (p.x > 17000) FLOOR_Y = 1030;
        if (p.x > 17150) FLOOR_Y = p.x*-0.75+13892.5;
        if (p.x > 17350) FLOOR_Y = 880;
        if (p.x > 17550) FLOOR_Y = p.x*-0.9+16675;
        if (p.x > 17750) FLOOR_Y = 700;

        // --- Obstacles ---
        if (p.x < camera.target.x - 480) p.x = camera.target.x-481;
        if (p.x <= 3385 && p.y > 1300) p.x = 3390;
        if (p.x >= 9045 && p.x <= 9050 && p.y > 1350) p.x = 9040;
        if (p.x >= 9350 && p.x <= 9405 && p.y > 1350) p.x = 9410;
        if (p.x >= 10095 && p.x <= 10150 && p.y > 1220) p.x = 10090;
        if (p.x >= 10245 && p.x <= 10300 && p.y > 1021) p.x = 10245;
        if (p.x >= 10600 && p.x <= 10705 && p.y > 1021) p.x = 10710;
      

        // --- Cheats ---
        if (IsKeyDown(KEY_L)) p.x = 18000;
        
        // --- Salto ---
        if (IsKeyPressed(KEY_W) && p.canJump) p.jump();

        //Aim direction
        if (IsKeyDown(KEY_W)) p.facingy = 1;
        else if (IsKeyDown(KEY_S)) p.facingy = -1;


        if (IsKeyPressed(KEY_F)) 
        {
            vpunts++;
            for (int i = 0; i < MAX_BULLETS; i++) {
                if (!bullets[i].active) {
                    bullets[i].x = (float)p.x;
                    bullets[i].y = (float)p.y;

                    
                    if (IsKeyDown(KEY_W)) {
                        bullets[i].vx = 0;
                        bullets[i].vy = -15.0f; // up
                    }
                    else if (IsKeyDown(KEY_S)) {
                        bullets[i].vx = 0;
                        bullets[i].vy = 15.0f; // down
                    }
                    else {
                        bullets[i].vx = 15.0f * p.facing; // left/right
                        bullets[i].vy = 0;
                    }

                    bullets[i].active = true;
                    break;
                }
            }

        }

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;

            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;

            if (bullets[i].x < 0 || bullets[i].x > worldWidth ||
                bullets[i].y < 0 || bullets[i].y > worldHeight) {
                bullets[i].active = false;
            }
        }

        // --- L�mites del mundo (bordes del fondo) ---
        if (p.x < 0) { p.x = 0;          if (p.vx < 0) p.vx = 0; }
        if (p.x > worldWidth) { p.x = worldWidth;  if (p.vx > 0) p.vx = 0; }

        // --- C�mara sigue al jugador, clampeada al mundo ---
        if (camera.target.x < p.x) camera.target.x = (float)p.x;  
        if (p.x >= 0 && p.x < 16200) {
            camera.target.y = (float)1100;
        } else if (p.x >= 16200 && p.x < 17750) {
            camera.target.y = FLOOR_Y-280;
        } else {
            camera.target.y = (float)520;
        }
        float halfW = screenWidth2 / 2.0f;
        float halfH = screenHeight2 / 2.0f;

        // Clamp horizontal: no mostrar fuera del fondo
        if (camera.target.x < halfW)              camera.target.x = halfW;
        if (camera.target.x > worldWidth - halfW) camera.target.x = worldWidth - halfW;

        //-----------------------------REMOVE VERTICAL CLAMP ------------------------------------------

        string ix = to_string(p.x);
        const char* cix = ix.c_str();

        string puntstext = "Punts: ";
        const char* cpuntstext = puntstext.c_str();

        string punts = to_string(vpunts);
        const char* cpunts = punts.c_str();

        UpdateMusicStream(musicArray[0]);

        // --- Dibujo ---
        BeginDrawing();
        ClearBackground(RED);

        BeginMode2D(camera);
        // Fondo en el origen del mundo
        Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
        Rectangle dest = { 0, 0, bg.width * bgScale, bg.height * bgScale };

        DrawTexturePro(bg, src, dest, { 0,0 }, 0.0f, WHITE);

        DrawText(cpunts, p.x, -10, 50, RED);

        // Jugador en su posici�n del mundo 
        if (p.vx == 0)
        {
            Vector2 position = { 0.0f, 0.0f };
            Rectangle posidle = { (float)p.x, (float)p.y, frameRecidle.width * 5, frameRecidle.height * 5 };
            DrawTexturePro(p1, frameRecidle, posidle, position, 0, WHITE);
            DrawText(cix, p.x, p.y, 20, RED);
        }
        else if (p.vx > 0 && p.facing == 1)
        {
            Vector2 position = { frameRecdretacorrent.width * 4.75f / 2, frameRecdretacorrent.height * 4.75f / 2 };
            Rectangle posdretacorrent = { (float)p.x + 25, (float)p.y + 138, frameRecdretacorrent.width * 4.75, frameRecdretacorrent.height * 4.75 };
            Rectangle poscap = { (float)p.x + 55, (float)p.y + 45, frameReccap.width * 4.75, frameReccap.height * 4.75 };
            DrawTexturePro(p1dretacorrentcames, frameRecdretacorrent, posdretacorrent, position, 0, WHITE);
            DrawTexturePro(p1cap, frameReccap, poscap, position, 0, WHITE);
            DrawText(cix, p.x, p.y, 20, RED);
        }
        else if (p.vx < 0 && p.facing == -1)
        {
            Vector2 position = { frameRecdretacorrent.width * 4.75f / 2, frameRecdretacorrent.height * 4.75f / 2 };
            Rectangle posesquerracorrent = { (float)p.x + 25, (float)p.y + 138,  -frameRecdretacorrent.width * 4.75, frameRecdretacorrent.height * 4.75 };
            Rectangle poscap = { (float)p.x + 55, (float)p.y + 45, -frameReccap.width * 4.75, frameReccap.height * 4.75 };
            DrawTexturePro(p1dretacorrentcames, frameRecdretacorrent, posesquerracorrent, position, 0, WHITE);
            DrawTexturePro(p1cap, frameReccap, poscap, position, 0 , WHITE);
            DrawText(cix, p.x, p.y, 20, RED);
        }
        
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;
            DrawTexture(bullet, (int)bullets[i].x, (int)bullets[i].y, WHITE);
        }

        EndMode2D();

        int textWidth = MeasureText(cpunts, 30);

        //mitj pantalla
        
        DrawText(TextFormat("%d", (int)vidaTimer.lifetime), screenWidth2 / 2, 20, 30, RED);

        // canotnada dreta
        DrawText(cpuntstext, screenWidth2 - textWidth - 140, 20, 30, RED);

        DrawText(cpunts, screenWidth2 - textWidth - 40, 20, 30, RED);

        //cantonada esquerra
        DrawText(cpunts, 20, 20, 30, RED);

        EndDrawing();
    }
    CloseAudioDevice();
    UnloadTexture(p1);
    UnloadTexture(p1cap);
    UnloadTexture(p1dretacorrentcames);
    UnloadTexture(bg);
    UnloadTexture(bullet);
    CloseWindow();
    return 0;
}