#pragma once
#include "Steve.h"

ModelPtr Steve::component[NUM_OF_COMPONENT];
bool Steve::first = true;

Steve::~Steve() {
	remove(root);
}

void Steve::remove(ComponentNode* current) {
	if(!current) return ;
	for(auto son : current->sonNode) {
		remove(son);
	}
	delete current;
}

const glm::vec3 Steve::TorsoConnectPoint[NUM_OF_COMPONENT] = {
	glm::vec3( 0, 0, 0),
	glm::vec3( 0, 0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z),
	glm::vec3( 0.5 * componentSize[Torso].x,  0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z),
	glm::vec3(-0.5 * componentSize[Torso].x,  0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z),
	glm::vec3( 0.25 * componentSize[Torso].x, -0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z),
	glm::vec3(-0.25 * componentSize[Torso].x, -0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z)
};

 const glm::vec3 Steve::initPosition[NUM_OF_COMPONENT] {
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(-0.5 * componentSize[LeftArm ].x, 0.5 * componentSize[LeftArm ].y, 0.5 * componentSize[LeftArm ].z),
	glm::vec3( 0.5 * componentSize[RightArm].x, 0.5 * componentSize[RightArm].y, 0.5 * componentSize[RightArm].z),
	glm::vec3(0, 0.5 * componentSize[LeftArm ].y, 0.5 * componentSize[LeftArm ].z),
	glm::vec3(0, 0.5 * componentSize[RightArm].y, 0.5 * componentSize[RightArm].z)
};

 // 初始旋转, 角度制
 const glm::vec3 Steve::initRotation[NUM_OF_COMPONENT] {
	glm::vec3(90, 0, 0),
	glm::vec3(0,  0, 0),
	glm::vec3(90, 0, 0),
	glm::vec3(90, 0, 0),
	glm::vec3(90, 0, 0),
	glm::vec3(90, 0, 0)
};

FrameData FrameData::operator+(const FrameData& rhs) const {
	FrameData res;
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		res.data[i] = data[i] + rhs.data[i];
	}
	return res;
}

FrameData FrameData::operator-(const FrameData& rhs) const {
	FrameData res;
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		res.data[i] = data[i] - rhs.data[i];
	}
	return res;
}

FrameData FrameData::operator*(float k) const {
	FrameData res;
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		res.data[i] = data[i] * k;
	}
	return res;
}

FrameData::FrameData(const glm::vec3 data[NUM_OF_COMPONENT]) {
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		this->data[i] = data[i];
	}
}

void FrameData::print() {
	cout << "{";
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		cout << "glm::vec3(";
		cout << data[i].x << ", " << data[i].y << ", "<< data[i].z;
		cout << ")";
		if(i < NUM_OF_COMPONENT - 1) cout << ",";  
	}
	cout << "}" << endl;
}


FrameData::FrameData() {
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		data[i] = glm::vec3(0.0);
	}
}

AnimateSequence::AnimateSequence() {
	curframe = 0;
}

void AnimateSequence::setDefault() {
	curframe = 0;
}

void AnimateSequence::next() {
	if(!frames.empty()) curframe = (curframe + 1) % frames.size();
}

void AnimateSequence::addFrame(FrameData frame, float inv) {
	frames.emplace_back(frame);
	interval.emplace_back(inv);
}



bool AnimateSequence::passLinearFrame(FrameData& current, float& t, float dt, float rate)  {
	if(frames.empty()) return false;
	float t1 = t, t2 = t1 + dt, T = interval[curframe] * rate;
	t += dt;
	if(t2 >= T) {
		current = frames[curframe];
		t = 0;
		next();
		return true;
	} else {
		current = (frames[curframe] - current) * ((t2 - t1) / (T - t1)) + current;
	}
	return false;
}

// 各部件的大小
const glm::vec3 Steve::componentSize[NUM_OF_COMPONENT] = { 
	glm::vec3(0.8, 1.2, 0.4),
	glm::vec3(0.8, 0.8, 0.8), 
	glm::vec3(0.4, 1.2, 0.4),
	glm::vec3(0.4, 1.2, 0.4),
	glm::vec3(0.4, 1.2, 0.4),
	glm::vec3(0.4, 1.2, 0.4) 
};


