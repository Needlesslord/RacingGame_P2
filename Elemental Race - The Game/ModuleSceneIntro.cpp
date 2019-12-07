#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// course
	//int n = 1;
	int num = 1;

	float CourseWater[180] = {
	5, 0, 0,
	-5, 0, 0,
	5, 0, 2,
	5, 0, -2,
	-5, 0, 2,
	-5, 0, -2

	};

	int n = 0;
	for (int i = 0; i < 18; i++) {
		if (n == 0) {
			posX = CourseWater[i];
			n++;
		}
		else if (n == 1) {
			posY = CourseWater[i];
			n++;
		}
		else if (n == 2) {
			posZ = CourseWater[i];
			n++;
		}
		if (n == 3) CreateObject_BorderCourse(posX, posY, posZ, num);
	}


	FromListToCreateObject();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

bool ModuleSceneIntro::CreateObject_BorderCourse(float posX, float posY, float posZ, int course) {
	bool ret = true;
	
	float massCube = 1;
	float sizeX = 1;
	float sizeY = 1;
	float sizeZ = 2;
	Cube c(sizeX, sizeY, sizeZ);
	c.SetPos(posX, posY, posZ);
	App->physics->AddBody(c, massCube);
	if (course == 1) { c.color = Blue; }			// The water course
	else if (course == 2) { c.color = Black; }		// The earth course
	else if (course == 3) { c.color = White; }		// The air course
	else { c.color = Red; }							// The fire course

	return ret;
}

bool ModuleSceneIntro::FromListToCreateObject() {
	bool ret = true;

	int size = 0;
	int course = 1;
	float list[3] = { 1, 2, 3};

	float posX = 0;
	float posY = 0;
	float posZ = 0;
	int n = 0;
	for (int i = 0; i < size; i++) {
		if (n == 0) {
			posX = list[i];
			n++;
		}
		else if (n == 1) {
			posY = list[i];
			n++;
		}
		else if (n == 2) {
			posZ = list[i];
			n++;
		}
		if (n == 3) CreateObject_BorderCourse(posX, posY, posZ, course);
	}

	return ret;
}


void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {}

