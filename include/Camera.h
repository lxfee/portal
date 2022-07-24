#pragma once
#include "Global.h"
#include "Shader.h"
#include "Framebuffer.h"

typedef enum{ORTHO, PERSPECTIVE} ProjectMode;


class Camera {
public:
	Camera(ProjectMode projectMode = PERSPECTIVE);
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getRotation();


	void mouseMotion(float mouseDeltaX, float mouseDeltaY);
	void mouseWheel(int button, int dir, int x, int y);
	glm::vec3 doMovement();
	void transCamera(ShaderPtr shader);


	glm::vec3 eye;
	float pitch;
	float yaw;

	#undef near
	#undef far
	float near = 0.01f, far = 1000.0f;
	float fov = 45.0;
	float aspect = 1.777f;

	float scale = 10;

private:
	ProjectMode projMode;
};

typedef shared_ptr<Camera> CameraPtr;