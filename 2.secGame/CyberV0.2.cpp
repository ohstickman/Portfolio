#include "raylib.h"
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <iomanip>

int wth = 800;
int hht = 700;

int maxEnem = 6;
int maxHeals = 2;
int enDamage = 15;
int enSpeed = 5;
int attackRange = 15;
int pattackRange = 30;

float mult = 1.0f;
int now = 1;

struct cord{ int x, y; };

struct Player;
extern std::vector<Player> enems;

struct skin{
    Color color;
    float size;
};

struct DamageText {
    float x, y;
    float speedY;
    float alpha;
    int value;
    Color color;
};

std::vector<DamageText> damageTexts;
int maxKills{};

void StatsSave();

void StatsOpen(){
    std::ifstream istt("stats.txt");
    if(istt.is_open()){
        istt >> maxKills;
        istt.close();
    }
}

struct Player {
    float x, y; 
    int hp, damage;
    skin skin;
    float speed;
    int kills;

    int hitTimer;

    void hit(int da){
        hp -= da;
        hitTimer = 20;
        float offsetX = (rand() % 40) - 20;
        float offsetY = (rand() % 40) - 20;

        DamageText text;
        text.x = x + offsetX;
        text.y = y + offsetY;
        text.speedY = -4.0f;
        text.alpha = 1.0f;
        text.value = -da;
        text.color = Color{ 255, 50, 50, 255 };
        damageTexts.push_back(text);
    }

    void heal(int amount){
        hp = std::min(150, hp + amount);
        float offsetX = (rand() % 40) - 20;
        float offsetY = (rand() % 40) - 20;

        DamageText text;
        text.x = x + offsetX;
        text.y = y + offsetY;
        text.speedY = -4.0f;
        text.alpha = 1.0f;
        text.value = +amount;
        text.color = Color{ 50, 255, 50, 255 };
        damageTexts.push_back(text);
    }

    bool isAttack = false;
    int attTime;

    void dd(){
        attTime = 5;
        if(isAttack) return;
        isAttack = true;
        for (int i = (int)enems.size() - 1; i >= 0; --i){
            float distance = std::hypot(enems[i].x - x, enems[i].y - y);
            if (distance > pattackRange) continue;
            if((enems[i].hp - damage) <= 0) {
                enems.erase(enems.begin() + i);
                kills++;
                maxKills = std::max(maxKills, kills);
                StatsSave();
            } else {
                enems[i].hit(damage);
            }
        }
    }
};

Player player;
Player enemm;
bool isHave = false;
bool isDie = false;
std::vector<Player> enems;
std::vector<cord> heals;

void StatsSave(){
    std::ofstream ostt("stats.txt");
    maxKills = std::max(maxKills, player.kills);
    if(ostt.is_open()){
        ostt << maxKills;
        ostt.close();
    }
}

int heal = 20;

skin walls;

std::vector<Rectangle> walls_geo1 = {
    {40 * mult, 40 * mult, 210 * mult, 10 * mult},
    {266 * mult, 40 * mult, 294 * mult, 10 * mult},
    {40 * mult, 50 * mult, 10 * mult, 200 * mult},
    {40 * mult, 280 * mult, 10 * mult, 370 * mult},
    {550 * mult, 40 * mult, 10 * mult, 480 * mult},
    {550 * mult, 550 * mult, 10 * mult, 110 * mult},
    {40 * mult, 650 * mult, 310 * mult, 10 * mult},
    {366 * mult, 650 * mult, 194 * mult, 10 * mult},
    {50 * mult, 110 * mult, 133 * mult, 10 * mult},
    {416 * mult, 110 * mult, 10 * mult, 200 * mult},
    {233 * mult, 200 * mult, 10 * mult, 180 * mult},
    {50 * mult, 380 * mult, 66 * mult, 10 * mult},
    {199 * mult, 380 * mult, 284 * mult, 10 * mult},
    {383 * mult, 390 * mult, 10 * mult, 270 * mult},
    {166 * mult, 500 * mult, 117 * mult, 10 * mult}
};

std::vector<Rectangle> exits_geo1 = {
    {250 * mult, 40 * mult, 16 * mult, 10 * mult},
    {40 * mult, 250 * mult, 10 * mult, 30 * mult},
    {550 * mult, 520 * mult, 10 * mult, 30 * mult},
    {350 * mult, 650 * mult, 16 * mult, 10 * mult}
};

std::vector<BoundingBox> exitsB_geo1 = {
    { {250 * mult, 40 * mult, -5.0f}, {266 * mult, 50 * mult, 5.0f} },
    { {40 * mult, 250 * mult, -5.0f}, {50 * mult, 280 * mult, 5.0f} },
    { {550 * mult, 520 * mult, -5.0f}, {560 * mult, 550 * mult, 5.0f} },
    { {350 * mult, 650 * mult, -5.0f}, {366 * mult, 660 * mult, 5.0f} }
};

