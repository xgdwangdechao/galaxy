//
//  PhysicsComponent.hpp
//  REngine3
//
//  Created by reworks on 10/11/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_PHYSICSCOMPONENT_HPP_
#define RENGINE3_PHYSICSCOMPONENT_HPP_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "re/types/Component.hpp"
#include "re/physics/Box2D/Dynamics/b2Body.h"

namespace re
{
	class PhysicsComponent : public Component
	{
		friend class boost::serialization::access;

	public:
		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Default Constructor.
		*/
		PhysicsComponent();

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up component.
		*/
		~PhysicsComponent() override;

		/*
		* IMPORTS: sol::table containing data.
		* EXPORTS: none
		* PURPOSE: Set up component.
		*/
		void init(sol::table& table) override;

	public:
		b2Body* m_body;
		bool m_isMovingVertically = false;
		bool m_isMovingHoritontally = false;

	private:
		// Boost.Serialization functions
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			ar & m_body->GetPosition().x;
			ar & m_body->GetPosition().y;
			ar & m_body->GetAngle();
		}

		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			float32 x, y, angle;
			ar & x;
			ar & y;
			ar & angle;

			m_body->SetTransform(b2Vec2(x, y), angle);
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()
	};
}

#endif
