#include "Camera.h"

Camera::Camera() { 
	up = glm::vec3(0.0, 1.0, 0.0); // 初始UP
	at = glm::vec3(0.0, 0.0, -1.0); // 初始朝向
	eye = glm::vec3(0.0, 0.0, 3.0); // 初始位置

	for(int i = 0; i < sizeof keys; i++) {
		keys[i] = false;
		isControl[i] = false;
	}
	for(int i = 0; i < sizeof "wasdq "; i++) {
		isControl["wasdq "[i]] = true;
	}
	maxYaw = 180;
	maxPitch = 89;
	projMode = ORTHO;
};

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(eye, -at, up);
}

glm::mat4 Camera::getProjectionMatrix() {
	switch(projMode) {
		case ORTHO: return glm::perspective(glm::radians(fov), aspect, near, far);
		case PERSPECTIVE: return glm::ortho(-scale, scale, -scale, scale, this->near, this->far);
        default: Print("未指明投影类型");
    }
}

glm::vec3 Camera::do_movement(float deltaTime) {
	GLfloat cameraSpeed = 20.0f * (deltaTime / 1000);
  	glm::vec3 translation = glm::vec4(0);
	if(keys[GLFW_KEY_W] && isControl[GLFW_KEY_W]) 
	  	translation += cameraSpeed * at;
  	if(keys[GLFW_KEY_S] && isControl[GLFW_KEY_S])
	  	translation += -cameraSpeed * at;
  	if(keys[GLFW_KEY_A] && isControl[GLFW_KEY_A])
	  	translation += -normalize(cross(at, up)) * cameraSpeed;
  	if(keys[GLFW_KEY_D] && isControl[GLFW_KEY_D])
	  	translation += normalize(cross(at, up)) * cameraSpeed;
	if(keys[' '] && isControl[' ']) { // 上
		translation += normalize(up) * cameraSpeed;
	}
	if(keys['q'] && isControl['q']) { // 下
		translation += -normalize(up) * cameraSpeed;
	}
	eye += translation;
	return translation;
}

void Camera::keyboard(unsigned char key, int x, int y, int action) {
	// 键盘事件处理
	if(action == GLFW_PRESS)
    	keys[key] = true;
	else if(action == GLFW_RELEASE)
    	keys[key] = false;
}
