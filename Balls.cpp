#include"Balls.h"
#define STB_IMAGE_IMPLEMENTATION
#include <glm/gtc/matrix_transform.hpp> 
#include "stb_image.h"
#include "LoadShaders.h"
#include "Camera.h"


using namespace glm;

void Balls::GetPointersId(GLuint shader, GLuint coordsId, GLuint normalsId, GLuint texId, GLuint textureId, int counter)
{
	programa = shader;
	this->coordsId = coordsId;
	this->normalsId = normalsId;
	this->texId = texId;
	this->textureId = textureId;
	this->counter = counter;
}

void Balls::Read(const std::string objFilepath) {
	using namespace std;
	cout << endl << endl << "Ball Indexes" << endl;
	cout << "Programa: " << programa << endl;
	cout << "coordsId: " << this->coordsId << endl;
	cout << "normalsId: " << this->normalsId << endl;
	cout << "texId: " << this->texId << endl;
	cout << "textureId: " << this->textureId << endl;
	cout << "counter: " << this->counter << endl;

	//Vertex Variables
	//definir os vetores para armazenar as posições dos vértices (vertexPosition), coordenadas de textura (vertexTexturesCoord) e normais (vertexNormal).
	std::vector<glm::fvec3> vertexPosition;
	std::vector<glm::fvec2> vertexTexturesCoord;
	std::vector<glm::fvec3> vertexNormal;

	//Vectors Variables para faces
	//aqui criados vetores adicionais para armazenar os índices dos vértices (vertexPositionIndicies), coordenadas de textura (vertexTextureCoordIndicies) e normais 
	std::vector<GLint> vertexPositionIndicies;
	std::vector<GLint> vertexTextureCoordIndicies;
	std::vector<GLint> vertexNormalIndicies;

	//File Variables
	std::stringstream ss;
	std::ifstream in_file(objFilepath);
	std::string line = "";
	std::string prefix = "";

	//Vectores Auxiliares Temporarios - utilizado para obter os todos indices 1 por 1
	glm::vec3 vec3_aux; 
	glm::vec2 vec2_aux; 
	std::string nextFile; 


	//Ver se pode abrir o file
	try {
		if (!in_file.is_open())
			throw std::runtime_error("Error opening the material file");
		// File opened successfully, continue processing
	}
	catch (const std::exception& e) {
		// Handle the exception
		std::cout << "Exception occurred: " << e.what() << std::endl;
	}
	//Para cada linha lida, o código verifica o prefixo para determinar qual tipo de dado está a ser lido: vértice (v), coordenada de textura (vt), normal (vn) ou face (f).
	while (getline(in_file, line)) { //read 1 line at time
		ss.str(line);
		ss >> prefix;
		if (prefix == "mtllib")
		{
			ss >> nextFile;
			Load(nextFile);
		}

		if (prefix == "v" || prefix == "vt" || prefix == "vn" || prefix == "f") {

			if (prefix == "v") { // vertex position

				ss >> vec3_aux.x >> vec3_aux.y >> vec3_aux.z; //put in temporary variable
				vertexPosition.push_back(vec3_aux); //store on vertexPosition array
			}
			else if (prefix == "vt") {

				ss >> vec2_aux.x >> vec2_aux.y;
				vertexTexturesCoord.push_back(vec2_aux);
			}
			else if (prefix == "vn") {
				ss >> vec3_aux.x >> vec3_aux.y >> vec3_aux.z;
				vertexNormal.push_back(vec3_aux);
			}
			else if (prefix == "f") {
				for (int a = 0; a < 3; a++)
				{
					std::string data;
					ss >> data;
					std::istringstream dss(data);

					std::string index;
					getline(dss, index, '/');
					int pindex = std::stoi(index) - 1;

					getline(dss, index, '/');
					int tcindex = std::stoi(index) - 1;

					getline(dss, index, '/');
					int nindex = std::stoi(index) - 1;


					vertex_positions.push_back(vertexPosition[pindex]);
					vertex_textures_coords.push_back(vertexTexturesCoord[tcindex]);
					vertex_normals.push_back(vertexNormal[nindex]);
				}
			}
		}
		ss.clear();
	}
	
	in_file.close();
}

