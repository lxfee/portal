#pragma once
#include "Global.h"
#include "Shader.h"


class Camera {
public:
	Camera();
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	// 处理相机的鼠标操作
	void mouseMotion(float deltaX, float deltaY);
	void mouseWheel(int button, int dir, int x, int y);

	// 相机移动函数
	void doMovement(unsigned char* KEYBUFFER);

	// 往着色器传递相机
	void transCamera(Shader* shader);

	// 相机位置参数
	float yaw;
	float pitch;
	glm::vec3 eye; // 相机位置
	glm::vec3 dir;  // 相机朝向
	glm::vec3 up;  // 相机上方朝向

	// 投影参数
	#undef near
	#undef far
	float near = 0.01;
	float far = 100.0;
	// 透视投影参数，单位：度
	float fov = 45.0;
    // 长宽度比例
	float aspect = 1.0;
	// 正交投影参数
	float scale = 1;


	// 投影模式
	enum{ORTHO, PERSPECTIVE} projMode;
};
