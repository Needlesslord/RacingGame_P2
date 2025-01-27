#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "PhysVehicle3D.h"
#include "Timer.h"


struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f
#define MAX_SPEED 200
#define MIN_SPEED -50

struct Checkpoint {

	float sizeX;
	float sizeY;
	float sizeZ;

	float posX_left;
	float posY_left;
	float posZ_left;

	float posX_right;
	float posY_right;
	float posZ_right;

	float posX_respawn_point;
	float posY_respawn_point;
	float posZ_respawn_point;
	
	int lap;
	bool activated;
	bool activated_last;
	int colour;			// 0 for red, not activated; 1 for orange, activated 1; 2 for yellow, activated 2; 3 for green, activated 3

	bool isStart = false;
	int num_checkpoint;

};

class ModulePlayer : public Module {
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	bool CleanUp();

public:
	VehicleInfo car;
	VehicleInfo bus;
	VehicleInfo bike;
	VehicleInfo truck;
	VehicleInfo trailer;
	VehicleInfo racingCar;
	VehicleInfo monsterTruck;
	VehicleInfo bigTruck;
	VehicleInfo bigTrailer;
	VehicleInfo mini;

	PhysVehicle3D* Player;
	PhysVehicle3D* Trailer;
	//STRUCT VEHICLE
	
	bool vehicleSelected;
	uint vehicleSelectedNum; // 0 -> car; 1 -> bus; 2 -> truck; 3 -> racingCar; 4 -> mosterTruck; 5 -> bigTruck; 6 -> mini; T -> add Trailer 
	bool trailerAdded;

	float turn;
	float acceleration;
	float brake;

	bool following_camera;
	vec3 dist_to_car;
	vec3 initialPosition;
	vec3 positionCheckpointActivated;

	//timer
	Timer			timer;
	bool			timerOn = false;
	bool			hasWon;
	bool			raceFinished = false;
	//music
	bool musicActivatedP;
	uint lockFx;
	uint winFx;
	uint loseFx;
	uint checkpointFx;
	uint turboFx;
	uint startCarFx;
	bool lockJustOnce;
	bool winJustOnce;
	bool loseJustOnce;
	bool startCarJustOnce;

};