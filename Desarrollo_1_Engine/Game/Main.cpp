#include "Game.h"

void main()
{
	Game* myGame = new Game();
	myGame->Play();
	if (myGame != nullptr) delete myGame;

	/*pixelShader = new Shader();
	pixelShader->CreateProgram("Res/Shaders/VertexShader.shader", "Res/Shaders/FragmentShader.shader");
	textureShader = new Shader();
	textureShader->CreateProgram("Res/Shaders/SpriteVertexShader.shader", "Res/Shaders/SpriteFragmentShader.shader");
	CallUniformShaders(textureShader);*/
}