#pragma once
#include <GLFW/glfw3.h>

// Code from Hell Engine
// Shout out to tokyospliff
namespace InputManager {
    enum class GameState {
        GAMEPLAY,
        INVENTORY,
        PAUSE_MENU,
		CLOSE_CURRENT_MENU,
    };

    enum class Actions {
        FORWARD,
        BACKWARDS,
        LEFT,
        RIGHT,
        JUMP,
        SHIFT,
        SPRINT,
        CLOSE_APPLICATION,
        COUNT
    };

    void Init(void* glfwWinodw);
    void Update();
    void ClearKeyStates();
    bool KeyPressed(Actions action);
    bool KeyDown(Actions action);
    bool KeyPressed(GameState state);
    bool KeyDown(GameState state);
    float GetMouseOffsetX();
    float GetMouseOffsetY();
    bool LeftMouseDown();
    bool RightMouseDown();
    bool MiddleMouseDown();
    bool LeftMousePressed();
    bool MiddleMousePressed();
    bool RightMousePressed();
    bool MouseWheelUp();
    bool MouseWheelDown();
    int GetMouseWheelValue();
    int GetMouseX();
    int GetMouseY();
    void PreventRightMouseHold();
    void DisableCursor();
    void HideCursor();
    void ShowCursor();
    int GetCursorScreenX();
    int GetCursorScreenY();
    void SetCursorPosition(int x, int y);
    int GetMouseXPreviousFrame();
    int GetMouseYPreviousFrame();
}