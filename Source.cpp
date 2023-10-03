#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#include"Balls.h"
#include "Lights.h"
#include "Camera.h"

#define WIDTH 1280
#define HEIGTH 720
void init(void);
void DrawPoolTable(std::vector<glm::vec3>, glm::mat4);
void MovePooltablle(GLFWwindow*, double, double);
void onClik(GLFWwindow*, int, int, int);
void Scroll(GLFWwindow*, double, double);
void PoolTableRendering();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void Animator();
bool isColliding();
std::vector<glm::vec3> CreateTableModel();
//Propriedades da Mesa
float tH = 3.0f;    // Valores posteriormente divididos por 2,
float tW = 25.0f;        // sendo que os v�rtice mais longe um do outro s�o esse valor 
float tL = 35.0f;       // positivo e negativo
float heightOffset = 0.0f;
//View Movemnt
glm::mat4 ballMatrix = glm::mat4(1.0f);
double lastMouseX = 0;
double lastMouseY = 0;
bool isMouseDragging = false;
bool animate;
bool animateStop = false;
int ballToBeAnimated = 7;

// Zoom level
float zoomLevel = 1.0f;


float angle = 0.0f;

GLuint poolVBO, poolVAO;
GLuint poolShader;
int poolTableVerticesCount;


GLuint programa, coordsid, texid, normalid, textureid;


void init() {
	glClearColor(0.0f, 0.0f, 0.f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);


	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER,"Balls.vert" },
		{ GL_FRAGMENT_SHADER, "Balls.frag" },
		{ GL_NONE, NULL }
	};  //GL_None marca o final da lista de shader info

	programa = LoadShaders(shaders);
	glUseProgram(programa);

	//Posição no shader (ponteiro da variavel do shader)
	coordsid = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "VPosition");      // obtém a localização do atributo 'vposition' no 'programa'.
	texid = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "texCoords");          // obtém a localização do atributo 'vtexture' no 'programa'.
	normalid = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "VNormals"); // obtém a localização do atributo 'vnormal' no 'programa'.


	textureid = glGetProgramResourceLocation(programa, GL_UNIFORM, "texSampler");
	
	ShaderInfo tableShaders[] = {
   { GL_VERTEX_SHADER,"PoolTable.vert" },
   { GL_FRAGMENT_SHADER, "PoolTable.frag" },
   { GL_NONE, NULL }   //GL_None marca o final da lista de shader info
	};


	//Shader ID
	poolShader = LoadShaders(tableShaders);
}

std::vector<glm::vec3> ballPos = {
	glm::vec3(0.0f, 2.5f, 0.0f),
	glm::vec3(-5.0f, 2.5f, 3.0f),
	glm::vec3(2.0f, 2.5f, 12.0f),
	glm::vec3(5.0f, 2.5f, 2.0f),
	glm::vec3(-4.0f, 2.5f, -14.0f),
	glm::vec3(10.0f, 2.5f, -3.0f),
	glm::vec3(10.0f, 2.5f, -9.0f),
	glm::vec3(-11.0f, 2.5f, 7.0f),
	glm::vec3(-7.0f, 2.5f, 13.0f),
	glm::vec3(-9.0f, 2.5f, -10.0f),
	glm::vec3(3.0f, 2.5f, -10.0f),
	glm::vec3(8.0f, 2.5f, -15.0f),
	glm::vec3(-1.0f, 2.5f, 6.0f),
	glm::vec3(-4.0f, 2.5f, -6.0f),
	glm::vec3(7.0f, 2.5f, 8.0f)
};
std::vector<glm::vec3> ballRotate = {
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0),
	glm::vec3(0, 0, 0)

};

