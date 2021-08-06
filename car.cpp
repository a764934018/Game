#include "car.h"
#include "draw.h"

Car::Car()
{		
	m_hertz = 60.0f;

	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	m_world = std::make_shared<b2World>(gravity);

	m_world->SetDebugDraw(&m_debugDraw);

	b2BodyDef bodyDef;
	m_groundBody = m_world->CreateBody(&bodyDef);

	m_speed = 50.0f;

	b2Body* ground = NULL;
	{
		b2BodyDef bd;
		ground = m_world->CreateBody(&bd);

		b2EdgeShape shape;

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 0.0f;
		fd.friction = 0.6f;

		shape.SetTwoSided(b2Vec2(-50.0f, 0.0f), b2Vec2(20.0f, 0.0f));
		ground->CreateFixture(&fd);

		std::vector<float> hs = {0.25f, 1.0f, 4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -2.0f, -1.25f, 0.0f};

		float x = 20.0f, y1 = 0.0f, dx = 5.0f;

		for (int32 i = 0; i < hs.size(); ++i)
		{
			float y2 = hs[i];
			shape.SetTwoSided(b2Vec2(x, y1), b2Vec2(x + dx, y2));
			ground->CreateFixture(&fd);
			y1 = y2;
			x += dx;
		}

		for (int32 i = 0; i < hs.size(); ++i)
		{
			float y2 = hs[i];
			shape.SetTwoSided(b2Vec2(x, y1), b2Vec2(x + dx, y2));
			ground->CreateFixture(&fd);
			y1 = y2;
			x += dx;
		}

		shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
		ground->CreateFixture(&fd);

		x += 80.0f;
		shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
		ground->CreateFixture(&fd);

		x += 40.0f;
		shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 10.0f, 5.0f));
		ground->CreateFixture(&fd);

		x += 20.0f;
		shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
		ground->CreateFixture(&fd);

		x += 40.0f;
		shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x, 0.0f));
		ground->CreateFixture(&fd);
	}

	// Teeter
	{
		b2BodyDef bd;
		bd.position.Set(140.0f, 1.0f);
		bd.type = b2_dynamicBody;
		b2Body* body = m_world->CreateBody(&bd);

		b2PolygonShape box;
		box.SetAsBox(10.0f, 0.25f);
		body->CreateFixture(&box, 1.0f);

		b2RevoluteJointDef jd;
		jd.Initialize(ground, body, body->GetPosition());
		jd.lowerAngle = -8.0f * b2_pi / 180.0f;
		jd.upperAngle = 8.0f * b2_pi / 180.0f;
		jd.enableLimit = true;
		m_world->CreateJoint(&jd);

		body->ApplyAngularImpulse(100.0f, true);
	}

	// Bridge
	{
		int32 N = 20;
		b2PolygonShape shape;
		shape.SetAsBox(1.0f, 0.125f);

		b2FixtureDef fd;
		fd.shape = &shape;
		fd.density = 1.0f;
		fd.friction = 0.6f;

		b2RevoluteJointDef jd;

		b2Body* prevBody = ground;
		for (int32 i = 0; i < N; ++i)
		{
			b2BodyDef bd;
			bd.type = b2_dynamicBody;
			bd.position.Set(161.0f + 2.0f * i, -0.125f);
			b2Body* body = m_world->CreateBody(&bd);
			body->CreateFixture(&fd);

			b2Vec2 anchor(160.0f + 2.0f * i, -0.125f);
			jd.Initialize(prevBody, body, anchor);
			m_world->CreateJoint(&jd);

			prevBody = body;
		}

		b2Vec2 anchor(160.0f + 2.0f * N, -0.125f);
		jd.Initialize(prevBody, ground, anchor);
		m_world->CreateJoint(&jd);
	}

	// Boxes
	{
		b2PolygonShape box;
		box.SetAsBox(0.5f, 0.5f);

		b2Body* body = NULL;
		b2BodyDef bd;
		bd.type = b2_dynamicBody;

		bd.position.Set(230.0f, 0.5f);
		body = m_world->CreateBody(&bd);
		body->CreateFixture(&box, 0.5f);

		bd.position.Set(230.0f, 1.5f);
		body = m_world->CreateBody(&bd);
		body->CreateFixture(&box, 0.5f);

		bd.position.Set(230.0f, 2.5f);
		body = m_world->CreateBody(&bd);
		body->CreateFixture(&box, 0.5f);

		bd.position.Set(230.0f, 3.5f);
		body = m_world->CreateBody(&bd);
		body->CreateFixture(&box, 0.5f);

		bd.position.Set(230.0f, 4.5f);
		body = m_world->CreateBody(&bd);
		body->CreateFixture(&box, 0.5f);
	}

	// Car
	{
		b2PolygonShape chassis;
		b2Vec2 vertices[8];
		vertices[0].Set(-1.5f, -0.5f);
		vertices[1].Set(1.5f, -0.5f);
		vertices[2].Set(1.5f, 0.0f);
		vertices[3].Set(0.0f, 0.9f);
		vertices[4].Set(-1.15f, 0.9f);
		vertices[5].Set(-1.5f, 0.2f);
		chassis.Set(vertices, 6);

		b2CircleShape circle;
		circle.m_radius = 0.4f;

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.position.Set(0.0f, 1.0f);
		m_car = m_world->CreateBody(&bd);
		m_car->CreateFixture(&chassis, 1.0f);

		b2FixtureDef fd;
		fd.shape = &circle;
		fd.density = 1.0f;
		fd.friction = 0.9f;

		bd.position.Set(-1.0f, 0.35f);
		m_wheel1 = m_world->CreateBody(&bd);
		m_wheel1->CreateFixture(&fd);

		bd.position.Set(1.0f, 0.4f);
		m_wheel2 = m_world->CreateBody(&bd);
		m_wheel2->CreateFixture(&fd);

		b2WheelJointDef jd;
		b2Vec2 axis(0.0f, 1.0f);

		float mass1 = m_wheel1->GetMass();
		float mass2 = m_wheel2->GetMass();

		float hertz = 4.0f;
		float dampingRatio = 0.7f;
		float omega = 2.0f * b2_pi * hertz;

		jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
		jd.motorSpeed = 0.0f;
		jd.maxMotorTorque = 20.0f;
		jd.enableMotor = true;
		jd.stiffness = mass1 * omega * omega;
		jd.damping = 2.0f * mass1 * dampingRatio * omega;
		jd.lowerTranslation = -0.25f;
		jd.upperTranslation = 0.25f;
		jd.enableLimit = true;
		m_spring1 = (b2WheelJoint*)m_world->CreateJoint(&jd);

		jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
		jd.motorSpeed = 0.0f;
		jd.maxMotorTorque = 10.0f;
		jd.enableMotor = false;
		jd.stiffness = mass2 * omega * omega;
		jd.damping = 2.0f * mass2 * dampingRatio * omega;
		jd.lowerTranslation = -0.25f;
		jd.upperTranslation = 0.25f;
		jd.enableLimit = true;
		m_spring2 = (b2WheelJoint*)m_world->CreateJoint(&jd);
	}
}


void Car::Step()
{
	auto new_origin = m_car->GetPosition();
	new_origin.y += 1.0;
	m_world->ShiftOrigin(new_origin);

	if (GetAsyncKeyState(VK_LEFT)) {
		m_spring1->SetMotorSpeed(m_speed);
	} else if (GetAsyncKeyState(VK_RIGHT)) {
		m_spring1->SetMotorSpeed(-m_speed);
	} else {
		m_spring1->SetMotorSpeed(0);
		m_spring2->SetMotorSpeed(0);
	}

	float timeStep = m_hertz > 0.0f ? 1.0f / m_hertz : float(0.0f);

	m_debugDraw.SetFlags(b2Draw::e_shapeBit);
	m_world->Step(timeStep, 8, 3);
	m_world->DebugDraw();
}
