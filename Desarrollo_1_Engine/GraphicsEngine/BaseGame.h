#ifndef BASEGAME_H
#define BASEGAME_H
#include "renderer.h"
#include "window.h"
#include "Shape.h"
#include "Sprite.h"
#include "Export.h"
#include "Input.h"
#include "Material2D.h"

struct GLFWmonitor;
struct GLFWwindow;

class ENGINE_API BaseGame
{
protected:
	Window* window;
	Renderer* renderer;
private:
	void UpdateBegin();
	void UpdateEnd();
	void EngineEnd();
	Input* input;

public:
	BaseGame();
	~BaseGame();
	int EngineInit(int width, int height, const char* title);// GLFWmonitor* monitor, GLFWwindow* share);
	int WindowInit(int width, int height, const char* title);
	void RendererInit();

	bool Running();
	void UpdateEngine();
	void virtual Update() = 0;
	void virtual Start() = 0;
	void virtual Delete() = 0;

	bool GetKey(int keycode);

	void SetCameraPosition(float x, float y, float z);
	void CameraMove(CameraDirection direction, float speed);
	void CameraRotate(float speedX, float speedY);
	void SetCameraProjection(CameraProjection selection);
	CameraProjection GetCameraProjection();
	void CameraFollowObj(CameraType type, glm::vec3 objPosReference, float distanceFromReferene);

};
#endif
