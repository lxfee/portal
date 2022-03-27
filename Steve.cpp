#pragma once
#include "Steve.h"


Model** Steve::component = nullptr;


const glm::vec3 Steve::TorsoConnectPoint[NUM_OF_COMPONENT] = {
	glm::vec3( 0, 0, 0),
	glm::vec3( 0, 0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z),
	glm::vec3( 0.5 * componentSize[Torso].x,  0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z),
	glm::vec3(-0.5 * componentSize[Torso].x,  0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z),
	glm::vec3( 0.25 * componentSize[Torso].x, -0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z),
	glm::vec3(-0.25 * componentSize[Torso].x, -0.5 * componentSize[Torso].y, 0.5 * componentSize[Torso].z)
};

 const glm::vec3 Steve::localConnectPoint[NUM_OF_COMPONENT] {
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
	glm::vec3(0, 0, 0),
	glm::vec3(90, 0, 0),
	glm::vec3(90, 0, 0),
	glm::vec3(90, 0, 0),
	glm::vec3(90, 0, 0)
};

frameData frameData::operator+(const frameData& rhs) const {
	frameData res;
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		res.data[i] = data[i] + rhs.data[i];
	}
	return res;
}

frameData frameData::operator-(const frameData& rhs) const {
	frameData res;
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		res.data[i] = data[i] - rhs.data[i];
	}
	return res;
}

frameData frameData::operator*(float k) const {
	frameData res;
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		res.data[i] = data[i] * k;
	}
	return res;
}

frameData::frameDate(const glm::vec3 data[NUM_OF_COMPONENT]) {
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		this->data[i] = data[i];
	}
}

void frameData::print() {
	cout << "frameData({";
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		cout << "glm::vec3(";
		cout << data[i].x << ", " << data[i].y << ", "<< data[i].z;
		cout << ")";
		if(i < NUM_OF_COMPONENT - 1) cout << ",";  
	}
	cout << "})" << endl;
}


frameData::frameDate() {
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		data[i] = glm::vec3(0.0);
	}
}

animateSequence::animateSequence() {
	curframe = 0;
}

void animateSequence::setDefault() {
	curframe = 0;
}

void animateSequence::next() {
	if(!frames.empty()) curframe = (curframe + 1) % frames.size();
}

void animateSequence::addFrame(frameData frame, float inv) {
	frames.emplace_back(frame);
	interval.emplace_back(inv);
}



void animateSequence::linearFrame(frameData& current, float& t, float dt, float rate)  {
	if(frames.empty()) return ;
	float t1 = t, t2 = t1 + dt, T = interval[curframe] * rate;
	t += dt;
	if(t2 >= T) {
		current = frames[curframe];
		t = 0;
		next();
	} else {
		current = (frames[curframe] - current) * ((t2 - t1) / (T - t1)) + current;
	}
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
	component = new Model*[NUM_OF_COMPONENT];
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
		component[i] = new Model((dir + objName[i]).c_str());
	}
}
Steve& Steve::setAnimate(SteveComponent partName) {
	isAnimate[partName] = true;
	return *this;
}

Steve::Steve() {
	if(!component) {
		setup();
	}
	
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		ComponentNode* node = new ComponentNode();
		node->component = component[i];
		node->component->rotation = initRotation[i];
		node->component->translation = -localConnectPoint[i];
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
	eye = new Camera();
	eye->maxPitch = 80;
	eye->maxPitch = 80;
	eye->maxYaw = 80;
	eye->fov = 60;

	walkingSeq = new animateSequence();
	
	walkingSeq->addFrame(frameData({glm::vec3(0, 0, 0),glm::vec3(0.0668911, 0, 0),glm::vec3(-60, 0, 0),glm::vec3(55, 0, 0),glm::vec3(35, 0, 0),glm::vec3(-30, 0, 0)}));
	walkingSeq->addFrame(frameData({glm::vec3(0, 0, 0),glm::vec3(0, 0, 0),glm::vec3(50.0393, 0, 0),glm::vec3(-45, 0, 0),glm::vec3(-40, 0, 0),glm::vec3(25, 0, 0)}));

	stoppingSeq = new animateSequence();
	stoppingSeq->addFrame(frameData());

	memset(isAnimate, 0, sizeof isAnimate);
}