int main() {
	GLFWwindow* window;
	Camera* camera;
	camera = camera->GetInstance();
	camera->InicializeCamera(45.0f, WIDTH, HEIGHT, glm::vec3(50.0f, 20.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0));

	if (!glfwInit()) return -1;
	std::vector<glm::vec3> tableModel = CreateTableModel();

	window = glfwCreateWindow(WIDTH, HEIGHT, "Pratical Work", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);


	glfwSetKeyCallback(window, lights::OnkeyPress);

	// Set up mouse input callbacks
	glfwSetCursorPosCallback(window, MovePooltablle);
	glfwSetMouseButtonCallback(window, onClik);
	glfwSetScrollCallback(window, Scroll);
	glfwSetKeyCallback(window, keyCallback);

	//Inicia o gestor de extensões GLEW
	glewExperimental = GL_TRUE;
	glewInit();
	init();

	//Table
	PoolTableRendering();
	
	int i = 0;

	Balls ball1, ball2, ball3, ball4, ball5, ball6, ball7, ball8, ball9, ball10, ball11, ball12, ball13, ball14, ball15;
	ball1.GetPointersId(programa, coordsid, normalid, texid, textureid, 0);
	ball2.GetPointersId(programa, coordsid, normalid, texid, textureid, 1);
	ball3.GetPointersId(programa, coordsid, normalid, texid, textureid, 2);
	ball4.GetPointersId(programa, coordsid, normalid, texid, textureid, 3);
	ball5.GetPointersId(programa, coordsid, normalid, texid, textureid, 4);
	ball6.GetPointersId(programa, coordsid, normalid, texid, textureid, 5);
	ball7.GetPointersId(programa, coordsid, normalid, texid, textureid, 6);
	ball8.GetPointersId(programa, coordsid, normalid, texid, textureid, 7);
	ball9.GetPointersId(programa, coordsid, normalid, texid, textureid, 8);
	ball10.GetPointersId(programa, coordsid, normalid, texid, textureid, 9);
	ball11.GetPointersId(programa, coordsid, normalid, texid, textureid, 10);
	ball12.GetPointersId(programa, coordsid, normalid, texid, textureid, 11);
	ball13.GetPointersId(programa, coordsid, normalid, texid, textureid, 12);
	ball14.GetPointersId(programa, coordsid, normalid, texid, textureid, 13);
	ball15.GetPointersId(programa, coordsid, normalid, texid, textureid, 14);

	ball1.Read("Poolballs/Ball1.obj");
	ball2.Read("Poolballs/Ball2.obj");
	ball3.Read("Poolballs/Ball3.obj");
	ball4.Read("Poolballs/Ball4.obj");
	ball5.Read("Poolballs/Ball5.obj");
	ball6.Read("Poolballs/Ball6.obj");
	ball7.Read("Poolballs/Ball7.obj");
	ball8.Read("Poolballs/Ball8.obj");
	ball9.Read("Poolballs/Ball9.obj");
	ball10.Read("Poolballs/Ball10.obj");
	ball11.Read("Poolballs/Ball11.obj");
	ball12.Read("Poolballs/Ball12.obj");
	ball13.Read("Poolballs/Ball13.obj");
	ball14.Read("Poolballs/Ball14.obj");
	ball15.Read("Poolballs/Ball15.obj");



	//poolTable.Send();	
	//balls[0].Send();6
	ball1.Send();
	ball2.Send();
	ball3.Send();
	ball4.Send();
	ball5.Send();
	ball6.Send();
	ball7.Send();
	ball8.Send();
	ball9.Send();
	ball10.Send();
	ball11.Send();
	ball12.Send();
	ball13.Send();
	ball14.Send();
	ball15.Send();

	lights::Lights(&ball1, programa);
	lights::Lights(&ball2, programa);
	lights::Lights(&ball3, programa);
	lights::Lights(&ball4, programa);
	lights::Lights(&ball5, programa);
	lights::Lights(&ball6, programa);
	lights::Lights(&ball7, programa);
	lights::Lights(&ball8, programa);
	lights::Lights(&ball9, programa);
	lights::Lights(&ball10, programa);
	lights::Lights(&ball11, programa);
	lights::Lights(&ball12, programa);
	lights::Lights(&ball13, programa);
	lights::Lights(&ball14, programa);
	lights::Lights(&ball15, programa);



	while (!glfwWindowShouldClose(window)) {


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 zoomMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(zoomLevel));
		glm::mat4 mvp = camera->projection * camera->view * ballMatrix * zoomMatrix;

		glUseProgram(programa);
		//Draw into screen
		//poolTable.Draw();
		//balls[0].Draw(glm::vec3(-5.0f, 2.0f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		ball1.Draw(ballPos[0], ballRotate[0], ballMatrix * zoomMatrix);
		ball2.Draw(ballPos[1], ballRotate[1], ballMatrix * zoomMatrix);
		ball3.Draw(ballPos[2], ballRotate[2], ballMatrix * zoomMatrix);
		ball4.Draw(ballPos[3], ballRotate[3], ballMatrix * zoomMatrix);
		ball5.Draw(ballPos[4], ballRotate[4], ballMatrix * zoomMatrix);
		ball6.Draw(ballPos[5], ballRotate[5], ballMatrix * zoomMatrix);
		ball7.Draw(ballPos[6], ballRotate[6], ballMatrix * zoomMatrix);
		ball8.Draw(ballPos[7], ballRotate[7], ballMatrix * zoomMatrix);
		ball9.Draw(ballPos[8], ballRotate[8], ballMatrix * zoomMatrix);
		ball10.Draw(ballPos[9], ballRotate[9], ballMatrix * zoomMatrix);
		ball11.Draw(ballPos[10], ballRotate[10], ballMatrix * zoomMatrix);
		ball12.Draw(ballPos[11], ballRotate[11], ballMatrix * zoomMatrix);
		ball13.Draw(ballPos[12], ballRotate[12], ballMatrix * zoomMatrix);
		ball14.Draw(ballPos[13], ballRotate[13], ballMatrix * zoomMatrix);
		ball15.Draw(ballPos[14], ballRotate[14], ballMatrix * zoomMatrix);

		DrawPoolTable(tableModel, mvp);
		glfwSwapBuffers(window);
		glfwPollEvents();

		Animator();
	}

	glfwTerminate();
	return 0;
}
void Scroll(GLFWwindow* window, double xoffset, double yoffset) {
	// Atualizar o nível de zoom com base no deslocamento do scroll
	float zoomAmount = 0.1f;
	zoomLevel += yoffset * zoomAmount;

	// Limitar o nível de zoom a um intervalo sensível
	float minZoom = 0.1f;
	float maxZoom = 10.0f;
	if (zoomLevel < minZoom) {
		zoomLevel = minZoom;
	}
	if (zoomLevel > maxZoom) {
		zoomLevel = maxZoom;
	}
}
void MovePooltablle(GLFWwindow* window, double xpos, double ypos) {
	// Verificar se o rato está a ser arrastado
	if (isMouseDragging) {
		// Calcular o movimento do rato apenas no eixo x
		double deltaX = xpos - lastMouseX;

		// Aplicar a rotação na matriz de modelo apenas no eixo y
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(static_cast<float>(deltaX)), glm::vec3(0.0f, 1.0f, 0.0f));
		ballMatrix = rotation * ballMatrix;

		// Atualizar a última posição do rato
		lastMouseX = xpos;
		lastMouseY = ypos;
	}
}

