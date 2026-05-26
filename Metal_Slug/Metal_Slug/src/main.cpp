#include "raylib.h"
#include "resource_dir.h"
#include <string>
#include <raymath.h>
using namespace std;
#define MAX_FRAME_SPEED 15
#define MIN_FRAME_SPEED  1
Sound soundArray[20];
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
    bool Omniman = false;
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
    float shootPauseTimer = 0.0f;
    int isshooting = -1; // 1 = si, -1 = no
    float grenadetimer = 0.0f;
    int currentframegranada = 0;
    bool tirgrenada = false;
};

class tank
{
public:
    float tx;
    int ty;
    int tvy;
    int tvx;
    int thp = 20;
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

class edificis
{
public:
    float edx;
    int edy;
    int edhp;
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
struct granada {
    float x, y;
    float vx;
    float vy;
    int direction;
    bool useGravity;
    bool active;
};

struct Bullete {
    float x, y;
    float vx;
    float vy;

    bool useGravity;
    bool active;
    bool boom;

    int currentframeexplo = 0;
    float timerexplo = 0.0f;
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

struct BulletsTank {
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
    bool potatoBullets = false;
    bool rampa = false;
    int konami = 0;
    int missionSoundStep = -1;
    float missionSoundDelay = 0.0f;
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
    soundArray[9] = LoadSound("LONG.mp3");
    soundArray[10] = LoadSound("MISSION.mp3");
    soundArray[11] = LoadSound("one.mp3");
    soundArray[12] = LoadSound("START.mp3");
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
    Texture potato = LoadTexture("bullet_the_OG.png");
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
    Texture porxoavio = LoadTexture("avioporxos.png");
    Texture peix = LoadTexture("peix.png");
    Texture edg1 = LoadTexture("edg1.png");
    Texture edg2 = LoadTexture("edg2.png");
    Texture edg3 = LoadTexture("edg3.png");
    Texture edg4 = LoadTexture("edg4.png");
    Texture edv1 = LoadTexture("edv1.png");
    Texture edv2 = LoadTexture("edv2.png");
    Texture edv3 = LoadTexture("edv3.png");
    Texture sgranada = LoadTexture("soldat granada.png");
    Texture sgranadap = LoadTexture("granada 2.png");
    Texture sgranadaex = LoadTexture("exposio granada.png");

    //BOSS
    Texture laser = LoadTexture("dixparar laser (ha d'estar a dalt).png");
    Texture bolaa = LoadTexture("disparar bola de terra.png");
    Texture morter_down = LoadTexture("disparar boss (quan es troba a baix).png");
    Texture morter_up = LoadTexture("disparar boss (quan es troba a dalt).png");



    Font timerNums = LoadFont("prova 2 tipografia.png");
    Font whiteFont = LoadFont("tipografia gris.png");
    Font yellowFont = LoadFont("nums1.png");

    int timerlife = 450;
    Timer vidaTimer = { 0 };
    startTimer(&vidaTimer, timerlife);
    Timer omnimanTimer = { 0 };


    float gunCooldown = 0.0f;

    float shootCooldown = 0.0f;
    int machineGunAmmo = 0;
    

    const float bgScale = 5.0f;
    const int   worldWidth = (int)(bg.width * bgScale);
    const int   worldHeight = (int)(bg.height * bgScale);
    int   FLOOR_Y = 1300;

    player p = { 400, 1220 , 0, 0, true };

    Raig r = {1985, 605};

    boss s1 = { 19750, 605 };
    soldier s2 = { 5450, 605 };
    soldier s3 = { 10450, 605 };
    soldier Jorge = { 3200, 800 };
    tank t1 = { 17500, 400 };
    soldier s4 = { 4750, 1000 };
    soldier s5 = { 6000, 1000 };
    soldier s6 = { 8850, 1380 };
    soldier s7 = { 12250, 780 };
    soldier s8 = { 13600, 780 }; 
    soldier s9 = { 15250, 900 };
    soldier cheat = { -10000, 0 };

    objecte o1 = { 5250, 605 };

    objecte o2 = { 12300, 605 };

    objecte ocheat = { -10000, 0 };

    edificis ed1 = { 13850, 680 , 40};

    edificis ed2 = { 14500, 550 , 30 };

    bool ed2b = true;

    bool ed1b = true;

    bool os1 = true;
    bool bocheat = true;
    bool os2 = true;
    bool killhim = false;

    bool KevinTheFuckingBoss = true;
    bool bs2 = true;
    bool bs3 = true;
    bool bJorge = true;
    bool bt1 = true;
    bool bs4 = true;
    bool bs5 = true;
    bool bs6 = true;
    bool bs7 = true;
    bool bs8 = true;
    bool bs9 = true;
    bool bcheat = true;

    Camera2D camera = { 0 };
    camera.offset = { 315, 350 };
    camera.rotation = 0.0f;
    camera.zoom = 0.65f;

    Rectangle framerectpeix = { 0, 0, (float)peix.width / 3, (float)peix.height };

    Rectangle framerectbase = { 0, 0, (float)tbase.width / 4, (float)tbase.height };
    Rectangle framereceidle = { 0, 0, (float)sidle.width / 4, (float)sidle.height };
    Rectangle framerececorr = { 0, 0, (float)scor.width / 12, (float)scor.height };
    Rectangle framerececorre = { 0, 0, (float)score.width / 12, (float)score.height };
    Rectangle framerecsgranad = { 0, 0, (float)sgranada.width / 16, (float)sgranada.height };
    Rectangle framerecsgranadp = { 0, 0, (float)sgranadap.width / 3, (float)sgranadap.height };
    Rectangle framrecsgranadex = { 0, 0, (float)sgranadaex.width / 6, (float)sgranadaex.height };

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

    Rectangle framerecmorterup = { 0, 0, (float)morter_up.width / 17, (float)morter_up.height };
    Rectangle framerecmorterdown = { 0, 0, (float)morter_down.width / 17, (float)morter_down.height };
    Rectangle framerecLaser = { 0, 0, (float)laser.width / 10, (float)laser.height };
    Rectangle framerecBola = { 0, 0, (float)bolaa.width / 10, (float)bolaa.height };

    int currentFrameidle = 0;
    int currentFramcorrer = 0;
    int currentFramsalt = 0;
    int currentFramtir = 0;
    int currentFramajupit = 0;
    int currentFrameobj = 0;
    int currentFrametirb1 = 0;
    int currentframeexplo = 0;
    int framesCounter = 0;
    int framesSpeed = 3;
    int framesSpeedtir = 4;
    int framesSpeedtirb = 4;
    int framesSpeedgranada = 6.1;
    int framesSpeedexplosio = 6;


    // BOSS 
    int   currentFrameBoss = 0;
    float bossFrameTimer = 0.0f;
    float bossFrameSpeed = 4.0f;   // frames per second — fast, fluid
    float bossFrameSpeedLaser = 3.0f;
    int   bossAnim = 0;             // 0=morter_up, 1=morter_down, 2=laser, 3=bola
    int   bossLastAnim = -1;        // track anim switches to reset frame counter
    bool  bossBulletFired = false;  // fire bullet once per animation cycle

    const int MAX_BULLETSE = 100;
    Bullete bulletse[MAX_BULLETSE] = {};
    const int MAX_BULLETS = 100;
    Bullet bullets[MAX_BULLETS] = {};
    const int MAX_BULLETSA1 = 3;
    Bulleta1 bulletsa1[MAX_BULLETSA1] = {};
    const int MAX_BULLETSA2 = 1;
    Bulleta2 bulletsa2[MAX_BULLETSA2] = {};
    const int MAX_BULLETSA3 = 1;
    Bulleta3 bulletsa3[MAX_BULLETSA3] = {};
    const int MAX_BULLETSTANK = 1;
    Bulleta3 bulletstank[MAX_BULLETSA3] = {};


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
        currentframeexplo++;
        if (currentframeexplo >= 6) currentframeexplo = 0;

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
        framrecsgranadex.x = (float)currentframeexplo * (float)sgranadaex.width / 6;

    }

    float currentBossSpeed = (bossAnim == 2 || bossAnim == 3) ? bossFrameSpeedLaser : bossFrameSpeed;

