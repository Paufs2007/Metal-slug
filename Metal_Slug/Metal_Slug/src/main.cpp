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
        vy = 45;
        canJump = false;
    }
};


class Raig
{
public:
    float x;

    float y;

    int isshooting = 0;
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
    float enemyShootTimer = 0.0f;
};

class tank
{
public:
    float tx;
    int ty;
    int tvy;
    int tvx;
    int thp = 50;
    int tfacing = 1; // 1 = right, -1 = left
    int tfacingy = 1; // 1 = up, -1 = down
    float tankShootTimer = 0.0f;
};

class boss
{
public:
    float ex;
    int ey;
    int vy;
    int evx;
    int ehp = 100;
    int efacing = 1; // 1 = right, -1 = left
    int efacingy = 1; // 1 = up, -1 = down
    float enemyShootTimer = 0.0f;
    int   burstCount = 0;      // bullets remaining in barrage
    float burstTimer = 0.0f;   // time between each burst shot
    const float burstInterval = 0.12f; // seconds between each shot in barrage
    int phase2BurstCount = 0;
    float phase2BurstTimer = 0.0f;
};

class objecte
{
public:
    float ox;
    int oy;
    int ovy;
    float punts;
    int alive = 1;
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
    int direction; // 1 = top -1 = bottom 2 = front -2 = back 
    bool useGravity;
    bool active;
};

class granada {
    
    public:
        float x, y;
        float vx;
        float vy;
        int direction;
        bool useGravity;
        bool active;
        void Allahuakbar(player p) {
            DrawRectangle(x, y, 10, 10, RED);
        }
};

struct Bullete {
    float x, y;
    float vx;
    float vy;

    bool useGravity;
    bool active;
};

struct Bulleta1 {
    float x, y;
    float vx;
    float vy;

    bool useGravity;
    bool active;
};

struct Bulleta2 {
    float x, y;
    float vx;
    float vy;

    bool useGravity;
    bool active;
};

struct Bulleta3 {
    float x, y;
    float vx;
    float vy;

    bool useGravity;
    bool active;
};


//atacs jefe no eliminar
bool raig = true;

bool bola = false;


