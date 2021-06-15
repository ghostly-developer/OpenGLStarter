#include<iostream>
#include<stdlib.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


using namespace std;

/* 
In the background the previous frame shown is being overwritten with new information, these are called buffers
front buffer = seen/read
back buffer = written

Vertex data: Vertex Shader > Shape Assembly > Geo Shader > Rasterization (Mathematical shape into pixels) > Fragment Shader (Adds colours to pixels, depends on a lot) > tests and blending

*/
int main() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Specifying OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Using core porfile (just modern libraries)

	GLfloat vert2d[] =
	{
		-0.28f, -0.5f * float(sqrt(3)) / 3, 0.0f,
		-0.5f, 1.0f * float(sqrt(3)) / 3, 0.0f,
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
	};

	// Vertex Shader source code
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	//Fragment Shader source code
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
		"}\n\0";

	//------------------------------------------------------------------------------------------------

	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Testing", NULL, NULL); //Width, Height, Name, Fullscreen, ~
	if (window == NULL) {
		cout << "Failed to create a window" << endl; //Just checking if the window failed to create
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); //tells glfw to make the window to hold 'all' of opengl

	gladLoadGL(); //load the config for opengl

	glViewport(0, 0, 800, 800); //where we want the opengl to show stuff

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //specifying we want to make a vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //ref value, using 1 string for whole shader, where source code is, ~
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram(); //wrap all the shaders into a shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader); //adding the shaders

	glLinkProgram(shaderProgram); //wrap up the program

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader); //delete these because they are already inside the program

	GLuint VBO, VAO;

	glGenVertexArrays(1, &VAO); //amnt of objects, reference
	

	glGenBuffers(1, &VBO); //amnt. of 3d objects and reference
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBindVertexArray(VAO); //where to find the VAO

	glBufferData(GL_ARRAY_BUFFER, sizeof(vert2d), vert2d, GL_STATIC_DRAW); //type of buffer, size of data in bytes, the data and the use. GL_Static (modified once, used many), Stream (mod. once, used a few times), Dynamic (modified multiple, used many)
	//VAO acts as pointers to VBO's and tells opengl on how to use them

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //position of vert attribute, values per vert, value type, if values art int, amnt of data between each vertex, offset (using void because our vertex is at the start of the array)
	glEnableVertexAttribArray(0); //enabling it, the var is the position of vert attribute

	glBindBuffer(GL_ARRAY_BUFFER, 0); //bind VBO and VOA
	glBindVertexArray(0);


	glClearColor(0.07f, 0.13f, 0.17f, 1.0f); //clears the colour of current background and replaces it. also this is navy blue
	glClear(GL_COLOR_BUFFER_BIT); //le execute with the color buffer
	glfwSwapBuffers(window);

	while (!glfwWindowShouldClose(window)) {

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); //type of primitive, starting index, amount of vertices
		glfwSwapBuffers(window);
		glfwPollEvents(); //this is to make the window actually, respond.
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	
	glfwDestroyWindow(window); //killing the window after we finish

	glfwTerminate(); //terminating glfw after function
	return 0;
}