bool Steve::isWalking(unsigned char* KEYBUFFER) {
	return KEYBUFFER['w'] || KEYBUFFER['d'] || KEYBUFFER['s'] || KEYBUFFER['a'];
}

void Steve::playAnimate(unsigned char* KEYBUFFER) {
	extern float frameTime;
	static float elapsedTime = 0;
	static enum{STOP, WALKING} status = STOP;
	frameData current = getFrameData();

	switch(status) {
		case STOP:
			stoppingSeq->linearFrame(current, elapsedTime, frameTime, 0.5);
			setAnimate(LeftArm).setAnimate(RightArm).setAnimate(LeftLeg).setAnimate(RightLeg);
			setFrameData(current);
			if(isWalking(KEYBUFFER)) {
				elapsedTime = 0;
				stoppingSeq->setDefault();
				status = WALKING;
			}
			break;
		case WALKING:
			walkingSeq->linearFrame(current, elapsedTime, frameTime, 0.5);
			setAnimate(LeftArm).setAnimate(RightArm).setAnimate(LeftLeg).setAnimate(RightLeg);
			setFrameData(current);
			if(!isWalking(KEYBUFFER)) {
				elapsedTime = 0;
				walkingSeq->setDefault();
				status = STOP;
			}
			break;
	}
}


void Steve::Draw(Shader* shader) {
	glm::mat4 model = glm::translate(glm::mat4(1.0), position);
	Draw(shader, root, model);
}

void Steve::Draw(Shader* shader, ComponentNode* current, glm::mat4 model) {
	if(!current) return ;
	glm::mat4 localTranslation = glm::translate(glm::mat4(1.0), current->fatherConnectPoint);
	glm::mat4 rotation(1.0);
	rotation = glm::rotate(rotation, glm::radians(current->Deg.x), glm::vec3(1.0, 0.0, 0.0));
	rotation = glm::rotate(rotation, glm::radians(current->Deg.y), glm::vec3(0.0, 1.0, 0.0));
	rotation = glm::rotate(rotation, glm::radians(current->Deg.z), glm::vec3(0.0, 0.0, 1.0));
	if(current->component) current->component->Draw(shader, model * localTranslation * rotation);
	model = model * localTranslation * rotation;
	for(auto son : current->sonNode) {
		Draw(shader, son, model);
	}
}


void Steve::doMovement(unsigned char* KEYBUFFER)  {
	static int cur = 0;
	if(KEYBUFFER['=']) {
		cur = (cur + 1) % NUM_OF_COMPONENT;
		KEYBUFFER['='] = false;
	}
	for(int i = 1; i <= 6; i++) {
		if(KEYBUFFER[i + '0']) index[cur]->Deg[(i + 1) / 2 - 1] += ((i + 1) % 2) ? 5 : -5;
	}
	static bool flag = false;
	if(flag) playAnimate(KEYBUFFER);
	if(KEYBUFFER['-']) {
		flag ^= 1;
		KEYBUFFER['-'] = false;
	}
	if(KEYBUFFER['p']) {
		getFrameData().print();
		KEYBUFFER['p'] = false;
	}
}

void Steve::setFrameData(const frameData& frame) {
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		if(isAnimate[i]) index[i]->Deg = frame.data[i];
		isAnimate[i] = false;
	}
}


frameData Steve::getFrameData() {
	frameData result;
	for(int i = 0; i < NUM_OF_COMPONENT; i++) {
		result.data[i] = index[i]->Deg;
	}
	return result;
}