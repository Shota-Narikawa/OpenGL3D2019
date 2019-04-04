/**
* @file GLFWE.cpp
*/
#include "GLFWEW.h"
#include <iostream>

/// GLFW��GLEW�����b�v���邽�߂̖��O���.
namespace GLFWEW {

/**
* OpenGL����̃G���[�񍐂���������.
*/
void APIENTRY OutputGLDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, GLvoid *userParam)
{
  std::cerr << message << "\n";
}

/**
* GLFW����̃G���[�񍐂���������.
*
* @param error �G���[�ԍ�.
* @param desc  �G���[�̓��e.
*/
void ErrorCallback(int error, const char* desc)
{
  std::cerr << "ERROR: " << desc << std::endl;
}

/**
* �V���O���g���C���X�^���X���擾����.
*
* @return Window�̃V���O���g���C���X�^���X.
*/
Window& Window::Instance()
{
  static Window instance;
  return instance;
}

/**
* �f�X�g���N�^.
*/
Window::~Window()
{
  if (isGLFWInitialized) {
    glfwTerminate();
  }
}

/**
* GLFW/GLEW�̏�����.
*
* @param w �E�B���h�E�̕`��͈͂̕�(�s�N�Z��).
* @param h �E�B���h�E�̕`��͈͂̍���(�s�N�Z��).
* @param title �E�B���h�E�^�C�g��(UTF-8��0�I�[������).
*
* @retval true ����������.
* @retval false ���������s.
*/
bool Window::Init(int w, int h, const char* title)
{
  if (isInitialized) {
    std::cerr << "ERROR: GLFWEW�͊��ɏ���������Ă��܂�." << std::endl;
    return false;
  }
  if (!isGLFWInitialized) {
    glfwSetErrorCallback(ErrorCallback);
    if (glfwInit() != GL_TRUE) {
      return false;
    }
    isGLFWInitialized = true;
  }

  if (!window) {
    window = glfwCreateWindow(w, h, title, nullptr, nullptr);
    if (!window) {
      return false;
    }
    glfwMakeContextCurrent(window);
  }

  if (glewInit() != GLEW_OK) {
    std::cerr << "ERROR: GLEW�̏������Ɏ��s���܂���." << std::endl;
    return false;
  }

  width = w;
  height = h;
  glDebugMessageCallback(OutputGLDebugMessage, nullptr);

  const GLubyte* renderer = glGetString(GL_RENDERER);
  std::cout << "Renderer: " << renderer << "\n";
  const GLubyte* version = glGetString(GL_VERSION);
  std::cout << "Version: " << version << "\n";
  GLint offsetAlignment = 0;
  glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetAlignment);
  std::cout << "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT: " << offsetAlignment << " Bytes\n";
  GLint maxVertexUniformComponents = 0;
  glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniformComponents);
  std::cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS: " << maxVertexUniformComponents / 1024 << " KBytes\n";
  GLint maxFragmentUniformComponents = 0;
  glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniformComponents);
  std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: " << maxFragmentUniformComponents / 1024 << " KBytes\n";
  GLint maxUniformBlockSize = 0;
  glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUniformBlockSize);
  std::cout << "GL_MAX_UNIFORM_BLOCK_SIZE: " << maxUniformBlockSize / 1024 / 1024 << " MBytes\n";

  ResetDeltaTime();

  isInitialized = true;
  return true;
}

/**
*
*/
void Window::Update()
{
  const double t = glfwGetTime();
  deltaTime = t - prevTime;
  if (deltaTime >= 0.5) {
    deltaTime = 1.0 / 60.0;
  }
  prevTime = t;
}

/**
*
*/
void Window::ResetDeltaTime()
{
  prevTime = glfwGetTime();
  deltaTime = 0;
}

/**
*
*/
bool Window::KeyDown(int key) const
{
  return glfwGetKey(window, key) == GLFW_PRESS;
}

/**
* �E�B���h�E�����ׂ������ׂ�.
*
* @retval true ����.
* @retval false ���Ȃ�.
*/
bool Window::ShouldClose() const
{
  return glfwWindowShouldClose(window) != 0;
}

/**
* �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@��؂�ւ���.
*/
void Window::SwapBuffers() const
{
  glfwPollEvents();
  glfwSwapBuffers(window);
}

} // namespace GLFWEW
