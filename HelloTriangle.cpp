//Generalizes functions calls for each platform
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/*GRAPHICS PIPELINE SIMPLIFIED 
	1. Vertex Shader
	2. Geometry Shader
	3. Shape Assembly
	4. Rasterization
	5. Fragment Shader
	6. Tests and Blending
*/

// Primitive - shape that is rendered from a collection of points
// Vertex Buffer Object (VBO) - stores a large number of vertices at once for batch sending to GPU
// Vertex Array Object (VAO) - stores vertex attribute configurations and calls to enable/disable attributes
// Element Buffer Object (EBO) - combines vertices and indices to remove overhead cause by a reused vertex

// XYZ coords range from -1.0 to 1.0, also known as normalized device coordinates

//Vertex Shader - works with one vertex at a time, the current shader only deals with position data
//Location = 0 refers to attribute index, can add more attributes this way
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//Fragment Shader - current shader only shows a single color on the screen
const char* fragShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

//Fragment Shader - current shader only shows a single color on the screen
const char* fragNewColorSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.7f, 0.0f, 0.5f, 1.0f);\n"
"}\0";

int createTriangle() {
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

	//Vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Bind shader to code from earlier, second param is how many strings
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}	
	
	//Fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Second fragment shader for new color
	unsigned int fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &fragNewColorSource, NULL);
	glCompileShader(fragmentShader2);

	glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Final shader, combines all previous shaders
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//Important to link in this order as the output of one shader becomes the input of the next shader
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Second combined shader for new color
	unsigned int shaderProgram2;
	shaderProgram2 = glCreateProgram();
	//Important to link in this order as the output of one shader becomes the input of the next shader
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);

	glGetShaderiv(shaderProgram2, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Don't need vertex/frag shaders anymore as they are in shaderProgram now
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);

	//Vertex data

	float vertices[] = {
		-0.5f,  0.3f, 0.0f,
		-0.8f, -0.3f, 0.0f,
		-0.2f, -0.3f, 0.0f,

	};

	float vertices2[] = {
		0.5f,  0.3f, 0.0f,
		0.8f, -0.3f, 0.0f,
		0.2f, -0.3f, 0.0f,
	};


	/*
	float vertices[] = {
		0.5f,   0.5f, 0.0f,
		0.5f,  -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};
	*/

	//Use indexed drawing
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	//Used to send large amounts of vertex data at once
	unsigned int VBO[2], VAO[2], EBO;
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO[0]);
	//VBO is attached to GL_ARRAY_BUFFER, and any changes will result in changes to VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//Copy vertices into VBO buffer
	//Parameters - Which array to add data to, size of data, actual data, and how often data will be used/changed
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind EBO
	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	*/

	/*
	Vertex attributes must be linked from data for shader to correctly interpret
	Currently, vertex data is comprised of 3 values, xyz, with each value at 4 bytes for a total of 12 bytes
	First param is index, which tells what vertex attribute will be stored here - 0 is position
	Second param is number of components per attribute, Third param is data type, Fourth param is normalized 
	Fifth param is byte offset between attributes, Sixth param is pointer to starting offset if applicable
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Change to wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Check if window has been instructed to close
	while (!glfwWindowShouldClose(window)) {
		//Check user input while window is open
		processInput(window);

		//RENDER COMMANDS HERE

		//Have to clear otherwise remnants of previous frame will remain
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//Can choose which buffer to clear
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		//Unneccesary to bind again for this program as there is only one VAO
		glBindVertexArray(VAO[0]);
		//First param is primitive type, second param is starting index of vertex array, third param is num of vertices to draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//First param is primitive type, second param is num of vertices to draw, third param is data type of indices, fourth param is offset in ebo
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glUseProgram(shaderProgram2);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Swap color buffer
		glfwSwapBuffers(window);
		//Check for any input
		glfwPollEvents();
	}

	//Deallocate arrays
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);

	glfwTerminate();
	return 0;

}

//Deal with user input on window
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	//First two parameters set location, last two set size of window
	//Size can be smaller than the GLFWwindow, but will render smaller
	glViewport(0, 0, width, height);
}
