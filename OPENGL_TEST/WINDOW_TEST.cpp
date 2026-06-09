#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSourceUniform = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* vertexShaderSourceRGB = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"    ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSourceOrange = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

const char* fragmentShaderSourceUniform = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"    FragColor = ourColor;\n"
"}\n";

const char* fragmentShaderSourceRGB = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(ourColor.x, ourColor.y, ourColor.z, 1.0f);\n"
"}\0";

void VersionBoilerPlate() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void VertexShaderBootstrap(unsigned int& vertexShader, const char* vertexShaderSource, int& success, char* infoLog) {
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //vertex shader compile check
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void FragmentShaderBootstrap(unsigned int& fragmentShaderLocation, const char* fragmentShaderSource, int& success, char* infoLog) {
	fragmentShaderLocation = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderLocation, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderLocation);
	glGetShaderiv(fragmentShaderLocation, GL_COMPILE_STATUS, &success); // compile check
	if (!success) {
		glGetShaderInfoLog(fragmentShaderLocation, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void ShaderProgramBootstrap(unsigned int& shaderProgram, unsigned int& vertexShader, unsigned int& fragmentShader, int& success, char* infoLog) {
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success); //shader program link check
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}
}

int main() {
	VersionBoilerPlate();
	//window bootstrap
	GLFWwindow* window = glfwCreateWindow(800, 600, "OPENGL_TEST", NULL, NULL);
	if (window == NULL) {
		std::cout << "FAILED TO CREATE GLFW WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback); //2nd argument is a function that sets size of window
	//glad check
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "FAILED TO INITIALIZE GLAD" << std::endl;
		return -1;
	}

	//vertex shader, see definition on top of file for vertex shader source
	int success;
	char infoLog[512];
	unsigned int vertexShader[2];
	VertexShaderBootstrap(vertexShader[0], vertexShaderSourceUniform, success, infoLog);
	VertexShaderBootstrap(vertexShader[1], vertexShaderSourceRGB, success, infoLog);
	//fragment shader, see definition on top of file for fragment shader source
	unsigned int fragmentShader[2];
	FragmentShaderBootstrap(fragmentShader[0], fragmentShaderSourceUniform, success, infoLog);
	FragmentShaderBootstrap(fragmentShader[1], fragmentShaderSourceRGB, success, infoLog);
	//linking shaders
	unsigned int shaderProgram[2];
	ShaderProgramBootstrap(shaderProgram[0], vertexShader[0], fragmentShader[0], success, infoLog);
	ShaderProgramBootstrap(shaderProgram[1], vertexShader[1], fragmentShader[1], success, infoLog);
	//cleaning up shader objects
	glDeleteShader(vertexShader[0]);
	glDeleteShader(vertexShader[1]);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(fragmentShader[1]);

	//define vertices
	float doubleTriVertices[] = {
		 0.0f, -0.4f, 0.0f,
		-0.8f, -0.4f, 0.0f,
		-0.4f,  0.4f, 0.0f,
	};
	float doubleTriVertices1[] = {
		// positions         colors
		 0.0f, -0.4f, 0.0f,  1.0f, 0.0f, 0.0f,//bottom right
		 0.8f, -0.4f, 0.0f,  0.0f, 1.0f, 0.0f,//bottom left
		 0.4f,  0.4f, 0.0f,  0.0f, 0.0f, 1.0f //top 
	};
	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3,
	};

	//vertex array object, vertex buffer object, element buffer object
	unsigned int VAOs[2], VBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);

	//1. bind vertex Array Object
	//2-3. copy vertices array into buffer for OpenGL
	//4. copy indices array into buffer, using glDrawElements, if commented out, use glDrawArrays
	//6-7. data -> vertex attribute (location <see vertex shader source>, amount of vertex attrib, type of data, normalize?, total vertex size, stride)

	glBindVertexArray(VAOs[0]); //x.1 bind vertex Array Object
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(doubleTriVertices), doubleTriVertices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //0.1 copy indices array into buffer, using glDrawElements, if commented out, use glDrawArrays
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //1. specifying what part of data goes to which vertex attribute
	glEnableVertexAttribArray(0);

	
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(doubleTriVertices1), doubleTriVertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//render loop
	while (!glfwWindowShouldClose(window)) {
		//input
		ProcessInput(window);

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram[0]);
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram[0], "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); //unbinding vertex array

		//check and call events, swap buffer
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	//optional deallocation
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram[0]);
	glDeleteProgram(shaderProgram[1]);

	glfwTerminate();

	return 0;
}