cord spawn1u = { 260, 70 };
cord spawn1l = { 70, 260 };
cord spawn1r = { 530, 530 };
cord spawn1d = { 360, 630 };

std::vector<Rectangle> walls_geo2 = {
    {50 * mult, 40 * mult, 270 * mult, 10 * mult},
    {340 * mult, 40 * mult, 200 * mult, 10 * mult},
    {40 * mult, 40 * mult, 10 * mult, 90 * mult},
    {40 * mult, 160 * mult, 10 * mult, 480 * mult},
    {540 * mult, 40 * mult, 10 * mult, 390 * mult},
    {540 * mult, 460 * mult, 10 * mult, 180 * mult},
    {50 * mult, 640 * mult, 190 * mult, 10 * mult},
    {260 * mult, 640 * mult, 280 * mult, 10 * mult},
    {50 * mult, 160 * mult, 120 * mult, 10 * mult},
    {410 * mult, 160 * mult, 130 * mult, 10 * mult},
    {170 * mult, 100 * mult, 10 * mult, 240 * mult},
    {170 * mult, 340 * mult, 250 * mult, 10 * mult},
    {270 * mult, 370 * mult, 10 * mult, 30 * mult},
    {270 * mult, 490 * mult, 10 * mult, 60 * mult},
    {280 * mult, 520 * mult, 260 * mult, 10 * mult}
};

std::vector<Rectangle> exits_geo2 = {
    {320 * mult, 40 * mult, 20 * mult, 10 * mult},
    {40 * mult, 130 * mult, 10 * mult, 30 * mult},
    {540 * mult, 430 * mult, 10 * mult, 30 * mult},
    {240 * mult, 640 * mult, 20 * mult, 10 * mult}
};

std::vector<BoundingBox> exitsB_geo2 = {
    { {320 * mult, 40 * mult, -5.0f}, {340 * mult, 50 * mult, 5.0f} },
    { {40 * mult, 130 * mult, -5.0f}, {50 * mult, 160 * mult, 5.0f} },
    { {540 * mult, 430 * mult, -5.0f}, {550 * mult, 460 * mult, 5.0f} },
    { {240 * mult, 640 * mult, -5.0f}, {260 * mult, 650 * mult, 5.0f} }
};

cord spawn2u = { 330 , 70 };
cord spawn2l = { 70, 145 };
cord spawn2r = { 510, 445 };
cord spawn2d = { 250, 610 };

std::vector<BoundingBox> exits;
Color exitColor;

void room1() {
    for (const auto& wall : walls_geo1) DrawRectangleRec(wall, walls.color);
    for (const auto& exit : exits_geo1) DrawRectangleRec(exit, exitColor);
}

void room2() {
    for (const auto& wall : walls_geo2) DrawRectangleRec(wall, walls.color);
    for (const auto& exit : exits_geo2) DrawRectangleRec(exit, exitColor);
}

bool check_wall_collision(float nextX, float nextY, float radius, int rom = 1, char caller = 'p') {
    std::vector<Rectangle> walls;
    std::vector<Rectangle> exits_rect;
    switch(rom){
        case 1: walls = walls_geo1; exits_rect = exits_geo1; break;
        case 2: walls = walls_geo2; exits_rect = exits_geo2; break;
    }
    for (const auto& wall : walls) {
        if (CheckCollisionCircleRec({nextX, nextY}, radius, wall)) return true;
    }
    if (caller == 'e') {
        for (const auto& exit : exits_rect) {
            if (CheckCollisionCircleRec({nextX, nextY}, radius, exit)) return true;
        }
    }
    return false;
}

void chooseRandRoom(int exitIndex) {
    int res;
    do {
        res = rand() % 2 + 1;
    } while (res == now);

    now = res;

    switch(res){
        case 1:
            if(exitIndex == 0) { player.x = spawn1d.x; player.y = spawn1d.y; }
            else if(exitIndex == 1) { player.x = spawn1r.x; player.y = spawn1r.y; }
            else if(exitIndex == 2) { player.x = spawn1l.x; player.y = spawn1l.y; }
            else if(exitIndex == 3) { player.x = spawn1u.x; player.y = spawn1u.y; }
            break;
        case 2:
            if(exitIndex == 0) { player.x = spawn2d.x; player.y = spawn2d.y; }
            else if(exitIndex == 1) { player.x = spawn2r.x; player.y = spawn2r.y; }
            else if(exitIndex == 2) { player.x = spawn2l.x; player.y = spawn2l.y; }
            else if(exitIndex == 3) { player.x = spawn2u.x; player.y = spawn2u.y; }
            break;
    }
    enems.clear();
    heals.clear();
    isHave = false;
}

