#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 ModelView;
uniform mat3 NormalMatrix;


in vec3 VPosition;			// Coordenadas locais do vértice
in vec3 VNormals;			// Normal do vértice
in vec2 texCoords;

out vec2 uv;
out vec3 normal;
out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;

void main()
{ 
	
	normal = VNormals;
	uv = texCoords;

	// Posição do vértice em coordenadas do olho.
	vPositionEyeSpace = (ModelView * vec4(VPosition, 1.0)).xyz;

	// Transformar a normal do vértice.
	vNormalEyeSpace = normalize(NormalMatrix * VNormals);

	// Coordenada de textura para o CubeMap
	//textureVector = VPosition;

	// Posição final do vértice (em coordenadas de clip)
	gl_Position = Projection * ModelView * vec4(VPosition, 1.0f);
}