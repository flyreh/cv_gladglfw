#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

// Vertex data for a cube
float vertices[] = {

    // Positions          // Texture Coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,

    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 0.0f, 0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
    0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

// Shader sources
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 vertexColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 fragColor;
void main() {
    fragColor = vec4(vertexColor, 1.0);
}
)";

// Variables para el mouse
float lastX = 400, lastY = 300;
bool firstMouse = true;

// Variables de cámara
float yaw = -90.0f;  // Mirando hacia adelante
float pitch = 0.0f;  // Sin inclinación

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  // posición inicial de la cámara

glm::vec3 cameraFront =
    glm::vec3(0.0f, 0.0f, -1.0f);  // Dirección de la cámara (hacia dónde "mira")

glm::vec3 cameraUp =
    glm::vec3(0.0f, 1.0f, 0.0f);  // Vector de "arriba" de la cámara para evitar que se voltee

// Callback del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;  // Invertir el eje Y
  lastX = xpos;
  lastY = ypos;

  // Ajustar los ángulos de yaw y pitch
  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  // Actualiza la dirección de la cámara
  yaw += xoffset;
  pitch += yoffset;

  // Limitar el pitch para evitar que la cámara se voltee
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  // Calcular la nueva dirección de la cámara
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);

  std::cout << "Camera Front: " << cameraFront.x << ", " << cameraFront.y << ", " << cameraFront.z
            << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

float deltaTime = 0.0f;  // Time between current frame and last frame
float lastFrame = 0.0f;  // Time of last frame

void processInput(GLFWwindow* window) {
  float cameraSpeed = 2.5f * deltaTime;  // Adjust accordingly
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  std::cout << "Camera Position: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z
            << std::endl;
}

int main() {

  GLFWwindow* _window = NULL;
  GLFWmonitor* _monitor;
  const GLFWvidmode* _mode;
  bool _forceCloseWindow = false;
  bool _windowHasFocus = true;
  int _windowedWidth = 0;
  int _windowedHeight = 0;
  int _fullscreenWidth = 0;
  int _fullscreenHeight = 0;
  int _currentWindowWidth = 0;
  int _currentWindowHeight = 0;
  int _presentTargetWidth = 0;
  int _presentTargetHeight = 0;

  void framebuffer_size_callback(GLFWwindow * window, int width, int height);
  void window_focus_callback(GLFWwindow * window, int focused);

  int width = 1920 * 0.7f;
  int height = 1080 * 0.7f;

  // Inicializa GLFW
  glfwInit();

  glfwSetErrorCallback([](int error, const char* description) {
    std::cout << "GLFW Error (" << std::to_string(error) << "): " << description << "\n";
  });

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);

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

  _window = glfwCreateWindow(_windowedWidth, _windowedHeight, "3D Cube with Camera", NULL, NULL);
  glfwSetWindowPos(_window, 100, 100);

  if (_window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(_window);
  glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);

  glfwSetCursorPosCallback(_window, mouse_callback);  // callback del mouse

  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Oculta el cursor

  // Inicializa GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  /*
		Lo de compilar Shaders irá en el módulo de API/GL_renderer
		.cpp
		.h
	*/
  // Compila shaders
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Enlaza shaders
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Elimina shaders ya que ya están enlazados
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  /*
		El VBO es un buffer en la memoria de la GPU donde se almacenan los datos de los vértices
		(posiciones, colores, coordenadas de textura, etc.). el array vertices define la posicion y coordenadas de textura

		El VAO es un objeto que almacena las configuraciones de cómo los datos de los vértices
		se interpretan (atributos como posición, color, textura, etc.)
	*/
  // Crea VBO y VAO
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Posición de los vértices
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Coordenadas de textura
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glfwShowWindow(_window);  // Muestra la ventana una vez que todo está listo

  // Bucle de renderizado
  while (!glfwWindowShouldClose(_window)) {
    // Calcula el tiempo delta
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // Procesa la entrada del usuario por teclado
    processInput(_window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);  // Habilitar el test de profundidad

    // Usa el programa de shader que compilamos anteriormente para dibujar el cubo

    glUseProgram(shaderProgram);

    // Matrices de transformación
    //
    // Son alternativas de la librería glm a las funciones de la matriz de transformación de OpenGL (como gluLlookAt o glperspective)

    glm::mat4 model = glm::mat4(1.0f);  // Matriz modelo (Es como hacer un PushMatrix-PopMatrix)

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);  // Matriz vista

    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), (float)_windowedWidth / (float)_windowedHeight, 0.1f,
                         100.0f);  // Matriz de proyección (define frustum sencillo)

    // usamos la función glUniformMatrix4fv para PASAR las matrices al shader y este pueda realizar las transformaciones de los vértices

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));

    // Dibuja el cubo mediante el VAO con las trasformaciones ya hechas por los shaders.

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 50);

    // Esto Dibuja un segundo cubo, con traslación

    model = glm::mat4(
        1.0f);  // Reinicia la matriz modelo a la identidad (Es como hacer un PushMatrix-PopMatrix)

    model = glm::translate(model, glm::vec3(5.0f, 5.0f, 5.0f));  // Aplica la traslación

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));

    glDrawArrays(GL_TRIANGLES, 0, 50);  // Dibuja el segundo cubo

    // Intercambia buffers y procesa eventos
    glfwSwapBuffers(_window);
    glfwPollEvents();
  }

  // Limpiar recursos
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}