std::vector<Player> SpawnEnems(){
    std::random_device dc;
    std::mt19937 gen(dc());
    std::uniform_int_distribution<int> distr_enemx(60, 540);
    std::uniform_int_distribution<int> distr_enemy(60, 590);

    Player enem;
    std::vector<Player> ppp;

    for(int i{}; i < maxEnem; i++){
        int x{}, y{};
        int attempts = 0;
        do {
            x = distr_enemx(gen);
            y = distr_enemy(gen);
            attempts++;
        } while((check_wall_collision(x, y, enemm.skin.size, now, 'e') || std::hypot(x - player.x, y - player.y) < 50) && attempts < 100);

        if (attempts >= 100) continue;

        enem.x = x;
        enem.y = y;
        enem.speed = enemm.speed;
        enem.skin = enemm.skin;
        enem.hp = enemm.hp;
        ppp.push_back(enem);
    }
    return ppp;
}

int spd{};
const int skip = 45;

std::vector<cord> SpawnHeals(){
    std::random_device dc;
    std::mt19937 gen(dc());
    std::uniform_int_distribution<int> distr_enemx(60, 540);
    std::uniform_int_distribution<int> distr_enemy(60, 590);
    std::vector<cord> HealS;

    for(int i{}; i < maxHeals; i++){
        int x{}, y{};
        int attempts = 0;
        do {
            x = distr_enemx(gen);
            y = distr_enemy(gen);
            attempts++;
        } while((check_wall_collision(x, y, 10, now, 'e') || std::hypot(x - player.x, y - player.y) < 50) && attempts < 100);

        if (attempts >= 100) continue;

        cord h = {x, y};
        HealS.push_back(h);
    }
    return HealS;
}

void checkHealPickup(){
    for (int i = (int)heals.size() - 1; i >= 0; i--) {
        if (CheckCollisionCircles({player.x, player.y}, player.skin.size, {(float)heals[i].x, (float)heals[i].y}, 6.0f)) {
            player.heal(heal);
            heals.erase(heals.begin() + i);
        }
    }
}

void EnemsAi(){
    if(!isHave){
        enems = SpawnEnems();
        heals = SpawnHeals();
        isHave = true;
    }
    if(enems.empty()){
        isHave = false;
        return;
    } 

    if(!heals.empty()){
        for(const cord &c : heals){
            DrawRectangle(c.x, c.y, 7, 7, GREEN);
        }
    }

    static int frameCounter = 0;
    static std::vector<int> directions(maxEnem, 0); 
    
    frameCounter++;
    bool chooseNewDirection = false;
    if (frameCounter >= 25) {
        chooseNewDirection = true;
        frameCounter = 0;
    }

    for (size_t i = 0; i < enems.size(); ++i) {
        Player &enemy = enems[i];
        float distance = std::hypot(enemy.x - player.x, enemy.y - player.y);

        if (distance < attackRange) {
            if(spd == skip){
                player.hit(enDamage);
                spd = 0;
            }
            DrawCircle(enemy.x, enemy.y, enemy.skin.size, enemy.skin.color);
            spd++;
            continue; 
        }

        if (chooseNewDirection) {
            directions[i] = rand() % 8;
        }

        int dx = 0, dy = 0;
        switch (directions[i]) {
            case 0: dx = -enemy.speed; break;
            case 1: dx =  enemy.speed; break;
            case 2: dy =  enemy.speed; break;
            case 3: dy = -enemy.speed; break;
            case 4: dy = -enemy.speed; dx = -enemy.speed; break;
            case 5: dy = -enemy.speed; dx =  enemy.speed; break;
            case 6: dy =  enemy.speed; dx = -enemy.speed; break;
            case 7: dy =  enemy.speed; dx =  enemy.speed; break;
        }

        int nextX = enemy.x + dx;
        int nextY = enemy.y + dy;

        if (!check_wall_collision(nextX, nextY, enemy.skin.size, now, 'e')) {
            enemy.x = nextX;
            enemy.y = nextY;
        } else {
            directions[i] = rand() % 4;
        }
        DrawCircle(enemy.x, enemy.y, enemy.skin.size, enemy.skin.color);
    }
}

void HUD(){
    DrawText(TextFormat("%i Lives", player.hp), GetScreenWidth()-150, 20, 20, WHITE);
    DrawText(TextFormat("%i Damage", player.damage), GetScreenWidth()-150, 40, 20, WHITE);
    DrawText(TextFormat("%i Kills", player.kills), GetScreenWidth()-150, 60, 20, WHITE);
    DrawText(TextFormat("%i Max Kills", maxKills), GetScreenWidth()-150, 80, 20, WHITE);
    if(isDie){
        int textWidth = MeasureText("DIE", 50); 
        DrawText("DIE", (GetScreenWidth()-textWidth)/2, (GetScreenHeight()-50)/2, 50, GRAY);
    }
}

