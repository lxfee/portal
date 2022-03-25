#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <GL/glew.h>
#include <map>
#include <set>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;


//----------------------------------------------------------------------------
//
// --- Include OpenGL header files and helpers ---
//
//   The location of these files vary by operating system.  We've included
//     copies of open-soruce project headers in the "GL" directory local
//     this this "include" directory.
//

#define GL_SILENCE_DEPRECATION
typedef enum{ORTHO, PERSPECTIVE} ProjectMode;

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
#define Print(x)  do { std::cerr << #x " = " << (x) << std::endl; } while(0)
// Keyboard action
#define GLFW_PRESS 0
#define GLFW_RELEASE 1
// Key val
#define GLFW_KEY_W 'w'
#define GLFW_KEY_S 's'
#define GLFW_KEY_A 'a'
#define GLFW_KEY_D 'd'