void Steve::setup() {
	string dir = "./models/steve/";
	vector<string> objName = {
		"torso.obj",
		"head.obj",
		"arm.obj",
		"arm.obj",
		"leftLeg.obj",
		"rightLeg.obj"
	};
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		component[i] = make_shared<Model>((dir + objName[i]).c_str());
	}
}
Steve& Steve::setAnimate(SteveComponent partName, unsigned char mask) {
	isAnimate[partName] = mask;
	return *this;
}
void Steve::bindCamera() {
	glm::mat4 model(1.0);
	getModel(Head, model);
	glm::vec3 eyeP1 = glm::vec3(0, 0.4, 0);
	glm::vec3 normal = glm::vec3(0, 0, 1.0);
	eyeP1 = glm::vec3(model * glm::vec4(eyeP1, 1.0f));
	normal = glm::transpose(glm::inverse(glm::mat3(model))) * normal;
	eye->eye = eyeP1 + 0.6f * normal;
	eye->dir = normal;
}

Steve::Steve() {
	if(first) {
		setup();
		first = false;
	}
	
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		ComponentNode* node = new ComponentNode();
		node->component = component[i];
		node->component->rotation = initRotation[i];
		node->component->translation = -initPosition[i];
		node->partName = SteveComponent(i);
		node->fatherConnectPoint = TorsoConnectPoint[i];
		node->Deg = glm::vec3(0);
		index[i] = node;
	}
	index[Torso]->sonNode.push_back(index[Head    ]);
	index[Torso]->sonNode.push_back(index[RightArm]);
	index[Torso]->sonNode.push_back(index[RightLeg]);
	index[Torso]->sonNode.push_back(index[LeftArm ]);
	index[Torso]->sonNode.push_back(index[LeftLeg ]);
	root = index[Torso];
	eye = make_shared<Camera>();
	bindCamera();

	glm::vec3 f1[NUM_OF_COMPONENT] = {glm::vec3(0, 0, 0),glm::vec3(0.0668911, 0, 0),glm::vec3(-60, 0, 0),glm::vec3(55, 0, 0),glm::vec3(35, 0, 0),glm::vec3(-30, 0, 0)};
	glm::vec3 f2[NUM_OF_COMPONENT] = {glm::vec3(0, 0, 0),glm::vec3(0, 0, 0),glm::vec3(50.0393, 0, 0),glm::vec3(-45, 0, 0),glm::vec3(-40, 0, 0),glm::vec3(25, 0, 0)};
	glm::vec3 f3[NUM_OF_COMPONENT] = {glm::vec3(0, 0, 0),glm::vec3(0, 0, 0),glm::vec3(0, 0, 2),glm::vec3(0, 0, -2),glm::vec3(0, 0, 0),glm::vec3(0, 0, 0)};
	walkingSeq.addFrame(FrameData(f1));
	walkingSeq.addFrame(FrameData(f2));

	stoppingSeq.addFrame(FrameData());
	stoppingSeq.addFrame(f3, 10000);
	stoppingSeq.addFrame(f3, 1000);
	stoppingSeq.addFrame(FrameData(), 10000);

	memset(isAnimate, 0, sizeof isAnimate);
}

bool Steve::isWalking() {
	extern unsigned char KEYBUFFER[1024];
	return KEYBUFFER['w'] || KEYBUFFER['d'] || KEYBUFFER['s'] || KEYBUFFER['a'];
}

void Steve::playAnimate() {
	extern unsigned char KEYBUFFER[1024];
	extern float frameTime;
	static float elapsedTime = 0;
	static enum{STOP, WALKING} status = STOP;
	FrameData current = getFrameData();

	switch(status) {
		case STOP:
			stoppingSeq.passLinearFrame(current, elapsedTime, frameTime, 0.1);
			setAnimate(LeftArm).setAnimate(RightArm).setAnimate(LeftLeg).setAnimate(RightLeg).setAnimate(Torso, 1);
			setFrameData(current);
			if(isWalking()) {
				elapsedTime = 0;
				stoppingSeq.setDefault();
				status = WALKING;
			}
			break;
		case WALKING:
			walkingSeq.passLinearFrame(current, elapsedTime, frameTime, 0.3);
			setAnimate(LeftArm).setAnimate(RightArm).setAnimate(LeftLeg).setAnimate(RightLeg).setAnimate(Torso, 1);
			setFrameData(current);
			if(!isWalking()) {
				elapsedTime = 0;
				walkingSeq.setDefault();
				status = STOP;
			}
			break;
	}
}


