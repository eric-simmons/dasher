
// dasher
#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

int main()
{
    // window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher");

    //(pixels/second)/second
    const int gravity{1000};

    Texture scarfy = LoadTexture("textures/scarfy.png");
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    AnimData nebulae[2]{};

    for (int i = 0; i < 2; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
    };

    nebulae[0].pos.x = windowDimensions[0];
    nebulae[1].pos.x = windowDimensions[0] + 300;

    AnimData scarfyData; 
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;
    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0 / 12.0;
    scarfyData.runningTime = 0.0;

    int nebulaVelocity{-100};
    int velocity{};

    bool isInAir{false};
    // pixles/sec
    const int jumpVel{-600};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time // time since last frame
        float dT = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);

        // update scarfy animation frame and freeze scarfy when he jumps
        scarfyData.runningTime += dT;
        if ((scarfyData.runningTime >= scarfyData.updateTime) && !isInAir)
        {
            scarfyData.runningTime = 0;
            scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
            scarfyData.frame++;
            if (scarfyData.frame > 5)
            {
                scarfyData.frame = 0;
            }
        }
        else
        {
            scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
        }

        // update nebula animation frame
        nebulae[0].runningTime += dT;
        // time to update frame
        if (nebulae[0].runningTime >= nebulae[0].updateTime)
        {
            nebulae[0].runningTime = 0.0;
            nebulae[0].rec.x = nebulae[0].frame * nebulae[0].rec.width;
            nebulae[0].frame++;
            if (nebulae[0].frame > 7)
            {
                nebulae[0].frame = 0;
            }
        }

        // ground check
        if (scarfyData.pos.y >= windowDimensions[1] - scarfyData.rec.height)
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            velocity += gravity * dT;
            isInAir = true;
        }

        // jump check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }

        // update nebula position
        nebulae[0].pos.x += nebulaVelocity * dT;

        // update scarfy position
        scarfyData.pos.y += velocity * dT;

        // draw nebula
        DrawTextureRec(nebula, nebulae[0].rec, nebulae[0].pos, WHITE);

        // draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}
