#include <glfw3.h>
#include "process.h"
#include "helper.h"
#include "offsets.h"
#include "Windows.h"
#include "string"

// Constants for window dimensions and ESP settings
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const float HEAD_HEIGHT = 88.0f;
const float BOX_WIDTH_RATIO = 4.9f;

// Helper function to draw ESP box
void DrawESPBox(const Vec2& feet, const Vec2& head, float width, float r, float g, float b) {
    glColor3f(r, g, b);
    
    // Left side
    glBegin(GL_LINES);
    glVertex2f(feet.x - (width / 2), feet.y);
    glVertex2f(feet.x - (width / 2), head.y);
    glEnd();

    // Top
    glBegin(GL_LINES);
    glVertex2f(feet.x - (width / 2), head.y);
    glVertex2f(feet.x + (width / 2), head.y);
    glEnd();

    // Right side
    glBegin(GL_LINES);
    glVertex2f(feet.x + (width / 2), head.y);
    glVertex2f(feet.x + (width / 2), feet.y);
    glEnd();

    // Bottom
    glBegin(GL_LINES);
    glVertex2f(feet.x + (width / 2), feet.y);
    glVertex2f(feet.x - (width / 2), feet.y);
    glEnd();
}

// Helper function to draw health indicator
void DrawHealthIndicator(const Vec2& feet, int health) {
    if(health <= 100) {
        if(health < 30) {
            glColor3f(1.0f, 0.0f, 0.0f); // Red for low health
        }
        else if(health < 60) {
            glColor3f(1.0f, 1.0f, 0.0f); // Yellow for medium health
        }
        else {
            glColor3f(0.0f, 1.0f, 0.0f); // Green for high health
        }
        
        glBegin(GL_LINES);
        glVertex2f(0.0f, -1.0f);
        glVertex2f(feet.x, feet.y);
        glEnd();
    }
}

int main() {
    GLFWwindow* window;

    if (!glfwInit())
        return 0;

    // Configure window properties
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CSGOCHEATV1", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Initialize game process and memory access
    int procId = GetProcessId(L"csgo.exe");
    std::cout << procId << std::endl;
    if (!procId) {
        std::cout << "Proc not found" << std::endl;
        return -1;
    }

    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"client.dll");
    uintptr_t localPlayer = moduleBase + offsets::aLocalPlayer;
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);

    std::vector<unsigned int> teamOffsets{ offsets::oiTeamNum };
    uintptr_t teamAddr = FinDMAAddy(hProc, localPlayer, teamOffsets);

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        ViewMatrix Matrix = Memory::Read<ViewMatrix>(hProc, (moduleBase + offsets::aViewMatrix));

        // Process each player entity
        for (int i = 0; i < 64; ++i) {
            uintptr_t Entity = Memory::Read<uintptr_t>(hProc, (moduleBase + offsets::aEntityList) + i * 0x10);
            if (Entity == NULL) continue;

            int Health = Memory::Read<int>(hProc, (Entity + offsets::oHealth));
            if (Health <= 0) continue;

            uintptr_t EntityTeam = Memory::Read<uintptr_t>(hProc, Entity + offsets::oiTeamNum);
            uintptr_t LocalTeam = Memory::Read<uintptr_t>(hProc, teamAddr);

            // Calculate positions
            Vec3 vecOrigin = Memory::Read<Vec3>(hProc, Entity + offsets::oVecOrigin);
            Vec3 headOrigin = Vec3(vecOrigin.x, vecOrigin.y, vecOrigin.z + HEAD_HEIGHT);
            Vec2 HeadCoords, FeetCoords;

            if (!WorldToScreen(vecOrigin, FeetCoords, Matrix.matrix) || 
                !WorldToScreen(headOrigin, HeadCoords, Matrix.matrix)) 
                continue;

            float height = HeadCoords.y - FeetCoords.y;
            float width = height / BOX_WIDTH_RATIO;

            // Draw ESP elements
            if (EntityTeam != LocalTeam) {
                DrawESPBox(FeetCoords, HeadCoords, width, 1.0f, 0.0f, 1.0f); // Purple for enemies
                DrawHealthIndicator(FeetCoords, Health);
            } else {
                DrawESPBox(FeetCoords, HeadCoords, width, 1.0f, 1.0f, 1.0f); // White for teammates
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