void Balls::Load(std::string fileName) {

    //recebe um parâmetro fileName, que é o nome do arquivo a ser aberto.
	std::ifstream in_file("PoolBalls/" + fileName);
	glm::vec3 vec3_aux;

	if (!in_file.is_open())
	{
		std::cout << "Error opening the file: " << fileName << std::endl;
		return;
	}

	//É criada uma string line para armazenar cada linha do arquivo.
	std::string line;

	//loop while é usado para ler cada linha do arquivo.
	while (std::getline(in_file, line))
	{
		//A linha lida é armazenada em um objeto std::istringstream chamado iss
		std::istringstream iss(line);
		std::string type;
		std::string texFileName;

		iss >> type;
		//type for "Ka", os três valores seguintes são lidos e armazenados nas componentes x, y e z do vetor ka.
		if (type == "Ka") // luz ambiental
		{
			iss >> vec3_aux.x >> vec3_aux.y >> vec3_aux.z;
			ka = vec3_aux;
		}
		//type for "Kd", os três valores seguintes são lidos e armazenados nas componentes x, y e z do vetor kd.
		else if (type == "Kd") // luz direcional
		{
			iss >> vec3_aux.x >> vec3_aux.y >> vec3_aux.z;
			kd = vec3_aux;
		}
		//type for "Ks", os três valores seguintes são lidos e armazenados nas componentes x, y e z do vetor ks.
		else if (type == "Ks") //luz especular
		{
			iss >> vec3_aux.x >> vec3_aux.y >> vec3_aux.z;
			ks = vec3_aux;
		}
		//type for "Ns", o valor seguinte é lido e armazenado na variável ns.
		else if (type == "Ns") // brilho
		{
			iss >> ns;
		}
		//ype for "map_Kd", o nome do arquivo de textura é lido e armazenado na string texFileName. Em seguida, a função Texture é chamada, passando o nome do arquivo para carregar a textura correspondente.
		else if (type == "map_Kd") // ficheiro da imagem
		{
			iss >> texFileName;
			Texture(texFileName);
		}
	}
	in_file.close();
}

void  Balls::Texture(const std::string textureFile)
{
	glActiveTexture(GL_TEXTURE0);
	///--------   Textura    ---------
	GLuint textureName = counter;
	this->textureName = textureName;
	using namespace std;
	//cout << "textureName: " << this->textureName << endl;

	std::string fileTemp = "PoolBalls/" + textureFile;

	// Gera um nome de textura
	glGenTextures(1, &textureName);

	glBindTexture(GL_TEXTURE_2D, textureName);

	// Define os parâmetros de filtragem (wrapping e ajuste de tamanho)
	// para a textura que está vinculada ao target GL_TEXTURE_CUBE_MAP da Unidade de Textura ativa.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	// Ativa a inversão vertical da imagem, aquando da sua leitura para memória.
	stbi_set_flip_vertically_on_load(true);

	// Leitura/descompressão do ficheiro com imagem de textura
	int width, height, nChannels;
	unsigned char* imageData = stbi_load(fileTemp.c_str(), &width, &height, &nChannels, 0);
	if (imageData) {
		// Carrega os dados da imagem para o Objeto de Textura vinculado ao target da face
		glTexImage2D(GL_TEXTURE_2D,
			0,					// Nível do Mipmap
			GL_RGB,				// Formato interno do OpenGL
			width, height,		// width, height
			0,					// border
			nChannels == 4 ? GL_RGBA : GL_RGB,	// Formato da imagem
			GL_UNSIGNED_BYTE,	// Tipos dos dados da imagem
			imageData);			// Apontador para os dados da imagem de textura

		// Gera o Mipmap para essa textura
		glGenerateMipmap(GL_TEXTURE_2D);

		// Liberta a imagem da memória do CPU
		stbi_image_free(imageData);
	}
	else {
		std::cout << "Error loading texture!" << std::endl;
	}

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "OpenGL Error: " << error << std::endl;
	}
}