void Steve::Draw(ShaderPtr shader) {
	glm::mat4 model = glm::translate(glm::mat4(1.0), position);
	Draw(shader, root, model);
}

void Steve::Draw(ShaderPtr shader, ComponentNode* current, glm::mat4 model) {
	if(!current) return ;
	glm::mat4 localTranslation = glm::translate(glm::mat4(1.0), current->fatherConnectPoint);
	glm::mat4 rotation(1.0);
	rotation = glm::rotate(rotation, glm::radians(current->Deg.y), glm::vec3(0.0, 1.0, 0.0));
	rotation = glm::rotate(rotation, glm::radians(current->Deg.x), glm::vec3(1.0, 0.0, 0.0));
	rotation = glm::rotate(rotation, glm::radians(current->Deg.z), glm::vec3(0.0, 0.0, 1.0));
	model = model * localTranslation * rotation;
	if(current->component) current->component->Draw(shader, model);
	for(auto son : current->sonNode) {
		Draw(shader, son, model);
	}
}


void Steve::doMovement()  {
	playAnimate();

	extern unsigned char KEYBUFFER[1024];
	extern float frameTime;
	// 简单移动
	float speed = 10.0f * (frameTime / 1000);
  	glm::vec3 translation(0);
	glm::vec3 dir = normalize(eye->dir);
	glm::vec3 up = glm::vec3(0, 1.0, 0);
	glm::vec3 right = glm::normalize(glm::cross(up, dir));
	glm::vec3 front = glm::normalize(glm::cross(right, up));

	if(KEYBUFFER['w']) translation += front * speed;
  	if(KEYBUFFER['s']) translation -= front * speed;
  	if(KEYBUFFER['a']) translation += right * speed;
  	if(KEYBUFFER['d']) translation -= right * speed;
	if(KEYBUFFER[' ']) translation += up * speed;
	if(KEYBUFFER['q']) translation -= up * speed;

	position += translation;
	bindCamera();
}

void Steve::setFrameData(const FrameData& frame) {
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		for(int j = 0; j < 3; j++) {
			if(isAnimate[i] & (1 << j)) index[i]->Deg[j] = frame.data[i][j];
		}
		isAnimate[i] = 0;
	}
}


FrameData Steve::getFrameData() {
	FrameData result;
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		result.data[i] = index[i]->Deg;
	}
	return result;
}

void Steve::mouseMotion(float mouseDeltaX, float mouseDeltaY) {
	float sensitivity = 0.2f;
	glm::vec3& Deg = index[Head]->Deg;
	Deg.x -= sensitivity * mouseDeltaY;
	if(Deg.x > 89.9) Deg.x = 89.9;
	if(Deg.x < -89.9) Deg.x = -89.9;

	Deg.y -= sensitivity * mouseDeltaX;
	if(Deg.y > 50.0) {
		Deg.y = 50.0;
		index[Torso]->Deg.y -= sensitivity * mouseDeltaX;
	}
	if(Deg.y < -50.0) {
		Deg.y = -50.0;
		index[Torso]->Deg.y -= sensitivity * mouseDeltaX;
	}
	
	bindCamera();
}

bool Steve::getModel(SteveComponent partName, glm::mat4& model) {
	model = glm::translate(glm::mat4(1.0), position);
	if(getModel(partName, root, model)) return true;
	else return false;
}

bool Steve::getModel(SteveComponent partName, ComponentNode* current ,glm::mat4& model) {
	if(!current) return false;
	glm::mat4 localTranslation = glm::translate(glm::mat4(1.0), current->fatherConnectPoint);
	glm::mat4 rotation(1.0);
	rotation = glm::rotate(rotation, glm::radians(current->Deg.y), glm::vec3(0.0, 1.0, 0.0));
	rotation = glm::rotate(rotation, glm::radians(current->Deg.x), glm::vec3(1.0, 0.0, 0.0));
	rotation = glm::rotate(rotation, glm::radians(current->Deg.z), glm::vec3(0.0, 0.0, 1.0));
	model = model * localTranslation * rotation;
	if(current->partName == partName) return true;
	for(auto son : current->sonNode) {
		auto tmodel = model;
		if(getModel(partName, son, tmodel)) {
			model = tmodel;
			return true;
		} 
	}
	return false;
}
