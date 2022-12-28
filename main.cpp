
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

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{

    // window dimensions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;
    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher");
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    float mgX{};
    float fgX{};

    //(pixels/second)/second
    const int gravity{1000};
    int nebulaVelocity{-300};
    int velocity{};

    const int sizeOfNebulae{6};
    AnimData nebulae[sizeOfNebulae]{};
    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].pos.x = windowDimensions[0] + i * 500;
    };

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

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

    bool isInAir{false};
    bool collision{};
    // pixles/sec
    const int jumpVel{-600};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // delta time // time since last frame
        const float dT = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dT;
        mgX -= 40 * dT;
        fgX -= 80 * dT;

        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }

        // draw background
        Vector2 bg1Pos{bgX, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);
        // draw midground
        Vector2 mg1Pos{mgX, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);
        // draw foreground
        Vector2 fg1Pos{fgX, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // update scarfy animation frame and freeze scarfy when he jumps
        scarfyData.runningTime += dT;
        if ((scarfyData.runningTime >= scarfyData.updateTime) && !isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }
        else
        {
            scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
        }

        // update animation frame for each nebulae
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        // ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
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

        // update each nebula position
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i].pos.x += nebulaVelocity * dT;
        }

        finishLine += nebulaVelocity * dT;

        // update scarfy position
        scarfyData.pos.y += velocity * dT;

     

        for (AnimData nebula : nebulae)
        {

            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad,
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height,
            };
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            DrawText("Game Over", windowDimensions[0]/2, windowDimensions[1]/2, 32, WHITE);
            // game over
        }
        else if (!collision && (scarfyData.pos.x > finishLine))
        {
            DrawText("You Win!", windowDimensions[0]/2, windowDimensions[1]/2, 32, WHITE);
        }
        else
        {
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        // draw nebula

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}
