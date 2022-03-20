/*--------------------*/
// TODO 实现调试小窗
/*--------------------*/



#ifndef STB_IMAGE_IMPLEMENTATION // 调用读取图片工具库
    #include "stb_image.h"
#endif
#include "global.hpp"

/*----------------Defination----------------*/
const int WIDTH = 800;
const int HEIGHT = 800;
/*----------------Defination_complete----------------*/




int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE); // 窗口支持双重缓冲、深度测试、超采样
	glutInitWindowPosition(10, 10);
	glutInitWindowSize(WIDTH, HEIGHT);
	int mainWindow = glutCreateWindow("Portal");
	glewExperimental = GL_TRUE;
	glewInit();


	glEnable(GL_DEPTH_TEST); // 开启深度测试
	glEnable(GL_CULL_FACE); // 开启背部剔除
	glutMainLoop(); 

	return 0;
}
