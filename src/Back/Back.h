#pragma once

#include "../Enviroment/CV_Enviroment.h"

namespace Back {

// Core
void Init();
void BeginFrame();
void UpdateSubSystems();
void EndFrame();
void CleanUp();

// Window
GLFWwindow* GetWindowPointer();
const WindowedMode& GetWindowMode();
void SetWindowPointer(GLFWwindow* window);
void CreateGLFWWindow(const WindowedMode& windowedMode);
void SetWindowedMode(const WindowedMode& windowedMode);
void ToggleFullscreen();
void ForceCloseWindow();
bool WindowIsOpen();
bool WindowHasFocus();
bool WindowHasNotBeenForceClosed();
bool WindowIsMinimized();
float GetWindowedWidth();
float GetWindowedHeight();
int GetCurrentWindowWidth();
int GetCurrentWindowHeight();
int GetFullScreenWidth();
int GetFullScreenHeight();

// Render Targets
void SetPresentTargetSize(int width, int height);
int GetPresentTargetWidth();
int GetPresentTargetHeight();
}  // namespace Back