int main() {
    InitWindow(wth * mult, hht * mult, "Cyber grind v0.2");
    SetTargetFPS(60);

    player.x = 300;
    player.y = 300;
    player.skin.size = 7.0f;
    player.speed = 2;
    player.damage = 10;
    player.hp = 100;
    player.skin.color = Color(57, 255, 20, 255);
    
    Color enemyColor = Color(255, 0, 128, 255);
    exitColor = Color(0, 245, 255, 255);
    walls.color = Color(38, 20, 64, 255);
    Color floorColor = Color(8, 5, 12, 255);

    enemm.skin.color = enemyColor;
    enemm.skin.size = 6;
    enemm.speed = 1;
    enemm.hp = 75;

    float czoom = 1.0f;
    StatsOpen();

    Camera2D cam = { 0 };
    cam.offset = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
    cam.rotation = 0.0f;

    bool isDebug = true;
    StatsSave();

    while(!WindowShouldClose()){
        if(player.hp <= 0) isDie = true;

        int wall_off = 0;

        if(!isDie){
            if(IsKeyDown(KEY_W) && player.y >= 50 + wall_off && !check_wall_collision(player.x, player.y - player.speed, player.skin.size, now)) player.y -= player.speed;
            if(IsKeyDown(KEY_S) && player.y <= 650 - wall_off && !check_wall_collision(player.x, player.y + player.speed, player.skin.size, now)) player.y += player.speed;
            if(IsKeyDown(KEY_A) && player.x >= 50 + wall_off && !check_wall_collision(player.x - player.speed, player.y, player.skin.size, now)) player.x -= player.speed;
            if(IsKeyDown(KEY_D) && player.x <= 550 - wall_off && !check_wall_collision(player.x + player.speed, player.y, player.skin.size, now)) player.x += player.speed;
            
            checkHealPickup();

            for (size_t i = 0; i < exits.size(); i++) {
                if (CheckCollisionBoxSphere(exits[i], {player.x, player.y, 0}, player.skin.size)) {
                    chooseRandRoom(i);
                    break;
                }
            }

            if(IsKeyPressed(KEY_SPACE)) player.dd();
            float wheel = GetMouseWheelMove();
            czoom += wheel * 0.03f;
            czoom = std::clamp(czoom, 0.9f, 2.0f);
        }
        if (player.hitTimer > 0 && !player.isAttack) {
            player.hitTimer--;
            player.skin.color = WHITE;
        } else {
            player.skin.color = Color(57, 255, 20, 255);
        }

        if(player.attTime > 0) player.attTime--;
        else player.isAttack = false;

        for (size_t i = 0; i < damageTexts.size(); ) {
            damageTexts[i].y += damageTexts[i].speedY;
            damageTexts[i].speedY *= 0.92f;
            damageTexts[i].alpha -= 0.02f;
            if (damageTexts[i].alpha <= 0.0f) damageTexts.erase(damageTexts.begin() + i);
            else i++;
        }

        if(czoom >= 1.2f) cam.target = { player.x, player.y };
        else cam.target = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
        cam.zoom = czoom;

        if(now == 1) exits = exitsB_geo1;
        else if(now == 2) exits = exitsB_geo2;

        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(cam);
                DrawRectangle(50 * mult, 50 * mult, 500, 600, floorColor);
                
                if(now == 1) room1();
                else if(now == 2) room2();

                DrawCircle(player.x, player.y, player.skin.size, player.skin.color);
                
                if(!isDie) EnemsAi();

                if (player.attTime > 0) {
                    DrawCircle(player.x, player.y, pattackRange, Color{255, 20, 20, 80});
                    DrawCircleLines(player.x, player.y, pattackRange, Color{200, 20, 20, 255});
                }

                for (const auto& text : damageTexts) {
                    unsigned char alphaByte = (unsigned char)(text.alpha * 255);
                    Color textColor = Color{ text.color.r, text.color.g, text.color.b, alphaByte };
                    if(text.value < 0) DrawText(TextFormat("%i", text.value), text.x, text.y, 18, textColor);
                    if(text.value > 0) DrawText(TextFormat("+%i", text.value), text.x, text.y, 18, textColor);
                }
            EndMode2D();

        if(isDebug){
            DrawText(TextFormat("Zoom: %.2f", czoom), 10, 10, 20, GREEN);
            DrawText(TextFormat("XY: %.2f %.2f", player.x, player.y), 10, 40, 20, GREEN);
        }
        HUD();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}