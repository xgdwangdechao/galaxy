///
/// PhysicsComponent.cpp
/// galaxy
///
/// Created by reworks on 10/11/2016.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/libs/loguru/loguru.hpp"
#include "galaxy/physics/Box2DHelper.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "PhysicsComponent.hpp"

namespace galaxy
{
	PhysicsComponent::PhysicsComponent()
		:m_body(nullptr), m_entity(0)
	{
	}

	PhysicsComponent::PhysicsComponent(const float x, const float y, const b2BodyType bodyType, const std::vector<b2FixtureDef>& fixtures, const bool isRotationFixed)
		:m_body(nullptr), m_entity(0)
	{
		// Construct body definition.
		b2BodyDef bodyDef;
		bodyDef.position.Set(Box2DHelper::pixelsToMeters<float32>(x), Box2DHelper::pixelsToMeters<float32>(y));

		bodyDef.type = bodyType;

		// Use box2d to create the body data in the physics world.
		m_body = Locator::box2dHelper->m_b2world->CreateBody(&bodyDef);

		if (!fixtures.empty())
		{
			for (auto& fixture : fixtures)
			{
				// Make sure when creating fixture it is properly created.
				if (!m_body->CreateFixture(&fixture))
				{
					LOG_S(ERROR) << "Failed to create fixturedef for PhysicsComponent.";
				}
			}
		}
		else
		{
			// This is only a warning because the game can still run with a body that has no fixtures. 
			// Although it would not work correctly, it would not cause a crash.
			LOG_S(WARNING) << "Fixture List is empty!";
		}

		m_body->SetFixedRotation(isRotationFixed);
	}

	PhysicsComponent::PhysicsComponent(const sol::table& table)
		:m_body(nullptr), m_entity(NULL)
	{
		// Construct body definition.
		b2BodyDef bodyDef;
		bodyDef.position.Set(Box2DHelper::pixelsToMeters<float32>(table.get<float>("x")), Box2DHelper::pixelsToMeters<float32>(table.get<float>("y")));

		// 0 = static, 1 = kinematic, 2 = dynamic
		switch (table.get<int>("bodyType"))
		{
		case 0:
			bodyDef.type = b2BodyType::b2_staticBody;
			break;

		case 1:
			bodyDef.type = b2BodyType::b2_kinematicBody;
			break;

		case 2:
			bodyDef.type = b2BodyType::b2_dynamicBody;
			break;
		}
		
		// Use box2d to create the body data in the physics world.
		m_body = Locator::box2dHelper->m_b2world->CreateBody(&bodyDef);

		// Set up the details of the body using lua scripts.
		sol::table fixtureList = table.get<sol::table>("fixtureList");
		if (!fixtureList.empty())
		{
			fixtureList.for_each([&](std::pair<sol::object, sol::object> pair)
			{
				auto first = pair.first.as<std::string>();
				auto second = pair.second.as<sol::table>();

				float32 w = second.get<float32>("w");
				float32 h = second.get<float32>("h");

				b2PolygonShape b2shape;
				b2shape.SetAsBox(Box2DHelper::pixelsToMeters<float32>(w / 2.0f), Box2DHelper::pixelsToMeters<float32>(h / 2.0f), b2Vec2(Box2DHelper::pixelsToMeters<float32>(w / 2.0f), Box2DHelper::pixelsToMeters<float32>(h / 2.0f)), second.get<float32>("angle"));

				b2FixtureDef fixtureDef;
				fixtureDef.density = second.get<float32>("density");
				fixtureDef.friction = second.get<float32>("friction");
				fixtureDef.restitution = second.get<float32>("restitution");
				fixtureDef.shape = &b2shape;
				fixtureDef.filter.categoryBits = second.get<std::uint16_t>("collisionType");

				sol::table cw = second.get<sol::table>("collidesWith");
				std::uint16_t mb = 0x000;
				
				cw.for_each([&](std::pair<sol::object, sol::object> pair)
				{
					mb = mb | pair.second.as<std::uint16_t>();
				});

				fixtureDef.filter.maskBits = mb;

				// Make sure when creating fixture it is properly created.
				if (!m_body->CreateFixture(&fixtureDef))
				{
					LOG_S(ERROR) << "Failed to create fixturedef for PhysicsComponent.";
				}
			});
		}
		else
		{
			// This is only a warning because the game can still run with a body that has no fixtures. 
			// Although it would not work correctly, it would not cause a crash.
			LOG_S(WARNING) << "Fixture List is empty!";
		}
		
		m_body->SetFixedRotation(table.get<bool>("fixedRotation"));
	}

	void PhysicsComponent::setFixtureEntity(const entt::DefaultRegistry::entity_type entity)
	{
		// Sets the entity this fixture belongs to to use it in collisions.
		m_entity = entity;
		for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
		{
			f->SetUserData(static_cast<void*>(&m_entity));
		}
	}
}