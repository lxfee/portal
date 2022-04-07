// TODO 逐部件控制动画？
#pragma once
#include "Global.h"
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#define NUM_OF_COMPONENT 6

enum  SteveComponent {
	Torso,		// 躯干
	Head,		// 头部
	RightArm,	// 右臂
	RightLeg,	// 右腿
	LeftArm,	// 左臂
	LeftLeg,	// 左腿
} ;


struct ComponentNode {
	glm::vec3 fatherConnectPoint;
	glm::vec3 Deg;
	Model* component;
	SteveComponent partName;
	vector<ComponentNode*> sonNode;
};

struct FrameData {
	FrameData();
	FrameData(const glm::vec3 data[NUM_OF_COMPONENT]);
	void print();
	glm::vec3 data[NUM_OF_COMPONENT];
	FrameData operator+(const FrameData& rhs) const ;
	FrameData operator-(const FrameData& rhs) const ;
	FrameData operator*(float k) const ;
};

class AnimateSequence {
public:
	AnimateSequence();
	void setDefault();
	void next();
	void addFrame(FrameData frame, float inv = 1000);
	bool passLinearFrame(FrameData& current, float& t, float dt, float rate);
private:
	vector<FrameData> frames;
	vector<float> interval;
	int curframe;
};



class Steve {
public:
	Steve();

	/************ 绘制机器人 ***********/
	void Draw(Shader* shader);
	void Draw(Shader* shader, ComponentNode* root, glm::mat4 model);
	void doMovement();
	void mouseMotion(float mouseDeltaX, float mouseDeltaY);
	bool getModel(SteveComponent partName, glm::mat4& model);
	bool getModel(SteveComponent partName, ComponentNode* current ,glm::mat4& model);

	Camera *eye; 
	glm::vec3 position = glm::vec3(0, 10, 0);

	/********** 初始变换，确保变换后局部坐标系中原点位置就是连接点的位置 ***********/
	static const glm::vec3 initPosition[NUM_OF_COMPONENT]; 	// 初始的部件旋转角度
	static const glm::vec3 initRotation[NUM_OF_COMPONENT]; 			// 初始的部件旋转角度

private:
	static void setup();											// 初始化
	static const glm::vec3 componentSize[NUM_OF_COMPONENT]; 		// 各部位的大小
	static const glm::vec3 TorsoConnectPoint[NUM_OF_COMPONENT]; 	// 各节点连接躯干位置（相对躯干，躯干坐标系）
	void bindCamera();
	ComponentNode* root;
	ComponentNode* index[NUM_OF_COMPONENT];
	static Model** component;

	/********** 动画相关 ***********/
	void playAnimate();
	static bool isWalking();
	AnimateSequence* walkingSeq;
	AnimateSequence* stoppingSeq;
	bool isAnimate[NUM_OF_COMPONENT];
	Steve& setAnimate(SteveComponent partName);
	void setFrameData(const FrameData& frame);
	FrameData getFrameData();
};

