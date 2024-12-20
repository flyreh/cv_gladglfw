#include "Back.h"
#include <iostream>

#include <string>
#include "../API/GL_renderer.h"
#include "../Input/Input.h"
#include "../Core/AssetManager.h"

namespace Back {

  GLFWwindow* _window = NULL;
  WindowedMode _windowedMode = WindowedMode::WINDOWED;
  GLFWmonitor* _monitor;
  const GLFWvidmode* _mode;
  bool _forceCloseWindow = false;
  bool _windowHasFocus = true;
  float _windowedWidth = 0;
  float _windowedHeight = 0;
  int _fullscreenWidth = 0;
  int _fullscreenHeight = 0;
  int _currentWindowWidth = 0;
  int _currentWindowHeight = 0;
  int _presentTargetWidth = 0;
  int _presentTargetHeight = 0;
  // Frame
  bool firstFrame = true;
  double currentFrame = 0.0f;
  double deltaTime = 0.0f;
  double lastFrame = 0.0f;

  // FPS
  double prevTime = 0.0;
  double crntTime = 0.0;
  double timeDiff;

  // frames counter
  unsigned int counter = 0;

  void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  void window_focus_callback(GLFWwindow* window, int focused);

  void Init() {

    float width = 1920 * 0.65;
    float height = 1080 * 0.65;

    glfwInit();
    glfwSetErrorCallback([](int error, const char* description) {
      std::cout << "GLFW Error (" << std::to_string(error) << "): " << description << "\n";
      });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

    // Resolution and window size
    _monitor = glfwGetPrimaryMonitor();
    _mode = glfwGetVideoMode(_monitor);

    glfwWindowHint(GLFW_RED_BITS, _mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, _mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, _mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, _mode->refreshRate);

    _fullscreenWidth = _mode->width;
    _fullscreenHeight = _mode->height;
    _windowedWidth = width;
    _windowedHeight = height;

    CreateGLFWWindow(WindowedMode::WINDOWED);

    if (_window == NULL) {
      std::cout << "Failed to create GLFW window\n";
      glfwTerminate();
      return;
    }

    glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
    glfwSetWindowFocusCallback(_window, window_focus_callback);


    glfwMakeContextCurrent(_window);

    gladLoadGL();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD\n";
      return;
    }
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* renderer = glGetString(GL_RENDERER);
    std::cout << "\nGPU: " << renderer << "\n";
    std::cout << "GL version: " << major << "." << minor << "\n\n";

    //Carga y Compila los Shaders para los objetos, Creaci�n de Buffers VAO, VBO y EBO
    OpenGLRenderer::InitMinimum();

    // Carga de los assets: Texturas y Modelos.
    AssetManager::LoadAssetPath();

    Input::Init();
    Camera::Init();


    glfwShowWindow(Back::GetWindowPointer());

  }

  void BeginFrame() {
    glfwPollEvents();
  }

  void EndFrame() {
    glfwSwapBuffers(_window);
  }

  void UpdateSubSystems() {

    Input::Update();

    // Updates counter and times
    crntTime = glfwGetTime();
    timeDiff = crntTime - prevTime;
    counter++;

    if (timeDiff >= 1.0 / 30.0) {
      std::string FPS = std::to_string((1.0 / timeDiff) * counter);
      std::string ms = std::to_string((timeDiff / counter) * 1000);
      std::string newTitle = "Frustum Culling/ " + FPS + "FPS / " + ms + "ms";
      glfwSetWindowTitle(Back::GetWindowPointer(), newTitle.c_str());
      prevTime = crntTime;
      counter = 0;
    }
    if (firstFrame) {
      currentFrame = glfwGetTime();
      firstFrame = false;
    }
    lastFrame = currentFrame;
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;

    Camera::activeCamera->UpdateCameraInput(deltaTime);

  }

  void CleanUp() {
    if (GetWindowMode() == WindowedMode::FULLSCREEN) {
      ToggleFullscreen();
    }
    glfwTerminate();
  }

  // Window
  GLFWwindow* GetWindowPointer() {
    return _window;
  }

  void SetWindowPointer(GLFWwindow* window) {
    _window = window;
  }

  void CreateGLFWWindow(const WindowedMode& windowedMode) {
    if (windowedMode == WindowedMode::WINDOWED) {
      _currentWindowWidth = _windowedWidth;
      _currentWindowHeight = _windowedHeight;
      _window = glfwCreateWindow(_windowedWidth, _windowedHeight, "CV", NULL, NULL);
      glfwSetWindowPos(_window, 400, 150);
    }
    else if (windowedMode == WindowedMode::FULLSCREEN) {
      _currentWindowWidth = _fullscreenWidth;
      _currentWindowHeight = _fullscreenHeight;
      _window = glfwCreateWindow(_fullscreenWidth, _fullscreenHeight, "CV", _monitor, NULL);
    }
    _windowedMode = windowedMode;
  }

  void SetWindowedMode(const WindowedMode& windowedMode) {
    if (windowedMode == WindowedMode::WINDOWED) {
      _currentWindowWidth = _windowedWidth;
      _currentWindowHeight = _windowedHeight;
      glfwSetWindowMonitor(_window, nullptr, 0, 0, _windowedWidth, _windowedHeight,
        _mode->refreshRate);
      glfwSetWindowPos(_window, 400, 150);
    }
    else if (windowedMode == WindowedMode::FULLSCREEN) {
      _currentWindowWidth = _fullscreenWidth;
      _currentWindowHeight = _fullscreenHeight;
      glfwSetWindowMonitor(_window, _monitor, 0, 0, _fullscreenWidth, _fullscreenHeight,
        _mode->refreshRate);
    }
    _windowedMode = windowedMode;
  }

  void ToggleFullscreen() {
    if (_windowedMode == WindowedMode::WINDOWED) {
      SetWindowedMode(WindowedMode::FULLSCREEN);
    }
    else {
      SetWindowedMode(WindowedMode::WINDOWED);
    }
  }

  void ForceCloseWindow() {
    _forceCloseWindow = true;
  }

  bool WindowHasFocus() {
    return _windowHasFocus;
  }

  bool WindowHasNotBeenForceClosed() {
    return !_forceCloseWindow;
  }

  float GetWindowedWidth() {
    return _windowedWidth;
  }

  float GetWindowedHeight() {
    return _windowedHeight;
  }

  int GetFullScreenWidth() {
    return _fullscreenWidth;
  }

  int GetFullScreenHeight() {
    return _fullscreenHeight;
  }

  int GetCurrentWindowWidth() {
    return _currentWindowWidth;
  }

  int GetCurrentWindowHeight() {
    return _currentWindowHeight;
  }

  double GetCurrentFrame() {
    return currentFrame;
  }

  double GetDeltaTime() {
    return deltaTime;
  }
  bool WindowIsOpen() {
    return !(glfwWindowShouldClose(_window) || _forceCloseWindow);
  }

  bool WindowIsMinimized() {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(_window, &width, &height);
    return (width == 0 || height == 0);
  }

  const WindowedMode& GetWindowMode() {
    return _windowedMode;
  }

  int GetPresentTargetWidth() {
    return _presentTargetWidth;
  }

  int GetPresentTargetHeight() {
    return _presentTargetHeight;
  }

  //callbacks

  void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
  }

  void window_focus_callback(GLFWwindow*, int focused) {
    if (focused) {
      Back::_windowHasFocus = true;
    }
    else {
      Back::_windowHasFocus = false;
    }
  }

}  // namespace Back