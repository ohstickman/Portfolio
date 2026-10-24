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

int maxEnem = 5;
int enDamage = 5;
int enSpeed = 5;
int attackRange = 15;
int pattackRange = 25;

float mult = 1.0f;

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
                }
                else {
                    enems[i].hit(damage);
                }
        }
    }
};

Player player;

void StatsSave(){
    std::ofstream ostt("stats.txt");
    
    maxKills = std::max(maxKills, player.kills);
    if(ostt.is_open()){
        ostt << maxKills;
        ostt.close();
    }

}

std::vector<Player> enems;

skin walls;




    std::vector<Rectangle> walls_geo1 = {
        {50 * mult, 40 * mult, 200 * mult, 10 * mult},
        {266 * mult, 40 * mult, 284 * mult, 10 * mult},
        
        {40 * mult, 40 * mult, 10 * mult, 210 * mult},
        {40 * mult, 280 * mult, 10 * mult, 380 * mult},
        
        {550 * mult, 40 * mult, 10 * mult, 480 * mult},
        {550 * mult, 550 * mult, 10 * mult, 110 * mult},
        
        {50 * mult, 650 * mult, 300 * mult, 10 * mult},
        {366 * mult, 650 * mult, 184 * mult, 10 * mult},

        {50 * mult, 110 * mult, 133 * mult, 10 * mult},
        {416 * mult, 110 * mult, 10 * mult, 200 * mult},
        {233 * mult, 200 * mult, 10 * mult, 180 * mult},
        
        {50 * mult, 380 * mult, 66 * mult, 10 * mult},
        {199 * mult, 380 * mult, 283 * mult, 10 * mult},
        
        {383 * mult, 390 * mult, 10 * mult, 270 * mult},
        {166 * mult, 500 * mult, 116 * mult, 10 * mult}
    };

    std::vector<Rectangle> exits_geo1 = {
        {250 * mult, 40 * mult, 16 * mult, 10 * mult},
        {40 * mult, 250 * mult, 10 * mult, 30 * mult},
        {550 * mult, 520 * mult, 10 * mult, 30 * mult},
        {350 * mult, 650 * mult, 16 * mult, 10 * mult}
    };

Color exitColor;
void room1() {
    for (const auto& wall : walls_geo1) {
        DrawRectangleRec(wall, walls.color);
    }

    for (const auto& exit : exits_geo1) {
        DrawRectangleRec(exit, exitColor);
    }
}



bool check_wall_collision(float nextX, float nextY, float radius, int rom = 1) {
    std::vector<Rectangle> walls;

    switch(rom){
        case 1:
            walls = walls_geo1;
            break;
    }
    
    for (const auto& wall : walls) {
        if (CheckCollisionCircleRec({nextX, nextY}, radius, wall)) {
            return true;
        }
    }
    return false;
}

Player enemm;

bool isHave = false;


std::vector<Player> SpawnEnems(){
    std::random_device dc;
    std::mt19937 gen(dc());
    std::uniform_int_distribution<int> distr_enemx(60, 540);
    std::uniform_int_distribution<int> distr_enemy(60, 590);

    Player enem;
    std::vector<Player> ppp;

    for(int i{}; i < maxEnem; i++){
        int x{}, y{};
        do{
            x = distr_enemx(gen);
            y = distr_enemy(gen);
        } while(check_wall_collision(x, y, enemm.skin.size));

        enem.x = x;
        enem.y = y;
        // ВАЖНО: передаем индивидуальные параметры каждому врагу из шаблона enemm
        enem.speed = enemm.speed;
        enem.skin = enemm.skin;
        enem.hp = enemm.hp;
        
        ppp.push_back(enem);
    }
    return ppp;
}

