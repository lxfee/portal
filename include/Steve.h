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
	vector<ComponentNode*> sonNode;
};

struct frameData {
	frameDate();
	frameDate(const glm::vec3 data[NUM_OF_COMPONENT]);
	void print();
	glm::vec3 data[NUM_OF_COMPONENT];
	frameData operator+(const frameData& rhs) const ;
	frameData operator-(const frameData& rhs) const ;
	frameData operator*(float k) const ;
};

class AnimateSequence {
public:
	AnimateSequence();
	void setDefault();
	void next();
	void addFrame(frameData frame, float inv = 1000);
	void linearFrame(frameData& current, float& t, float dt, float rate);
private:
	vector<frameData> frames;
	vector<float> interval;
	int curframe;
};



class Steve {
public:
	Steve();

	/************ 绘制机器人 ***********/
	void Draw(Shader* shader);
	void Draw(Shader* shader, ComponentNode* root, glm::mat4 model);
	void doMovement(unsigned char* KEYBUFFER);
	void setFrameData(const frameData& frame);
	frameData getFrameData();

	Camera *eye; 
	glm::vec3 position = glm::vec3(0, 10, 0);

	/********** 初始变换，确保变换后局部坐标系中原点位置就是连接点的位置 ***********/
	static const glm::vec3 localConnectPoint[NUM_OF_COMPONENT]; 	// 初始的部件旋转角度
	static const glm::vec3 initRotation[NUM_OF_COMPONENT]; 			// 初始的部件旋转角度

private:
	static void setup();											// 初始化
	static const glm::vec3 componentSize[NUM_OF_COMPONENT]; 		// 各部位的大小
	static const glm::vec3 TorsoConnectPoint[NUM_OF_COMPONENT]; 	// 各节点连接躯干位置（相对躯干，躯干坐标系）
	ComponentNode* root;
	ComponentNode* index[NUM_OF_COMPONENT];
	static Model** component;
	
	/********** 动画相关 ***********/
	void playAnimate(unsigned char* KEYBUFFER);
	static bool isWalking(unsigned char* KEYBUFFER);
	AnimateSequence* walkingSeq;
	AnimateSequence* stoppingSeq;
	bool isAnimate[NUM_OF_COMPONENT];
	Steve& setAnimate(SteveComponent partName);
};



