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

// 使用下面宏定义，支持旧的glut函数，提高兼容性
#define GL_SILENCE_DEPRECATION

#define KEY_PRESS 0
#define KEY_RELEASE 1