void onClik(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			isMouseDragging = true;
			glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
		}
		else if (action == GLFW_RELEASE) {
			isMouseDragging = false;
		}
	}
}
void PoolTableRendering() {
	// criar e o bind do VAO
	glGenVertexArrays(1, &poolVAO);
	glBindVertexArray(poolVAO);

	// Criar o VBO VBO
	glGenBuffers(1, &poolVBO);
	glBindBuffer(GL_ARRAY_BUFFER, poolVBO);

	// dar uplod aos dados da mesa
	std::vector<glm::vec3> tableModel = CreateTableModel();
	poolTableVerticesCount = tableModel.size();

	glBufferData(GL_ARRAY_BUFFER, tableModel.size() * sizeof(glm::vec3), tableModel.data(), GL_STATIC_DRAW);

	// epecificar os vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
	glEnableVertexAttribArray(0);

	// dar ubind do VAO e do VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void DrawPoolTable(std::vector<glm::vec3> tableModel, glm::mat4 mvp) {          

	glUseProgram(poolShader);
	glBindVertexArray(poolVAO);
	glUniformMatrix4fv(glGetUniformLocation(poolShader, "mvp"), 1, false, glm::value_ptr(mvp));
	glDrawArrays(GL_QUADS, 0, tableModel.size());
	glUseProgram(0);
}

