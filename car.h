#include <memory>
#include "box2d/box2d.h"
#include "draw.h"
#include <memory>

class Car
{
public:
	Car();
	void Step();

	std::shared_ptr<b2World> m_world;
	b2Body* m_groundBody;
	b2Body* m_car;
	b2Body* m_wheel1;
	b2Body* m_wheel2;

	float m_speed;
	b2WheelJoint* m_spring1;
	b2WheelJoint* m_spring2;
	DebugDraw m_debugDraw;

	float m_hertz;
};
