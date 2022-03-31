#pragma once
#include "Global.h"
#include "Shader.h"

typedef enum{ORTHO, PERSPECTIVE} ProjectMode;

class Camera {
public:
	Camera(ProjectMode projectMode = PERSPECTIVE);
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	static glm::mat4 getClippedProjectionMatrix(glm::mat4 viewMat, glm::mat4 projMat, glm::vec4 clipPlane);

	void mouseMotion(float mouseDeltaX, float mouseDeltaY);	// 相机鼠标事件
	void mouseWheel(int button, int dir, int x, int y); 	// 相机移动函数
	glm::vec3 doMovement(); 	// 相机移动函数
	void transCamera(Shader* shader); 						// 往着色器传递相机
	void getPitchYaw(float &pitch, float &yaw);
	void setPitchYaw(float pitch, float yaw);



	/*********** 相机位置参数 **********/
	glm::vec3 eye; // 相机位置
	glm::vec3 dir; // 方向
	glm::vec3 up;  // 相机上方朝向
	float maxPitch = 89.0;
	glm::vec4* pannel = nullptr;


	/*********** 透视投影参数 **********/
	#undef near
	#undef far
	float near = 0.1, far = 1000.0;	// 近平面，远平面
	float fov = 45.0; 				// 透视投影参数，单位：度
	float aspect = 1.777; 			// 长宽度比例，16:9

	/*********** 正交投影参数 **********/
	float scale = 10;

private:
	ProjectMode projMode; // 投影模式
};