int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(975, 714, "Metal Slug"); //1300 952
    SetTargetFPS(60);

    bool logoscreen = true;
    bool inMenu = true;
    bool winscreen = false;
    bool music = true;
    bool menuSoundPlayed = false;
    bool winSoundPlayed = false;
    bool lose = false;
    float enemyShootInterval = 3.0f;
    bool gameOver = false;
    float hitCooldown = 0.0f;
    int vpunts = 0;
    bool rampa = false;
    SearchAndSetResourceDir("resources");
    InitAudioDevice();

    soundArray[0] = LoadSound("sexy_death.mp3");
    soundArray[1] = LoadSound("vaca.mp3");
    soundArray[2] = LoadSound("pipa.mp3");
    soundArray[3] = LoadSound("Metal_Slug.mp3");
    soundArray[4] = LoadSound("mission_complete.mp3");
    soundArray[5] = LoadSound("Game_Over.ogg");
    soundArray[6] = LoadSound("explode.mp3");
    soundArray[7] = LoadSound("Fahhhh.mp3");
    soundArray[8] = LoadSound("MACHINE_GUN.mp3");
    SetSoundVolume(soundArray[7], 100.0f);

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
    Texture logo = LoadTexture("logoExplota.png");
    Texture scor = LoadTexture("scorr.png");
    Texture score = LoadTexture("scorre.png");
    Texture bulletes = LoadTexture("balae.png");
    Texture bulleta = LoadTexture("balaamunt.png");
    Texture bulletb = LoadTexture("balaball.png");
    Texture bulletee = LoadTexture("benemice.png");
    Texture p1cbaix = LoadTexture("cajupit.png");
    Texture p1cbaixe = LoadTexture("cajupite.png");
    Texture mgun = LoadTexture("MACHINE GUN.png");
    Texture at1b1 = LoadTexture("at1b1.png");
    Texture at1b2 = LoadTexture("at1b2.png");
    Texture at1b3 = LoadTexture("at1b3.png");
    Texture at1b4 = LoadTexture("at1b4.png");
    Texture at1b5 = LoadTexture("at1b5.png");
    Texture at1b6 = LoadTexture("at1b6.png");
    Texture tbase = LoadTexture("tankbaix.png");

    Font timerNums = LoadFont("tipografia.png");
    Font whiteFont = LoadFont("nums1.png");
    Font yellowFont = LoadFont("nums1.png");

    int timerlife = 450;
    Timer vidaTimer = { 0 };
    startTimer(&vidaTimer, timerlife);

    float gunCooldown = 0.0f;

    float shootCooldown = 0.0f;
    int machineGunAmmo = 0;


    const float bgScale = 5.0f;
    const int   worldWidth = (int)(bg.width * bgScale);
    const int   worldHeight = (int)(bg.height * bgScale);
    int   FLOOR_Y = 1300;

    player p = { 400, 1220 , 0, 0, true };

    Raig r = {1985, 605};

    boss s1 = { 19895, 605 };
    soldier s2 = { 5450, 605 };
    soldier s3 = { 10450, 605 };
    soldier Jorge = { 3200, 800 };
    tank t1 = { 17000, 1000 };

    objecte o1 = { 5250, 605 };

    bool os1 = true;
    bool killhim = false;

    bool KevinTheFuckingBoss = true;
    bool bs2 = true;
    bool bs3 = true;
    bool bJorge = true;
    bool bt1 = true;

    Camera2D camera = { 0 };
    camera.offset = { 315, 350 };
    camera.rotation = 0.0f;
    camera.zoom = 0.65f;

    Rectangle framerectbase = { 0, 0, (float)tbase.width / 4, (float)tbase.height };

    Rectangle framereceidle = { 0, 0, (float)sidle.width / 4, (float)sidle.height };
    Rectangle framerececorr = { 0, 0, (float)scor.width / 12, (float)scor.height };
    Rectangle framerececorre = { 0, 0, (float)score.width / 12, (float)score.height };

    Rectangle framecrecat1b1 = { 0, 0, (float)at1b1.width / 3, (float)at1b1.height };
    Rectangle framecrecat1b2 = { 0, 0, (float)at1b2.width / 3, (float)at1b2.height };
    Rectangle framecrecat1b3 = { 0, 0, (float)at1b3.width / 3, (float)at1b3.height };
    Rectangle framecrecat1b4 = { 0, 0, (float)at1b4.width / 3, (float)at1b4.height };
    Rectangle framecrecat1b5 = { 0, 0, (float)at1b5.width / 3, (float)at1b5.height };
    Rectangle framecrecat1b6 = { 0, 0, (float)at1b6.width / 3, (float)at1b6.height };

    Rectangle framereccajupite = { 0, 0, (float)p1cbaixe.width / 7, (float)p1cbaixe.height };
    Rectangle framereccajupit = { 0, 0, (float)p1cbaix.width / 7, (float)p1cbaix.height };
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

    Rectangle frameRecmgun = { 0, 0, (float)mgun.width / 2, (float)mgun.height };

    granada g;

    int currentFrameidle = 0;
    int currentFramcorrer = 0;
    int currentFramsalt = 0;
    int currentFramtir = 0;
    int currentFramajupit = 0;
    int currentFrameobj = 0;
    int currentFrametirb1 = 0;
    int framesCounter = 0;
    int framesSpeed = 3;
    int framesSpeedtir = 4;
    int framesSpeedtirb = 4;

    const int MAX_BULLETSE = 100;
    Bullete bulletse[MAX_BULLETSE] = {};
    const int MAX_BULLETS = 100;
    Bullet bullets[MAX_BULLETS] = {};
    const int MAX_BULLETSA1 = 3;
    Bulleta1 bulletsa1[MAX_BULLETSE] = {};
    const int MAX_BULLETSA2 = 1;
    Bulleta2 bulletsa2[MAX_BULLETS] = {};
    const int MAX_BULLETSA3 = 1;
    Bulleta3 bulletsa3[MAX_BULLETS] = {};


    while (!WindowShouldClose())
    {
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
            currentFramajupit++;
            if (currentFramajupit >= 7) currentFramajupit = 0;
            currentFrameobj++;
            if (currentFrameobj >= 2) currentFrameobj = 0;
            
            frameRecidle.x = (float)currentFrameidle * (float)p1.width / 4;
            frameReccap.x = (float)currentFrameidle * (float)p1cap.width / 4;
            framereceidle.x = (float)currentFrameidle * (float)sidle.width / 4;
            frameReccape.x = (float)currentFrameidle * (float)p1cape.width / 4;
            frameRecidlee.x = (float)currentFrameidle * (float)p1e.width / 4;
            framereccamq.x = (float)currentFrameidle * (float)p1camq.width / 4;
            framereccamqe.x = (float)currentFrameidle * (float)p1camqe.width / 4;
            framerecbaix.x = (float)currentFrameidle * (float)p1baix.width / 4;
            framerecbaixe.x = (float)currentFrameidle * (float)p1baixe.width / 4;
            framerectbase.x = (float)currentFrameidle * (float)tbase.width / 4;
            framerecscap.x = (float)currentFramsalt * (float)p1scap.width / 6;
            framerecscape.x = (float)currentFramsalt * (float)p1scape.width / 6;
            framerecscames.x = (float)currentFramsalt * (float)p1scames.width / 6;
            framerecscamese.x = (float)currentFramsalt * (float)p1scamese.width / 6;
            frameRecdretacorrent.x = (float)currentFramcorrer * (float)p1dretacorrentcames.width / 12;
            frameesquerracorrent.x = (float)currentFramcorrer * (float)p1esquerracorrentcames.width / 12;
            framerececorr.x = (float)currentFramcorrer * (float)scor.width / 12;
            framerececorre.x = (float)currentFramcorrer * (float)score.width / 12;
            framereccajupit.x = (float)currentFramajupit * (float)p1cbaix.width / 7;
            framereccajupite.x = (float)currentFramajupit * (float)p1cbaixe.width / 7;
            frameRecmgun.x = (float)currentFrameobj * (float)mgun.width / 2;
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

        if (framesCounter >= (60 / framesSpeedtirb))
        {
            currentFrametirb1++;
            if (currentFrametirb1 >= 3) currentFrametirb1 = 0;

            framecrecat1b1.x = (float)currentFrametirb1 * (float)at1b1.width / 3;
            framecrecat1b2.x = (float)currentFrametirb1 * (float)at1b2.width / 3;
            framecrecat1b3.x = (float)currentFrametirb1 * (float)at1b3.width / 3;
            framecrecat1b4.x = (float)currentFrametirb1 * (float)at1b4.width / 3;
            framecrecat1b5.x = (float)currentFrametirb1 * (float)at1b5.width / 3;
            framecrecat1b6.x = (float)currentFrametirb1 * (float)at1b6.width / 3;
        }

        if (p.x > 4200 && p.x < 4510 && p.y <= 1220) FLOOR_Y = 1200;
        if (p.x > 4600 && p.x < 5500 && p.y <= 1000) FLOOR_Y = 1000;
        if (p.x > 5500 && p.x < 5785 && p.y <= 1220) FLOOR_Y = 1200;
        if (p.x > 5800 && p.x < 6470 && p.y <= 1000) FLOOR_Y = 1000;
        if (p.x > 6765 && p.x < 7515 && p.y <= 1000) FLOOR_Y = 1000;
        if (p.x > 7570 && p.x < 7900 && p.y <= 1220) FLOOR_Y = 1200;
        if (p.x > 10750 && p.x < 10900 && p.y <= 780) FLOOR_Y = 780;
        if (p.x > 11250 && p.x < 11600 && p.y <= 1020) FLOOR_Y = 1020;
        if (p.x > 11650 && p.x < 11850 && p.y <= 825) FLOOR_Y = 820;
        if (p.x > 11850 && p.x < 11950 && p.y <= 825) FLOOR_Y = p.x * -0.4 + 5560, rampa = true;
        if (p.x > 11950 && p.x < 12600 && p.y <= 785) FLOOR_Y = 780;
        else if (p.x > 12300 && p.x < 12400 && p.y <= 980) FLOOR_Y = 980;
        else if (p.x > 12150 && p.x < 12350 && p.y <= 1180) FLOOR_Y = 1180;
        if (p.x > 12850 && p.x < 13200 && p.y <= 980) FLOOR_Y = 980;
        if (p.x > 13400 && p.x < 13600 && p.y <= 780) FLOOR_Y = 780;
        if (p.x > 18600 && p.x < 19100 && p.y <= 230) FLOOR_Y = 230;
        else if (p.x > 18950 && p.x < 19150 && p.y <= 550) FLOOR_Y = 550;
        else if (p.x > 19150 && p.x < 19350 && p.y <= 400) FLOOR_Y = 400;
        if (p.x > 19350 && p.x < 19750 && p.y <= 230) FLOOR_Y = 230;
        if (p.x > 19700) p.x = 19700;
        
        if (o1.ox >= p.x && o1.ox <= p.x + 100 && o1.oy >= p.y - 20 && o1.oy <= p.y + 200)
        {
            o1.alive--;
        }

        p.x += p.vx;
        p.y -= p.vy/2;

        if (p.x >= 4500) s2.evx = -5;
        s2.ex += s2.evx;
        if (!inMenu) Jorge.evx = -5;
        Jorge.ex += Jorge.evx;

        if (p.y < FLOOR_Y)
        {
            if (p.canJump != false && p.y > FLOOR_Y - 10 && rampa) p.y = FLOOR_Y;
            if (p.vy > -20) p.vy -= 2;
        }
        else
        {
            p.y = FLOOR_Y;
            p.canJump = true;
            p.vy = 0;
        }

        s1.ey += s1.vy;
        s1.vy += 4;

        if (s1.ey >= 700)
        {
            s1.ey = 700;
            s1.vy = 0;
        }

        s2.ey += s2.vy;
        s2.vy += 4;

        if (s2.ey >= 1380)
        {
            s2.ey = 1380;
            s2.vy = 0;
        }

        Jorge.ey += Jorge.vy;
        Jorge.vy += 4;

        if (Jorge.ey >= FLOOR_Y)
        {
            Jorge.ey = FLOOR_Y;
            Jorge.vy = 0;
        }

        o1.oy += o1.ovy;
        o1.ovy += 4;

        if (o1.oy >= FLOOR_Y)
        {
            o1.oy = FLOOR_Y;
            o1.ovy = 0;
        }

        s3.ey += s3.vy;
        s3.vy += 4;

        if (s3.ey >= 1020)
        {
            s3.ey = 1020;
            s3.vy = 0;
        }

        float camLeft = camera.target.x - (camera.offset.x) / camera.zoom;
        float camRight = camera.target.x + 975;
        float camTop = camera.target.y - (camera.offset.y) / camera.zoom;
        float camBottom = camera.target.y + (camera.offset.y) / camera.zoom;

        if (p.x > 0) FLOOR_Y = 1220;
        if (p.x > 3370) FLOOR_Y = 1380, rampa = false;
        if (p.x > 8900) FLOOR_Y = p.x * -0.8 + 8500, rampa = true;
        if (p.x > 9050) FLOOR_Y = 1260, rampa = false;
        if (p.x > 9400) FLOOR_Y = 1380;
        if (p.x > 10111) FLOOR_Y = 1220;
        if (p.x > 10255) FLOOR_Y = 1020;
        if (p.x > 10700) FLOOR_Y = 1380, rampa = false;
        if (p.x > 16300) FLOOR_Y = p.x * -0.8 + 14420, rampa = true;
        if (p.x > 16550) FLOOR_Y = 1180, rampa = false;
        if (p.x > 16750) FLOOR_Y = p.x * -0.6 + 11230, rampa = true;
        if (p.x > 17000) FLOOR_Y = 1030, rampa = false;
        if (p.x > 17150) FLOOR_Y = p.x * -0.75 + 13892.5, rampa = true;
        if (p.x > 17350) FLOOR_Y = 880, rampa = false;
        if (p.x > 17550) FLOOR_Y = p.x * -0.9 + 16675, rampa = true;
        if (p.x > 17750) FLOOR_Y = 700, rampa = false;

        if (p.x <= camLeft) p.x = camLeft + 5;
        if (p.x <= 3385 && p.y > 1220) p.x = 3390;
        if (p.x >= 9350 && p.x <= 9410 && p.y > 1260) p.x = 9415;
        if (p.x >= 10095 && p.x <= 10150 && p.y > 1220) p.x = 10088;
        if (p.x >= 10245 && p.x <= 10300 && p.y > 1021) p.x = 10238;
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

            if (bulletse[i].useGravity) {
                bulletse[i].vy += 0.5f; //Gravity strength
            }

            bulletse[i].x += bulletse[i].vx;
            bulletse[i].y += bulletse[i].vy;

            if (bulletse[i].x < camLeft || bulletse[i].x > camRight ||
                bulletse[i].y < camTop || bulletse[i].y > camBottom) {
                bulletse[i].active = false;
            }
        }

        for (int i = 0; i < MAX_BULLETSA1; i++) {
            if (!bulletsa1[i].active) continue;

            if (bulletsa1[i].useGravity) {
                bulletsa1[i].vy += 0.5f; //Gravity strength
            }

            bulletsa1[i].x += bulletsa1[i].vx;
            bulletsa1[i].y += bulletsa1[i].vy;

            if (bulletsa1[i].x < camLeft || bulletsa1[i].x > camRight ||
                bulletsa1[i].y < camTop - 200 || bulletsa1[i].y > camBottom) {
                bulletsa1[i].active = false;
            }
        }

        for (int i = 0; i < MAX_BULLETSA2; i++) {
            if (!bulletse[i].active) continue;

            if (bulletsa2[i].useGravity) {
                bulletsa2[i].vy += 0.5f; //Gravity strength
            }

            bulletsa2[i].x += bulletsa2[i].vx;
            bulletsa2[i].y += bulletsa2[i].vy;

            if (bulletsa2[i].x < camLeft || bulletsa2[i].x > camRight ||
                bulletsa2[i].y < camTop || bulletsa2[i].y > camBottom) {
                bulletsa2[i].active = false;
            }
        }

        for (int i = 0; i < MAX_BULLETSA3; i++) {
            if (!bulletsa3[i].active) continue;

            if (bulletsa3[i].useGravity) {
                bulletsa3[i].vy += 0.5f; //Gravity strength
            }

            bulletsa3[i].x += bulletsa3[i].vx;
            bulletsa3[i].y += bulletsa3[i].vy;

            if (bulletsa3[i].x < camLeft || bulletsa3[i].x > camRight ||
                bulletsa3[i].y < camTop || bulletsa3[i].y > camBottom) {
                bulletsa3[i].active = false;
            }
        }

        if (p.x < 0) { p.x = 0; if (p.vx < 0) p.vx = 0; }
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
        float halfW = 975 / 2.0f;
        float halfH = 714 / 2.0f;

        if (camera.target.x >  19000) camera.target.x = 19000; // LOCKS CAMERA ON BOSS AREA. - Aidan.

        if (camera.target.x < halfW)              camera.target.x = halfW;
        if (camera.target.x > worldWidth - halfW) camera.target.x = worldWidth - halfW;

        string ix = to_string(p.x);
        const char* cix = ix.c_str();
        string puntstext = "Punts: ";
        const char* cpuntstext = puntstext.c_str();
        string punts = to_string(vpunts);
        const char* cpunts = punts.c_str();
        
        g.x = p.x;
        g.y = p.y;

        BeginDrawing();

        ClearBackground(BLACK);
        if (hitCooldown > 0.0f) hitCooldown -= GetFrameTime();
        if (shootCooldown > 0.0f) shootCooldown -= GetFrameTime();

        BeginMode2D(camera);
        Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
        Rectangle dest = { 0, 0, bg.width * bgScale, bg.height * bgScale };

        DrawTexturePro(bg, src, dest, { 0,0 }, 0.0f, WHITE);
        Vector2 positiont = {p.x+100, p.y-100 };
        DrawTextEx(timerNums, "0123456789", positiont, 50, 0, WHITE); //-------------------------------------------------------------------------------------------------------------------------------------

        // bales jugador

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;
            
            if (bullets[i].direction == 2)
            {
                DrawTexture(bullet, (int)bullets[i].x, (int)bullets[i].y, WHITE);
            }
            else if (bullets[i].direction == -2)
            {
                DrawTexture(bulletes, (int)bullets[i].x, (int)bullets[i].y, WHITE);
            }
            else if (bullets[i].direction == 1)
            {
                DrawTexture(bulleta, (int)bullets[i].x, (int)bullets[i].y, WHITE);
            }
            else if (bullets[i].direction == -1)
            {
                DrawTexture(bulletb, (int)bullets[i].x, (int)bullets[i].y, WHITE);
            }

            if (bullets[i].x >= s1.ex && bullets[i].x <= s1.ex + 100 && bullets[i].y >= s1.ey && bullets[i].y <= s1.ey + 200)
            {
                s1.ehp--;
                vpunts += 10;
                bullets[i].active = false;
            }
            if (bullets[i].x >= s2.ex && bullets[i].x <= s2.ex + 100 && bullets[i].y >= s2.ey && bullets[i].y <= s2.ey + 200)
            {
                s2.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= Jorge.ex && bullets[i].x <= Jorge.ex + 100 && bullets[i].y >= Jorge.ey && bullets[i].y <= Jorge.ey + 200)
            {
                Jorge.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= s3.ex && bullets[i].x <= s3.ex + 100 && bullets[i].y >= s3.ey && bullets[i].y <= s3.ey + 200)
            {
                s3.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= t1.tx && bullets[i].x <= t1.tx + 100 && bullets[i].y >= t1.ty && bullets[i].y <= t1.ty + 200)
            {
                bullets[i].active = false;
                t1.thp--;
            }
        }

        //bales enemic basic

        for (int i = 0; i < MAX_BULLETSE; i++) {
            if (!bulletse[i].active) continue;
            DrawTexture(bulletee, (int)bulletse[i].x, (int)bulletse[i].y, WHITE);
            if (bulletse[i].x >= p.x && bulletse[i].x <= p.x + 100 && bulletse[i].y >= p.y && bulletse[i].y <= p.y + 200 && p.isajupit == -1 || bulletse[i].x >= p.x && bulletse[i].x <= p.x + 100 && bulletse[i].y >= p.y + 100 && bulletse[i].y <= p.y + 200 && p.isajupit == 1)
            {
                bulletse[i].active = false;
                hitCooldown = 1.5f;
                p.vides--;
                p.vx = 0;
                p.vy = 20;
                if (p.vides <= 0) 
                {
                    p.credits--;
                    p.vides = 3;
                    if (p.credits <= 0)
                    {
                        lose = true;
                    }
                }
            }
        }

        //bales atac 1 jefe (morter) // el fa durant primera fase i nomes si arribes a la segona plataforma durnat 2na fase

        for (int i = 0; i < MAX_BULLETSE; i++) {
            if (!bulletsa1[i].active) continue;
            //atac 1
            Vector2 position = { framecrecat1b1.width * 4.75f / 2, framecrecat1b1.height * 4.75f / 2 };
            Rectangle posat1b1 = { (int)bulletsa1[i].x, (int)bulletsa1[i].y, framecrecat1b1.width * 4.75, framecrecat1b1.height * 4.75 };
            DrawTexturePro(at1b1, framecrecat1b1, posat1b1, position, 0, WHITE);
            //atac2
            Vector2 position2 = { framecrecat1b2.width * 4.75f / 2, framecrecat1b2.height * 4.75f / 2 };
            Rectangle posat1b2 = { (int)bulletsa1[i].x, (int)bulletsa1[i].y + 5, framecrecat1b2.width * 4.75, framecrecat1b2.height * 4.75 };
            DrawTexturePro(at1b2, framecrecat1b2, posat1b2, position2, 0, WHITE);
            //atac3
            Vector2 positio3 = { framecrecat1b3.width * 4.75f / 2, framecrecat1b3.height * 4.75f / 2 };
            Rectangle posat1b3 = { (int)bulletsa1[i].x, (int)bulletsa1[i].y + 10, framecrecat1b3.width * 4.75, framecrecat1b3.height * 4.75 };
            DrawTexturePro(at1b3, framecrecat1b3, posat1b3, positio3, 0, WHITE);
            //atac4
            Vector2 position4 = { framecrecat1b4.width * 4.75f / 2, framecrecat1b4.height * 4.75f / 2 };
            Rectangle posat1b4 = { (int)bulletsa1[i].x, (int)bulletsa1[i].y, framecrecat1b4.width * 4.75, framecrecat1b4.height * 4.75 };
            DrawTexturePro(at1b4, framecrecat1b4, posat1b4, position4, 0, WHITE);
            //atac5
            Vector2 position5 = { framecrecat1b5.width * 4.75f / 2, framecrecat1b5.height * 4.75f / 2 };
            Rectangle posat1b5 = { (int)bulletsa1[i].x, (int)bulletsa1[i].y, framecrecat1b5.width * 4.75, framecrecat1b5.height * 4.75 };
            DrawTexturePro(at1b5, framecrecat1b5, posat1b5, position5, 0, WHITE);
            //atac6
            Vector2 position6 = { framecrecat1b6.width * 4.75f / 2, framecrecat1b6.height * 4.75f / 2 };
            Rectangle posat1b6 = { (int)bulletsa1[i].x, (int)bulletsa1[i].y, framecrecat1b6.width * 4.75, framecrecat1b6.height * 4.75 };
            DrawTexturePro(at1b6, framecrecat1b6, posat1b6, position6, 0, WHITE);
            if (bulletsa1[i].x >= p.x && bulletsa1[i].x <= p.x + 100 && bulletsa1[i].y >= p.y && bulletsa1[i].y <= p.y + 200 && p.isajupit == -1 || bulletsa1[i].x >= p.x && bulletsa1[i].x <= p.x + 100 && bulletsa1[i].y >= p.y + 100 && bulletsa1[i].y <= p.y + 200 && p.isajupit == 1)
            {
                bulletsa1[i].active = false;
                hitCooldown = 1.5f;
                p.vides--;
                p.vx = 0;
                p.vy = 20;
                if (p.vides <= 0)
                {
                    p.credits--;
                    p.vides = 3;
                    if (p.credits <= 0)
                    {
                        lose = true;
                    }
                }
            }
        }

        //bales atac 2 jefe (laser) // durant segona fase l'alterna entre raig i bola vaixa constantment sense importar la distancia

        for (int i = 0; i < MAX_BULLETSE; i++) {
            if (!bulletse[i].active) continue;
            DrawTexture(bulletee, (int)bulletse[i].x, (int)bulletse[i].y, WHITE);
            if (bulletse[i].x >= p.x && bulletse[i].x <= p.x + 100 && bulletse[i].y >= p.y && bulletse[i].y <= p.y + 200 && p.isajupit == -1 || bulletse[i].x >= p.x && bulletse[i].x <= p.x + 100 && bulletse[i].y >= p.y + 100 && bulletse[i].y <= p.y + 200 && p.isajupit == 1)
            {
                bulletse[i].active = false;
                hitCooldown = 1.5f;
                p.vides--;
                p.vx = 0;
                p.vy = 20;
                if (p.vides <= 0)
                {
                    p.credits--;
                    p.vides = 3;
                    if (p.credits <= 0)
                    {
                        lose = true;
                    }
                }
            }
        }

        //bales atac 3 jefe (bola terra)

        for (int i = 0; i < MAX_BULLETSE; i++) {
            if (!bulletsa3[i].active) continue;
            DrawTexture(bulletee, (int)bulletsa3[i].x, (int)bulletsa3[i].y, WHITE);
            if (bulletsa3[i].x >= p.x && bulletsa3[i].x <= p.x + 100 && bulletsa3[i].y >= p.y && bulletsa3[i].y <= p.y + 200 && p.isajupit == -1 || bulletsa3[i].x >= p.x && bulletsa3[i].x <= p.x + 100 && bulletsa3[i].y >= p.y + 100 && bulletsa3[i].y <= p.y + 200 && p.isajupit == 1)
            {
                bulletsa3[i].active = false;
                hitCooldown = 1.5f;
                p.vides--;
                p.vx = 0;
                p.vy = 20;
                if (p.vides <= 0)
                {
                    p.credits--;
                    p.vides = 3;
                    if (p.credits <= 0)
                    {
                        lose = true;
                    }
                }
            }
        }

        if (!inMenu && !winscreen && !lose) {

            if (IsKeyPressed(KEY_L))p.x = 19000; // USED FOR TESTING THE BOSS YOU STUPID ASS HOES
            if (IsKeyPressed(KEY_X))p.x = 16500;

            if (gunCooldown > 0.0f) gunCooldown -= GetFrameTime();

            if (IsKeyPressed(KEY_J) && gunCooldown <= 0.0f ) {
                PlaySound(soundArray[2]);
                p.isshooting = 1;
                gunCooldown = 0.3f;
                currentFramtir = 0;
                if (p.facing == 1) {

                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].x = (float)p.x + 175; 
                            bullets[i].y = (float)p.y + 55; 
                            if (p.isajupit == 1) {
                                bullets[i].y = (float)p.y + 100; 
                                bullets[i].vx = 30.0f;
                                bullets[i].vy = 0;
                            }
                            else if (p.facingy == 1) {
                                bullets[i].x = (float)p.x + 35;
                                bullets[i].y = (float)p.y - 55;
                                bullets[i].vx = 0;
                                bullets[i].vy = -30.0f;
                                bullets[i].direction = 1;
                            }
                            else if (p.facingy == -1) {
                                bullets[i].vx = 0;
                                bullets[i].vy = 30.0f;
                                bullets[i].direction = -1;
                            }
                            else {
                                bullets[i].vx = 30.0f * p.facing;
                                bullets[i].vy = 0;
                                bullets[i].direction = 2 * p.facing;
                            }
                            bullets[i].active = true;
                            break;
                        }
                    }
                }
                else if (p.facing == -1) {
                    for (int i = 0; i < MAX_BULLETS; i++) {
                        if (!bullets[i].active) {
                            bullets[i].x = (float)p.x - 0;
                            bullets[i].y = (float)p.y + 55;

                            if (p.isajupit == 1) {
                                bullets[i].y = (float)p.y + 100;
                                bullets[i].vx = -30.0f;
                                bullets[i].vy = 0;
                            }
                            else if (p.facingy == 1) {
                                bullets[i].x = (float)p.x + 50; 
                                bullets[i].y = (float)p.y - 55;
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
                                bullets[i].direction = 2 * p.facing;
                            }
                            bullets[i].active = true;
                            break;
                        }

                    }

                }
                
            }
        }

        //Item;
        if (o1.alive == 1)
        {
            Vector2 position = { 0.0f, 0.0f };
            Rectangle posmgun = { (float)o1.ox, (float)o1.oy, frameRecmgun.width * 5, frameRecmgun.height * 5 };
            DrawTexturePro(mgun, frameRecmgun, posmgun, position, 0, WHITE);
        }
        else if (os1)
        {
            PlaySound(soundArray[8]);
            vpunts = vpunts + 100;
            os1 = false;
            killhim = true;
            machineGunAmmo = 200;

        }


        if (killhim) 
        {
            if (IsKeyDown(KEY_J) && shootCooldown <= 0.0f && machineGunAmmo > 0) {
                PlaySound(soundArray[2]);
                p.isshooting = 1;
                currentFramtir = 0;
                shootCooldown = 0.10f;
                machineGunAmmo--;

                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!bullets[i].active) {
                        if (p.facing == 1) {
                            bullets[i].x = (float)p.x + 175;
                        }
                        else {
                            bullets[i].x = (float)p.x;
                        }
                        bullets[i].y = (float)p.y + 55;

                        if (p.isajupit == 1) {
                            bullets[i].y = (float)p.y + 100;
                            bullets[i].vx = 30.0f * p.facing;
                            bullets[i].vy = 0;
                            bullets[i].direction = 2 * p.facing;
                        }
                        else if (p.facingy == 1) {
                            bullets[i].x = (float)p.x + (p.facing == 1 ? 35 : 50);
                            bullets[i].y = (float)p.y - 55;
                            bullets[i].vx = 0;
                            bullets[i].vy = -30.0f;
                            bullets[i].direction = 1;
                        }
                        else if (p.facingy == -1) {
                            bullets[i].vx = 0;
                            bullets[i].vy = 30.0f;
                            bullets[i].direction = -1;
                        }
                        else {
                            bullets[i].vx = 30.0f * p.facing;
                            bullets[i].vy = 0;
                            bullets[i].direction = 2 * p.facing;
                        }

                        bullets[i].active = true;
                        break;
                    }
                }
            }

            if (machineGunAmmo <= 0) {
                killhim = false;
            }
        }


        if (s1.ehp >= 1) 
        {
            if (s1.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidles1 = { (float)s1.ex, (float)s1.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidles1, position, 0, WHITE);
                DrawText(cix, s1.ex, s1.ey, 20, RED);
            }
            else if (s1.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidles1 = { (float)s1.ex, (float)s1.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidles1, position, 0, WHITE);
                DrawText(cix, s1.ex, s1.ey, 20, RED);
            }

            if (s1.ehp > 75)
            {
                if (!inMenu && !winscreen && !lose)
                {
                    s1.enemyShootTimer += GetFrameTime();

                    if (s1.enemyShootTimer >= enemyShootInterval)
                    {
                        s1.enemyShootTimer = 0.0f;
                        s1.burstCount = 3;
                    }

                    if (s1.burstCount > 0)
                    {
                        s1.burstTimer += GetFrameTime();
                        if (s1.burstTimer >= 0.4f)
                        {
                            s1.burstTimer = 0.0f;
                            s1.burstCount--;

                            for (int i = 0; i < MAX_BULLETSE; i++)
                            {
                                if (!bulletsa1[i].active)
                                {
                                    bulletsa1[i].x = s1.ex;
                                    bulletsa1[i].y = s1.ey + 30;

                                    float gravity = 0.5f;
                                    float vy = -22.5f;

                                    float spread = 200.0f;
                                    float step = spread / 4.0f;
                                    float targetX = (p.x ) + s1.burstCount * step;
                                    float targetY = p.y;

                                    float dy = targetY - bulletsa1[i].y;


                                    float timeOfFlight = (-2.0f * vy) / gravity;
                                    bulletsa1[i].vx = (targetX - s1.ex) / timeOfFlight;
                                    bulletsa1[i].vy = (dy - 0.5f * gravity * timeOfFlight * timeOfFlight) / timeOfFlight;

                                    bulletsa1[i].useGravity = true; // TURNS ON GRAVITY GILIPOLLAS!
                                    bulletsa1[i].active = true;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else if (s1.ehp <= 75)
            {
                
                    if (!inMenu && !winscreen && !lose)
                    {
                        if (p.y <= 400) {


                            s1.enemyShootTimer += GetFrameTime();

                            if (s1.enemyShootTimer >= enemyShootInterval)
                            {
                                s1.enemyShootTimer = 0.0f;
                                s1.burstCount = 3;
                            }

                            if (s1.burstCount > 0)
                            {
                                s1.burstTimer += GetFrameTime();
                                if (s1.burstTimer >= 0.4f)
                                {
                                    s1.burstTimer = 0.0f;
                                    s1.burstCount--;

                                    for (int i = 0; i < MAX_BULLETSE; i++)
                                    {
                                        if (!bulletsa1[i].active)
                                        {
                                            bulletsa1[i].x = s1.ex;
                                            bulletsa1[i].y = s1.ey + 30;

                                            float gravity = 0.5f;
                                            float vy = -22.5f;
                                            float targetY = p.y;

                                            float dy = targetY - bulletsa1[i].y;

                                            float spread = 200.0f;
                                            float step = spread / 4.0f;
                                            float targetX = (p.x) + s1.burstCount * step;

                                            float timeOfFlight = (-2.0f * vy) / gravity;
                                            bulletsa1[i].vx = (targetX - s1.ex) / timeOfFlight;
                                            bulletsa1[i].vy = (dy - 0.5f * gravity * timeOfFlight * timeOfFlight) / timeOfFlight;

                                            bulletsa1[i].useGravity = true; // TURNS ON GRAVITY GILIPOLLAS!
                                            bulletsa1[i].active = true;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        else {
                            if (raig && !bola) {

                                //------------------------- FAST
                                s1.enemyShootTimer += GetFrameTime();

                                // Dispara cada X segons
                                if (s1.enemyShootTimer >= enemyShootInterval)
                                {
                                    s1.enemyShootTimer = 0.0f;

                                    // Posicio inicial
                                    for (int i = 0; i < MAX_BULLETSE; i++)
                                    {
                                        if (!bulletse[i].active)
                                        {
                                            bulletse[i].x = s1.ex;
                                            bulletse[i].y = s1.ey - 25 ;

                                            // Direccio cap al jugador
                                            float dx = p.x - s1.ex - 20;
                                            float dy = p.y - s1.ey;

                                            // Longitud del vector
                                            float length = sqrt(dx * dx + dy * dy);

                                            // Normalitzar + velocitat lenta
                                            float speed = 20.0f;

                                            bulletse[i].vx = (dx / length) * speed;

                                            bulletse[i].active = true;
                                            bulletse[i].useGravity = false;

                                            raig = false;
                                            bola = true;
                                            break;
                                        }
                                    }
                                }

                            }
                            else if (bola && !raig) {
                                s1.enemyShootTimer += GetFrameTime();

                                // Dispara cada X segons
                                if (s1.enemyShootTimer >= enemyShootInterval)
                                {
                                    s1.enemyShootTimer = 0.0f;

                                    for (int i = 0; i < MAX_BULLETSE; i++)
                                    {
                                        if (!bulletse[i].active)
                                        {
                                            // Posicio inicial
                                            bulletse[i].x = s1.ex;
                                            bulletse[i].y = s1.ey + 100;

                                            // Direccio cap al jugador
                                            float dx = p.x - s1.ex;
                                            float dy = p.y - s1.ey;

                                            // Longitud del vector
                                            float length = sqrt(dx * dx + dy * dy);

                                            // Normalitzar + velocitat lenta
                                            float speed = 8.0f;

                                            bulletse[i].vx = (dx / length) * speed;

                                            bulletse[i].active = true;
                                            bulletse[i].useGravity = false;

                                            raig = true;
                                            bola = false;

                                            break;
                                        }
                                    }
                                }
                            }
                            

                        }

                    }
                


                
            }
        }


        // BERNAT DO NOT DELETE, ES FARA SERVIR PER ELS ENEMICS.
        //if (!inMenu && !winscreen && !lose)
        //{
        //    s1.enemyShootTimer += GetFrameTime();
        //    if (s1.enemyShootTimer >= enemyShootInterval)
        //    {
        //        s1.enemyShootTimer = 0.0f;

        //        for (int i = 0; i < MAX_BULLETSE; i++)
        //        {
        //            if (!bulletse[i].active)
        //            {
        //                bulletse[i].x = s1.ex;
        //                bulletse[i].y = s1.ey + 30;

        //                float gravity = 0.5f;   // matches bullet update loop, WHY ARE YOU GAY?
        //                float vy = -12.0f; // Arc parabola

        //                // Frames until bullet returns to same Y
        //                float timeOfFlight = (-2.0f * vy) / gravity; // = 48 frames

        //                // vx needed to land exactly on player's X
        //                bulletse[i].vx = (p.x - s1.ex) / timeOfFlight;
        //                bulletse[i].vy = vy;

        //                bulletse[i].useGravity = true; // ACTIVA LA GRAVETAT GILIPOLLAS
        //                bulletse[i].active = true;
        //                break;
        //            }
        //        }
        //    }
        //}
        //}


        else if (KevinTheFuckingBoss)
        {
            vpunts = vpunts + 1000;
            KevinTheFuckingBoss = false;
            PlaySound(soundArray[0]);
            
            winscreen = true;
        }

        if (s2.ehp == 1) 
        {
            
            if (s2.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidles1 = { (float)s2.ex, (float)s2.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidles1, position, 0, WHITE);
                DrawText(cix, s2.ex, s2.ey, 20, RED);
            }
            else if (s2.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)s2.ex, (float)s2.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, s2.ex, s2.ey, 20, RED);
            }

            if (!inMenu && !winscreen && !lose) 
            {
                    s2.enemyShootTimer += GetFrameTime();
                    if (s2.enemyShootTimer >= enemyShootInterval)
                    {
                        s2.enemyShootTimer = 0.0f;

                        for (int i = 0; i < MAX_BULLETSE; i++)
                        {
                            if (!bulletse[i].active)
                            {
                                bulletse[i].x = s2.ex;
                                bulletse[i].y = s2.ey + 30;

                                float gravity = 0.5f;   // matches bullet update loop, WHY ARE YOU GAY?
                                float vy = -12.0f; // Arc parabola

                                // Frames until bullet returns to same Y
                                float timeOfFlight = (-2.0f * vy) / gravity; // = 48 frames

                                // vx needed to land exactly on player's X
                                bulletse[i].vx = (p.x - s2.ex) / timeOfFlight;
                                bulletse[i].vy = vy;

                                bulletse[i].useGravity = true; // ACTIVA LA GRAVETAT GILIPOLLAS
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
            s2.ex = 100000000;
            PlaySound(soundArray[0]);
            bs2 = false;
        }

        if (Jorge.ehp == 1)
        {

            if (Jorge.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidles1 = { (float)Jorge.ex, (float)Jorge.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidles1, position, 0, WHITE);
                DrawText(cix, Jorge.ex, Jorge.ey, 20, RED);
            }
            else if (Jorge.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)Jorge.ex, (float)Jorge.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, Jorge.ex, Jorge.ey, 20, RED);
            }

            if (!inMenu && !winscreen && !lose)
            {
                Jorge.enemyShootTimer += GetFrameTime();
                if (Jorge.enemyShootTimer >= enemyShootInterval)
                {
                    Jorge.enemyShootTimer = 0.0f;

                    for (int i = 0; i < MAX_BULLETSE; i++)
                    {
                        if (!bulletse[i].active)
                        {
                            bulletse[i].x = Jorge.ex;
                            bulletse[i].y = Jorge.ey + 30;

                            float gravity = 0.5f;   // matches bullet update loop, WHY ARE YOU GAY?
                            float vy = -12.0f; // Arc parabola

                            // Frames until bullet returns to same Y
                            float timeOfFlight = (-2.0f * vy) / gravity; // = 48 frames

                            // vx needed to land exactly on player's X
                            bulletse[i].vx = (p.x - Jorge.ex) / timeOfFlight;
                            bulletse[i].vy = vy;

                            bulletse[i].useGravity = true; // ACTIVA LA GRAVETAT GILIPOLLAS
                            bulletse[i].active = true;
                            break;
                        }
                    }
                }
            }
        }
        else if (bJorge)
        {
            vpunts = vpunts + 10;
            PlaySound(soundArray[0]);
            Jorge.ex = 100000000;
            bJorge = false;
        }

        if (s3.ehp == 1) {
            Vector2 position = { 0.0f, 0.0f };
            Rectangle posidles3 = { (float)s3.ex, (float)s3.ey, framereceidle.width * 5, framereceidle.height * 5 };
            DrawTexturePro(sidle, framereceidle, posidles3, position, 0, WHITE);
            DrawText(cix, s3.ex, s3.ey, 20, RED);
            if (!inMenu && !winscreen && !lose) 
            {
                s3.enemyShootTimer += GetFrameTime();
                if (s3.enemyShootTimer >= enemyShootInterval)
                {
                    s3.enemyShootTimer = 0.0f;

                    for (int i = 0; i < MAX_BULLETSE; i++)
                    {
                        if (!bulletse[i].active)
                        {
                            bulletse[i].x = s3.ex;
                            bulletse[i].y = s3.ey + 30;

                            float gravity = 0.5f;   // matches bullet update loop, WHY ARE YOU GAY?
                            float vy = -12.0f; // Arc parabola

                            // Frames until bullet returns to same Y
                            float timeOfFlight = (-2.0f * vy) / gravity; // = 48 frames

                            // vx needed to land exactly on player's X
                            bulletse[i].vx = (p.x - s3.ex) / timeOfFlight;
                            bulletse[i].vy = vy;

                            bulletse[i].useGravity = true; // ACTIVA LA GRAVETAT GILIPOLLAS
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
            PlaySound(soundArray[0]);
            s3.ex = 100000000;
            bs3 = false;
        }


        if (t1.thp >= 1)
        {
            if (t1.tvx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidles1 = { (float)t1.tx, (float)t1.ty, framerectbase.width * 5, framerectbase.height * 5 };
                DrawTexturePro(tbase, framerectbase, posidles1, position, 0, WHITE);
                DrawText(cix, t1.tx, t1.ty, 20, RED);
            }
            else if (t1.tvx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)t1.tx, (float)t1.ty, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, t1.tx, t1.ty, 20, RED);
            }

            if (!inMenu && !winscreen && !lose)
            {
                t1.tankShootTimer += GetFrameTime();
                if (t1.tankShootTimer >= enemyShootInterval)
                {
                    t1.tankShootTimer = 0.0f;
                    for (int i = 0; i < MAX_BULLETSE; i++)
                    {
                        if (!bulletse[i].active) {
                            bulletse[i].x = t1.tx;
                            bulletse[i].y = t1.ty + 30;
                            bulletse[i].vx = (p.x < t1.tx) ? -8.0f : 8.0f;
                            bulletse[i].vy = 0;
                            bulletse[i].useGravity = true;
                            bulletse[i].active = true;
                            break;
                        }
                    }
                }
            }
        }
        else if (bt1)
        {
            vpunts = vpunts + 100;
            t1.tx = 100000000;
            PlaySound(soundArray[0]);
            bt1 = false;
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
                Rectangle posdretacorrent = { (float)p.x - 5, (float)p.y + 108, frameRecdretacorrent.width * 4.75, frameRecdretacorrent.height * 4.75 };
                Rectangle poshota = { (float)p.x - 10, (float)p.y - 210, framerecalttir.width * 5, framerecalttir.height * 5 };
                DrawTexturePro(p1alttir, framerecalttir, poshota, position, 0, WHITE);
                DrawTexturePro(p1dretacorrentcames, frameRecdretacorrent, posdretacorrent, position, 0, WHITE);
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
                Rectangle posesquerracorrent = { (float)p.x - 30, (float)p.y + 108,  frameesquerracorrent.width * 4.75, frameesquerracorrent.height * 4.75 };
                Rectangle poshotae = { (float)p.x - 50, (float)p.y - 210, framerecalttire.width * 5, framerecalttire.height * 5 };
                DrawTexturePro(p1alttire, framerecalttire, poshotae, position, 0, WHITE);
                DrawTexturePro(p1esquerracorrentcames, frameesquerracorrent, posesquerracorrent, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.canJump == false && p.facing == 1 && p.isshooting == 1 && p.facingy == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscamess = { (float)p.x, (float)p.y + 90, framerecscames.width * 5, framerecscames.height * 5 };
                Rectangle poshota = { (float)p.x - 10, (float)p.y - 220, framerecalttir.width * 5, framerecalttir.height * 5 };
                DrawTexturePro(p1alttir, framerecalttir, poshota, position, 0, WHITE);
                DrawTexturePro(p1scames, framerecscames, poscamess, position, 0, WHITE);
                DrawText(cix, p.x, p.y, 20, RED);
            }
            else if (p.canJump == false && p.facing == -1 && p.isshooting == 1 && p.facingy == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscamesse = { (float)p.x, (float)p.y + 90, framerecscamese.width * 5, framerecscamese.height * 5 };
                Rectangle poshotae = { (float)p.x - 10, (float)p.y - 210, framerecalttire.width * 5, framerecalttire.height * 5 };
                DrawTexturePro(p1alttire, framerecalttire, poshotae, position, 0, WHITE);
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
                Rectangle posaju = { (float)p.x, (float)p.y + 70, framereccajupit.width * 5, framereccajupit.height * 5 };
                DrawTexturePro(p1cbaix, framereccajupit, posaju, position, 0, WHITE);
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
                DrawTexturePro(p1cbaixe, framereccajupite, posajue, position, 0, WHITE);
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

        if (logoscreen) {

            float width = logo.width * 2;
            float height = logo.height * 2;

            Rectangle dest4 = {
                (975 - width) / 2,
                (714 - height) / 2 - 100,
                width,
                height
            };
            Rectangle src4 = { 0, 0, (float)logo.width, (float)logo.height };

            if (!winSoundPlayed) {

                PlaySound(soundArray[6]);
                winSoundPlayed = true;
            }            
            DrawRectangle(0, 0, 975, 714, BLACK);
            DrawTexturePro(logo, src4, dest4, { 0, 0 }, 0.0f, WHITE);
            DrawText("PRESS ENTER TO START", 250, 500, 40, RED);

            DrawText("                        Explota Studio's recreation of Metal Slug\n Projecte 1 - Grau en Disseny i Desenvolupament de Videojocs - UPC Terrassa\n             Aidan Herzog - Pau Fornons - Emma Riera - Bernat Deu\n                                 Tutor: Alejandro Paris", 225, 550, 15, RED);

            if (IsKeyPressed(KEY_ENTER))
            {
                UnloadTexture(logo);
                logoscreen = false;
                inMenu = true;
                winSoundPlayed = false;

            }
        }
        else if (inMenu && !logoscreen) {



            if (!menuSoundPlayed) {
                PlaySound(soundArray[3]);
                menuSoundPlayed = true;
            }

            Rectangle src2 = { 0, 0, (float)start.width, (float)start.height };
            Rectangle dest2 = { 0, 0, 975, 714 };

            DrawTexturePro(start, src2, dest2, { 0, 0 }, 0.0f, WHITE);

            int textWidth = MeasureText(cpunts, 30);


            DrawText(TextFormat("%i", p.credits), 975 - textWidth - 100, 714 - 100, 40, RED);
            if (IsKeyPressed(KEY_C))
                p.credits++;
            if (IsKeyPressed(KEY_ENTER) && p.credits > 0)
            {
                p.credits--;
                inMenu = false;
            }
        }
        else if (!inMenu && !logoscreen) {

            int textWidth = MeasureText(cpunts, 30);

            DrawText(TextFormat("%i", p.credits), 975 - textWidth - 100, 714 - 100, 40, RED);
            DrawText(cpuntstext, 975 - textWidth - 140, 20, 30, RED);
            DrawText(cpunts, 975 - textWidth - 40, 20, 30, RED);
            DrawText(TextFormat("%i", p.vides), 20, 20, 30, RED);

            if (killhim) {

                DrawText(TextFormat("Bullets: %i", machineGunAmmo), 50, 30, 20, RED);
            }

            if (music == false) {
            }
            else {
                UpdateMusicStream(musicArray[0]);
            }
            if (IsKeyPressed(KEY_M)) {
                SetSoundVolume(soundArray[0], 0.0f);
                SetSoundVolume(soundArray[1], 0.0f);
                SetSoundVolume(soundArray[2], 0.0f);
                SetSoundVolume(soundArray[4], 0.0f);
                SetSoundVolume(soundArray[5], 0.0f);

                PauseMusicStream(musicArray[0]);
                music = false;
            }
            if (IsKeyPressed(KEY_N)) {

                SetSoundVolume(soundArray[0], 1.0f);
                SetSoundVolume(soundArray[1], 1.0f);
                SetSoundVolume(soundArray[2], 1.0f);
                SetSoundVolume(soundArray[4], 1.0f);
                SetSoundVolume(soundArray[5], 1.0f);

                ResumeMusicStream(musicArray[0]);
                music = true;
            }

            if (IsKeyPressed(KEY_C)) p.credits++;

            if (IsKeyPressed(KEY_R)) {
                lose = false;
                vpunts -= vpunts;
                KevinTheFuckingBoss = true;
                bs2 = true;
                bs3 = true;
                bJorge = true;
                os1 = true;
                winscreen = false;
                s1.ehp = 10;
                s2.ehp = 1;
                killhim = false;
                s3.ehp = 1;
                t1.thp = 50;
                bt1 = true;
                o1.alive = 1;
                Jorge.ehp = 1;
                Jorge.ex = 3200;
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
                s3.ehp = 1;

                camera.target.x = p.x;
                camera.target.y = 1100;
            }


            DrawText(TextFormat("%d", (int)vidaTimer.lifetime), 975 / 2, 20, 30, RED);
            updatetimer(&vidaTimer);

            if ((int)vidaTimer.lifetime == 0)
            {
                lose = true;
            }

            float maxSpeed =
                (p.isajupit == 1) ? 4 :
                (!p.canJump) ? 10 :
                8;

            if (p.vx > maxSpeed) p.vx = maxSpeed;
            if (p.vx < -maxSpeed) p.vx = -maxSpeed;

            if (IsKeyDown(KEY_D) && p.vx < maxSpeed && !IsKeyDown(KEY_A))
            {
                p.vx += 5;
                p.facing = 1;
            }
            else if (IsKeyDown(KEY_A) && p.vx > -maxSpeed && !IsKeyDown(KEY_D))
            {
                p.vx -= 5;
                p.facing = -1;
            }
            else if (!IsKeyDown(KEY_D) && !IsKeyDown(KEY_A))
            {
                p.vx = 0;
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
            Rectangle dest3 = { 0, 0, 975, 714 };
            DrawTexturePro(win, src3, dest3, { 0, 0 }, 0.0f, WHITE);
            DrawText("PRESS P TO RETURN TO MENU\nPRESS R TO RESTART LEVEL", 375, 800, 40, RED);
            if (IsKeyPressed(KEY_P)) {
                winscreen = false;
                inMenu = true;
                winSoundPlayed = false;
                menuSoundPlayed = false;
                lose = false;
                vpunts -= vpunts;
                KevinTheFuckingBoss = true;
                bs2 = true;
                bs3 = true;
                bJorge = true;
                os1 = true;
                winscreen = false;
                killhim = false;
                s1.ehp = 100;
                p.vides = 3;
                s2.ehp = 1;
                s3.ehp = 1;
                o1.alive = 1;
                Jorge.ehp = 1;
                Jorge.ex = 3200;
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
                s3.ehp = 1;

                camera.target.x = p.x;
                camera.target.y = 1100;
            }
        }
        if (lose == true) {
            if (p.credits < 0) p.credits = 0;
            PauseMusicStream(musicArray[0]);
            if (!winSoundPlayed) {
                PlaySound(soundArray[7]);

                PlaySound(soundArray[5]);
                winSoundPlayed = true;
            }
            Rectangle src3 = { 0, 0, (float)gameover.width, (float)gameover.height };
            Rectangle dest3 = { 0, 0, 975, 714 };
            DrawTexturePro(gameover, src3, dest3, { 0, 0 }, 0.0f, WHITE);
            DrawText("PRESS P TO RETURN TO MENU\nPRESS R TO RESTART LEVEL", 375, 800, 40, RED);
            if (IsKeyPressed(KEY_P)) {
                winscreen = false;
                menuSoundPlayed = false;
                inMenu = true;
                lose = false;
                vpunts -= vpunts;
                KevinTheFuckingBoss = true;
                bs2 = true;
                bs3 = true;
                bJorge = true;
                os1 = true;
                winscreen = false;
                s1.ehp = 100;
                s2.ehp = 1;
                s3.ehp = 1;
                p.vides = 3;
                o1.alive = 1;
                killhim = false;
                Jorge.ehp = 1;
                Jorge.ex = 3200;
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
                s3.ehp = 1;

                camera.target.x = p.x;
                camera.target.y = 1100;
            }
        }

        if (winscreen || lose) {
            for (int i = 0; i < MAX_BULLETS; i++) {
                bullets[i].active = false;
            }
            for (int i = 0; i < MAX_BULLETSE; i++) {
                bulletse[i].active = false;
            }
        }

        g.Allahuakbar(p);
        DrawRectangle(p.x, p.y, 100, 100, RED);

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
    UnloadTexture(logo);
    UnloadTexture(bullete); 
    UnloadTexture(scor);
    UnloadTexture(bulletes);
    UnloadTexture(bulleta);
    UnloadTexture(bulletb);
    UnloadTexture(bulletee);
    UnloadTexture(score);
    UnloadTexture(p1cbaix);
    UnloadTexture(p1cbaixe);
    UnloadTexture(mgun);
    UnloadTexture(at1b1);
    UnloadTexture(at1b2);
    UnloadTexture(at1b3);
    UnloadTexture(at1b4);
    UnloadTexture(at1b5);
    UnloadTexture(at1b6);
    CloseWindow();
    return 0;
}
// Copyright (c) 2026 Explota Studio
// Licensed under the MIT License