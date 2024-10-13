#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shaders/shader.h>
#include <iostream>


//Mipmap - smaller subsequent textures of an original texutre, used when object/render is small or far away so as to decrease memory usage
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main() {
	glfwInit();
	//Tell client API version that created context must be compatible with
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Using core profile to get access to a subset of functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create GLFWwindow
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Looking in main solution folder
	Shader firstShader("Shaders/firstVertexShader.vs", "Shaders/firstFragShader.fs");

	float vertices[] = {
		//Positions        
		0.0f,  0.5f, 0.0f, 
	   -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	//Texture coordinates map image on
	float texCoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f
	};

	//Choose texture wrapping for each axis, where S,T,R equal X,Y,Z
	//Happens when we specify texture coords outside of 0,0 and 1,1 range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	//If we chose clamp to border as texture wrap
	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//Texture filtering is used for extrapolating pixels when resizing shape/texture
	//GL_NEAREST produces a more pixelated appearance, while GL_LINEAR blends colors together for a blur
	

	return 0;
}

