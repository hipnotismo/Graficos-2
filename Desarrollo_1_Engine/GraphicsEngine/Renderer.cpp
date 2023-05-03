#include "renderer.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <sstream>


Renderer* Renderer::myRef;

Renderer::Renderer()
{
	modelLoc = 0;
	projectLoc = 0;
	viewLoc = 0;
	lightLoc = 0;
	cam = new Camera();
	pixelShader = nullptr;
	textureShader = nullptr;
}

Renderer::~Renderer()
{
	if (cam) delete cam;
	if (pixelShader) delete pixelShader;
	if (materialShader) delete materialShader;
	if (textureShader) delete textureShader;
}

void Renderer::CreateShaders()
{
	pixelShader = new Shader();
	pixelShader->CreateProgram("Res/Shaders/VertexShader.shader", "Res/Shaders/FragmentShader.shader");
	textureShader = new Shader();
	textureShader->CreateProgram("Res/Shaders/SpriteVertexShader.shader", "Res/Shaders/SpriteFragmentShader.shader");
	materialShader = new Shader();
	materialShader->CreateProgram("Res/Shaders/MaterialVertexShader.shader", "Res/Shaders/MaterialFragmentShader.shader");
	CallUniformShaders(textureShader);
}

void Renderer::Draw(float* vertex, int vertexLength, unsigned int* index, int indexLength, glm::mat4 modelMatrix)
{
	DefVertexAttribute();
	CallUniformShaders(pixelShader);
	pixelShader->ActiveProgram();

	UpdateModelUniformShaders(modelMatrix);
	UpdateProjectUniformShaders(cam->projection);
	UpdateViewUniformShaders(cam->view);
	/*UpdateProjectUniformShaders(projection);
	UpdateViewUniformShaders(view);*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexLength, vertex, GL_STATIC_DRAW); //set info to buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indexLength, index, GL_STATIC_DRAW); //set info to buffer
	glDrawElements(GL_TRIANGLES, indexLength, GL_UNSIGNED_INT, 0);
}

void Renderer::SpriteDraw(float* vertex, int vertexLength, unsigned int* index, int indexLength, glm::mat4 modelMatrix, bool alpha)
{
	DefVertexSpriteAttribute();
	CallUniformShaders(textureShader);
	textureShader->ActiveProgram();
	DrawLight(textureShader);

	if (alpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{

		glDisable(GL_BLEND);
	}
	UpdateModelUniformShaders(modelMatrix);
	UpdateProjectUniformShaders(cam->projection);
	UpdateViewUniformShaders(cam->view);
	/*UpdateProjectUniformShaders(projection);
	UpdateViewUniformShaders(view);*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexLength, vertex, GL_STATIC_DRAW); //set info to buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indexLength, index, GL_STATIC_DRAW); //set info to buffer
	glDrawElements(GL_TRIANGLES, indexLength, GL_UNSIGNED_INT, 0);
}

void Renderer::MaterialDraw(float* vertex, int vertexLength, unsigned int* index, int indexLength, glm::mat4 modelMatrix, bool alpha,
	glm::vec4 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) 
{
	DefVertexMaterialAttribute();
	CallUniformShaders(materialShader);
	materialShader->ActiveProgram();
	DrawLight(textureShader);
	SetMaterial(materialShader, color, ambient, diffuse, specular, shininess);

	if (alpha) // TODO: clean pls
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{

		glDisable(GL_BLEND);
	}
	UpdateModelUniformShaders(modelMatrix);
	UpdateProjectUniformShaders(cam->projection);
	UpdateViewUniformShaders(cam->view);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexLength, vertex, GL_STATIC_DRAW); //set info to buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * indexLength, index, GL_STATIC_DRAW); //set info to buffer
	glDrawElements(GL_TRIANGLES, indexLength, GL_UNSIGNED_INT, 0);
}

void Renderer::CreateBuffers()
{
	glGenVertexArrays(1, &VAO); // first: Specifies the number of vertex array object
								// second: the generated vertex array object names are stored
	glGenBuffers(1, &VBO); //first: the number of buffer object
	glGenBuffers(1, &EBO); //second: an array in which the generated buffer object names are stored
}

void Renderer::BindBuffers()
{
	glBindVertexArray(VAO);// Specifies the name of the vertex array to bind
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //first: type buffer to bound
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //second: the buffer
}
void Renderer::DefVertexAttribute()
{
	//first: defPosMemoryShader(Layout),second:countData, three: TypeDataAttribute, four: Normalize
	//five: countDataForVertex * SizeOf(typeData), six:StartCount (void*)0) the first point in data to count
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); //activate first: defPosMemoryShader(Layout)

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Renderer::DefVertexSpriteAttribute()
{
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Renderer::DefVertexMaterialAttribute()
{
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Renderer::CallUniformShaders(Shader* shader)
{
	modelLoc = glGetUniformLocation(shader->GetProgram(), "model");//search the model in the shader
	projectLoc = glGetUniformLocation(shader->GetProgram(), "proj");//search the project in the shader
	viewLoc = glGetUniformLocation(shader->GetProgram(), "view");
}

void Renderer::UpdateModelUniformShaders(glm::mat4 modelMatrix)
{
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix)); //update model in the shader
}

void Renderer::UpdateProjectUniformShaders(glm::mat4 projectMatrix)
{
	//UpdateProjection();
	cam->UpdateProjection();
	glUniformMatrix4fv(projectLoc, 1, GL_FALSE, glm::value_ptr(projectMatrix)); //update project in the shader

}

void Renderer::UpdateViewUniformShaders(glm::mat4 viewMatrix)
{
	//UpdateView();
	cam->UpdateView();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix)); //update view in the shader


}
//void Renderer::UpdateView()
//{
//	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//}
//void Renderer::CameraMove(float x, float y, float z)
//{
//	cameraPos.x = x;
//	cameraPos.y = y;
//	cameraPos.z = z;
//	//w
//	//cameraPos += cameraSpeed * cameraFront;
//	//s
//	//cameraPos -= cameraSpeed * cameraFront;
//	//a
//	//cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//	//d
//	//cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//}


void Renderer::SetStaticRenderer(Renderer* newRef)
{
	myRef = newRef;
}
Renderer* Renderer::GetStaticRenderer()
{
	return myRef;
}

Camera* Renderer::GetCamera()
{
	return cam;
}

void Renderer::DrawLight(Shader* shader)
{
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "ambient.color"), 1, &Light::ambient[0]);
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "ambient.str"), Light::ambientStrength);
}

void Renderer::SetMaterial(Shader* shader, glm::vec4 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{

	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "material.color"), 1, &color[0]);
	glUniform3fv(glGetUniformLocation(shader->GetProgram(), "material.ambient"), 1, &ambient[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "material.diffuse"), 1, &diffuse[0]);
	glUniform4fv(glGetUniformLocation(shader->GetProgram(), "material.specular"), 1, &specular[0]);
	glUniform1f(glGetUniformLocation(shader->GetProgram(), "material.shininess"), shininess);
}