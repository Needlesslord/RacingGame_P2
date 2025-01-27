#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "Primitive.h"
#include "Color.h"
#include "ModuleAudio.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), Player(NULL)
{
	turn = acceleration = brake = 0.0f;

	//CAMERA
	following_camera = false;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	// Bus properties (small) ----------------------------------------
	bus.chassis_size.Set(4, 5.5, 11.5);
	bus.chassis_offset.Set(0, 2.5, 0);
	bus.mass = 500.0f;
	bus.suspensionStiffness = 15.88f;
	bus.suspensionCompression = 0.83f;
	bus.suspensionDamping = 0.88f;
	bus.maxSuspensionTravelCm = 1000.0f;
	bus.frictionSlip = 50.5;
	bus.maxSuspensionForce = 6000.0f;
	bus.chasisColor = Purple;

	// Wheel properties (small) ---------------------------------------
	float bus_connection_height = 0.6f;
	float bus_wheel_radius = 0.8f;
	float bus_wheel_width = 0.65f;
	float bus_suspensionRestLength = 1.2f;

	// Don't change anything below this line (small) ------------------

	float bus_half_width = bus.chassis_size.x * 0.5f;
	float bus_half_length = bus.chassis_size.z * 0.5f;

	vec3 bus_direction(0, -1, 0);
	vec3 bus_axis(-1, 0, 0);

	bus.num_wheels = 6;
	bus.wheels = new Wheel[6];


	// FRONT-LEFT ------------------------
	bus.wheels[0].connection.Set(bus_half_width - 0.3f * bus_wheel_width, bus_connection_height, bus_half_length * 0.95f - bus_wheel_radius);
	bus.wheels[0].direction = bus_direction;
	bus.wheels[0].axis = bus_axis;
	bus.wheels[0].suspensionRestLength = bus_suspensionRestLength;
	bus.wheels[0].radius = bus_wheel_radius;
	bus.wheels[0].width = bus_wheel_width;
	bus.wheels[0].front = true;
	bus.wheels[0].drive = true;
	bus.wheels[0].brake = false;
	bus.wheels[0].steering = true;
	bus.wheels[0].color = Black;

	// FRONT-RIGHT ------------------------
	bus.wheels[1].connection.Set(-bus_half_width + 0.3f * bus_wheel_width, bus_connection_height, bus_half_length * 0.95f - bus_wheel_radius);
	bus.wheels[1].direction = bus_direction;
	bus.wheels[1].axis = bus_axis;
	bus.wheels[1].suspensionRestLength = bus_suspensionRestLength;
	bus.wheels[1].radius = bus_wheel_radius;
	bus.wheels[1].width = bus_wheel_width;
	bus.wheels[1].front = true;
	bus.wheels[1].drive = true;
	bus.wheels[1].brake = false;
	bus.wheels[1].steering = true;
	bus.wheels[1].color = Black;

	// REAR-LEFT ------------------------
	bus.wheels[2].connection.Set(bus_half_width - 0.3f * bus_wheel_width - bus_wheel_width, bus_connection_height, -bus_half_length * 0.80f + bus_wheel_radius);
	bus.wheels[2].direction = bus_direction;
	bus.wheels[2].axis = bus_axis;
	bus.wheels[2].suspensionRestLength = bus_suspensionRestLength;
	bus.wheels[2].radius = bus_wheel_radius;
	bus.wheels[2].width = bus_wheel_width;
	bus.wheels[2].front = false;
	bus.wheels[2].drive = false;
	bus.wheels[2].brake = true;
	bus.wheels[2].steering = false;
	bus.wheels[2].color = Black;

	// REAR-RIGHT ------------------------
	bus.wheels[3].connection.Set(-bus_half_width + 0.3f * bus_wheel_width + bus_wheel_width, bus_connection_height, -bus_half_length * 0.80f + bus_wheel_radius);
	bus.wheels[3].direction = bus_direction;
	bus.wheels[3].axis = bus_axis;
	bus.wheels[3].suspensionRestLength = bus_suspensionRestLength;
	bus.wheels[3].radius = bus_wheel_radius;
	bus.wheels[3].width = bus_wheel_width;
	bus.wheels[3].front = false;
	bus.wheels[3].drive = false;
	bus.wheels[3].brake = true;
	bus.wheels[3].steering = false;
	bus.wheels[3].color = Black;

	// BACK-MID-LEFT ------------------------
	bus.wheels[4].connection.Set(bus_half_width, bus_connection_height, -bus_half_length * 0.80f + bus_wheel_radius);
	bus.wheels[4].direction = bus_direction;
	bus.wheels[4].axis = bus_axis;
	bus.wheels[4].suspensionRestLength = bus_suspensionRestLength;
	bus.wheels[4].radius = bus_wheel_radius;
	bus.wheels[4].width = bus_wheel_width;
	bus.wheels[4].front = false;
	bus.wheels[4].drive = false;
	bus.wheels[4].brake = true;
	bus.wheels[4].steering = false;
	bus.wheels[4].color = Black;

	// BACK-MID-RIGHT ------------------------
	bus.wheels[5].connection.Set(-bus_half_width, bus_connection_height, -bus_half_length * 0.80f + bus_wheel_radius);
	bus.wheels[5].direction = bus_direction;
	bus.wheels[5].axis = bus_axis;
	bus.wheels[5].suspensionRestLength = bus_suspensionRestLength;
	bus.wheels[5].radius = bus_wheel_radius;
	bus.wheels[5].width = bus_wheel_width;
	bus.wheels[5].front = false;
	bus.wheels[5].drive = false;
	bus.wheels[5].brake = true;
	bus.wheels[5].steering = false;
	bus.wheels[5].color = Black;
	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Truck properties ----------------------------------------

	truck.chassis_size.Set(6, 8, 24);
	truck.chassis_offset.Set(0, 4.5, 0);
	truck.mass = 1000.0f;
	truck.suspensionStiffness = 15.88f;
	truck.suspensionCompression = 0.83f;
	truck.suspensionDamping = 0.88f;
	truck.maxSuspensionTravelCm = 1000.0f;
	truck.frictionSlip = 50.5;
	truck.maxSuspensionForce = 6000.0f;
	truck.chasisColor = White;

	// Wheel properties ---------------------------------------
	float truck_connection_height = 0.8f;
	float truck_wheel_radius = 1.2f;
	float truck_wheel_width = 1.2f;
	float truck_suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float truck_half_width = truck.chassis_size.x * 0.5f;
	float truck_half_length = truck.chassis_size.z * 0.5f;

	vec3 truck_direction(0, -1, 0);
	vec3 truck_axis(-1, 0, 0);

	truck.num_wheels = 12;
	truck.wheels = new Wheel[12];

	// FRONT-LEFT ------------------------
	truck.wheels[0].connection.Set(truck_half_width - 0.6f * truck_wheel_width, truck_connection_height, truck_half_length * 0.95f - truck_wheel_radius);
	truck.wheels[0].direction = truck_direction;
	truck.wheels[0].axis = truck_axis;
	truck.wheels[0].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[0].radius = truck_wheel_radius;
	truck.wheels[0].width = truck_wheel_width;
	truck.wheels[0].front = true;
	truck.wheels[0].drive = true;
	truck.wheels[0].brake = false;
	truck.wheels[0].steering = true;
	truck.wheels[0].color = Black;

	// FRONT-RIGHT ------------------------
	truck.wheels[1].connection.Set(-truck_half_width + 0.6f * truck_wheel_width, truck_connection_height, truck_half_length * 0.95f - truck_wheel_radius);
	truck.wheels[1].direction = truck_direction;
	truck.wheels[1].axis = truck_axis;
	truck.wheels[1].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[1].radius = truck_wheel_radius;
	truck.wheels[1].width = truck_wheel_width;
	truck.wheels[1].front = true;
	truck.wheels[1].drive = true;
	truck.wheels[1].brake = false;
	truck.wheels[1].steering = true;
	truck.wheels[1].color = Black;



	// FRONT-BACK-LEFT ------------------------
	truck.wheels[2].connection.Set(truck_half_width - 0.6f * truck_wheel_width, truck_connection_height, truck_half_length * 0.20f + truck_wheel_radius);
	truck.wheels[2].direction = truck_direction;
	truck.wheels[2].axis = truck_axis;
	truck.wheels[2].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[2].radius = truck_wheel_radius;
	truck.wheels[2].width = truck_wheel_width;
	truck.wheels[2].front = false;
	truck.wheels[2].drive = false;
	truck.wheels[2].brake = true;
	truck.wheels[2].steering = false;
	truck.wheels[2].color = Black;

	// FRONT-BACK-RIGHT ------------------------
	truck.wheels[3].connection.Set(-truck_half_width + 0.6f * truck_wheel_width, truck_connection_height, truck_half_length * 0.20f + truck_wheel_radius);
	truck.wheels[3].direction = truck_direction;
	truck.wheels[3].axis = truck_axis;
	truck.wheels[3].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[3].radius = truck_wheel_radius;
	truck.wheels[3].width = truck_wheel_width;
	truck.wheels[3].front = false;
	truck.wheels[3].drive = false;
	truck.wheels[3].brake = true;
	truck.wheels[3].steering = false;
	truck.wheels[3].color = Black;

//

	// REAR-LEFT ------------------------
	truck.wheels[4].connection.Set(truck_half_width - 0.3f * truck_wheel_width - truck_wheel_width, truck_connection_height, -truck_half_length * 0.65f + truck_wheel_radius);
	truck.wheels[4].direction = truck_direction;
	truck.wheels[4].axis = truck_axis;
	truck.wheels[4].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[4].radius = truck_wheel_radius;
	truck.wheels[4].width = truck_wheel_width;
	truck.wheels[4].front = false;
	truck.wheels[4].drive = false;
	truck.wheels[4].brake = true;
	truck.wheels[4].steering = false;
	truck.wheels[4].color = Black;

	// REAR-RIGHT ------------------------
	truck.wheels[5].connection.Set(-truck_half_width + 0.3f * truck_wheel_width + truck_wheel_width, truck_connection_height, -truck_half_length * 0.65f + truck_wheel_radius);
	truck.wheels[5].direction = truck_direction;
	truck.wheels[5].axis = truck_axis;
	truck.wheels[5].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[5].radius = truck_wheel_radius;
	truck.wheels[5].width = truck_wheel_width;
	truck.wheels[5].front = false;
	truck.wheels[5].drive = false;
	truck.wheels[5].brake = true;
	truck.wheels[5].steering = false;
	truck.wheels[5].color = Black;



	// BACK-FRONT-MID-LEFT ------------------------
	truck.wheels[6].connection.Set(truck_half_width, truck_connection_height, -truck_half_length * 0.65f + truck_wheel_radius);
	truck.wheels[6].direction = truck_direction;
	truck.wheels[6].axis = truck_axis;
	truck.wheels[6].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[6].radius = truck_wheel_radius;
	truck.wheels[6].width = truck_wheel_width;
	truck.wheels[6].front = false;
	truck.wheels[6].drive = false;
	truck.wheels[6].brake = true;
	truck.wheels[6].steering = false;
	truck.wheels[6].color = Black;

	// BACK-FRONT-MID-RIGHT ------------------------
	truck.wheels[7].connection.Set(-truck_half_width, truck_connection_height, -truck_half_length * 0.65f + truck_wheel_radius);
	truck.wheels[7].direction = truck_direction;
	truck.wheels[7].axis = truck_axis;
	truck.wheels[7].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[7].radius = truck_wheel_radius;
	truck.wheels[7].width = truck_wheel_width;
	truck.wheels[7].front = false;
	truck.wheels[7].drive = false;
	truck.wheels[7].brake = true;
	truck.wheels[7].steering = false;
	truck.wheels[7].color = Black;



	// BACK-BACK-LEFT ------------------------
	truck.wheels[8].connection.Set(truck_half_width - 0.3f * truck_wheel_width - truck_wheel_width, truck_connection_height, -truck_half_length * 0.90f + truck_wheel_radius);
	truck.wheels[8].direction = truck_direction;
	truck.wheels[8].axis = truck_axis;
	truck.wheels[8].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[8].radius = truck_wheel_radius;
	truck.wheels[8].width = truck_wheel_width;
	truck.wheels[8].front = false;
	truck.wheels[8].drive = false;
	truck.wheels[8].brake = true;
	truck.wheels[8].steering = false;
	truck.wheels[8].color = Black;

	// BACK-BACK-RIGHT ------------------------
	truck.wheels[9].connection.Set(-truck_half_width + 0.3f * truck_wheel_width + truck_wheel_width, truck_connection_height, -truck_half_length * 0.90f + truck_wheel_radius);
	truck.wheels[9].direction = truck_direction;
	truck.wheels[9].axis = truck_axis;
	truck.wheels[9].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[9].radius = truck_wheel_radius;
	truck.wheels[9].width = truck_wheel_width;
	truck.wheels[9].front = false;
	truck.wheels[9].drive = false;
	truck.wheels[9].brake = true;
	truck.wheels[9].steering = false;
	truck.wheels[9].color = Black;



	// BACK-BACK-MID-LEFT ------------------------
	truck.wheels[10].connection.Set(truck_half_width, truck_connection_height, -truck_half_length * 0.90f + truck_wheel_radius);
	truck.wheels[10].direction = truck_direction;
	truck.wheels[10].axis = truck_axis;
	truck.wheels[10].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[10].radius = truck_wheel_radius;
	truck.wheels[10].width = truck_wheel_width;
	truck.wheels[10].front = false;
	truck.wheels[10].drive = false;
	truck.wheels[10].brake = true;
	truck.wheels[10].steering = false;
	truck.wheels[10].color = Black;

	// BACK-BACK-MID-RIGHT ------------------------
	truck.wheels[11].connection.Set(-truck_half_width, truck_connection_height, -truck_half_length * 0.90f + truck_wheel_radius);
	truck.wheels[11].direction = truck_direction;
	truck.wheels[11].axis = truck_axis;
	truck.wheels[11].suspensionRestLength = truck_suspensionRestLength;
	truck.wheels[11].radius = truck_wheel_radius;
	truck.wheels[11].width = truck_wheel_width;
	truck.wheels[11].front = false;
	truck.wheels[11].drive = false;
	truck.wheels[11].brake = true;
	truck.wheels[11].steering = false;
	truck.wheels[11].color = Black;


	// Trailer properties ----------------------------------------
	trailer.chassis_size.Set(5, 3, 7);
	trailer.chassis_offset.Set(0, 2.25f, 0);
	trailer.mass = 200.0f;
	trailer.suspensionStiffness = 5.80f;
	trailer.suspensionCompression = 0.63f;
	trailer.suspensionDamping = 0.68f;
	trailer.maxSuspensionTravelCm = 800.0f;
	trailer.frictionSlip = 30.5;
	trailer.maxSuspensionForce = 4000.0f;
	trailer.chasisColor = Green;

	// Wheel properties ---------------------------------------
	float trailer_connection_height = 0.8f;
	float trailer_wheel_radius = 1.2f;
	float trailer_wheel_width = 0.5f;
	float trailer_suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float trailer_half_width = trailer.chassis_size.x * 0.5f;
	float trailer_half_length = trailer.chassis_size.z * 0.5f;

	vec3 trailer_direction(0, -1, 0);
	vec3 trailer_axis(-1, 0, 0);

	trailer.num_wheels = 4;
	trailer.wheels = new Wheel[4];
	
	// FRONT-LEFT ------------------------
	trailer.wheels[0].connection.Set(trailer_half_width - 0.3f * trailer_wheel_width, trailer_connection_height, trailer_half_length * 0.95f - trailer_wheel_radius);
	trailer.wheels[0].direction = trailer_direction;
	trailer.wheels[0].axis = trailer_axis;
	trailer.wheels[0].suspensionRestLength = trailer_suspensionRestLength;
	trailer.wheels[0].radius = trailer_wheel_radius;
	trailer.wheels[0].width = trailer_wheel_width;
	trailer.wheels[0].front = true;
	trailer.wheels[0].drive = false;
	trailer.wheels[0].brake = false;
	trailer.wheels[0].steering = false;
	trailer.wheels[0].color = Black;

	// FRONT-RIGHT ------------------------
	trailer.wheels[1].connection.Set(-trailer_half_width + 0.3f * trailer_wheel_width, trailer_connection_height, trailer_half_length * 0.95f - trailer_wheel_radius);
	trailer.wheels[1].direction = trailer_direction;
	trailer.wheels[1].axis = trailer_axis;
	trailer.wheels[1].suspensionRestLength = trailer_suspensionRestLength;
	trailer.wheels[1].radius = trailer_wheel_radius;
	trailer.wheels[1].width = trailer_wheel_width;
	trailer.wheels[1].front = true;
	trailer.wheels[1].drive = false;
	trailer.wheels[1].brake = false;
	trailer.wheels[1].steering = false;
	trailer.wheels[1].color = Black;

	// REAR-LEFT ------------------------
	trailer.wheels[2].connection.Set(trailer_half_width - 0.3f * trailer_wheel_width, trailer_connection_height, -trailer_half_length * 0.95f + trailer_wheel_radius);
	trailer.wheels[2].direction = trailer_direction;
	trailer.wheels[2].axis = trailer_axis;
	trailer.wheels[2].suspensionRestLength = trailer_suspensionRestLength;
	trailer.wheels[2].radius = trailer_wheel_radius;
	trailer.wheels[2].width = trailer_wheel_width;
	trailer.wheels[2].front = false;
	trailer.wheels[2].drive = false;
	trailer.wheels[2].brake = false;
	trailer.wheels[2].steering = false;
	trailer.wheels[2].color = Black;

	// REAR-RIGHT ------------------------
	trailer.wheels[3].connection.Set(-trailer_half_width + 0.3f * trailer_wheel_width, trailer_connection_height, -trailer_half_length * 0.95f + trailer_wheel_radius);
	trailer.wheels[3].direction = trailer_direction;
	trailer.wheels[3].axis = trailer_axis;
	trailer.wheels[3].suspensionRestLength = trailer_suspensionRestLength;
	trailer.wheels[3].radius = trailer_wheel_radius;
	trailer.wheels[3].width = trailer_wheel_width;
	trailer.wheels[3].front = false;
	trailer.wheels[3].drive = false;
	trailer.wheels[3].brake = false;
	trailer.wheels[3].steering = false;
	trailer.wheels[3].color = Black;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Car properties ----------------------------------------
	car.chassis_size.Set(2, 2, 4);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;
	car.chasisColor = White;

	// Wheel properties ---------------------------------------
	float car_connection_height = 1.2f;
	float car_wheel_radius = 0.6f;
	float car_wheel_width = 0.5f;
	float car_suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float car_half_width = car.chassis_size.x*0.5f;
	float car_half_length = car.chassis_size.z*0.5f;

	vec3 car_direction(0, -1, 0);
	vec3 car_axis(-1, 0, 0);

	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(car_half_width - 0.3f * car_wheel_width, car_connection_height, car_half_length - car_wheel_radius);
	car.wheels[0].direction = car_direction;
	car.wheels[0].axis = car_axis;
	car.wheels[0].suspensionRestLength = car_suspensionRestLength;
	car.wheels[0].radius = car_wheel_radius;
	car.wheels[0].width = car_wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;
	car.wheels->color = Blue;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-car_half_width + 0.3f * car_wheel_width, car_connection_height, car_half_length - car_wheel_radius);
	car.wheels[1].direction = car_direction;
	car.wheels[1].axis = car_axis;
	car.wheels[1].suspensionRestLength = car_suspensionRestLength;
	car.wheels[1].radius = car_wheel_radius;
	car.wheels[1].width = car_wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(car_half_width - 0.3f * car_wheel_width, car_connection_height, -car_half_length + car_wheel_radius);
	car.wheels[2].direction = car_direction;
	car.wheels[2].axis = car_axis;
	car.wheels[2].suspensionRestLength = car_suspensionRestLength;
	car.wheels[2].radius = car_wheel_radius;
	car.wheels[2].width = car_wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-car_half_width + 0.3f * car_wheel_width, car_connection_height, -car_half_length + car_wheel_radius);
	car.wheels[3].direction = car_direction;
	car.wheels[3].axis = car_axis;
	car.wheels[3].suspensionRestLength = car_suspensionRestLength;
	car.wheels[3].radius = car_wheel_radius;
	car.wheels[3].width = car_wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// MonsterTruck properties ----------------------------------------
	monsterTruck.chassis_size.Set(6, 5, 7);
	monsterTruck.chassis_offset.Set(0, 4.5, 0);
	monsterTruck.mass = 1500.0f;
	monsterTruck.suspensionStiffness = 15.88f;
	monsterTruck.suspensionCompression = 0.83f;
	monsterTruck.suspensionDamping = 0.88f;
	monsterTruck.maxSuspensionTravelCm = 1000.0f;
	monsterTruck.frictionSlip = 50.5;
	monsterTruck.maxSuspensionForce = 6000.0f;
	monsterTruck.chasisColor = Red;

	// Wheel properties ---------------------------------------
	float monsterTruck_connection_height = 3.2f;
	float monsterTruck_wheel_radius = 3.5f;
	float monsterTruck_wheel_width = 3.5f;
	float monsterTruck_suspensionRestLength = 3.5f;

	// Don't change anything below this line ------------------

	float monsterTruck_half_width = monsterTruck.chassis_size.x*0.5f;
	float monsterTruck_half_length = monsterTruck.chassis_size.z*0.5f;

	vec3 monsterTruck_direction(0, -1, 0);
	vec3 monsterTruck_axis(-1, 0, 0);

	monsterTruck.num_wheels = 4;
	monsterTruck.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	monsterTruck.wheels[0].connection.Set(monsterTruck_half_width + 0.1f * monsterTruck_wheel_width, monsterTruck_connection_height, monsterTruck_half_length + monsterTruck_wheel_radius * 0.2f);
	monsterTruck.wheels[0].direction = monsterTruck_direction;
	monsterTruck.wheels[0].axis = monsterTruck_axis;
	monsterTruck.wheels[0].suspensionRestLength = monsterTruck_suspensionRestLength;
	monsterTruck.wheels[0].radius = monsterTruck_wheel_radius;
	monsterTruck.wheels[0].width = monsterTruck_wheel_width;
	monsterTruck.wheels[0].front = true;
	monsterTruck.wheels[0].drive = true;
	monsterTruck.wheels[0].brake = true;
	monsterTruck.wheels[0].steering = true;
	monsterTruck.wheels->color = Black;

	// FRONT-RIGHT ------------------------
	monsterTruck.wheels[1].connection.Set(-monsterTruck_half_width - 0.1f * monsterTruck_wheel_width, monsterTruck_connection_height, monsterTruck_half_length + monsterTruck_wheel_radius * 0.2f);
	monsterTruck.wheels[1].direction = monsterTruck_direction;
	monsterTruck.wheels[1].axis = monsterTruck_axis;
	monsterTruck.wheels[1].suspensionRestLength = monsterTruck_suspensionRestLength;
	monsterTruck.wheels[1].radius = monsterTruck_wheel_radius;
	monsterTruck.wheels[1].width = monsterTruck_wheel_width;
	monsterTruck.wheels[1].front = true;
	monsterTruck.wheels[1].drive = true;
	monsterTruck.wheels[1].brake = true;
	monsterTruck.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	monsterTruck.wheels[2].connection.Set(monsterTruck_half_width + 0.1f * monsterTruck_wheel_width, monsterTruck_connection_height, -monsterTruck_half_length - monsterTruck_wheel_radius * 0.2);
	monsterTruck.wheels[2].direction = monsterTruck_direction;
	monsterTruck.wheels[2].axis = monsterTruck_axis;
	monsterTruck.wheels[2].suspensionRestLength = monsterTruck_suspensionRestLength;
	monsterTruck.wheels[2].radius = monsterTruck_wheel_radius;
	monsterTruck.wheels[2].width = monsterTruck_wheel_width;
	monsterTruck.wheels[2].front = false;
	monsterTruck.wheels[2].drive = true;
	monsterTruck.wheels[2].brake = true;
	monsterTruck.wheels[2].steeringOposite = true;

	// REAR-RIGHT ------------------------
	monsterTruck.wheels[3].connection.Set(-monsterTruck_half_width - 0.1f * monsterTruck_wheel_width, monsterTruck_connection_height, -monsterTruck_half_length - monsterTruck_wheel_radius * 0.2);
	monsterTruck.wheels[3].direction = monsterTruck_direction;
	monsterTruck.wheels[3].axis = monsterTruck_axis;
	monsterTruck.wheels[3].suspensionRestLength = monsterTruck_suspensionRestLength;
	monsterTruck.wheels[3].radius = monsterTruck_wheel_radius;
	monsterTruck.wheels[3].width = monsterTruck_wheel_width;
	monsterTruck.wheels[3].front = false;
	monsterTruck.wheels[3].drive = true;
	monsterTruck.wheels[3].brake = true;
	monsterTruck.wheels[3].steeringOposite = true;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Mini properties ----------------------------------------
	mini.chassis_size.Set(2, 1, 2);
	mini.chassis_offset.Set(0, 1.0, 0);
	mini.mass = 300.0f;
	mini.suspensionStiffness = 15.88f;
	mini.suspensionCompression = 0.83f;
	mini.suspensionDamping = 0.88f;
	mini.maxSuspensionTravelCm = 1000.0f;
	mini.frictionSlip = 50.5;
	mini.maxSuspensionForce = 6000.0f;
	mini.chasisColor = Yellow_T;

	// Wheel properties ---------------------------------------
	float mini_connection_height = 1.2f;
	float mini_wheel_radius = 0.4f;
	float mini_wheel_width = 0.3f;
	float mini_suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float mini_half_width = mini.chassis_size.x*0.5f;
	float mini_half_length = mini.chassis_size.z*0.5f;

	vec3 mini_direction(0, -1, 0);
	vec3 mini_axis(-1, 0, 0);

	mini.num_wheels = 4;
	mini.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	mini.wheels[0].connection.Set(mini_half_width - 0.3f * mini_wheel_width, mini_connection_height, mini_half_length - mini_wheel_radius);
	mini.wheels[0].direction = mini_direction;
	mini.wheels[0].axis = mini_axis;
	mini.wheels[0].suspensionRestLength = mini_suspensionRestLength;
	mini.wheels[0].radius = mini_wheel_radius;
	mini.wheels[0].width = mini_wheel_width;
	mini.wheels[0].front = true;
	mini.wheels[0].drive = true;
	mini.wheels[0].brake = false;
	mini.wheels[0].steering = true;
	mini.wheels->color = Black;

	// FRONT-RIGHT ------------------------
	mini.wheels[1].connection.Set(-mini_half_width + 0.3f * mini_wheel_width, mini_connection_height, mini_half_length - mini_wheel_radius);
	mini.wheels[1].direction = mini_direction;
	mini.wheels[1].axis = mini_axis;
	mini.wheels[1].suspensionRestLength = mini_suspensionRestLength;
	mini.wheels[1].radius = mini_wheel_radius;
	mini.wheels[1].width = mini_wheel_width;
	mini.wheels[1].front = true;
	mini.wheels[1].drive = true;
	mini.wheels[1].brake = false;
	mini.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	mini.wheels[2].connection.Set(mini_half_width - 0.3f * mini_wheel_width, mini_connection_height, -mini_half_length + mini_wheel_radius);
	mini.wheels[2].direction = mini_direction;
	mini.wheels[2].axis = mini_axis;
	mini.wheels[2].suspensionRestLength = mini_suspensionRestLength;
	mini.wheels[2].radius = mini_wheel_radius;
	mini.wheels[2].width = mini_wheel_width;
	mini.wheels[2].front = false;
	mini.wheels[2].drive = false;
	mini.wheels[2].brake = true;
	mini.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	mini.wheels[3].connection.Set(-mini_half_width + 0.3f * mini_wheel_width, mini_connection_height, -mini_half_length + mini_wheel_radius);
	mini.wheels[3].direction = mini_direction;
	mini.wheels[3].axis = mini_axis;
	mini.wheels[3].suspensionRestLength = mini_suspensionRestLength;
	mini.wheels[3].radius = mini_wheel_radius;
	mini.wheels[3].width = mini_wheel_width;
	mini.wheels[3].front = false;
	mini.wheels[3].drive = false;
	mini.wheels[3].brake = true;
	mini.wheels[3].steering = false;

	hasWon = false;
	vehicleSelected = false;
	trailerAdded = false;
	initialPosition.Set(-72.5, 1, -5);
	//timer
	timerOn = false;
	//title values
	int laps = 0;
	//music
	lockFx = App->audio->LoadFx("audio/fx/checkpoint_2.wav");
	lockJustOnce = true;
	startCarJustOnce = true;
	winJustOnce = true;
	loseJustOnce = true;
	musicActivatedP = true;
	winFx = App->audio->LoadFx("audio/fx/win.wav");
	loseFx = App->audio->LoadFx("audio/fx/lose.wav");
	checkpointFx = App->audio->LoadFx("audio/fx/checkpoint.wav");
	turboFx = App->audio->LoadFx("audio/fx/turbo.wav");
	startCarFx = App->audio->LoadFx("audio/fx/startCar.wav");


	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	//CAMERA --------------------------------------------------------------------
	{
		// camera set on the beggining of the course, not set on vehicle
		if (!vehicleSelected) following_camera = false;

		// vehicle selected, camera set on vehicle
		else {

			// selected vehicle, following vehicle from now on 
			following_camera = true;

			//------------------- CAR, vehicleSelectedNum == 0 -------------------//
			if (vehicleSelectedNum == 0) {
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				// setting camera on vehicle
				float speed_cam = 0.05f;
				if (trailerAdded) dist_to_car = { -30.0f, 20.0f, -30.0f };
				else dist_to_car = { -25.0f, 15.0f, -25.0f };
				vec3 new_camera_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 camera_disp_vec = new_camera_position - App->camera->Position;
				App->camera->Look(App->camera->Position + (speed_cam * camera_disp_vec), p);
			}

			//------------------- BUS, vehicleSelectedNum == 1 -------------------//
			if (vehicleSelectedNum == 1) {
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				// setting camera on vehicle
				float speed_cam = 0.05f;
				if (trailerAdded) dist_to_car = { -70.0f, 40.0f, -70.0f };
				else dist_to_car = { -60.0f, 35.0f, -60.0f };
				vec3 new_camera_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 camera_disp_vec = new_camera_position - App->camera->Position;
				App->camera->Look(App->camera->Position + (speed_cam * camera_disp_vec), p);
			}
			//------------------- TRUCK, vehicleSelectedNum == 2 -------------------//
			if (vehicleSelectedNum == 2) {
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				// setting camera on vehicle
				float speed_cam = 0.05f;
				if (trailerAdded) dist_to_car = { -85.0f, 45.0f, -85.0f };
				else dist_to_car = { -75.0f, 40.0f, -75.0f };
				vec3 new_camera_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 camera_disp_vec = new_camera_position - App->camera->Position;
				App->camera->Look(App->camera->Position + (speed_cam * camera_disp_vec), p);
			}
			//------------------- MONSTERTRUCK, vehicleSelectedNum == 3 -------------------//
			if (vehicleSelectedNum == 3) {
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				// setting camera on vehicle
				float speed_cam = 0.05f;
				if (trailerAdded) dist_to_car = { -85.0f, 45.0f, -85.0f };
				else dist_to_car = { -75.0f, 40.0f, -75.0f };
				vec3 new_camera_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 camera_disp_vec = new_camera_position - App->camera->Position;
				App->camera->Look(App->camera->Position + (speed_cam * camera_disp_vec), p);
			}
			//------------------- MINI, vehicleSelectedNum == 4 -------------------//
			if (vehicleSelectedNum == 4) {
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				// setting camera on vehicle
				float speed_cam = 0.05f;
				if (trailerAdded) dist_to_car = { -30.0f, 20.0f, -30.0f };
				else dist_to_car = { -15.0f, 10.0f, -15.0f };
				vec3 new_camera_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 camera_disp_vec = new_camera_position - App->camera->Position;
				App->camera->Look(App->camera->Position + (speed_cam * camera_disp_vec), p);
			}
		}

		//music activated
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) musicActivatedP = !musicActivatedP;
		
		// following vehicle
		if (following_camera)
		{

			//------------------- CAR, vehicleSelectedNum == 0 -------------------//
			if (vehicleSelectedNum == 0) {
				float speed_cam = 0.09;
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				dist_to_car = { -8.0f, 5.0f, -8.0f };
				vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 speed_camera = camera_new_position - App->camera->Position;
				App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);
			}
			//------------------- BUS, vehicleSelectedNum == 1 -------------------//
			if (vehicleSelectedNum == 1) {
				float speed_cam = 0.09;
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				dist_to_car = { -8.0f, 5.0f, -8.0f };
				vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 speed_camera = camera_new_position - App->camera->Position;
				App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);
			}
			//------------------- TRUCK, vehicleSelectedNum == 2 -------------------//
			if (vehicleSelectedNum == 2) {
				float speed_cam = 0.09;
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				dist_to_car = { -8.0f, 5.0f, -8.0f };
				vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 speed_camera = camera_new_position - App->camera->Position;

				App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);
			}
			//------------------- MONSTERTRUCK, vehicleSelectedNum == 3 -------------------//
			if (vehicleSelectedNum == 3) {
				float speed_cam = 0.09;
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				dist_to_car = { -8.0f, 5.0f, -8.0f };
				vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 speed_camera = camera_new_position - App->camera->Position;
				App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);
			}
			//------------------- MINI, vehicleSelectedNum == 4 -------------------//
			if (vehicleSelectedNum == 4) {
				float speed_cam = 0.09;
				vec3 p = Player->GetPos();
				vec3 f = Player->GetForwardVector();
				dist_to_car = { -8.0f, 5.0f, -8.0f };
				vec3 camera_new_position = { p.x + (f.x * dist_to_car.x), p.y + f.y + dist_to_car.y, p.z + (f.z * dist_to_car.z) };
				vec3 speed_camera = camera_new_position - App->camera->Position;
				App->camera->Look(App->camera->Position + (speed_cam * speed_camera), p);
			}
		}

		// Activate/deactivate camera on car.
		if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		{
			following_camera = !following_camera;
		}
	}
	//CAMERA END --------------------------------------------------------------------

	//MUSIC -------------------------------------------------------------------------

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && musicActivatedP)
	{
		App->audio->PlayFx(turboFx);
	}

	//MUSIC END ---------------------------------------------------------------------	
	
	if (App->scene_intro->currentStep == SelectVehicle) {
		if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) {
			Player = App->physics->AddVehicle(car);
			Player->SetPos(-72.5, 1, -5);
			vehicleSelected = true;
			vehicleSelectedNum = 0;
			App->scene_intro->currentStep = LockVehicle;


			if (lockJustOnce && musicActivatedP) {
				App->audio->PlayFx(lockFx);
				lockJustOnce = !lockJustOnce;
			}

		}
		else if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) { //BUS
			Player = App->physics->AddVehicle(bus);
			Player->SetPos(-72.5, 1, -5);
			vehicleSelected = true;
			vehicleSelectedNum = 1;
			App->scene_intro->currentStep = LockVehicle;


			if (lockJustOnce && musicActivatedP) {
				App->audio->PlayFx(lockFx);
				lockJustOnce = !lockJustOnce;
			}

		}
		else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) { //
			Player = App->physics->AddVehicle(truck);
			Player->SetPos(-72.5, 1, -5);
			vehicleSelected = true;
			vehicleSelectedNum = 2;
			App->scene_intro->currentStep = LockVehicle;


			if (lockJustOnce && musicActivatedP) {
				App->audio->PlayFx(lockFx);
				lockJustOnce = !lockJustOnce;
			}

		}
		else if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) {
			Player = App->physics->AddVehicle(monsterTruck);
			Player->SetPos(-72.5, 1, -5);
			vehicleSelected = true;
			vehicleSelectedNum = 3;
			App->scene_intro->currentStep = LockVehicle;


			if (lockJustOnce && musicActivatedP) {
				App->audio->PlayFx(lockFx);
				lockJustOnce = !lockJustOnce;
			}

		}
		else if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN) {
			Player = App->physics->AddVehicle(mini);
			Player->SetPos(-72.5, 1, -5);
			vehicleSelected = true;
			vehicleSelectedNum = 4;
			App->scene_intro->currentStep = LockVehicle;


			if (lockJustOnce && musicActivatedP) {
				App->audio->PlayFx(lockFx);
				lockJustOnce = !lockJustOnce;
			}

		}
		char title[150];
		sprintf_s(title, "SELECT:               CAR(0)               BUS(1)               TRUCK(2)               MONSTERTRUCK(3)               MINI(4)");
		App->window->SetTitle(title);
	}

	else if (App->scene_intro->currentStep == LockVehicle) {
		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && vehicleSelectedNum == 0 && !trailerAdded) { // TRAILER TO CAR
			Trailer = App->physics->AddVehicle(trailer);
			Trailer->SetPos(-72.5, 2, -15);
			App->physics->AddConstraintP2P(*Player, *Trailer, { 0, 0, 0 }, { 0, 0, 10.0f });
			trailerAdded = true;
		}
		else if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && vehicleSelectedNum == 1 && !trailerAdded) { // TRAILER TO BUS
			Trailer = App->physics->AddVehicle(trailer);
			Trailer->SetPos(-72.5, 2, -20);
			App->physics->AddConstraintP2P(*Player, *Trailer, { 0, 0, 0 }, { 0, 0, 12.5f });
			trailerAdded = true;
		}
		else if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && vehicleSelectedNum == 2 && !trailerAdded) { // TRAILER TO TRUCK
			Trailer = App->physics->AddVehicle(trailer);
			Trailer->SetPos(-72.5, 2, -25);
			App->physics->AddConstraintP2P(*Player, *Trailer, { 0, 0, 0 }, { 0, 0, 17.5f });
			trailerAdded = true;
		}
		else if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && vehicleSelectedNum == 4 && !trailerAdded) { // TRAILER TO TRUCK
			Trailer = App->physics->AddVehicle(trailer);
			Trailer->SetPos(-72.5, 2, -7.5);
			App->physics->AddConstraintP2P(*Player, *Trailer, { 0, 0, 0 }, { 0, 0, 7.5f });
			trailerAdded = true;
		}
		else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
			App->scene_intro->currentStep = Running;
			timer.Start();
		}

		if (vehicleSelectedNum == 3) {
			char title2[50];
			sprintf_s(title2, "PRESS ENTER TO START");
			App->window->SetTitle(title2);
		}
		else {
			char titleToStart[50];
			sprintf_s(titleToStart, "PRESS T TO ADD A TRAILER OR PRESS ENTER TO START");
			App->window->SetTitle(titleToStart);
		}

		Player->Render();
		if (trailerAdded) Trailer->Render();
	}

	else if (App->scene_intro->currentStep == Running) {

		if (startCarJustOnce && musicActivatedP) {
			App->audio->PlayFx(startCarFx);
			startCarJustOnce = !startCarJustOnce;
		}

		turn = acceleration = brake = 0.0f;

		if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN) {
			if (App->scene_intro->checkpointActivated == -1)
				Player->SetPos(initialPosition.x, initialPosition.y, initialPosition.z);
			else Player->SetPos(App->scene_intro->Checkpoints[App->scene_intro->checkpointActivated]->GetPos().x, 
				App->scene_intro->Checkpoints[App->scene_intro->checkpointActivated]->GetPos().y, 
				App->scene_intro->Checkpoints[App->scene_intro->checkpointActivated]->GetPos().z);
		}

		if (vehicleSelectedNum == 0) { // CAR
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
				if (Player->GetKmh() < MAX_SPEED) acceleration = MAX_ACCELERATION;

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
				if (Player->GetKmh() > 3) brake = BRAKE_POWER;
				else {
					if (Player->GetKmh() > MIN_SPEED)
						acceleration = -BRAKE_POWER;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				if (turn < TURN_DEGREES)
					turn += TURN_DEGREES;
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				if (turn > -TURN_DEGREES)
					turn -= TURN_DEGREES;
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
				acceleration *= 4;
			}
		}

		else if (vehicleSelectedNum == 1) {	//BUS
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
				if (Player->GetKmh() < MAX_SPEED)
					acceleration = MAX_ACCELERATION / 2;
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
				if (Player->GetKmh() > 3) brake = BRAKE_POWER / 3;
				else {
					if (Player->GetKmh() > MIN_SPEED)
						acceleration = -BRAKE_POWER / 3;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				if (turn < TURN_DEGREES * 0.8)
					turn += TURN_DEGREES * 0.8;
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				if (turn > -TURN_DEGREES * 0.8)
					turn -= TURN_DEGREES * 0.8;
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
				acceleration *= 4;
			}
		}


		else if (vehicleSelectedNum == 2) {	//TRUCK
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
				if (Player->GetKmh() < MAX_SPEED)
					acceleration = MAX_ACCELERATION;
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
				if (Player->GetKmh() > 3) brake = BRAKE_POWER / 5;
				else {
					if (Player->GetKmh() > MIN_SPEED)
						acceleration = -BRAKE_POWER / 3;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				if (turn < TURN_DEGREES * 2.5) {
					turn += TURN_DEGREES * 2.5;
					acceleration *= 9;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				if (turn > -TURN_DEGREES * 2.5) {
					turn -= TURN_DEGREES * 2.5;
					acceleration *= 9;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
				acceleration *= 4;
			}
		}

		else if (vehicleSelectedNum == 3) {	// MONSTER TRUCK
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
				if (Player->GetKmh() < MAX_SPEED)
					acceleration = MAX_ACCELERATION;
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
				if (Player->GetKmh() > 3) brake = BRAKE_POWER;
				else {
					if (Player->GetKmh() > MIN_SPEED)
						acceleration = -BRAKE_POWER;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				if (turn < TURN_DEGREES) {
					turn += TURN_DEGREES;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				if (turn > -TURN_DEGREES) {
					turn -= TURN_DEGREES;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
				acceleration *= 4;
			}
		}

		else if (vehicleSelectedNum == 4) {	// MINI
			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
				if (Player->GetKmh() < MAX_SPEED)
					acceleration = MAX_ACCELERATION;
			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
				if (Player->GetKmh() > 3) brake = BRAKE_POWER;
				else {
					if (Player->GetKmh() > MIN_SPEED)
						acceleration = -BRAKE_POWER;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
				if (turn < TURN_DEGREES) {
					turn += TURN_DEGREES;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
				if (turn > -TURN_DEGREES) {
					turn -= TURN_DEGREES;
				}
			}

			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
				acceleration *= 4;
			}
		}
		Player->ApplyEngineForce(acceleration);
		Player->Turn(turn);
		Player->Brake(brake);
		Player->Render();
		if (trailerAdded) Trailer->Render();

		char title[40];		
		sprintf_s(title, "%.1f Km/h %d Seconds", Player->GetKmh(), 210 - timer.Read() / 1000);
		App->window->SetTitle(title);
	}


	else if (App->scene_intro->currentStep == GameOver) {
		timer.Stop();
		Player->Render();
		if (trailerAdded) Trailer->Render();
		char title[60];
		if (hasWon) {
			sprintf_s(title, "Congratulations! You won with %d seconds left", 210 - timer.Read() / 1000);

			if (winJustOnce && musicActivatedP) {
				App->audio->PlayFx(winFx);
				winJustOnce = !winJustOnce;
			}

		}
		else {
			sprintf_s(title, "You Lose :( Want to try again?");
			
			if (loseJustOnce && musicActivatedP) {
				App->audio->PlayFx(loseFx);
				loseJustOnce = !loseJustOnce;
			}

		}
		App->window->SetTitle(title);
	}

	if (timer.Read()/1000 > 210 && vehicleSelected) {
		App->scene_intro->currentStep = GameOver;
		timer.Stop();
	}

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) {

		if (vehicleSelectedNum == 0) { // CAR
			Player->SetPos(-72.5, 1, -5);
		}
		else if (vehicleSelectedNum == 1) { // BUS
			Player->SetPos(-72.5, 1, -5);
		}
		else if (vehicleSelectedNum == 2) { // TRUCK
			Player->SetPos(-72.5, 1, -5);
		}
		else if (vehicleSelectedNum == 3) { // MONSTERTRUCK
			Player->SetPos(-72.5, 1, -5);
		}
		else if (vehicleSelectedNum == 4) { // MINI
			Player->SetPos(-72.5, 1, -5);
		}

		if (trailerAdded) {
			if (vehicleSelectedNum == 0) { // TRAILER TO CAR
				Trailer->SetPos(-72.5, 2, -15);
			}
			else if (vehicleSelectedNum == 1) { // TRAILER TO BUS
				Trailer->SetPos(-72.5, 2, -20);
			}
			else if (vehicleSelectedNum == 2) { // TRAILER TO TRUCK
				Trailer->SetPos(-72.5, 2, -25);
			}
			else if (vehicleSelectedNum == 4) { // TRAILER TO MINI
				Trailer->SetPos(-72.5, 2, -7.5);
			}
		}

		App->scene_intro->lap = 0;

		for (int i = 0; i<2; i++) App->scene_intro->checkpoint_cubes[i]->color = Red;

		App->scene_intro->checkpointActivated = -1;
		App->scene_intro->checkpointToActivate = 0;

		App->scene_intro->currentStep = Running;
		timer.Start();

	}


	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2) {

}
