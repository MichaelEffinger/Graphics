#include <raylib-cpp.hpp>


//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
int screenWidth = 800;
int screenHeight = 450;

inline constexpr float DISTANCE_BETWEEN_BASES{6.0f};
inline constexpr float HEIGHT_OF_PLAYERS{1.0f};
inline constexpr float CAMERA_HEIGHT{2.0f};
inline constexpr float OOB_MULT{100.f};
inline constexpr float GRAVITY{9.31};
inline const raylib::Vector3 BALL_POSITION(0.0f, HEIGHT_OF_PLAYERS, 0.0f);                // Position where draw billboard
inline const raylib::Vector3 GOLFER_POSITION_RIGHT_HANDED{BALL_POSITION.x,HEIGHT_OF_PLAYERS, -2.5f};
inline const raylib::Vector3 CAMERA_POSITION(-DISTANCE_BETWEEN_BASES, CAMERA_HEIGHT, 0);
inline const raylib::Vector3& HOME_POSITION(CAMERA_POSITION);
inline const raylib::Vector3 OUT_OF_BOUNDS{OOB_MULT * DISTANCE_BETWEEN_BASES, OOB_MULT * DISTANCE_BETWEEN_BASES, OOB_MULT * DISTANCE_BETWEEN_BASES};
inline const raylib::Vector2 OUT_OF_BOUNDS_2D{OUT_OF_BOUNDS.x, OUT_OF_BOUNDS.y};

std::partial_ordering operator<=>(const raylib::Vector3& lhs, const raylib::Vector3& rhs)
{
    if (lhs == rhs) return std::partial_ordering::equivalent;
    const bool isLeftBigger{lhs.x > rhs.x || lhs.y > rhs.y || lhs.z > rhs.z};
    const bool isLeftSmaller{lhs.x < rhs.x || lhs.y < rhs.y || lhs.z < rhs.z};
    if (isLeftBigger && not isLeftSmaller) return std::partial_ordering::greater;
    if (isLeftSmaller && not isLeftBigger) return std::partial_ordering::less;
    return std::partial_ordering::unordered;
}

struct golf_ball { //why is Baseball one word but golf ball two?
    raylib::Vector3 position;
    raylib::Vector3 velocity;

    void update(float dt) {
        velocity.y -= GRAVITY * dt;
        position += velocity * dt;
        if (position.y < 0) position.y = 0.f;
    }

    void draw() const {
        DrawSphere(position, .1f, WHITE); // Small white sphere
    }
};

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    raylib::Window window(screenWidth, screenHeight, "No longer Baseball!");

    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    raylib::Camera camera(
    CAMERA_POSITION,  // Back-left-up offset
    BALL_POSITION,  // Look at batter
    raylib::Vector3(0.0f, 1.0f, 0.0f),
    45.0f,
    CAMERA_PERSPECTIVE);

    raylib::Texture2D bill("../testCat.png");     // Our texture billboard
    DisableCursor();

    golf_ball theBall{BALL_POSITION, {}};


    //std::vector<Baseball> baseballs;


    // Main game loop
    while (!window.ShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        camera.Update(CAMERA_CUSTOM);

        //for looking 'round
         static float yaw{}; //doing this means that if you hold a or d enough, you could break everything!

        if (IsKeyPressed(KEY_A)) { //Look left!
            TraceLog(LOG_INFO,"A pressed!");
            yaw += 0.2f;

        }
        if (IsKeyPressed(KEY_D)) { //Look left!
            TraceLog(LOG_INFO,"D pressed!");
            yaw -= 0.2f;
        }
        const auto whereamI = raylib::Vector3{camera.position};
        static float distance = whereamI.Distance(theBall.position);

        Vector3 offset = { sinf(yaw) * distance,  CAMERA_HEIGHT, cosf(yaw) * distance };
        camera.position = Vector3Add(theBall.position, offset);
        camera.target = theBall.position;

        float dt = GetFrameTime();

        // Press SPACE to golf!
        if (IsKeyPressed(KEY_SPACE)) {
            TraceLog(LOG_INFO,"Space pressed!");
            theBall.velocity = raylib::Vector3{100,100,0};
        }

        theBall.update(dt);



        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(SKYBLUE);

        camera.BeginMode();
        //WHERE IT ALL HAPPENS
        {
            { //DRAW FLOOR, BASES AND LINES THEREFOR
                DrawPlane({theBall.position.x, 0.0f, theBall.position.y}, {1000, 1000}, GREEN);
                // DrawCubeV(FIRST_POSITION, {1, 1, 1}, RED);
                // DrawCubeV(SECOND_POSITION, {1, 1, 1}, YELLOW);
                // DrawCubeV(THIRD_POSITION, {1, 1, 1}, VIOLET);
                // DrawLine3D(CAMERA_POSITION, FIRST_POSITION, WHITE);
                // DrawLine3D(FIRST_POSITION, SECOND_POSITION, WHITE);
                // DrawLine3D(SECOND_POSITION, THIRD_POSITION, WHITE);
                // DrawLine3D(THIRD_POSITION, HOME_POSITION, WHITE);
            }

            theBall.draw();
            bill.DrawBillboard(camera, GOLFER_POSITION_RIGHT_HANDED, 2.0f);
        }
        camera.EndMode();


        DrawFPS(10, 10);
        DrawText("My name Michael... making game too hard :(", 20, 30, 20, BLACK);
        DrawText("Wonderful, if you insist....", 20, window.GetHeight() -50, 20, RED);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------