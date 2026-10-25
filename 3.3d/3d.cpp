#include "raylib.h"


bool DrawButton(Rectangle rect, const char* text, Color bColor, Color hColor) {
    Vector2 mousePos = GetMousePosition();
    Color cColor = bColor;
    bool cс = false;

    if (CheckCollisionPointRec(mousePos, rect)) {
        cColor = hColor;
        
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            cс = true;
        }
    }
    
    DrawRectangleRec(rect, cColor);

    DrawRectangleLinesEx(rect, 2, BLACK);
    
    DrawText(text, rect.x + 15, rect.y + (rect.height / 2) - 10, 20, WHITE);

    return cс;
}


float DrawSlider(Rectangle rect, float value, Color bColor, Color hColor) {
    Vector2 mousePos = GetMousePosition();
    static bool isDragging = false;
    static Rectangle activeSlider = { 0 };

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        if (CheckCollisionPointRec(mousePos, rect) || (isDragging && activeSlider.y == rect.y)) {
            isDragging = true;
            activeSlider = rect;
        }
    } else {
        isDragging = false;
    }

    if (isDragging && activeSlider.y == rect.y) {
        value = (mousePos.x - rect.x) / rect.width;
        if (value < 0.0f) value = 0.0f;
        if (value > 1.0f) value = 1.0f;
    }

    DrawRectangleRec(rect, bColor);
    DrawRectangleLinesEx(rect, 1, BLACK);

    float handleX = rect.x + (value * rect.width) - 5;
    DrawRectangle(handleX, rect.y - 4, 10, rect.height + 8, hColor);
    DrawRectangleLines(handleX, rect.y - 4, 10, rect.height + 8, BLACK);

    return value;
}



int main(){
    InitWindow(1000, 800, "3D");
    SetTargetFPS(60);

    float rotationSpeed = 0.3f;



    Camera3D camera = { 0 };
    camera.position = { 10.0f, 10.0f, 10.0f };  // где камера
    camera.target = { 0.0f, 0.0f, 0.0f };       // куда смотрит
    camera.up = { 0.0f, 1.0f, 0.0f };           // где "верх"
    camera.fovy = 45.0f;                        // угол обзора
    camera.projection = CAMERA_PERSPECTIVE;

    int ToDraw = 1;

    bool isWire = false;

    float cux =0.2;
    float cuy =0.2;
    float cuz =0.2;

    float rad =0.2;
    float wXo{0.25}, wYo{0.25};

    float rad1 =0.2;
    float rotx =0.0;
    float roty =0.0;
    float rotz =0.0;

    float minr = 0.1f;
    float maxr = 20.0f;
    int minY = 1;
    int maxX = 64;

    float min = 0.1f;
    float max = 20.0f;

    float maxxR{360}, minnR{0};

    int maxSl = 64;
    int minSl = 2;
        
    float go1 = 0.25;
    float go2 = 0.25;

    
    bool isGrid = false;

    while (!WindowShouldClose()) {
        

        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
            int wheel = GetMouseWheelMove();
            camera.position.z += wheel * rotationSpeed;
        }


        float cuuz = min + cuz * (max - min);
        float cuuy = min + cuy * (max - min);
        float cuux = min + cux * (max - min);


        float radd = minr + rad * (maxr - minr);
        float rad2 = minr + rad1 * (maxr - minr);
        float wX = minY + wXo * (maxX - minY);
        float wY = minY + wYo * (maxX - minY);
        
        float rXo = minnR + rotx * (maxxR - minnR);
        float rYo = minnR + roty * (maxxR - minnR);
        float rZo = minnR + rotz * (maxxR - minnR);


        int gridSl = minSl + go1 * (maxSl - minSl);;
        int gridSp = minSl + go2 * (maxSl - minSl);;


        BeginDrawing();
            ClearBackground(GRAY);

            BeginMode3D(camera);
                switch(ToDraw){
                    case 1: DrawCube({ 0, 0, 0 }, cuux, cuuy, cuuz, RAYWHITE); if(isWire) DrawCubeWires({ 0, 0, 0 }, cuux, cuuy, cuuz, BLACK);break;
                    case 2: DrawSphere({ 0, 0, 0 }, radd, RAYWHITE); if(isWire) DrawSphereWires({ 0, 0, 0 }, radd, wX, wY,  BLACK); break;
                    case 3: DrawCircle3D({0, 0, 0}, rad2, {rXo, rYo, rZo}, 0, WHITE);
                }

                if(isGrid){
                    DrawGrid(gridSl, gridSp);
                }
                
                

            EndMode3D();

            if(isGrid){
                go1 = DrawSlider({ (float)GetScreenWidth() - 400.0f, 110.0f, 180.0f, 30.0f }, go1, GRAY, DARKGRAY);
                go2 = DrawSlider({ (float)GetScreenWidth() - 400.0f, 200.0f, 180.0f, 30.0f }, go2, GRAY, DARKGRAY);
            }

            if(DrawButton({ (float)GetScreenWidth() - 400.0f, 30.0f, 180.0f, 60.0f }, "Grid", GRAY, DARKGRAY))
                isGrid =  isGrid ? false : true;

            if (DrawButton({ (float)GetScreenWidth() - 200.0f, 30.0f, 180.0f, 60.0f }, "Cube", GRAY, DARKGRAY))
                ToDraw = 1;
            
            if (DrawButton({ (float)GetScreenWidth() - 200.0f, 110.0f, 180.0f, 60.0f }, "Sphere", GRAY, DARKGRAY))
                ToDraw = 2;

            if (DrawButton({ (float)GetScreenWidth() - 200.0f, 200.0f, 180.0f, 60.0f }, "Circle", GRAY, DARKGRAY))
                ToDraw = 3;

            if (DrawButton({ (float)GetScreenWidth() - 200.0f, 290.0f, 180.0f, 60.0f }, "Wires", GRAY, DARKGRAY))
                isWire = isWire ? false : true;
            
            if(ToDraw == 1){
                cux = DrawSlider({ (float)GetScreenWidth() - 200.0f, 90+290.0f, 180.0f, 30.0f }, cux, GRAY, DARKGRAY);
                cuy = DrawSlider({ (float)GetScreenWidth() - 200.0f, 90+350.0f, 180.0f, 30.0f }, cuy, GRAY, DARKGRAY);
                cuz = DrawSlider({ (float)GetScreenWidth() - 200.0f, 90+410.0f, 180.0f, 30.0f }, cuz, GRAY, DARKGRAY);
            }
            if(ToDraw == 2){
                rad = DrawSlider({ (float)GetScreenWidth() - 200.0f, 90+290.0f, 180.0f, 30.0f }, rad, GRAY, DARKGRAY);
                wXo = DrawSlider({ (float)GetScreenWidth() - 200.0f, 90+350.0f, 180.0f, 30.0f }, wXo, GRAY, DARKGRAY);
                wYo = DrawSlider({ (float)GetScreenWidth() - 200.0f, 90+410.0f, 180.0f, 30.0f }, wYo, GRAY, DARKGRAY);
            }

            if(ToDraw == 3){
                rad1 = DrawSlider({ (float)GetScreenWidth() - 200.0f, 90+290.0f, 180.0f, 30.0f }, rad1, GRAY, DARKGRAY);
            }


        EndDrawing();
    }

    CloseWindow();
    return 0;
}