int spd{};
const int skip = 45;
void EnemsAi(){
    if(!isHave){
        enems = SpawnEnems();
        isHave = true;
    }
    if(enems.empty()){
        isHave = false;
        return;
    } 



    // Таймер: враги меняют направление или шагают не каждый кадр, а раз в 10 кадров
    static int frameCounter = 0;
    static std::vector<int> directions(maxEnem, 0); // Храним направление для каждого врага
    
    frameCounter++;
    bool chooseNewDirection = false;
    if (frameCounter >= 25) { // Чем больше число, тем реже они меняют направление
        chooseNewDirection = true;
        frameCounter = 0;
    }

    for (size_t i = 0; i < enems.size(); ++i) {
        Player &enemy = enems[i];

        // 1. Проверка дистанции атаки
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

        // 2. Выбор направления (если пришло время)
        if (chooseNewDirection) {
            directions[i] = rand() % 7;
        }

        int dx = 0, dy = 0;
        switch (directions[i]) {
            case 0: dx = -enemy.speed; break; // влево
            case 1: dx =  enemy.speed; break; // вправо
            case 2: dy =  enemy.speed; break; // вниз
            case 3: dy = -enemy.speed; break; // вверх
            case 4: dy = -enemy.speed; dx = -enemy.speed; break; // вверх
            case 5: dy = -enemy.speed; dx =  enemy.speed; break; // вверх
            case 6: dy =  enemy.speed; dx = -enemy.speed; break; // вниз
            case 7: dy =  enemy.speed; dx =  enemy.speed; break; // вниз
        }

        int nextX = enemy.x + dx;
        int nextY = enemy.y + dy;

        // 3. Проверка коллизии (добавлено НЕ '!')
        if (!check_wall_collision(nextX, nextY, enemy.skin.size)) {
            enemy.x = nextX;
            enemy.y = nextY;
        } else {
            // Если врезался в стену — пусть сразу сменит направление на случайное
            directions[i] = rand() % 4;
        }

        // Отрисовка конкретного врага с его размером и цветом
        DrawCircle(enemy.x, enemy.y, enemy.skin.size, enemy.skin.color);
    }
}

bool isDie = false;

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
    player.skin.color;
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

        if(player.hp <= 0)
            isDie = true;

        int wall_off = 12;

        if(!isDie){
            if(IsKeyDown(KEY_W) && player.y >= 50+wall_off && !check_wall_collision(player.x, player.y - player.speed, player.skin.size, 1)) player.y -= player.speed;
            if(IsKeyDown(KEY_S) && player.y <= 650-wall_off && !check_wall_collision(player.x, player.y + player.speed, player.skin.size, 1)) player.y += player.speed;
            if(IsKeyDown(KEY_A) && player.x >= 50+wall_off && !check_wall_collision(player.x - player.speed, player.y, player.skin.size, 1)) player.x -= player.speed;
            if(IsKeyDown(KEY_D) && player.x <= 550-wall_off && !check_wall_collision(player.x + player.speed, player.y, player.skin.size, 1)) player.x += player.speed;
            if(IsKeyPressed(KEY_SPACE)) player.dd();
            float wheel = GetMouseWheelMove();
            czoom += wheel * 0.03f;
            czoom = std::clamp(czoom, 0.9f, 2.0f);
        }
        if (player.hitTimer > 0 && !player.isAttack) {
            player.hitTimer--;
            player.skin.color = WHITE;
        }
        else {
            player.skin.color = Color(57, 255, 20, 255);
        }

        if(player.attTime > 0){
            player.attTime--;
        }
        else{
            player.isAttack = false;
        }

        // 1. Физика летящего текста (Обновление позиций)
        for (size_t i = 0; i < damageTexts.size(); ) {
            damageTexts[i].y += damageTexts[i].speedY; // Двигаем вверх
            damageTexts[i].speedY *= 0.92f;           // Замедление (умножаем на число < 1.0)
            damageTexts[i].alpha -= 0.02f;            // Плавное исчезновение

            // Если текст полностью растворился, удаляем его из вектора
            if (damageTexts[i].alpha <= 0.0f) {
                damageTexts.erase(damageTexts.begin() + i);
            } else {
                i++; // Идем к следующему, только если текущий не удалили
            }
        }

        
        if(czoom >= 1.2f)
            cam.target = { player.x, player.y };
        else
            cam.target = {GetScreenWidth()/2.0f, GetScreenHeight()/2.0f};
        cam.zoom = czoom;

        BeginDrawing();
            ClearBackground(BLACK);
            
            BeginMode2D(cam);
                DrawRectangle(50 * mult, 50*mult, 500, 600, floorColor);
                DrawCircle(player.x, player.y, player.skin.size, player.skin.color);
                if(!isDie){EnemsAi();}
                room1();

                if (player.attTime > 0) {
                    DrawCircle(player.x, player.y, pattackRange, Color{255, 20, 20, 80}); // Сделали прозрачность 80, чтобы не слепило
                    DrawCircleLines(player.x, player.y, pattackRange, Color{200, 20, 20, 255});
                }

                for (const auto& text : damageTexts) {
                    unsigned char alphaByte = (unsigned char)(text.alpha * 255);
                    Color textColor = Color{ 255, 50, 50, alphaByte }; // Красный затухающий цвет

                    DrawText(TextFormat("%i", text.value), text.x, text.y, 18, textColor);
                }

            EndMode2D();

        if(isDebug){
            DrawText(TextFormat("Zoom: %.2f", czoom), 10, 10, 20, GREEN);
            DrawText(TextFormat("XY: %.2f %.2f", player.x, player.y) , 10, 40, 20, GREEN);
        }

        HUD();
        EndDrawing();

    }

    CloseWindow();

    return 0;
}