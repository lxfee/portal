#pragma once
#include "Global.h"


class Camera {
public:
	Camera();
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	// 处理相机的键盘操作
	void keyboard(unsigned char key, int x, int y, int action);

	// 相机移动函数
	glm::vec3 do_movement(float deltaTime);

	// 相机位置参数
	float yawOffset = -90;
	float pitchOffset = 0;
	float yaw = yawOffset;
	float pitch = pitchOffset;
	glm::vec3 eye; // 相机位置
	glm::vec3 at;  // 相机视点
	glm::vec3 up;  // 相机上方位置

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
	float scale = 24;
	// 任意视锥体参数
	float left, right, bottom, top;
	
	// 相机控制选项
	bool keys[1024];
	bool isControl[1024];
	float maxPitch;
	float maxYaw;


	// 投影模式
	enum{ORTHO, PERSPECTIVE} projMode;
};