std::vector<glm::vec3> CreateTableModel() {
	float poolW = tW / 2;
	float poolH = tH / 2 + heightOffset;
	float poolL = tL / 2;

	std::vector<glm::vec3> tableModel = {
		// Frente
		glm::vec3(-poolW, -poolH, poolL),
		glm::vec3(poolW, -poolH, poolL),
		glm::vec3(poolW, poolH, poolL),
		glm::vec3(-poolW, poolH, poolL),

		// Trás
		glm::vec3(-poolW, -poolH, -poolL),
		glm::vec3(-poolW, poolH, -poolL),
		glm::vec3(poolW, poolH, -poolL),
		glm::vec3(poolW, -poolH, -poolL),

		// Lado Esquerdo
		glm::vec3(-poolW, -poolH, poolL),
		glm::vec3(-poolW, poolH, poolL),
		glm::vec3(-poolW, poolH, -poolL),
		glm::vec3(-poolW, -poolH, -poolL),

		// Lado Direito
		glm::vec3(poolW, -poolH, poolL),
		glm::vec3(poolW, -poolH, -poolL),
		glm::vec3(poolW, poolH, -poolL),
		glm::vec3(poolW, poolH, poolL),

		// Lado de cima
		glm::vec3(-poolW, poolH, poolL),
		glm::vec3(poolW, poolH, poolL),
		glm::vec3(poolW, poolH, -poolL),
		glm::vec3(-poolW, poolH, -poolL),

		// Lado de baixo 
		glm::vec3(-poolW, -poolH, poolL),
		glm::vec3(-poolW, -poolH, -poolL),
		glm::vec3(poolW, -poolH, -poolL),
		glm::vec3(poolW, -poolH, poolL)
	};

	return tableModel;

}


bool isColliding() {
	float ballRadius = 1.0f;

	for (int i = 0; i < ballPos.size(); i++) {
		if (i != ballToBeAnimated) {
			float distance = glm::distance(ballPos[i], ballPos[ballToBeAnimated]);

			// se colidiu com alguma bola
			if (distance <= 2 * ballRadius) {
				return true;
			}
		}
	}

	// se colidiu com os limites da mesa
	if (ballPos[ballToBeAnimated].x + ballRadius >= 12.5f || ballPos[ballToBeAnimated].x - ballRadius <= -12.5f ||
		ballPos[ballToBeAnimated].z + ballRadius >= 17.5f || ballPos[ballToBeAnimated].z - ballRadius <= -17.5f) {
		return true;
	}

	return false;
}

void Animator()
{
	// se animação da bola iniciou
	if (animate && !animateStop) {
		// move a bola
		ballPos[ballToBeAnimated].x += 0.05f;
		//ballPos[ballToBeAnimated].z += 0.01f;
		ballRotate[ballToBeAnimated].z += -1.0f;


		// se colidiu com outro objeto
		if (isColliding()) {
			animate = false;
			animateStop = true;
		}
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (animateStop) return;
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		animate = !animate;
	}
}