void Balls::Send(void) {

	GLfloat BindPos[24192 * 3];// 8064 * 3 (vertice triangulo) * 3 (posiçoes x,y,z)
	GLfloat BindN[24192 * 3];
	GLfloat BindTex[24192 * 2];// tudo igual mas no fim é *2 por causa das u,v 

	for (int i = 0; i < vertex_positions.size(); i++) {

		//Vertices
		BindPos[i * 3] = vertex_positions[i].x;
		BindPos[i * 3 + 1] = vertex_positions[i].y;
		BindPos[i * 3 + 2] = vertex_positions[i].z;
		
		//Normais
		BindN[i * 3] = vertex_normals[i].x;
		BindN[i * 3 + 1] = vertex_normals[i].y;
		BindN[i * 3 + 2] = vertex_normals[i].z;

		//Coordenadas de Texture
		BindTex[i * 2] = vertex_textures_coords[i].x;
		BindTex[i * 2 + 1] = vertex_textures_coords[i].y;

		
	}

	glGenVertexArrays(1, &VAO);// Gerar nomes para VAOs.
	glBindVertexArray(VAO);// Faz bind do VAO

	//Guardar a informação no buffer
	glGenBuffers(3, Buffers);// Gera 'NumBuffers' nomes para VBOs.

	for (int i = 0; i < 3 ; i++) {
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]);
		if (i == 0)
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(BindPos), BindPos, 0);  //Info dos vertices - Inicializa o VBO (que está ativo) com dados imutáveis.
		if (i == 1)
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(BindN), BindN, 0); 
		if (i == 2)
			glBufferStorage(GL_ARRAY_BUFFER, sizeof(BindTex), BindTex, 0); 
	}

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glVertexAttribPointer(coordsId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	glVertexAttribPointer(normalsId, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	glVertexAttribPointer(texId, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	//habilita o atributo com localização 'coordsid', 'textid', normalid para o vao ativo.
	glEnableVertexAttribArray(coordsId);
	glEnableVertexAttribArray(normalsId);
	glEnableVertexAttribArray(texId);

	glProgramUniform1i(programa, textureId, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cout << "OpenGL Error: " << error << std::endl;                           
	}
}

void Balls::Draw(glm::vec3 position, glm::vec3 orientation, glm::mat4 modelMatrix) {

	Camera::GetInstance()->Update();

	mat4 tempball = modelMatrix;

	
	tempball = translate(tempball, position);

	
	//Orientation é o pitch, yaw, roll em graus
	tempball = rotate(tempball, radians(orientation.x), vec3(1, 0, 0)); //pitch
	tempball = rotate(tempball, radians(orientation.y), vec3(0, 1, 0)); //yaw
	tempball = rotate(tempball, radians(orientation.z), vec3(0, 0, 1)); //roll

	GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(tempball));

	mat4 modelView = Camera::GetInstance()->view * tempball;
	GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
	glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(modelView));

	mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelView));
	GLint normalMatrixId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
	glProgramUniformMatrix4fv(programa, normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	GLint viewID = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(programa, viewID, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->view));

	GLint projectionId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Projection");
	glProgramUniformMatrix4fv(programa, projectionId, 1, GL_FALSE, glm::value_ptr(Camera::GetInstance()->projection));

	glBindTexture(GL_TEXTURE_2D, textureName +1);

	glBindVertexArray(VAO);

	// Envia comando para desenho de primitivas GL_TRIANGLES, que utilizará os dados do VAO vinculado.
	glDrawArrays(GL_TRIANGLES, 0, vertex_positions.size());
	// glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, (void*)0); 
}