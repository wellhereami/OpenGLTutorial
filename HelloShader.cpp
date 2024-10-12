//Generalizes functions calls for each platform
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shaders/shader.h>
#include <iostream>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//out vars of vertex shader correlate to in vars of frag shader as order is vertex then frag
//uniform - global variables that can be accessed from any shader and code outside the shader
	//if uniform isn't used in GLSL code anywhere, the compiler will automatically remove it, so make sure to use it

int helloShader() {
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
	 //Positions        //Colors
	 0.0f,  0.5f, 0.0f, 1.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f, 0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f,  0.0f, 1.0f,
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Stride changes to be every 6 floats as 6 values separate the same attribute ie) one position to the next position

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	float offsetChange = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		//Check user input while window is open
		processInput(window);

		//RENDER COMMANDS HERE

		//Have to clear otherwise remnants of previous frame will remain
		glClearColor(0.13f, 0.60f, 0.59f, 1.0f);
		//Can choose which buffer to clear
		glClear(GL_COLOR_BUFFER_BIT);

		//have to use program before changing uniform variable as only current shader program is affected
		firstShader.use();
		firstShader.setFloat("offset", offsetChange);
		//Use time to make changes smoother
		float timeVal = glfwGetTime();
		float posVal = (sin(timeVal) / 2.0f) + 0.5f;
		offsetChange = posVal;
		/*
		//get uniform variable from fragment shader to change color in the code
		//get running time
		float timeVal = glfwGetTime();
		//create green value between 0 and 1 using sin
		float greenVal = (sin(timeVal) / 2.0f) + 0.5f;
		//Get uniform variable location
		int vertexColorLoc = glGetUniformLocation(shaderProgram, "ourColor");
		
		//change uniform variable
		glUniform4f(vertexColorLoc, 0.0f, greenVal, 0.0f, 1.0f);
		*/
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Swap color buffer
		glfwSwapBuffers(window);
		//Check for any input
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
