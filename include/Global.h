#pragma once
#include <cmath>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <GL/glew.h>
#include <map>
#include <memory>
#include <set>
#include <stack>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

// support old glut funcations, improve compability
#define GL_SILENCE_DEPRECATION

#define KEY_PRESS 0
#define KEY_RELEASE 1

#define EPSILON 1e-6