    bossFrameTimer += GetFrameTime();
    if (bossFrameTimer >= 1.0f / currentBossSpeed)
    {
        bossFrameTimer = 0.0f;

        // Reset frame counter when animation changes
        if (bossAnim != bossLastAnim)
        {
            currentFrameBoss = 0;
            bossBulletFired = false;
            bossLastAnim = bossAnim;
        }

        int maxFrames = (bossAnim == 2 || bossAnim == 3) ? 10 : 17;
        currentFrameBoss++;
        if (currentFrameBoss >= maxFrames)
        {
            currentFrameBoss = 0;
            bossBulletFired = false;  // allow next cycle to fire again
        }

        // Update the correct rectangle for whichever anim is active
        framerecmorterup.x = (float)currentFrameBoss * morter_up.width / 17;
        framerecmorterdown.x = (float)currentFrameBoss * morter_down.width / 17;
        framerecLaser.x = (float)currentFrameBoss * laser.width / 10;
        framerecBola.x = (float)currentFrameBoss * bolaa.width / 10;

        if (s1.ehp >= 1 && !inMenu && !winscreen && !lose &&
            (bossAnim == 0 || bossAnim == 1) &&
            (currentFrameBoss == 11 || currentFrameBoss == 12 || currentFrameBoss == 13))
        {
            for (int i = 0; i < MAX_BULLETSA1; i++)
            {
                if (!bulletsa1[i].active)
                {
                    if (bossAnim == 0)  // morter_up: cannon tip is high on the sprite
                    {
                        bulletsa1[i].x = s1.ex + 200.0f;
                        bulletsa1[i].y = s1.ey - 100.0f;
                    }
                    else  // morter_down: cannon tip is lower (sprite drawn at s1.ey + 150)
                    {
                        bulletsa1[i].x = s1.ex + 150.0f;
                        bulletsa1[i].y = s1.ey + 400.0f;
                    }
                    float gravity = 0.5f;
                    float vy = -22.5f;
                    float timeOfFlight = (-2.0f * vy) / gravity;
                    float spread = 200.0f;
                    float step = spread / 4.0f;
                    float targetX = p.x + step;
                    float targetY = p.y;
                    float dy = targetY - bulletsa1[i].y;
                    bulletsa1[i].vx = (targetX - bulletsa1[i].x) / timeOfFlight;
                    bulletsa1[i].vy = (dy - 0.5f * gravity * timeOfFlight * timeOfFlight) / timeOfFlight;
                    bulletsa1[i].useGravity = true;
                    bulletsa1[i].active = true;
                    break;
                }
            }
        }
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

        framerecsgranadp.x = (float)currentFrametirb1 * (float)sgranadap.width / 3;

        framerectpeix.x = (float)currentFrametirb1 * (float)peix.width / 3;
    }

    if (s2.isshooting == -1)
    {
        s2.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - s2.ex) * (p.x - s2.ex) + (p.y - s2.ey) * (p.y - s2.ey));

        if (s2.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            s2.enemyShootTimer = 0.0f;

            s2.isshooting = 1;
            s2.currentframegranada = 0;
            s2.tirgrenada = false;

            s2.shootPauseTimer = 16.0f / framesSpeedgranada;
            s2.evx = 0;
        }
    }

    if (s2.isshooting == 1)
    {
        s2.grenadetimer += GetFrameTime();

        if (s2.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            s2.grenadetimer = 0;
            s2.currentframegranada++;

            if (s2.currentframegranada == 10 && !s2.tirgrenada)
            {
                s2.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = s2.ex;
                        bulletse[i].y = s2.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - s2.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }

            if (s2.currentframegranada >= 16)
            {
                s2.currentframegranada = 0;
                s2.isshooting = -1;
                s2.tirgrenada = false;
            }
        }
    }

