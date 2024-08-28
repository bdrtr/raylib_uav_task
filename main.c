#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <math.h>

#define UAV_SHOW_ANGLE_CLOSE 50
//#define UAV_SHOW_ANGLE_DISTANCE 10
#define SCREEN_UPPER 40
#define UAV_SPEED_X 10
#define GRAVITION 0.3
#define WIND_X 0.0
#define WIND_Y 0

typedef struct 
{
    float x;
    float y;
    float width;
    float height;
    Rectangle rec;

    Vector2 speed;
    Vector2 cam[3];

    float angle;

    bool gameObj;

} UAV;

typedef struct 
{
    float x;
    float y;
    float radius;

    Vector2 speed;

    float massive;
    bool gameObj;

}Ball;



int main(void) {

    const int screenWidh = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidh, screenHeight, "prog");

    UAV uav = {GetScreenWidth() - 60.0f,  SCREEN_UPPER+10, 20, 10, (Rectangle){GetScreenWidth() - 60.0f, SCREEN_UPPER+10, 20,10},
               (Vector2){UAV_SPEED_X, 0.0f}};

    Ball ball = {uav.rec.x, uav.rec.y, 4, (Vector2){0.0f,0.0f}};
    ball.massive = 1;

    int areaRadius = 16;
    Vector2 areaPos = (Vector2){GetScreenWidth()/2.0f,GetScreenHeight() - 56 + areaRadius};


    double h = (1080-90)*2/GRAVITION;
    double _h = sqrtf(h);
    int  UAV_SHOW_ANGLE_DISTANCE = _h * uav.speed.x - areaRadius/2 - 8;




    Vector2  uavCam[3] = {  (Vector2){uav.rec.x,uav.rec.y},
                            (Vector2){uav.rec.x-UAV_SHOW_ANGLE_CLOSE, GetScreenHeight()-20},
                            (Vector2){uav.rec.x-UAV_SHOW_ANGLE_DISTANCE, GetScreenHeight()-20}};
    
    uav.cam[0] = uavCam[0];
    uav.cam[1] = uavCam[1];
    uav.cam[2] = uavCam[2];

    unsigned frameCounter = 0;
    bool pause = 0;
    bool collision = false;
    float angle = 0.0f;
    Vector2 wind = (Vector2){WIND_X, WIND_Y};
    bool shoot_ball = false;
    bool complete = false;


    
    
    SetTargetFPS(60);

    while(!WindowShouldClose()) {


        if (!pause) {

            uav.rec.x -= uav.speed.x + wind.x;
            uav.rec.y -= uav.speed.y + wind.y;

            uav.cam[0] = (Vector2){uav.rec.x, uav.rec.y };
            uav.cam[1] = (Vector2){uav.rec.x-UAV_SHOW_ANGLE_CLOSE, GetScreenHeight()-20};
            uav.cam[2] = (Vector2){uav.rec.x-UAV_SHOW_ANGLE_DISTANCE, GetScreenHeight()-20};


            if (!shoot_ball) {

                ball.x = uav.rec.x + uav.width;
                ball.y = uav.rec.y + uav.height;

            } 
            
            else { 
                ball.x -= (ball.speed.x + wind.x+ uav.speed.x) ;
                ball.speed.y += GRAVITION;
                ball.y += ball.speed.y + wind.y;
            }

            if (ball.y >= GetScreenHeight()-40) {
                ball.y = GetScreenHeight()-40;
                ball.speed.y = 0;
                uav.speed.x = 0;
                ball.speed.x = 0;
            }
        }

        else frameCounter++;

        if (IsKeyPressed(KEY_SPACE)) pause = !pause;
        //if (IsKeyPressed(KEY_W)) 
        if (IsKeyPressed(KEY_S)) shoot_ball = true;

        if (CheckCollisionCircleRec(areaPos, areaRadius, 
                (Rectangle){uav.cam[2].x, GetScreenHeight()-uav.rec.height*6, UAV_SHOW_ANGLE_DISTANCE-UAV_SHOW_ANGLE_CLOSE, 40})) {
                    
            collision = true;
            

        } else collision = false;

        if (CheckCollisionCircles(areaPos, areaRadius, (Vector2){ball.x, ball.y}, ball.radius)) {

            complete = true;
        }

        //uav.angle = atan2f(uav.cam[2].x, uav.cam[2].y);
        
        //DRAW
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawRectangleRec((Rectangle){0,0, GetScreenWidth(),SCREEN_UPPER},collision ? BLACK:LIGHTGRAY); //ust bilgilendirme
            DrawText(TextFormat("wind --> (+) (x -> %.2f, y -> %.2f, ball_speed -> %.2f",wind.x, wind.y, ball.speed.y), 180, 200, 20, LIGHTGRAY);
            if (collision) {
               DrawText("COLLISION!", GetScreenWidth()/2-20, 100, 20,BLACK);
               shoot_ball = true; 
            }
            DrawText(TextFormat("V_x = %.2f, V_y = %.2f, x -> %.2f, y -> %.2f angle -> %.2f",
                                uav.speed.x, uav.speed.y, uav.rec.x, uav.rec.y, uav.angle), 100, 10, 20, collision ? WHITE:BLACK);
            DrawRectangle(0,GetScreenHeight()-60, GetScreenWidth(), 40, GREEN); // yer yüzü
            DrawRectangleLines(0,GetScreenHeight()-60,GetScreenWidth(), 40, BLACK); // gorus acisi
            DrawCircleV(areaPos, areaRadius, RED); //alan
            DrawCircleV((Vector2){ball.x,ball.y}, ball.radius, BLUE);
            DrawTriangleLines(uav.cam[1], uav.cam[0], uav.cam[2], BLACK);

            if (complete) {
                DrawText("TASK IS DONE!!", GetScreenWidth()/2-20, 100, 20,BLACK);
            }
            
            DrawRectangleRec(uav.rec, RED);
            //DrawRectangleRec(,RED);

            DrawFPS(10,10);

            if (pause) DrawText("PAUSE!", GetScreenWidth()/2, GetScreenHeight()/2, 50, LIGHTGRAY);

        EndDrawing();
        
    }

    CloseWindow();
    return 0;
}