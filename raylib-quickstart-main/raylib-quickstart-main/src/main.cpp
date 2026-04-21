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
    int isshooting = -1; // 1 = si, -1 = no
    int isajupit = -1; // 1 = si, -1 = no
    int credits;
    int vides = 3;
    int hp = 1;
    bool menu = true;
    void jump() {
        vy = 50;
        canJump = false;
    }
};

class soldier
{
public:
    float ex;
    int ey;
    int vy;
    int evx;
    int ehp = 1;
    int efacing = 1; // 1 = right, -1 = left
    int efacingy = 1; // 1 = up, -1 = down
};

struct Timer
{
    float lifetime;
};

void startTimer(Timer* timer, float lifetime)
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

bool TimerDone(Timer* timer)
{
    if (timer != NULL)
    {
        return timer->lifetime <= 0;
    }
    return true;
}

struct Bullet {
    float x, y;
    float vx;
    float vy;
    bool active;
};

struct Bullete {
    float x, y;
    float vx;
    float vy;
    bool active;
};

int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(1300, 952, "Metal Slug");
    SetTargetFPS(60);

    bool inMenu = true;
    bool winscreen = false;
    bool music = false;
    bool menuSoundPlayed = false;
    bool winSoundPlayed = false;
    bool lose = false;
    float enemyShootTimer = 0.0f;
    float enemyShootInterval = 3.0f;
    bool gameOver = false;
    float hitCooldown = 0.0f;
    int vpunts = 0;
    int screenWidth2 = GetScreenWidth();
    int screenHeight2 = GetScreenHeight();
    SearchAndSetResourceDir("resources");
    InitAudioDevice();

    soundArray[0] = LoadSound("sexy_death.mp3");
    soundArray[1] = LoadSound("vaca.mp3");
    soundArray[2] = LoadSound("pipa.mp3");
    soundArray[3] = LoadSound("Metal_Slug.mp3");
    soundArray[4] = LoadSound("mission_complete.mp3");
    soundArray[5] = LoadSound("Game_Over.ogg");

    musicArray[0] = LoadMusicStream("bo.mp3");
    musicArray[0].looping = true;
    PlayMusicStream(musicArray[0]);
    float pitch = 0.5f;

    Texture p1 = LoadTexture("p1idle.png");
    Texture p1cap = LoadTexture("capquiet.png");
    Texture p1dretacorrentcames = LoadTexture("camescorrent.png");
    Texture bg = LoadTexture("MetalSlug-Mission1.png");
    Texture bullet = LoadTexture("bullet.png");
    Texture sidle = LoadTexture("soldatidle.png");
    Texture start = LoadTexture("Metal_Slug_Start.png");
    Texture p1cape = LoadTexture("capquiete.png");
    Texture p1esquerracorrentcames = LoadTexture("camescorrente.png");
    Texture p1e = LoadTexture("p1idlee.png");
    Texture p1scap = LoadTexture("scap.png");
    Texture p1scape = LoadTexture("scape.png");
    Texture p1scames = LoadTexture("scames.png");
    Texture p1scamese = LoadTexture("scamese.png");
    Texture p1shot = LoadTexture("captir.png");
    Texture p1shote = LoadTexture("captire.png");
    Texture p1camq = LoadTexture("camesquiet.png");
    Texture p1camqe = LoadTexture("camesquiete.png");
    Texture win = LoadTexture("mission-1-complete.png");
    Texture p1baix = LoadTexture("ajupit.png");
    Texture p1baixe = LoadTexture("ajupite.png");
    Texture p1baixtir = LoadTexture("tirajupit.png");
    Texture p1baixtire = LoadTexture("tirajupite.png");
    Texture p1alttir = LoadTexture("tiramunt.png");
    Texture p1alttire = LoadTexture("tiramunte.png");
    Texture gameover = LoadTexture("Game_Over.png");
    Texture bullete = LoadTexture("benemic.png");

    int timerlife = 450;
    Timer vidaTimer = { 0 };
    startTimer(&vidaTimer, timerlife);

    const float bgScale = 5.0f;
    const int   worldWidth = (int)(bg.width * bgScale);
    const int   worldHeight = (int)(bg.height * bgScale);
    int   FLOOR_Y = 1300;

    player p = { 400, 1220 , 0, 0, true };

    soldier s1 = { 19500, 605 };
    soldier s2 = { 5450, 605 };
    soldier s3 = { 10450, 605 };

    bool bs1 = true;
    bool bs2 = true;
    bool bs3 = true;

    Camera2D camera = { 0 };
    camera.offset = { 550, 459 };
    camera.rotation = 0.0f;
    camera.zoom = 0.85f;

    Rectangle framereceidle = { 0, 0, (float)sidle.width / 4, (float)sidle.height };

    Rectangle framerecalttire = { 0, 0, (float)p1alttire.width / 10, (float)p1alttire.height };
    Rectangle framerecalttir = { 0, 0, (float)p1alttir.width / 10, (float)p1alttir.height };
    Rectangle framerecbaixtire = { 0, 0, (float)p1baixtire.width / 10, (float)p1baixtire.height };
    Rectangle framerecbaixtir = { 0, 0, (float)p1baixtir.width / 10, (float)p1baixtir.height };
    Rectangle framerecbaixe = { 0, 0, (float)p1baixe.width / 4, (float)p1baixe.height };
    Rectangle framerecbaix = { 0, 0, (float)p1baix.width / 4, (float)p1baix.height };
    Rectangle framereccamqe = { 0, 0, (float)p1camqe.width / 4, (float)p1camqe.height };
    Rectangle framereccamq = { 0, 0, (float)p1camq.width / 4, (float)p1camq.height };
    Rectangle framerectire = { 0, 0, (float)p1shote.width / 10, (float)p1shote.height };
    Rectangle framerectir = { 0, 0, (float)p1shot.width / 10, (float)p1shot.height };
    Rectangle framerecscamese = { 0, 0, (float)p1scamese.width / 6, (float)p1scamese.height };
    Rectangle framerecscames = { 0, 0, (float)p1scames.width / 6, (float)p1scames.height };
    Rectangle framerecscape = { 0, 0, (float)p1scape.width / 6, (float)p1scape.height };
    Rectangle framerecscap = { 0, 0, (float)p1scap.width / 6, (float)p1scap.height };
    Rectangle frameRecidlee = { 0, 0, (float)p1e.width / 4, (float)p1e.height };
    Rectangle frameesquerracorrent = { 0, 0, (float)p1esquerracorrentcames.width / 12, (float)p1esquerracorrentcames.height };
    Rectangle frameReccape = { 0, 0, (float)p1cape.width / 4, (float)p1cape.height };
    Rectangle frameReccap = { 0, 0, (float)p1cap.width / 4, (float)p1cap.height };
    Rectangle frameRecdretacorrent = { 0, 0, (float)p1dretacorrentcames.width / 12, (float)p1dretacorrentcames.height };
    Rectangle frameRecidle = { 0, 0, (float)p1.width / 4, (float)p1.height };

    int currentFrameidle = 0;
    int currentFramcorrer = 0;
    int currentFramsalt = 0;
    int currentFramtir = 0;
    int framesCounter = 0;
    int framesSpeed = 3;
    int framesSpeedtir = 4;

    const int MAX_BULLETSE = 20000;
    Bullete bulletse[MAX_BULLETSE] = {};
    const int MAX_BULLETS = 20000;
    Bullet bullets[MAX_BULLETS] = {};

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();

        framesCounter++;
        if (framesCounter >= (60 / framesSpeed))
        {
            framesCounter = 0;
            currentFrameidle++;
            if (currentFrameidle >= 4) currentFrameidle = 0;
            currentFramcorrer++;
            if (currentFramcorrer >= 12) currentFramcorrer = 0;
            currentFramsalt++;
            if (currentFramsalt >= 6) currentFramsalt = 0;
            frameRecidle.x = (float)currentFrameidle * (float)p1.width / 4;
            frameReccap.x = (float)currentFrameidle * (float)p1cap.width / 4;
            framereceidle.x = (float)currentFrameidle * (float)sidle.width / 4;
            frameReccape.x = (float)currentFrameidle * (float)p1cape.width / 4;
            frameRecidlee.x = (float)currentFrameidle * (float)p1e.width / 4;
            framereccamq.x = (float)currentFrameidle * (float)p1camq.width / 4;
            framereccamqe.x = (float)currentFrameidle * (float)p1camqe.width / 4;
            framerecbaix.x = (float)currentFrameidle * (float)p1baix.width / 4;
            framerecbaixe.x = (float)currentFrameidle * (float)p1baixe.width / 4;
            framerecscap.x = (float)currentFramsalt * (float)p1scap.width / 6;
            framerecscape.x = (float)currentFramsalt * (float)p1scape.width / 6;
            framerecscames.x = (float)currentFramsalt * (float)p1scames.width / 6;
            framerecscamese.x = (float)currentFramsalt * (float)p1scamese.width / 6;
            frameRecdretacorrent.x = (float)currentFramcorrer * (float)p1dretacorrentcames.width / 12;
            frameesquerracorrent.x = (float)currentFramcorrer * (float)p1esquerracorrentcames.width / 12;
        }

        if (framesCounter >= (60 / framesSpeedtir))
        {
            currentFramtir++;

            if (currentFramtir >= 10)
            {
                currentFramtir = 0;
                p.isshooting = -1;
            }

            framerectir.x = (float)currentFramtir * (float)p1shot.width / 10;
            framerectire.x = (float)currentFramtir * (float)p1shote.width / 10;
            framerecbaixtir.x = (float)currentFramtir * (float)p1baixtir.width / 10;
            framerecbaixtire.x = (float)currentFramtir * (float)p1baixtire.width / 10;
            framerecalttir.x = (float)currentFramtir * (float)p1alttir.width / 10;
            framerecalttire.x = (float)currentFramtir * (float)p1alttire.width / 10;
        }

        p.x += p.vx;
        p.y -= p.vy;

        if (p.x >= 4500) s2.evx = -5;
        s2.ex += s2.evx;

        if (p.y < FLOOR_Y)
        {
            if (p.canJump != false && p.y > FLOOR_Y - 10) p.y = FLOOR_Y;
            if (p.vy > -20) p.vy -= 4;
        }
        else
        {
            p.y = FLOOR_Y;
            p.canJump = true;
            p.vy = 0;
        }

        s1.ey += s1.vy;
        s1.vy += 4;

        if (s1.ey >= FLOOR_Y)
        {
            s1.ey = FLOOR_Y;
            s1.vy = 0;
        }

        s2.ey += s2.vy;
        s2.vy += 4;

        if (s2.ey >= FLOOR_Y)
        {
            s2.ey = FLOOR_Y;
            s2.vy = 0;
        }

        s3.ey += s3.vy;
        s3.vy += 4;

        if (s3.ey >= 1020)
        {
            s3.ey = 1020;
            s3.vy = 0;
        }

        float camLeft = camera.target.x - (camera.offset.x) / camera.zoom;
        float camRight = camera.target.x + (screenWidth2 - camera.offset.x) / camera.zoom;
        float camTop = camera.target.y - (camera.offset.y) / camera.zoom;
        float camBottom = camera.target.y + (screenHeight2 - camera.offset.y) / camera.zoom;

        if (p.x > 0) FLOOR_Y = 1220;
        if (p.x > 3370) FLOOR_Y = 1380;
        if (p.x > 8900) FLOOR_Y = p.x * -0.8 + 8500;
        if (p.x > 9050) FLOOR_Y = 1260;
        if (p.x > 9400) FLOOR_Y = 1380;
        if (p.x > 10100) FLOOR_Y = 1220;
        if (p.x > 10250) FLOOR_Y = 1020;
        if (p.x > 10700) FLOOR_Y = 1380;
        if (p.x > 16300) FLOOR_Y = p.x * -0.8 + 14420;
        if (p.x > 16550) FLOOR_Y = 1180;
        if (p.x > 16750) FLOOR_Y = p.x * -0.6 + 11230;
        if (p.x > 17000) FLOOR_Y = 1030;
        if (p.x > 17150) FLOOR_Y = p.x * -0.75 + 13892.5;
        if (p.x > 17350) FLOOR_Y = 880;
        if (p.x > 17550) FLOOR_Y = p.x * -0.9 + 16675;
        if (p.x > 17750) FLOOR_Y = 700;

        if (p.x <= camLeft) p.x = camLeft + 5;
        if (p.x <= 3385 && p.y > 1220) p.x = 3390;
        if (p.x >= 9350 && p.x <= 9410 && p.y > 1260) p.x = 9415;
        if (p.x >= 10095 && p.x <= 10150 && p.y > 1220) p.x = 10090;
        if (p.x >= 10245 && p.x <= 10300 && p.y > 1021) p.x = 10240;
        if (p.x >= 10600 && p.x <= 10710 && p.y > 1021) p.x = 10715;

        if (IsKeyDown(KEY_W) && !IsKeyDown(KEY_S)) p.facingy = 1;
        else if (IsKeyDown(KEY_S) && !IsKeyDown(KEY_W) && p.y < FLOOR_Y) p.facingy = -1;
        else p.facingy = 0;

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;
            bullets[i].x += bullets[i].vx;
            bullets[i].y += bullets[i].vy;
            if (bullets[i].x < camLeft || bullets[i].x > camRight ||
                bullets[i].y < camTop || bullets[i].y > camBottom) {
                bullets[i].active = false;
            }
        }

        for (int i = 0; i < MAX_BULLETSE; i++) {
            if (!bulletse[i].active) continue;
            bulletse[i].x += bulletse[i].vx;
            bulletse[i].y += bulletse[i].vy;
            if (bulletse[i].x < camLeft || bulletse[i].x > camRight ||
                bulletse[i].y < camTop || bulletse[i].y > camBottom) {
                bulletse[i].active = false;
            }
        }

        if (p.x < 0) { p.x = 0;          if (p.vx < 0) p.vx = 0; }
        if (p.x > worldWidth) { p.x = worldWidth;  if (p.vx > 0) p.vx = 0; }

        if (camera.target.x < p.x) camera.target.x = (float)p.x;
        if (p.x >= 0 && p.x < 16200) {
            camera.target.y = (float)1100;
        }
        else if (p.x >= 16200 && p.x < 17800 && camera.target.y != 540) {
            camera.target.y = -0.35 * camera.target.x + 6770;
        }
        else {
            camera.target.y = (float)540;
        }
        float halfW = screenWidth2 / 2.0f;
        float halfH = screenHeight2 / 2.0f;

        if (camera.target.x < halfW)              camera.target.x = halfW;
        if (camera.target.x > worldWidth - halfW) camera.target.x = worldWidth - halfW;

        string ix = to_string(p.x);
        const char* cix = ix.c_str();
        string puntstext = "Punts: ";
        const char* cpuntstext = puntstext.c_str();
        string punts = to_string(vpunts);
        const char* cpunts = punts.c_str();

        BeginDrawing();
        ClearBackground(BLACK);
        if (hitCooldown > 0.0f) hitCooldown -= GetFrameTime();

        BeginMode2D(camera);
        Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
        Rectangle dest = { 0, 0, bg.width * bgScale, bg.height * bgScale };

        DrawTexturePro(bg, src, dest, { 0,0 }, 0.0f, WHITE);

        DrawText(cpunts, p.x, -10, 50, RED);

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;
            DrawTexture(bullet, (int)bullets[i].x, (int)bullets[i].y, WHITE);
            if (bullets[i].x >= s1.ex && bullets[i].x <= s1.ex + 100 && bullets[i].y >= s1.ey && bullets[i].y <= s1.ey + 200)
            {
                s1.ehp--;
            }
            if (bullets[i].x >= s2.ex && bullets[i].x <= s2.ex + 100 && bullets[i].y >= s2.ey && bullets[i].y <= s2.ey + 200)
            {
                s2.ehp--;
            }
            if (bullets[i].x >= s3.ex && bullets[i].x <= s3.ex + 100 && bullets[i].y >= s3.ey && bullets[i].y <= s3.ey + 200)
            {
                s3.ehp--;
            }
        }

        for (int i = 0; i < MAX_BULLETSE; i++) {
            if (!bulletse[i].active) continue;
            DrawTexture(bullete, (int)bulletse[i].x, (int)bulletse[i].y, WHITE);
            if (bulletse[i].x >= p.x && bulletse[i].x <= p.x + 100 && bulletse[i].y >= p.y && bulletse[i].y <= p.y + 200 && p.isajupit == -1 || bulletse[i].x >= p.x && bulletse[i].x <= p.x + 100 && bulletse[i].y >= p.y + 100 && bulletse[i].y <= p.y + 200 && p.isajupit == 1)
            {
                bulletse[i].active = false;
                hitCooldown = 1.5f;
                p.credits--;
                p.vx = (bulletse[i].vx > 0) ? 0 : 0;
                p.vy = 20;
                if (p.credits <= 0) {
                    lose = true;
                }
            }
        }

        if (!inMenu && !winscreen && !lose) {

            if (IsKeyPressed(KEY_F)) {
                PlaySound(soundArray[2]);
                p.isshooting = 1;
                currentFramtir = 0;
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bullets[i].active) {
                        bullets[i].x = (float)p.x + 15;
                        bullets[i].y = (float)p.y + 55;
                        if (p.facingy == 1) {
                            bullets[i].vx = 0;
                            bullets[i].vy = -30.0f;
                        }
                        else if (p.facingy == -1) {
                            bullets[i].vx = 0;
                            bullets[i].vy = 30.0f;
                        }
                        else {
                            bullets[i].vx = 30.0f * p.facing;
                            bullets[i].vy = 0;
                        }
                        bullets[i].active = true;
                        break;
                    }
                }
            }
        }

        if (s1.ehp == 1) {
            Vector2 position = { 0.0f, 0.0f };
            Rectangle posidles1 = { (float)s1.ex, (float)s1.ey, framereceidle.width * 5, framereceidle.height * 5 };
            DrawTexturePro(sidle, framereceidle, posidles1, position, 0, WHITE);
            DrawText(cix, s1.ex, s1.ey, 20, RED);
            if (!inMenu && !winscreen && !lose) {
                enemyShootTimer += GetFrameTime();
                if (enemyShootTimer >= enemyShootInterval) {
                    enemyShootTimer = 0.0f;
                    for (int i = 0; i < MAX_BULLETSE; i++) {
                        if (!bulletse[i].active) {
                            bulletse[i].x = s1.ex;
                            bulletse[i].y = s1.ey + 30;
                            bulletse[i].vx = (p.x < s1.ex) ? -8.0f : 8.0f;
                            bulletse[i].vy = 0;
                            bulletse[i].active = true;
                            break;
                        }
                    }
                }
            }
        }
        else if (bs1)
        {
            vpunts = vpunts + 10;
            bs1 = false;
            PlaySound(soundArray[0]);
            winscreen = true;
        }
        if (s2.ehp == 1) {
            Vector2 position = { 0.0f, 0.0f };
            Rectangle posidles2 = { (float)s2.ex, (float)s2.ey, framereceidle.width * 5, framereceidle.height * 5 };
            DrawTexturePro(sidle, framereceidle, posidles2, position, 0, WHITE);
            DrawText(cix, s2.ex, s2.ey, 20, RED);
            if (!inMenu && !winscreen && !lose) {
                enemyShootTimer += GetFrameTime();
                if (enemyShootTimer >= enemyShootInterval) {
                    enemyShootTimer = 0.0f;
                    for (int i = 0; i < MAX_BULLETSE; i++) {
                        if (!bulletse[i].active) {
                            bulletse[i].x = s2.ex;
                            bulletse[i].y = s2.ey + 30;
                            bulletse[i].vx = (p.x < s2.ex) ? -8.0f : 8.0f;
                            bulletse[i].vy = 0;
                            bulletse[i].active = true;
                            break;
                        }
                    }
                }
            }
        }
        else if (bs2)
        {
            vpunts = vpunts + 10;
            bs2 = false;
            PlaySound(soundArray[0]);
        }

        if (s3.ehp == 1) {
            Vector2 position = { 0.0f, 0.0f };
            Rectangle posidles3 = { (float)s3.ex, (float)s3.ey, framereceidle.width * 5, framereceidle.height * 5 };
            DrawTexturePro(sidle, framereceidle, posidles3, position, 0, WHITE);
            DrawText(cix, s3.ex, s3.ey, 20, RED);
            if (!inMenu && !winscreen && !lose) {
                enemyShootTimer += GetFrameTime();
                if (enemyShootTimer >= enemyShootInterval) {
                    enemyShootTimer = 0.0f;
                    for (int i = 0; i < MAX_BULLETSE; i++) {
                        if (!bulletse[i].active) {
                            bulletse[i].x = s3.ex;
                            bulletse[i].y = s3.ey + 30;
                            bulletse[i].vx = (p.x < s3.ex) ? -8.0f : 8.0f;
                            bulletse[i].vy = 0;
                            bulletse[i].active = true;
                            break;
                        }
                    }
                }
            }
        }
        else if (bs3)
        {
            vpunts = vpunts + 10;
            bs3 = false;
            PlaySound(soundArray[0]);
        }

        if (p.isajupit == -1) 
        {
            if (p.vx == 0 && p.facing == 1 && p.canJump == true && p.isshooting == -1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)p.x, (float)p.y, frameRecidle.width * 5, frameRecidle.height * 5 };
                DrawTexturePro(p1, frameRecidle, posidle, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx > 0 && p.facing == 1 && p.canJump == true && p.isshooting == -1)
            {
                Vector2 position = { frameRecdretacorrent.width * 4.75f / 2, frameRecdretacorrent.height * 4.75f / 2 };
                Rectangle posdretacorrent = { (float)p.x + 30, (float)p.y + 138, frameRecdretacorrent.width * 4.75, frameRecdretacorrent.height * 4.75 };
                Rectangle poscap = { (float)p.x + 60, (float)p.y + 45, frameReccap.width * 4.75, frameReccap.height * 4.75 };
                DrawTexturePro(p1dretacorrentcames, frameRecdretacorrent, posdretacorrent, position, 0, WHITE);
                DrawTexturePro(p1cap, frameReccap, poscap, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx < 0 && p.facing == -1 && p.canJump == true && p.isshooting == -1)
            {
                Vector2 position = { frameesquerracorrent.width * 4.75f / 2, frameesquerracorrent.height * 4.75f / 2 };
                Rectangle posesquerracorrent = { (float)p.x + 80, (float)p.y + 138,  frameesquerracorrent.width * 4.75, frameesquerracorrent.height * 4.75 };
                Rectangle poscap = { (float)p.x + 60, (float)p.y + 45, frameReccape.width * 4.75, frameReccape.height * 4.75 };
                DrawTexturePro(p1esquerracorrentcames, frameesquerracorrent, posesquerracorrent, position, 0, WHITE);
                DrawTexturePro(p1cape, frameReccape, poscap, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx == 0 && p.facing == -1 && p.canJump == true && p.isshooting == -1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)p.x, (float)p.y, frameRecidlee.width * 5, frameRecidlee.height * 5 };
                DrawTexturePro(p1e, frameRecidlee, posidle, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.canJump == false && p.facing == 1 && p.isshooting == -1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscamess = { (float)p.x, (float)p.y, framerecscames.width * 5, framerecscames.height * 5 };
                Rectangle poscaps = { (float)p.x, (float)p.y - 100, framerecscap.width * 5, framerecscap.height * 5 };
                DrawTexturePro(p1scap, framerecscap, poscaps, position, 0, WHITE);
                DrawTexturePro(p1scames, framerecscames, poscamess, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.canJump == false && p.facing == -1 && p.isshooting == -1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscamesse = { (float)p.x, (float)p.y, framerecscamese.width * 5, framerecscamese.height * 5 };
                Rectangle poscapse = { (float)p.x, (float)p.y - 100, framerecscape.width * 5, framerecscape.height * 5 };
                DrawTexturePro(p1scape, framerecscape, poscapse, position, 0, WHITE);
                DrawTexturePro(p1scamese, framerecscamese, poscamesse, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx == 0 && p.canJump == true && p.facing == 1 && p.isshooting == 1 && p.facingy == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle camqu = { (float)p.x - 20, (float)p.y + 105, framereccamq.width * 5, framereccamq.height * 5 };
                Rectangle poshota = { (float)p.x - 10, (float)p.y - 210, framerecalttir.width * 5, framerecalttir.height * 5 };
                DrawTexturePro(p1alttir, framerecalttir, poshota, position, 0, WHITE);
                DrawTexturePro(p1camq, framereccamq, camqu, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx > 0 && p.canJump == true && p.facing == 1 && p.isshooting == 1 && p.facingy == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle camqu = { (float)p.x - 20, (float)p.y + 105, framereccamq.width * 5, framereccamq.height * 5 };
                Rectangle poshota = { (float)p.x - 10, (float)p.y - 210, framerecalttir.width * 5, framerecalttir.height * 5 };
                DrawTexturePro(p1alttir, framerecalttir, poshota, position, 0, WHITE);
                DrawTexturePro(p1camq, framereccamq, camqu, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx == 0 && p.canJump == true && p.facing == -1 && p.isshooting == 1 && p.facingy == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle camque = { (float)p.x - 20, (float)p.y + 105, framereccamqe.width * 5, framereccamqe.height * 5 };
                Rectangle poshotae = { (float)p.x - 50, (float)p.y - 210, framerecalttire.width * 5, framerecalttire.height * 5 };
                DrawTexturePro(p1alttire, framerecalttire, poshotae, position, 0, WHITE);
                DrawTexturePro(p1camqe, framereccamqe, camque, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx < 0 && p.canJump == true && p.facing == -1 && p.isshooting == 1 && p.facingy == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle camque = { (float)p.x - 20, (float)p.y + 105, framereccamqe.width * 5, framereccamqe.height * 5 };
                Rectangle poshotae = { (float)p.x - 50, (float)p.y - 210, framerecalttire.width * 5, framerecalttire.height * 5 };
                DrawTexturePro(p1alttire, framerecalttire, poshotae, position, 0, WHITE);
                DrawTexturePro(p1camqe, framereccamqe, camque, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.canJump == false && p.facing == 1 && p.isshooting == 1 && p.facingy == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscamess = { (float)p.x, (float)p.y + 90, framerecscames.width * 5, framerecscames.height * 5 };
                Rectangle poshots = { (float)p.x + 10, (float)p.y - 30, framerectir.width * 5, framerectir.height * 5 };
                DrawTexturePro(p1shot, framerectir, poshots, position, 0, WHITE);
                DrawTexturePro(p1scames, framerecscames, poscamess, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.canJump == false && p.facing == -1 && p.isshooting == 1 && p.facingy == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscamesse = { (float)p.x, (float)p.y + 90, framerecscamese.width * 5, framerecscamese.height * 5 };
                Rectangle poshotse = { (float)p.x - 170, (float)p.y - 20, framerectire.width * 5, framerectire.height * 5 };
                DrawTexturePro(p1shote, framerectire, poshotse, position, 0, WHITE);
                DrawTexturePro(p1scamese, framerecscamese, poscamesse, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.canJump == false && p.facing == 1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscamess = { (float)p.x, (float)p.y + 90, framerecscames.width * 5, framerecscames.height * 5 };
                Rectangle poshots = { (float)p.x + 10, (float)p.y - 30, framerectir.width * 5, framerectir.height * 5 };
                DrawTexturePro(p1shot, framerectir, poshots, position, 0, WHITE);
                DrawTexturePro(p1scames, framerecscames, poscamess, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.canJump == false && p.facing == -1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscamesse = { (float)p.x, (float)p.y + 90, framerecscamese.width * 5, framerecscamese.height * 5 };
                Rectangle poshotse = { (float)p.x - 170, (float)p.y - 20, framerectire.width * 5, framerectire.height * 5 };
                DrawTexturePro(p1shote, framerectire, poshotse, position, 0, WHITE);
                DrawTexturePro(p1scamese, framerecscamese, poscamesse, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx == 0 && p.canJump == true && p.facing == 1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle camqu = { (float)p.x - 20, (float)p.y + 105, framereccamq.width * 5, framereccamq.height * 5 };
                Rectangle poshots = { (float)p.x + 15, (float)p.y - 25, framerectir.width * 5, framerectir.height * 5 };
                DrawTexturePro(p1shot, framerectir, poshots, position, 0, WHITE);
                DrawTexturePro(p1camq, framereccamq, camqu, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx > 0 && p.canJump == true && p.facing == 1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posdretacorrent = { (float)p.x - 10, (float)p.y + 100, frameRecdretacorrent.width * 4.75, frameRecdretacorrent.height * 4.75 };
                Rectangle poshots = { (float)p.x + 15, (float)p.y - 25, framerectir.width * 5, framerectir.height * 5 };
                DrawTexturePro(p1shot, framerectir, poshots, position, 0, WHITE);
                DrawTexturePro(p1dretacorrentcames, frameRecdretacorrent, posdretacorrent, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx == 0 && p.canJump == true && p.facing == -1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle camque = { (float)p.x - 20, (float)p.y + 105, framereccamqe.width * 5, framereccamqe.height * 5 };
                Rectangle poshotse = { (float)p.x - 215, (float)p.y - 5, framerectire.width * 5, framerectire.height * 5 };
                DrawTexturePro(p1shote, framerectire, poshotse, position, 0, WHITE);
                DrawTexturePro(p1camqe, framereccamqe, camque, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.vx < 0 && p.canJump == true && p.facing == -1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posesquerracorrent = { (float)p.x - 20, (float)p.y + 105,  frameesquerracorrent.width * 4.75, frameesquerracorrent.height * 4.75 };
                Rectangle poshotse = { (float)p.x - 215, (float)p.y - 5, framerectire.width * 5, framerectire.height * 5 };
                DrawTexturePro(p1shote, framerectire, poshotse, position, 0, WHITE);
                DrawTexturePro(p1esquerracorrentcames, frameesquerracorrent, posesquerracorrent, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
        }
        else if (p.isajupit == 1) 
        {
            if (p.vx == 0 && p.facing == 1 && p.isshooting == -1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posaju = { (float)p.x, (float)p.y + 70, framerecbaix.width * 5, framerecbaix.height * 5 };
                DrawTexturePro(p1baix, framerecbaix, posaju, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            if (p.vx > 0 && p.facing == 1 && p.isshooting == -1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posaju = { (float)p.x, (float)p.y + 70, framerecbaix.width * 5, framerecbaix.height * 5 };
                DrawTexturePro(p1baix, framerecbaix, posaju, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            if (p.vx == 0 && p.facing == -1 && p.isshooting == -1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posajue = { (float)p.x, (float)p.y + 70, framerecbaixe.width * 5, framerecbaixe.height * 5 };
                DrawTexturePro(p1baixe, framerecbaixe, posajue, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            if (p.vx < 0 && p.facing == -1 && p.isshooting == -1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posajue = { (float)p.x, (float)p.y + 70, framerecbaixe.width * 5, framerecbaixe.height * 5 };
                DrawTexturePro(p1baixe, framerecbaixe, posajue, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            if (p.vx == 0 && p.facing == 1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posajutir = { (float)p.x + 20, (float)p.y + 45, framerecbaixtir.width * 5, framerecbaixtir.height * 5 };
                DrawTexturePro(p1baixtir, framerecbaixtir, posajutir, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            if (p.vx > 0 && p.facing == 1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posajutir = { (float)p.x + 20, (float)p.y + 45, framerecbaixtir.width * 5, framerecbaixtir.height * 5 };
                DrawTexturePro(p1baixtir, framerecbaixtir, posajutir, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            if (p.vx == 0 && p.facing == -1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posajutire = { (float)p.x - 140, (float)p.y + 45, framerecbaixtire.width * 5, framerecbaixtire.height * 5 };
                DrawTexturePro(p1baixtire, framerecbaixtire, posajutire, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            if (p.vx < 0 && p.facing == -1 && p.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posajutire = { (float)p.x - 140, (float)p.y + 45, framerecbaixtire.width * 5, framerecbaixtire.height * 5 };
                DrawTexturePro(p1baixtire, framerecbaixtire, posajutire, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }

        }
        EndMode2D();

        if (inMenu == true) {
            if (!menuSoundPlayed) {
                PlaySound(soundArray[3]);
                menuSoundPlayed = true;
            }

            Rectangle src2 = { 0, 0, (float)start.width, (float)start.height };
            Rectangle dest2 = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };

            DrawTexturePro(start, src2, dest2, { 0, 0 }, 0.0f, WHITE);

            if (IsKeyPressed(KEY_C))
                p.credits++;
            if (IsKeyPressed(KEY_ENTER) && p.credits > 0)
            {
                p.credits--;
                UnloadTexture(start);
                inMenu = false;
            }
        }
        else {
            if (music == false) {
            }
            else {
                UpdateMusicStream(musicArray[0]);
            }
            if (IsKeyPressed(KEY_M)) {
                PauseMusicStream(musicArray[0]);
                music = false;
            }
            if (IsKeyPressed(KEY_N)) {
                ResumeMusicStream(musicArray[0]);
                music = true;
            }

            if (IsKeyPressed(KEY_C)) p.credits++;

            if (IsKeyPressed(KEY_R)) {
                lose = false;
                vpunts = 0;
                bs1 = true;
                winscreen = false;
                s1.ehp = 1;
                winSoundPlayed = false;
                timerlife = 450;
                startTimer(&vidaTimer, timerlife);

                ResumeMusicStream(musicArray[0]);

                p.x = 400;
                p.y = 1220;
                p.vx = 0;
                p.vy = 0;
                s2.ex = 5450;
                s2.evx = 0;

                camera.target.x = p.x;
                camera.target.y = 1100;
            }


            DrawText(TextFormat("%d", (int)vidaTimer.lifetime), screenWidth2 / 2, 20, 30, RED);
            updatetimer(&vidaTimer);

            if ((int)vidaTimer.lifetime == 0) {
                lose = true;
            }

            if (IsKeyDown(KEY_D) && p.vx < 10 && !IsKeyDown(KEY_A)) {
                p.vx += 5;
                p.facing = 1;
            }
            else if (IsKeyDown(KEY_A) && p.vx > -10 && !IsKeyDown(KEY_D)) {
                p.vx -= 5;
                p.facing = -1;
            }
            else if (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) p.vx = 0;
            if (IsKeyPressed(KEY_SPACE) && p.canJump) p.jump();
            if (IsKeyDown(KEY_S) && !IsKeyDown(KEY_W) && p.canJump)
            {
                p.isajupit = 1;
            }
            else
            {
                p.isajupit = -1;
            }
        }

        if (winscreen == true) {
            PauseMusicStream(musicArray[0]);
            if (!winSoundPlayed) {
                PlaySound(soundArray[4]);
                winSoundPlayed = true;
            }
            Rectangle src3 = { 0, 0, (float)win.width, (float)win.height };
            Rectangle dest3 = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
            DrawTexturePro(win, src3, dest3, { 0, 0 }, 0.0f, WHITE);
        }
        if (lose == true) {
            if (p.credits < 0)p.credits = 0;
            PauseMusicStream(musicArray[0]);
            if (!winSoundPlayed) {
                PlaySound(soundArray[5]);
                winSoundPlayed = true;
            }
            Rectangle src3 = { 0, 0, (float)gameover.width, (float)gameover.height };
            Rectangle dest3 = { 0, 0, (float)GetScreenWidth(), (float)GetScreenHeight() };
            DrawTexturePro(gameover, src3, dest3, { 0, 0 }, 0.0f, WHITE);
        }

        if (winscreen || lose) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                bullets[i].active = false;
            }
            for (int i = 0; i < MAX_BULLETSE; i++) {
                bulletse[i].active = false;
            }
        }

        int textWidth = MeasureText(cpunts, 30);



        DrawText(TextFormat("%i", p.credits), screenWidth2 - textWidth - 100, screenHeight2 - 100, 40, RED);
        DrawText(cpuntstext, screenWidth2 - textWidth - 140, 20, 30, RED);
        DrawText(cpunts, screenWidth2 - textWidth - 40, 20, 30, RED);
        DrawText(cpunts, 20, 20, 30, RED);

        EndDrawing();
    }
    CloseAudioDevice();
    UnloadTexture(p1);
    UnloadTexture(p1cap);
    UnloadTexture(p1dretacorrentcames);
    UnloadTexture(bg);
    UnloadTexture(bullet);
    UnloadTexture(sidle);
    UnloadTexture(p1cape);
    UnloadTexture(p1esquerracorrentcames);
    UnloadTexture(p1e);
    UnloadTexture(p1scap);
    UnloadTexture(p1scape);
    UnloadTexture(p1scames);
    UnloadTexture(p1scamese);
    UnloadTexture(p1shot);
    UnloadTexture(p1shote);
    UnloadTexture(p1camq);
    UnloadTexture(p1camqe);
    UnloadTexture(win);
    UnloadTexture(p1baix);
    UnloadTexture(p1baixe);
    UnloadTexture(p1baixtir);
    UnloadTexture(p1baixtire);
    UnloadTexture(p1alttir);
    UnloadTexture(p1alttire);
    UnloadTexture(gameover);
    UnloadTexture(bullete);
    CloseWindow();
    return 0;
}
// Copyright (c) 2026 Explota Studio
// Licensed under the MIT License