    if (Jorge.isshooting == -1)
    {
        Jorge.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - Jorge.ex) * (p.x - Jorge.ex) + (p.y - Jorge.ey) * (p.y - Jorge.ey));

        if (Jorge.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            Jorge.enemyShootTimer = 0.0f;

            Jorge.isshooting = 1;
            Jorge.currentframegranada = 0;
            Jorge.tirgrenada = false;

            Jorge.shootPauseTimer = 16.0f / framesSpeedgranada;
            Jorge.evx = 0;
        }
    }

    if (Jorge.isshooting == 1)
    {
        Jorge.grenadetimer += GetFrameTime();

        if (Jorge.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            Jorge.grenadetimer = 0;
            Jorge.currentframegranada++;

            if (Jorge.currentframegranada == 10 && !Jorge.tirgrenada)
            {
                Jorge.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = Jorge.ex;
                        bulletse[i].y = Jorge.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - Jorge.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }

            if (Jorge.currentframegranada >= 16)
            {
                Jorge.currentframegranada = 0;
                Jorge.isshooting = -1;
                Jorge.tirgrenada = false;
            }
        }
    }

    if (s3.isshooting == -1)
    {
        s3.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - s3.ex) * (p.x - s3.ex) + (p.y - s3.ey) * (p.y - s3.ey));

        if (s3.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            s3.enemyShootTimer = 0.0f;

            s3.isshooting = 1;
            s3.currentframegranada = 0;
            s3.tirgrenada = false;

            s3.shootPauseTimer = 16.0f / framesSpeedgranada;
            s3.evx = 0;
        }
    }

    if (s3.isshooting == 1)
    {
        s3.grenadetimer += GetFrameTime();

        if (s3.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            s3.grenadetimer = 0;
            s3.currentframegranada++;

            if (s3.currentframegranada == 10 && !s3.tirgrenada)
            {
                s3.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = s3.ex;
                        bulletse[i].y = s3.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - s3.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }

            if (s3.currentframegranada >= 16)
            {
                s3.currentframegranada = 0;
                s3.isshooting = -1;
                s3.tirgrenada = false;
            }
        }
    }

    if (s4.isshooting == -1)
    {
        s4.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - s4.ex) * (p.x - s4.ex) + (p.y - s4.ey) * (p.y - s4.ey));

        if (s4.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            s4.enemyShootTimer = 0.0f;

            s4.isshooting = 1;
            s4.currentframegranada = 0;
            s4.tirgrenada = false;

            s4.shootPauseTimer = 16.0f / framesSpeedgranada;
            s4.evx = 0;
        }
    }

    if (s4.isshooting == 1)
    {
        s4.grenadetimer += GetFrameTime();

        if (s4.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            s4.grenadetimer = 0;
            s4.currentframegranada++;

            if (s4.currentframegranada == 10 && !s4.tirgrenada)
            {
                s4.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = s4.ex;
                        bulletse[i].y = s4.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - s4.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }

            if (s4.currentframegranada >= 16)
            {
                s4.currentframegranada = 0;
                s4.isshooting = -1;
                s4.tirgrenada = false;
            }
        }
    }

    if (s5.isshooting == -1)
    {
        s5.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - s5.ex) * (p.x - s5.ex) + (p.y - s5.ey) * (p.y - s5.ey));

        if (s5.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            s5.enemyShootTimer = 0.0f;

            s5.isshooting = 1;
            s5.currentframegranada = 0;
            s5.tirgrenada = false;

            s5.shootPauseTimer = 16.0f / framesSpeedgranada;
            s5.evx = 0;
        }
    }

    if (s5.isshooting == 1)
    {
        s5.grenadetimer += GetFrameTime();

        if (s5.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            s5.grenadetimer = 0;
            s5.currentframegranada++;

            if (s5.currentframegranada == 10 && !s5.tirgrenada)
            {
                s5.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = s5.ex;
                        bulletse[i].y = s5.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - s5.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }
            if (s5.currentframegranada >= 16)
            {
                s5.currentframegranada = 0;
                s5.isshooting = -1;
                s5.tirgrenada = false;
            }
        }
    }

    if (s6.isshooting == -1)
    {
        s6.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - s6.ex) * (p.x - s6.ex) + (p.y - s6.ey) * (p.y - s6.ey));

        if (s6.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            s6.enemyShootTimer = 0.0f;

            s6.isshooting = 1;
            s6.currentframegranada = 0;
            s6.tirgrenada = false;

            s6.shootPauseTimer = 16.0f / framesSpeedgranada;
            s6.evx = 0;
        }
    }

    if (s6.isshooting == 1)
    {
        s6.grenadetimer += GetFrameTime();

        if (s6.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            s6.grenadetimer = 0;
            s6.currentframegranada++;

            if (s6.currentframegranada == 10 && !s6.tirgrenada)
            {
                s6.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = s6.ex;
                        bulletse[i].y = s6.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - s6.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }

            if (s6.currentframegranada >= 16)
            {
                s6.currentframegranada = 0;
                s6.isshooting = -1;
                s6.tirgrenada = false;
            }
        }
    }

    if (s7.isshooting == -1)
    {
        s7.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - s7.ex) * (p.x - s7.ex) + (p.y - s7.ey) * (p.y - s7.ey));

        if (s7.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            s7.enemyShootTimer = 0.0f;

            s7.isshooting = 1;
            s7.currentframegranada = 0;
            s7.tirgrenada = false;

            s7.shootPauseTimer = 16.0f / framesSpeedgranada;
            s7.evx = 0;
        }
    }

    if (s7.isshooting == 1)
    {
        s7.grenadetimer += GetFrameTime();

        if (s7.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            s7.grenadetimer = 0;
            s7.currentframegranada++;

            if (s7.currentframegranada == 10 && !s7.tirgrenada)
            {
                s7.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = s7.ex;
                        bulletse[i].y = s7.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - s7.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }

            if (s7.currentframegranada >= 16)
            {
                s7.currentframegranada = 0;
                s7.isshooting = -1;
                s7.tirgrenada = false;
            }
        }
    }
    if (s8.isshooting == -1)
    {
        s8.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - s8.ex) * (p.x - s8.ex) + (p.y - s8.ey) * (p.y - s8.ey));

        if (s8.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            s8.enemyShootTimer = 0.0f;

            s8.isshooting = 1;
            s8.currentframegranada = 0;
            s8.tirgrenada = false;

            s8.shootPauseTimer = 16.0f / framesSpeedgranada;
            s8.evx = 0;
        }
    }

    if (s8.isshooting == 1)
    {
        s8.grenadetimer += GetFrameTime();

        if (s8.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            s8.grenadetimer = 0;
            s8.currentframegranada++;

            if (s8.currentframegranada == 10 && !s8.tirgrenada)
            {
                s8.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = s8.ex;
                        bulletse[i].y = s8.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - s8.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }

            if (s8.currentframegranada >= 16)
            {
                s8.currentframegranada = 0;
                s8.isshooting = -1;
                s8.tirgrenada = false;
            }
        }
    }
    if (s9.isshooting == -1)
    {
        s9.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - s9.ex) * (p.x - s9.ex) + (p.y - s9.ey) * (p.y - s9.ey));

        if (s9.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            s9.enemyShootTimer = 0.0f;

            s9.isshooting = 1;
            s9.currentframegranada = 0;
            s9.tirgrenada = false;

            s9.shootPauseTimer = 16.0f / framesSpeedgranada;
            s9.evx = 0;
        }
    }

    if (s9.isshooting == 1)
    {
        s9.grenadetimer += GetFrameTime();

        if (s9.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            s9.grenadetimer = 0;
            s9.currentframegranada++;

            if (s9.currentframegranada == 10 && !s9.tirgrenada)
            {
                s9.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = s9.ex;
                        bulletse[i].y = s9.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - s9.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }

            if (s9.currentframegranada >= 16)
            {
                s9.currentframegranada = 0;
                s9.isshooting = -1;
                s9.tirgrenada = false;
            }
        }
    }
    if (cheat.isshooting == -1)
    {
        cheat.enemyShootTimer += GetFrameTime();

        float dist = sqrt((p.x - cheat.ex) * (p.x - cheat.ex) + (p.y - cheat.ey) * (p.y - cheat.ey));

        if (cheat.enemyShootTimer >= enemyShootInterval && dist < 925)
        {
            cheat.enemyShootTimer = 0.0f;

            cheat.isshooting = 1;
            cheat.currentframegranada = 0;
            cheat.tirgrenada = false;

            cheat.shootPauseTimer = 16.0f / framesSpeedgranada;
            cheat.evx = 0;
        }
    }

    if (cheat.isshooting == 1)
    {
        cheat.grenadetimer += GetFrameTime();

        if (cheat.grenadetimer >= 1.0f / framesSpeedgranada)
        {
            cheat.grenadetimer = 0;
            cheat.currentframegranada++;

            if (cheat.currentframegranada == 10 && !cheat.tirgrenada)
            {
                cheat.tirgrenada = true;

                for (int i = 0; i < MAX_BULLETSE; i++)
                {
                    if (!bulletse[i].active)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].x = cheat.ex;
                        bulletse[i].y = cheat.ey + 30;

                        float gravity = 0.5f;
                        float vy = -12.0f;

                        float timeOfFlight = (-2.0f * vy) / gravity;

                        bulletse[i].vx = (p.x - cheat.ex) / timeOfFlight;
                        bulletse[i].vy = vy;

                        bulletse[i].useGravity = true;
                        bulletse[i].active = true;
                        break;
                    }
                }
            }

            if (cheat.currentframegranada >= 16)
            {
                cheat.currentframegranada = 0;
                cheat.isshooting = -1;
                cheat.tirgrenada = false;
            }
        }
    }

    if (p.x > 4200 && p.x < 4510 && p.y <= 1220) FLOOR_Y = 1200;
    if (p.x > 4600 && p.x < 5500 && p.y <= 1000) FLOOR_Y = 1000;
    if (p.x > 5500 && p.x < 5785 && p.y <= 1220) FLOOR_Y = 1200;
    if (p.x > 5800 && p.x < 6470 && p.y <= 1000) FLOOR_Y = 1000;
    if (p.x > 6765 && p.x < 7515 && p.y <= 1000) FLOOR_Y = 1000;
    if (p.x > 7570 && p.x < 7900 && p.y <= 1220) FLOOR_Y = 1200;
    if (p.x > 6730 && p.x < 7515 && p.y <= 1300 && p.y > 1295)  p.vy = -5;
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

    if (sqrt((p.x - o1.ox) * (p.x - o1.ox) + (p.y - o1.oy) * (p.y - o1.oy)) < 50)
    {
        o1.alive--;
    }

    if (sqrt((p.x - ocheat.ox) * (p.x - ocheat.ox) + (p.y - ocheat.oy) * (p.y - ocheat.oy)) < 50)
    {
        ocheat.alive--;
    }

    if (sqrt((p.x - o2.ox) * (p.x - o2.ox) + (p.y - o2.oy) * (p.y - o2.oy)) < 50)
    {
        o2.alive--;
    }


        p.x += p.vx;
        p.y -= p.vy/2;

        if (s2.shootPauseTimer > 0.0f)
        {
            s2.shootPauseTimer -= GetFrameTime();
            s2.evx = 0;
        }
        else if (p.x >= 4500)
        {
            s2.evx = -5;
        }
        s2.ex += s2.evx;

        if (Jorge.shootPauseTimer > 0.0f)
        {
            Jorge.shootPauseTimer -= GetFrameTime();
            Jorge.evx = 0;
        }
        else if (!inMenu)
        {
            Jorge.evx = -5;
        }
        Jorge.ex += Jorge.evx;

        if (IsKeyPressed(KEY_Y)) p.y = 0;

        if (s5.shootPauseTimer > 0.0f)
        {
            s5.shootPauseTimer -= GetFrameTime();
            s5.evx = 0;
        }
        else if (p.x > 5400)
        {
            s5.evx = -5;
        }
        s5.ex += s5.evx;
        s5.vy = -8;
        if (s5.ex > 5760) s5.vy = 0;
        if (s5.ex > 5500 && s5.ey > 1202) s5.vy = 0;
        if (s5.ex > 3500 && s5.ey > 1382) s5.vy = 0;
        s5.ey -= s5.vy;

        if (s6.shootPauseTimer > 0.0f)
        {
            s6.shootPauseTimer -= GetFrameTime();
            s6.evx = 0;
        }
        else if (p.x > 8000)
        {
            s6.evx = -5;
        }
        s6.ex += s6.evx;    

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

        if (IsKeyPressed(KEY_Z)) cheat.ex = p.x, cheat.ey = p.y, bcheat = true, cheat.ehp = 1;
        if (IsKeyPressed(KEY_I)) bocheat = true, ocheat.alive = 1, ocheat.ox = p.x + 55, ocheat.oy = p.y - 55;

        s1.ey += s1.vy;
        s1.vy += 4;

        if (s1.ey >= 450)
        {
            s1.ey = 450;
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

        if (o1.oy >= 1400)
        {
            o1.oy = 1400;
            o1.ovy = 0;
        }

        ocheat.oy += ocheat.ovy;
        ocheat.ovy += 4;

        if (ocheat.oy >= FLOOR_Y)
        {
            ocheat.oy = FLOOR_Y;
            ocheat.ovy = 0;
        }

        o2.oy += o2.ovy;
        o2.ovy += 4;

        if (o2.oy >= 1400)
        {
            o2.oy = 1400;
            o2.ovy = 0;
        }

        s3.ey += s3.vy;
        s3.vy += 4;

        if (s3.ey >= 1020)
        {
            s3.ey = 1020;
            s3.vy = 0;
        }


        int tankFloor = 1380;

        if (t1.tx > 16300) tankFloor = t1.tx * -0.8 + 14420;
        if (t1.tx > 16550) tankFloor = 1180;
        if (t1.tx > 16750) tankFloor = t1.tx * -0.6 + 11230;
        if (t1.tx > 17000) tankFloor = 1030;
        if (t1.tx > 17150) tankFloor = t1.tx * -0.75 + 13892.5;
        if (t1.tx > 17350) tankFloor = 880;
        if (t1.tx > 17550) tankFloor = t1.tx * -0.9 + 16675;
        if (t1.tx > 17750) tankFloor = 700;

        t1.ty += t1.tvy;
        t1.tvy += 4;

        if (t1.tvy >= 0 && t1.ty > tankFloor - 10) t1.ty = tankFloor;

        if (t1.ty >= tankFloor)
        {
            t1.ty = tankFloor;
            t1.tvy = 0;
        }

        const float tankStopDistance = 350.00f;
        float distToPlayer = p.x - t1.tx;

        if (p.x > 16000 && distToPlayer < -tankStopDistance)
        {
            t1.tvx = -3;
        }
        else if (abs(distToPlayer) <= tankStopDistance || p.x <= 16000)
        {
            t1.tvx = 0;
        }

        t1.tx += t1.tvx;

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

        if (ed1.edhp >= 1)
        {
            if (p.x >= ed1.edx - 100 && p.x <= ed1.edx + 1000 && p.y >= ed1.edy && p.y <= ed1.edy + 1000)
                p.x = ed1.edx - 100;
        }

        if (ed2.edhp >= 1)
        {
            if (p.x >= ed2.edx + 500 && p.x <= ed2.edx + 1600 && p.y >= ed2.edy && p.y <= ed2.edy + 1000)
                p.x = ed2.edx + 500;
        }

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

            if (!bulletse[i].boom) {
                bulletse[i].x += bulletse[i].vx;
                bulletse[i].y += bulletse[i].vy;
            }

            if (bulletse[i].x < camLeft || bulletse[i].x > camRight ||
                bulletse[i].y < camTop || bulletse[i].y > camBottom) {
                bulletse[i].active = false;
            }

            if (bulletse[i].y >= FLOOR_Y + 200 && !bulletse[i].boom) {
                bulletse[i].boom = true;
                currentframeexplo = 0;
                bulletse[i].timerexplo = 0.0f;
                PlaySound(soundArray[6]);
                if (!p.Omniman && sqrt((p.x - bulletse[i].x) * (p.x - bulletse[i].x) + (p.y - bulletse[i].y) * (p.y - bulletse[i].y)) < 400) p.vides--;
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

            if (bulletse[i].boom)
            {
                bulletse[i].timerexplo += GetFrameTime();

                const float frameTime = 0.08f;

                while (bulletse[i].timerexplo >= frameTime)
                {
                    bulletse[i].timerexplo -= frameTime;
                    bulletse[i].currentframeexplo++;

                    if (bulletse[i].currentframeexplo >= 6)
                    {
                        bulletse[i].boom = false;
                        bulletse[i].active = false;
                        bulletse[i].currentframeexplo = 0;
                        break;
                    }
                }
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
                bulletsa1[i].y < camTop - 800 || bulletsa1[i].y > camBottom) {
                bulletsa1[i].active = false;
            }
        }

        for (int i = 0; i < MAX_BULLETSA2; i++) {
            if (!bulletsa2[i].active) continue;

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

        for (int i = 0; i < MAX_BULLETSTANK; i++) {
            if (!bulletstank[i].active) continue;

            if (bulletstank[i].useGravity) {
                bulletstank[i].vy += 0.5f; //Gravity strength
            }

            bulletstank[i].x += bulletstank[i].vx;
            bulletstank[i].y += bulletstank[i].vy;

            if (bulletstank[i].x < camLeft || bulletstank[i].x > camRight ||
                bulletstank[i].y < camTop || bulletstank[i].y > camBottom) {
                bulletstank[i].active = false;
            }
        }

        if (p.x < 0) { p.x = 0; if (p.vx < 0) p.vx = 0; }
        if (p.x > worldWidth) { p.x = worldWidth;  if (p.vx > 0) p.vx = 0; }

        if (camera.target.x < p.x) camera.target.x = (float)p.x;
        if (p.x >= 0 && p.x < 16200) {
            camera.target.y = (float)1100;
        }
        else if (p.x >= 16200 && p.x < 17800 && camera.target.y != 540) {
            camera.target.y = -0.35*p.x + 6770;
        }
        else {
            camera.target.y = (float)540;
        }
        //if (p.x >= 0 && p.x < 16200) {
        //    camera.target.y = (float)1100;
        //}
        //else if (p.x >= 16200 && p.x < 17800 && camera.target.y != 540) {
        //    camera.target.y = -0.35 * camera.target.x + 6770;
        //}
        //else {
        //    camera.target.y = (float)540;
        //}
        float halfW = 975 / 2.0f;
        float halfH = 714 / 2.0f;

        if (camera.target.x >  19000) camera.target.x = 19000; // LOCKS CAMERA ON BOSS AREA. - Aidan. /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
         //apagat per testing
        if (ed1.edhp >= 1)
        {
            if (camera.target.x > 13550) camera.target.x = 13550;

        }
        
        if (ed2.edhp >= 1)
        {
            if (camera.target.x > 14700) camera.target.x = 14700;
        }

        if (camera.target.x < halfW)              camera.target.x = halfW;
        if (camera.target.x > worldWidth - halfW) camera.target.x = worldWidth - halfW;

        int vitimi = (int)vidaTimer.lifetime;
        string ix = to_string(p.x);
        string iy = to_string(p.y);
        string viti = to_string(vitimi);
        const char* cix = ix.c_str();
        const char* ciy = iy.c_str();
        const char* cviti = viti.c_str();
        string puntstext = "Punts: ";
        const char* cpuntstext = puntstext.c_str();
        string punts = to_string(vpunts);
        const char* cpunts = punts.c_str();

        BeginDrawing();

        ClearBackground(BLACK);
        if (hitCooldown > 0.0f) hitCooldown -= GetFrameTime();
        if (shootCooldown > 0.0f) shootCooldown -= GetFrameTime();

        BeginMode2D(camera);
        Rectangle src = { 0, 0, (float)bg.width, (float)bg.height };
        Rectangle dest = { 0, 0, bg.width * bgScale, bg.height * bgScale };

        DrawTexturePro(bg, src, dest, { 0,0 }, 0.0f, WHITE);
        Vector2 positiont = {p.x+100, p.y-100 };
        
        //escenari

        // bales jugador

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) continue;
            
            if (bullets[i].direction == 2)
            {
                DrawTexture(potatoBullets ? potato : bullet, (int)bullets[i].x, (int)bullets[i].y, WHITE);
            }
            else if (bullets[i].direction == -2)
            {
                DrawTexture(potatoBullets ? potato : bulletes, (int)bullets[i].x, (int)bullets[i].y, WHITE);
            }
            else if (bullets[i].direction == 1)
            {
                DrawTexture(potatoBullets ? potato : bulleta, (int)bullets[i].x, (int)bullets[i].y, WHITE);
            }
            else if (bullets[i].direction == -1)
            {
                DrawTexture(potatoBullets ? potato : bulletb, (int)bullets[i].x, (int)bullets[i].y, WHITE);
            }

            if (bullets[i].x >= s1.ex && bullets[i].x <= s1.ex + 100 && bullets[i].y >= s1.ey && bullets[i].y <= s1.ey + 400)
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
            if (bullets[i].x >= s4.ex && bullets[i].x <= s4.ex + 100 && bullets[i].y >= s4.ey && bullets[i].y <= s4.ey + 200)
            {
                s4.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= s5.ex && bullets[i].x <= s5.ex + 100 && bullets[i].y >= s5.ey && bullets[i].y <= s5.ey + 200)
            {
                s5.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= s6.ex && bullets[i].x <= s6.ex + 100 && bullets[i].y >= s6.ey && bullets[i].y <= s6.ey + 200)
            {
                s6.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= s7.ex && bullets[i].x <= s7.ex + 100 && bullets[i].y >= s7.ey && bullets[i].y <= s7.ey + 200)
            {
                s7.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= s8.ex && bullets[i].x <= s8.ex + 100 && bullets[i].y >= s8.ey && bullets[i].y <= s8.ey + 200)
            {
                s8.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= s9.ex && bullets[i].x <= s9.ex + 100 && bullets[i].y >= s9.ey && bullets[i].y <= s9.ey + 200)
            {
                s9.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= cheat.ex && bullets[i].x <= cheat.ex + 100 && bullets[i].y >= cheat.ey && bullets[i].y <= cheat.ey + 200)
            {
                cheat.ehp--;
                bullets[i].active = false;
            }
            if (bullets[i].x >= t1.tx && bullets[i].x <= t1.tx + 100 && bullets[i].y >= t1.ty && bullets[i].y <= t1.ty + 200)
            {
                bullets[i].active = false;
                t1.thp--;
            }
            if (bullets[i].x >= ed1.edx + 500 && bullets[i].x <= ed1.edx + 1000 && bullets[i].y >= ed1.edy && bullets[i].y <= ed1.edy + 1000)
            {
                bullets[i].active = false;
                ed1.edhp--;
            }
            if (bullets[i].x >= ed2.edx + 1100 && bullets[i].x <= ed2.edx + 1600 && bullets[i].y >= ed2.edy && bullets[i].y <= ed2.edy + 1000)
            {
                bullets[i].active = false;
                ed2.edhp--;
            }
        }
        //bales enemic basic

        for (int i = 0; i < MAX_BULLETSE; i++) {
            if (!bulletse[i].active) continue;
            if (!bulletse[i].boom)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posbullete = { (int)bulletse[i].x, (int)bulletse[i].y, framerecsgranadp.width * 5, framerecsgranadp.height * 5 };
                DrawTexturePro(sgranadap, framerecsgranadp, posbullete, position, 0, WHITE);
            }
            if (bulletse[i].boom)
            {
                Rectangle src;
                src.width = sgranadaex.width / 6; 
                src.height = sgranadaex.height;
                src.x = bulletse[i].currentframeexplo * src.width;
                src.y = 0;
                Rectangle dest = { bulletse[i].x - 50,bulletse[i].y - 400,src.width * 12, src.height * 12};
                DrawTexturePro(sgranadaex, src, dest, { 0,0 }, 0, WHITE);
            }
            if (!p.Omniman && bulletse[i].x >= p.x && bulletse[i].x <= p.x + 100 && bulletse[i].y >= p.y && bulletse[i].y <= p.y + 200 && p.isajupit == -1 || !p.Omniman && bulletse[i].x >= p.x && bulletse[i].x <= p.x + 100 && bulletse[i].y >= p.y + 100 && bulletse[i].y <= p.y + 200 && p.isajupit == 1)
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

        for (int i = 0; i < MAX_BULLETSA1; i++) {

            float len = sqrtf(bulletsa1[i].vx * bulletsa1[i].vx + bulletsa1[i].vy * bulletsa1[i].vy);

            float dirX = bulletsa1[i].vx / len;
            float dirY = bulletsa1[i].vy / len;

            // separaci� entre boles
            float spacing = 18.0f;

            if (!bulletsa1[i].active) continue;
            //atac 1
            Vector2 position = { 0.0f, 0.0f };
            Rectangle posat1b1 = { (int)bulletsa1[i].x, (int)bulletsa1[i].y, framecrecat1b1.width * 4.75, framecrecat1b1.height * 4.75 };
            DrawTexturePro(at1b1, framecrecat1b1, posat1b1, position, 0, WHITE);
            //atac2
            Vector2 position2 = { 0.0f, 0.0f };
            Rectangle posat1b2 = { (int)bulletsa1[i].x - dirX * spacing, (int)bulletsa1[i].y - dirY * spacing, framecrecat1b2.width * 4.75, framecrecat1b2.height * 4.75 };
            DrawTexturePro(at1b2, framecrecat1b2, posat1b2, position2, 0, WHITE);
            //atac3
            Vector2 positio3 = { 0.0f, 0.0f };
            Rectangle posat1b3 = { (int)bulletsa1[i].x - dirX * spacing * 2, (int)bulletsa1[i].y - dirY * spacing * 2, framecrecat1b3.width * 4.75, framecrecat1b3.height * 4.75 };
            DrawTexturePro(at1b3, framecrecat1b3, posat1b3, positio3, 0, WHITE);
            //atac4
            Vector2 position4 = { 0.0f, 0.0f };
            Rectangle posat1b4 = { (int)bulletsa1[i].x - dirX * spacing * 2.5, (int)bulletsa1[i].y - dirY * spacing * 2.5, framecrecat1b4.width * 4.75, framecrecat1b4.height * 4.75 };
            DrawTexturePro(at1b4, framecrecat1b4, posat1b4, position4, 0, WHITE);
            //atac5
            Vector2 position5 = { 0.0f, 0.0f };
            Rectangle posat1b5 = { (int)bulletsa1[i].x - dirX * spacing * 3, (int)bulletsa1[i].y - dirY * spacing * 3, framecrecat1b5.width * 4.75, framecrecat1b5.height * 4.75 };
            DrawTexturePro(at1b5, framecrecat1b5, posat1b5, position5, 0, WHITE);
            //atac6
            Vector2 position6 = { 0.0f, 0.0f };
            Rectangle posat1b6 = { (int)bulletsa1[i].x - dirX * spacing * 3.5, (int)bulletsa1[i].y - dirY * spacing * 3.5, framecrecat1b6.width * 4.75, framecrecat1b6.height * 4.75 };
            DrawTexturePro(at1b6, framecrecat1b6, posat1b6, position6, 0, WHITE);
            if (!p.Omniman && bulletsa1[i].x >= p.x && bulletsa1[i].x <= p.x + 100 && bulletsa1[i].y >= p.y && bulletsa1[i].y <= p.y + 200 && p.isajupit == -1 || !p.Omniman && bulletsa1[i].x >= p.x && bulletsa1[i].x <= p.x + 100 && bulletsa1[i].y >= p.y + 100 && bulletsa1[i].y <= p.y + 200 && p.isajupit == 1)
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

        for (int i = 0; i < MAX_BULLETSA2; i++) {
            if (!bulletsa2[i].active) continue;
            DrawTexture(bulletee, (int)bulletsa2[i].x, (int)bulletsa2[i].y, WHITE);
            if (!p.Omniman && bulletsa2[i].x >= p.x && bulletsa2[i].x <= p.x + 100 && bulletsa2[i].y >= p.y && bulletsa2[i].y <= p.y + 200 && p.isajupit == -1 || !p.Omniman && bulletsa2[i].x >= p.x && bulletsa2[i].x <= p.x + 100 && bulletsa2[i].y >= p.y + 100 && bulletsa2[i].y <= p.y + 200 && p.isajupit == 1)
            {
                bulletsa2[i].active = false;
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

        for (int i = 0; i < MAX_BULLETSA3; i++) {
            if (!bulletsa3[i].active) continue;
            DrawTexture(bulletee, (int)bulletsa3[i].x, (int)bulletsa3[i].y, WHITE);
            if (!p.Omniman && bulletsa3[i].x >= p.x && bulletsa3[i].x <= p.x + 100 && bulletsa3[i].y >= p.y && bulletsa3[i].y <= p.y + 200 && p.isajupit == -1 || !p.Omniman && bulletsa3[i].x >= p.x && bulletsa3[i].x <= p.x + 100 && bulletsa3[i].y >= p.y + 100 && bulletsa3[i].y <= p.y + 200 && p.isajupit == 1)
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

        for (int i = 0; i < MAX_BULLETSTANK; i++) {
            if (!bulletstank[i].active) continue;
            DrawTexture(bulletee, (int)bulletstank[i].x, (int)bulletstank[i].y, WHITE);

            if (!p.Omniman &&
                bulletstank[i].x >= p.x && bulletstank[i].x <= p.x + 100 &&
                bulletstank[i].y >= p.y && bulletstank[i].y <= p.y + 200)
            {
                bulletstank[i].active = false;
                hitCooldown = 1.5f;
                p.vides--;
                p.vx = 0;
                p.vy = 20;
                if (p.vides <= 0)
                {
                    p.credits--;
                    p.vides = 3;
                    if (p.credits <= 0) lose = true;
                }
            }
        }

        if (!inMenu && !winscreen && !lose) {

            PauseSound(soundArray[7]);
            winSoundPlayed = false;


            if (IsKeyPressed(KEY_L))p.x = 19000, ed1.edhp = 0, ed2.edhp = 0; // USED FOR TESTING THE BOSS YOU STUPID ASS HOES
            if (IsKeyPressed(KEY_X))p.x = 16500, ed1.edhp = 0, ed2.edhp = 0;
            if (IsKeyPressed(KEY_K))p.x = 12500;
            if (IsKeyPressed(KEY_O))p.x = 14000;
            if (IsKeyPressed(KEY_B)) potatoBullets = !potatoBullets;


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
            Rectangle posmgun = { (float)o1.ox, o1.oy+50, frameRecmgun.width * 5, frameRecmgun.height * 5 };
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

        if (ocheat.alive == 1)
        {
            Vector2 position = { 0.0f, 0.0f };
            Rectangle posmgun = { (float)ocheat.ox, ocheat.oy + 50, frameRecmgun.width * 5, frameRecmgun.height * 5 };
            DrawTexturePro(mgun, frameRecmgun, posmgun, position, 0, WHITE);
        }
        else if (bocheat)
        {
            PlaySound(soundArray[8]);
            vpunts = vpunts + 100;
            bocheat = false;
            killhim = true;
            machineGunAmmo = 200;
        }

        if (o2.alive == 1)
        {
            Vector2 position = { 0.0f, 0.0f };
            Rectangle posmgun = { (float)o2.ox, o2.oy+50, frameRecmgun.width * 5, frameRecmgun.height * 5 };
            DrawTexturePro(mgun, frameRecmgun, posmgun, position, 0, WHITE);
        }
        else if (os2)
        {
            PlaySound(soundArray[9]);
            PauseMusicStream(musicArray[0]);
            vpunts = vpunts + 100;
            os2 = false;
            p.Omniman = true;

            startTimer(&omnimanTimer, 20.0f);
        }

        //edificis destructibles
        if (ed1.edhp >= 1)
        {
            if (ed1.edhp >= 30)
            {
                Vector2 originedg4 = { 0.0f, 0.0f };
                Rectangle edg41 = { 0, 0, edg4.width, edg4.height };
                Rectangle edg42 = { (float)ed1.edx, ed1.edy, edg4.width * 5.5, edg4.height * 5.5 };
                DrawTexturePro(edg4, edg41, edg42, originedg4, 0.0f, WHITE);
            }
            else if (ed1.edhp >= 20)
            {
                Vector2 originedg3 = { 0.0f, 0.0f };
                Rectangle edg31 = { 0, 0, edg3.width, edg3.height };
                Rectangle edg32 = { (float)ed1.edx, ed1.edy, edg3.width * 5.5, edg3.height * 5.5 };
                DrawTexturePro(edg3, edg31, edg32, originedg3, 0.0f, WHITE);
            }
            else if (ed1.edhp >= 10)
            {
                Vector2 originedg2 = { 0.0f, 0.0f };
                Rectangle edg21 = { 0, 0, edg2.width, edg2.height };
                Rectangle edg22 = { (float)ed1.edx, ed1.edy, edg2.width * 5.5, edg2.height * 5.5 };
                DrawTexturePro(edg2, edg21, edg22, originedg2, 0.0f, WHITE);
            }
            else if (ed1.edhp >= 1)
            {
                Vector2 originedg1 = { 0.0f, 0.0f };
                Rectangle edg11 = { 0, 0, edg1.width, edg1.height };
                Rectangle edg12 = { (float)ed1.edx, ed1.edy, edg1.width * 5.5, edg1.height * 5.5 };
                DrawTexturePro(edg1, edg11, edg12, originedg1, 0.0f, WHITE);
            }

        }
        else if (ed1b)
        {
            ed1.edx = 100000000;
            PlaySound(soundArray[0]);
            ed1b = false;
        }

        if (ed2.edhp >= 1)
        {
            if (ed2.edhp >= 20)
            {
                Vector2 originedv1 = { 0.0f, 0.0f };
                Rectangle edv11 = { 0, 0, edv1.width, edv1.height };
                Rectangle edv12 = { (float)ed2.edx, ed2.edy, edv1.width * 5.5, edv1.height * 5.5 };
                DrawTexturePro(edv1, edv11, edv12, originedv1, 0.0f, WHITE);
            }
            else if (ed2.edhp >= 10)
            {
                Vector2 originedv2 = { 0.0f, 0.0f };
                Rectangle edv21 = { 0, 0, edv2.width, edv2.height };
                Rectangle edv22 = { (float)ed2.edx, ed2.edy, edv2.width * 5.5, edv2.height * 5.5 };
                DrawTexturePro(edv2, edv21, edv22, originedv2, 0.0f, WHITE);
            }
            else if (ed2.edhp >= 1)
            {
                Vector2 originedv3 = { 0.0f, 0.0f };
                Rectangle edv31 = { 0, 0, edv3.width, edv3.height };
                Rectangle edv32 = { (float)ed2.edx, ed2.edy, edv3.width * 5.5, edv3.height * 5.5 };
                DrawTexturePro(edv3, edv31, edv32, originedv3, 0.0f, WHITE);
            }
        }
        else if (ed2b)
        {
            ed2.edx = 100000000;
            PlaySound(soundArray[0]);
            ed2b = false;
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
            // --- Determine which animation to play ---
            if (s1.ehp > 75 || (s1.ehp <= 75 && p.y <= 400))
            {
                static float morterTimer = 0.0f;
                morterTimer += GetFrameTime();
                if (morterTimer >= 4.0f)
                {
                    morterTimer = 0.0f;
                    bossAnim = (bossAnim == 0) ? 1 : 0;
                }
            }
            else
            {
                // Phase 2, player on lower platform: alternate laser / bola
                if (raig && !bola)  bossAnim = 2;
                else                bossAnim = 3;
            }

            // --- Draw the correct sprite at the correct Y ---
            // morter_up   (bossAnim 0): boss at normal height (s1.ey)
            // morter_down (bossAnim 1): boss lower  (+150 px to Y so feet stay grounded)
            // laser       (bossAnim 2): boss at normal height
            // bola        (bossAnim 3): boss crouched (+150 px)

            Vector2 bossOrigin = { 0.0f, 0.0f };

            if (bossAnim == 0)
            {
                Rectangle dest_boss = { (float)s1.ex, (float)s1.ey,
                                         framerecmorterup.width * 5.0f,
                                         framerecmorterup.height * 5.0f };
                DrawTexturePro(morter_up, framerecmorterup, dest_boss, bossOrigin, 0, WHITE);
            }
            else if (bossAnim == 1)
            {
                Rectangle dest_boss = { (float)s1.ex, (float)s1.ey + 150.0f,
                                         framerecmorterdown.width * 5.0f,
                                         framerecmorterdown.height * 5.0f };
                DrawTexturePro(morter_down, framerecmorterdown, dest_boss, bossOrigin, 0, WHITE);
            }
            else if (bossAnim == 2)
            {
                Rectangle dest_boss = { (float)s1.ex, (float)s1.ey,
                                         framerecLaser.width * 5.0f,
                                         framerecLaser.height * 5.0f };
                DrawTexturePro(laser, framerecLaser, dest_boss, bossOrigin, 0, WHITE);
            }
            else if (bossAnim == 3)
            {
                Rectangle dest_boss = { (float)s1.ex, (float)s1.ey + 150.0f,
                                         framerecBola.width * 5.0f,
                                         framerecBola.height * 5.0f };
                DrawTexturePro(bolaa, framerecBola, dest_boss, bossOrigin, 0, WHITE);
            }

            // --- Fire bullet tied to animation frame, not a separate timer ---
            if (!inMenu && !winscreen && !lose)
            {
                if (s1.ehp <= 75 && p.y > 400)
                {
                    // Original timer-based firing — unchanged
                    s1.enemyShootTimer += GetFrameTime();

                    if (s1.enemyShootTimer >= enemyShootInterval)
                    {
                        s1.enemyShootTimer = 0.0f;

                        if (raig && !bola)
                        {
                            for (int i = 0; i < MAX_BULLETSA2; i++)
                            {
                                if (!bulletsa2[i].active)
                                {
                                    bulletsa2[i].x = s1.ex;
                                    bulletsa2[i].y = 650;
                                    float dx = p.x - s1.ex - 20;
                                    float dy = p.y - s1.ey;
                                    float length = sqrtf(dx * dx + dy * dy);
                                    bulletsa2[i].vx = (dx / length) * 20.0f;
                                    bulletsa2[i].vy = 0;
                                    bulletsa2[i].active = true;
                                    bulletsa2[i].useGravity = false;
                                    raig = false;
                                    bola = true;
                                    break;
                                }
                            }
                        }
                        else if (bola && !raig)
                        {
                            for (int i = 0; i < MAX_BULLETSA3; i++)
                            {
                                if (!bulletsa3[i].active)
                                {
                                    bulletsa3[i].x = s1.ex;
                                    bulletsa3[i].y = 825;
                                    float dx = p.x - s1.ex;
                                    float dy = p.y - s1.ey;
                                    float length = sqrtf(dx * dx + dy * dy);
                                    bulletsa3[i].vx = (dx / length) * 8.0f;
                                    bulletsa3[i].vy = 0;
                                    bulletsa3[i].active = true;
                                    bulletsa3[i].useGravity = false;
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
        else if (KevinTheFuckingBoss)
        {
            vpunts += 1000;
            KevinTheFuckingBoss = false;
            PlaySound(soundArray[0]);
            winscreen = true;
        }


        if (s2.ehp == 1) 
        {
            if (s2.evx == 0 && s2.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad; 
                src.x = s2.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)s2.ex, (float)s2.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (s2.evx == 0)
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
            if (Jorge.evx == 0 && Jorge.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad;
                src.x = Jorge.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)Jorge.ex, (float)Jorge.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (Jorge.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)Jorge.ex, (float)Jorge.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidle, position, 0, WHITE);
                DrawText(cix, Jorge.ex, Jorge.ey, 20, RED);
            }
            else if (Jorge.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)Jorge.ex, (float)Jorge.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, Jorge.ex, Jorge.ey, 20, RED);
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
            
            if (s3.evx == 0 && s3.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad;
                src.x = s3.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)s3.ex, (float)s3.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (s3.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)s3.ex, (float)s3.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidle, position, 0, WHITE);
                DrawText(cix, s3.ex, s3.ey, 20, RED);
            }

            else if (s3.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)s3.ex, (float)s3.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, s3.ex, s3.ey, 20, RED);
            }
        }
        else if (bs3)
        {
            vpunts = vpunts + 10;
            PlaySound(soundArray[0]);
            s3.ex = 100000000;
            bs3 = false;
        }

        if (s4.ehp == 1) {

            if (s4.evx == 0 && s4.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad;
                src.x = s4.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)s4.ex, (float)s4.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (s4.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)s4.ex, (float)s4.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidle, position, 0, WHITE);
                DrawText(cix, s4.ex, s4.ey, 20, RED);
            }

            else if (s4.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)s4.ex, (float)s4.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, s4.ex, s4.ey, 20, RED);
            }
        }
        else if (bs4)
        {
            vpunts = vpunts + 10;
            PlaySound(soundArray[0]);
            s4.ex = 100000000;
            bs4 = false;
        }

        if (s5.ehp == 1) {
            
            if (s5.evx == 0 && s5.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad;
                src.x = s5.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)s5.ex, (float)s5.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (s5.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)s5.ex, (float)s5.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidle, position, 0, WHITE);
                DrawText(cix, s5.ex, s5.ey, 20, RED);
            }

            else if (s5.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)s5.ex, (float)s5.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, s5.ex, s5.ey, 20, RED);
            }
        }
        else if (bs5)
        {
            vpunts = vpunts + 10;
            PlaySound(soundArray[0]);
            s5.ex = 100000000;
            bs5 = false;
        }

        if (s6.ehp == 1)
        {
            if (s6.evx == 0 && s6.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad;
                src.x = s6.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)s6.ex, (float)s6.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (s6.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)s6.ex, (float)s6.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidle, position, 0, WHITE);
                DrawText(cix, s6.ex, s6.ey, 20, RED);
            }

            else if (s6.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)s6.ex, (float)s6.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, s6.ex, s6.ey, 20, RED);
            }
        }
        else if (bs6)
        {
            vpunts = vpunts + 10;
            PlaySound(soundArray[0]);
            s6.ex = 100000000;
            bs6 = false;
        }
        
        if (s7.ehp == 1)
        {
            if (s7.evx == 0 && s7.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad;
                src.x = s7.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)s7.ex, (float)s7.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (s7.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)s7.ex, (float)s7.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidle, position, 0, WHITE);
                DrawText(cix, s7.ex, s7.ey, 20, RED);
            }

            else if (s7.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)s7.ex, (float)s7.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, s7.ex, s7.ey, 20, RED);
            }
        }
        else if (bs7)
        {
            vpunts = vpunts + 10;
            PlaySound(soundArray[0]);
            s7.ex = 100000000;
            bs7 = false;
        }

        if (s8.ehp == 1)
        {
            if (s8.evx == 0 && s8.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad;
                src.x = s8.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)s8.ex, (float)s8.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (s8.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)s8.ex, (float)s8.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidle, position, 0, WHITE);
                DrawText(cix, s8.ex, s8.ey, 20, RED);
            }

            else if (s8.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)s8.ex, (float)s8.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, s8.ex, s8.ey, 20, RED);
            }
        }
        else if (bs8)
        {
            vpunts = vpunts + 10;
            PlaySound(soundArray[0]);
            s8.ex = 100000000;
            bs8 = false;
        }
        if (s9.ehp == 1)
        {
            if (s9.evx == 0 && s9.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad;
                src.x = s9.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)s9.ex, (float)s9.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (s9.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)s9.ex, (float)s9.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidle, position, 0, WHITE);
                DrawText(cix, s9.ex, s9.ey, 20, RED);
            }

            else if (s9.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)s9.ex, (float)s9.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, s9.ex, s9.ey, 20, RED);
            }
        }
        else if (bs9)
        {
            vpunts = vpunts + 10;
            PlaySound(soundArray[0]);
            s9.ex = 100000000;
            bs9 = false;
        }

        if (cheat.ehp == 1)
        {
            if (cheat.evx == 0 && cheat.isshooting == 1)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle src = framerecsgranad;
                src.x = cheat.currentframegranada * (sgranada.width / 16);
                Rectangle posgranad = { (float)cheat.ex, (float)cheat.ey, src.width * 5, src.height * 5 };
                DrawTexturePro(sgranada, src, posgranad, position, 0, WHITE);
            }
            else if (cheat.evx == 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle posidle = { (float)cheat.ex, (float)cheat.ey, framereceidle.width * 5, framereceidle.height * 5 };
                DrawTexturePro(sidle, framereceidle, posidle, position, 0, WHITE);
                DrawText(cix, cheat.ex, cheat.ey, 20, RED);
            }

            else if (cheat.evx < 0)
            {
                Vector2 position = { 0.0f, 0.0f };
                Rectangle poscorr = { (float)cheat.ex, (float)cheat.ey, framerececorr.width * 5, framerececorr.height * 5 };
                DrawTexturePro(scor, framerececorr, poscorr, position, 0, WHITE);
                DrawText(cix, cheat.ex, cheat.ey, 20, RED);
            }
        }
        else if (bcheat)
        {
            vpunts = vpunts + 10;
            PlaySound(soundArray[0]);
            cheat.ex = 100000000;
            bcheat = false;
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

                    for (int i = 0; i < MAX_BULLETSTANK; i++)
                    {
                        if (!bulletstank[i].active)
                        {
                            bulletstank[i].x = t1.tx;
                            bulletstank[i].y = t1.ty + 30;

                            float gravity = 0.3f;   // matches bullet update loop, WHY ARE YOU GAY?
                            float vy = -12.0f; // Arc parabola

                            // Frames until bullet returns to same Y
                            float timeOfFlight = (-2.0f * vy) / gravity; // = 48 frames

                            // vx needed to land exactly on player's X
                            bulletstank[i].vx = (p.x - t1.tx) / timeOfFlight;
                            bulletstank[i].vy = vy;

                            bulletstank[i].useGravity = true; // ACTIVA LA GRAVETAT GILIPOLLAS
                            bulletstank[i].active = true;
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
                DrawText(ciy, p.x, p.y + 30, 20, RED);
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

        if (p.Omniman)
        {
            updatetimer(&omnimanTimer);
            if (TimerDone(&omnimanTimer))
            {
                p.Omniman = false;
                PauseSound(soundArray[9]);
                ResumeMusicStream(musicArray[0]);
            }
        }

        //escenari davant jugador

        //porxo
        Rectangle p1porxo = { 0, 0, porxoavio.width, porxoavio.height };
        Rectangle p2porxo = { 6800, 1180, porxoavio.width * 5, porxoavio.height * 5};
        Vector2 originporxo = { 0, 0 };
        DrawTexturePro(porxoavio, p1porxo, p2porxo, originporxo, 0.0f, WHITE);
        
        // peix
        //Rectangle p2peix = { 13400, 1280, porxoavio.width * 0.75, porxoavio.height * 1.75 };
        //Vector2 originpeix = { 0, 0 };
        //DrawTexturePro(peix, framerectpeix, p2peix, originpeix, 0.0f, WHITE);

        EndMode2D();

        if (missionSoundDelay > 0.0f) missionSoundDelay -= GetFrameTime();

        if (missionSoundStep == 0) {
            PlaySound(soundArray[10]);
            missionSoundStep++;
            missionSoundDelay = 1.0f;
        }
        else if (missionSoundStep == 1 && missionSoundDelay <= 0.0f) {
            PlaySound(soundArray[11]);
            missionSoundStep++;
            missionSoundDelay = 0.6f;
        }
        else if (missionSoundStep == 2 && missionSoundDelay <= 0.0f) {
            PlaySound(soundArray[12]);
            missionSoundStep++;
        }

        if (logoscreen) {

            if (IsKeyPressed(KEY_M)) {
                for (int i = 0; i < 20; i++)
                    SetSoundVolume(soundArray[i], 0.0f);
                PauseMusicStream(musicArray[0]);
                music = false;
            }
            if (IsKeyPressed(KEY_N)) {
                for (int i = 0; i < 20; i++)
                    SetSoundVolume(soundArray[i], 1.0f);
                SetSoundVolume(soundArray[7], 100.0f); // restore Fahhhh's original loud volume
                ResumeMusicStream(musicArray[0]);
                music = true;
            }

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

            if (IsKeyPressed(KEY_M)) {
                for (int i = 0; i < 20; i++)
                    SetSoundVolume(soundArray[i], 0.0f);
                PauseMusicStream(musicArray[0]);
                music = false;
            }
            if (IsKeyPressed(KEY_N)) {
                for (int i = 0; i < 20; i++)
                    SetSoundVolume(soundArray[i], 1.0f);
                SetSoundVolume(soundArray[7], 100.0f); // restore Fahhhh's original loud volume
                ResumeMusicStream(musicArray[0]);
                music = true;
            }


            if (!menuSoundPlayed) {
                PlaySound(soundArray[3]);
                menuSoundPlayed = true;
            }

            Rectangle src2 = { 0, 0, (float)start.width, (float)start.height };
            Rectangle dest2 = { 0, 0, 975, 714 };

            DrawTexturePro(start, src2, dest2, { 0, 0 }, 0.0f, WHITE);

            int textWidth = MeasureText(cpunts, 30);

            Vector2 cartera = { 975 - textWidth - 150, 652 };
            DrawTextEx(whiteFont, TextFormat("%d", (int)p.credits), cartera, 98, -70, WHITE); //Afegir imatge "CREDIT "
            if (IsKeyPressed(KEY_C))
                p.credits++;
            if (IsKeyPressed(KEY_ENTER) && p.credits > 0)
            {
                p.credits--;
                inMenu = false;
                missionSoundStep = 0;

            }
        }
        else if (!inMenu && !logoscreen) {

            int textWidth = MeasureText(cpunts, 30);

            Vector2 cartera2 = { 975 - textWidth - 150, 652 };
            DrawTextEx(whiteFont, TextFormat("%d", (int)p.credits), cartera2, 98, -70, WHITE); //Afegir imatge "CREDIT "
            Vector2 health2 = { 70, -30 };
            DrawTextEx(whiteFont, cpunts, health2, 100, -60, WHITE);
            Vector2 health = { 80, 60 };
            DrawTextEx(timerNums, TextFormat("%d", p.vides), health, 50, -60, WHITE);


            if (killhim) {
                Vector2 health = { 250, 30 };
                DrawTextEx(timerNums, TextFormat("%d", machineGunAmmo), health, 50, -20, WHITE);
            }

            if (music == false) {
            }
            else {
                UpdateMusicStream(musicArray[0]);
            }
            if (IsKeyPressed(KEY_M)) {
                for (int i = 0; i < 20; i++)
                    SetSoundVolume(soundArray[i], 0.0f);
                PauseMusicStream(musicArray[0]);
                music = false;
            }
            if (IsKeyPressed(KEY_N)) {
                for (int i = 0; i < 20; i++)
                    SetSoundVolume(soundArray[i], 1.0f);
                SetSoundVolume(soundArray[7], 100.0f); // restore Fahhhh's original loud volume
                ResumeMusicStream(musicArray[0]);
                music = true;
            }

            if (IsKeyPressed(KEY_C)) p.credits++;

            if (IsKeyPressed(KEY_R)) {
                p.Omniman = false;
                lose = false;
                vpunts -= vpunts;
                KevinTheFuckingBoss = true;
                bs2 = true;
                bs3 = true;
                bs4 = true;
                bs5 = true;
                bs6 = true;
                bs7 = true;
                bJorge = true;
                os1 = true;
                os2 = true;
                winscreen = false;
                s1.ehp = 100;
                s2.ehp = 1;
                killhim = false;
                s3.ehp = 1;
                s4.ehp = 1;
                s5.ehp = 1;
                s6.ehp = 1;
                s7.ehp = 1;
                s5.evx = 0; 
                s6.evx = 0;
                t1.thp = 25;
                bt1 = true;
                o1.alive = 1;
                o2.alive = 1;
                Jorge.ehp = 1;
                Jorge.ex = 3200;
                winSoundPlayed = false;
                timerlife = 450;
                omnimanTimer = { 0 };
                ed1.edhp = 40;
                ed2.edhp = 30;
                ed1b = true;
                ed2b = true;
                potatoBullets = false;
                ed1.edx = 13850;
                ed2.edx = 14500;
                startTimer(&vidaTimer, timerlife);

                StopMusicStream(musicArray[0]);
                missionSoundStep = 0;
                PlayMusicStream(musicArray[0]);
                StopSound(soundArray[9]);

                p.x = 400;
                p.y = 1220;
                p.vx = 0;
                p.vy = 0;
                s2.ex = 5450;
                s2.evx = 0;
                s3.ehp = 1;
                s3.ex = 10450;
                s4.ex = 4750;
                s5.ex = 6000;
                s6.ex = 8850;
                s7.ex = 12250;
                s5.ey = 1000;


                t1.tx = 17500;
                t1.ty = 880;
                t1.tvy = 0;
                t1.tvx = 0;

                camera.target.x = p.x;
                camera.target.y = 1100;
            }
            Vector2 clock = {340, 0};
            DrawTextEx(timerNums, TextFormat("%d", (int)vidaTimer.lifetime), clock, 120, -60, WHITE);
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

            if (IsKeyPressed(KEY_W)) {
                if (konami == 0 || konami == 1) konami++;
                else konami = 0;
            } else if (IsKeyPressed(KEY_S)) {
                if (konami == 2 || konami == 3) konami++;
                else konami = 0;
            } else if (IsKeyPressed(KEY_A)) {
                if (konami == 4 || konami == 6) konami++;
                else konami = 0;
            } else if (IsKeyPressed(KEY_D)) {
                if (konami == 5 || konami == 7) konami++;
                else konami = 0;
            } else if (IsKeyPressed(KEY_J)) {
                if (konami == 8) konami++;
                else konami = 0;
            } else if (IsKeyPressed(KEY_SPACE)) {
                if (konami == 9) {
                    konami++;
                    p.Omniman = true;
                    PlaySound(soundArray[9]);
                    PauseMusicStream(musicArray[0]);
                } else konami = 0;
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
            killhim = false;

            Rectangle src3 = { 0, 0, (float)win.width, (float)win.height };
            Rectangle dest3 = { 0, 0, 975, 714 };
            DrawTexturePro(win, src3, dest3, { 0, 0 }, 0.0f, WHITE);
            DrawText("PRESS P TO RETURN TO MENU\nPRESS R TO RESTART LEVEL", 375, 800, 40, RED);
            if (IsKeyPressed(KEY_P)) {
                p.Omniman = false;
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
                bt1 = true;
                os1 = true;
                os2 = true;
                winscreen = false;
                s1.ehp = 100;
                p.vides = 3;
                s2.ehp = 1;
                s3.ehp = 1;
                s5.evx = 0;
                s6.evx = 0;
                o1.alive = 1;
                o2.alive = 1;
                Jorge.ehp = 1;
                t1.thp = 25;
                Jorge.ex = 3200;
                timerlife = 450;
                omnimanTimer = { 0 };
                ed1.edhp = 40;
                ed2.edhp = 30;
                ed1b = true;
                ed2b = true;
                ed1.edx = 13850;
                ed2.edx = 14500;
                potatoBullets = false;
                startTimer(&vidaTimer, timerlife);

                StopMusicStream(musicArray[0]);                
                menuSoundPlayed = false;
                PlayMusicStream(musicArray[0]);
                StopSound(soundArray[9]);


                p.x = 400;
                p.y = 1220;
                p.vx = 0;
                p.vy = 0;
                s2.ex = 5450;
                s2.evx = 0;
                s3.ehp = 1;
                s3.ex = 10450;
                s4.ehp = 1;
                s4.ex = 4750;
                s5.ehp = 1;
                s5.ex = 6000;
                s6.ehp = 1;
                s6.ex = 8850;
                s7.ehp = 1;
                s7.ex = 12250;
                s5.ey = 1000;

                t1.tx = 17500;
                t1.ty = 880;
                t1.tvy = 0;
                t1.tvx = 0;

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
            killhim = false;

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
                os2 = true;
                winscreen = false;
                s1.ehp = 100;
                s2.ehp = 1;
                s3.ehp = 1;
                p.vides = 3;
                bt1 = true;
                o1.alive = 1;
                o2.alive = 1;
                t1.thp = 25;
                Jorge.ehp = 1;
                Jorge.ex = 3200;
                winSoundPlayed = false;
                timerlife = 450;
                omnimanTimer = { 0 };
                ed1.edhp = 40;
                ed2.edhp = 30;
                ed1b = true;
                ed2b = true;
                ed1.edx = 13850;
                ed2.edx = 14500;
                potatoBullets = false;
                startTimer(&vidaTimer, timerlife);


                StopMusicStream(musicArray[0]);
                menuSoundPlayed = false;
                PlayMusicStream(musicArray[0]);
                StopSound(soundArray[9]);


                p.x = 400;
                p.y = 1220;
                p.vx = 0;
                p.vy = 0;
                s2.ex = 5450;
                s2.evx = 0;
                s3.ehp = 1;
                s3.ex = 10450;
                s4.ehp = 1;
                s4.ex = 4750;
                s5.ehp = 1;
                s5.ex = 6000;
                s6.ehp = 1;
                s6.ex = 8850;
                s7.ehp = 1;
                s7.ex = 12250;


                t1.tx = 17500;
                t1.ty = 880;
                t1.tvy = 0;
                t1.tvx = 0;

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
            for (int i = 0; i < MAX_BULLETSA1; i++) {
                bulletsa1[i].active = false;
            }
            for (int i = 0; i < MAX_BULLETSA2; i++) {
                bulletsa2[i].active = false;
            }
            for (int i = 0; i < MAX_BULLETSA3; i++) {
                bulletsa3[i].active = false;
            }
        }
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
    UnloadTexture(potato);
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
    UnloadTexture(tbase);
    UnloadTexture(porxoavio);
    UnloadTexture(peix);
    UnloadTexture(edg1);
    UnloadTexture(edg2);
    UnloadTexture(edg3);
    UnloadTexture(edg4);
    UnloadTexture(edv1);
    UnloadTexture(edv2);
    UnloadTexture(edv3);
    UnloadTexture(sgranada);
    UnloadTexture(laser);
    UnloadTexture(bolaa);
    UnloadTexture(morter_down);
    UnloadTexture(morter_up);
    UnloadTexture(sgranadap);
    UnloadTexture(sgranadaex);
    CloseWindow();
    return 0;
}
// Copyright (c) 2026 Explota Studio
// Licensed under the MIT License