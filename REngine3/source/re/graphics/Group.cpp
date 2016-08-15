//
//  Group.cpp
//  REngine3
//
//  Created by reworks on 11/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/component/SpriteComponent.hpp"

#include "Group.hpp"

namespace re
{
	Group::~Group()
	{
		m_entitys.clear();
	}

	void Group::AddEntity(std::shared_ptr<Entity> entity)
	{
		m_entitys.push_back(entity);
	}

	void Group::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		// backup states because we are calling this draw function multiple times
		sf::RenderStates old = states;

		for (auto& v : m_entitys)
		{
			// may need to be an override?
			states.transform *= v->Get<SpriteComponent>()->m_transform.getTransform();
			
			states.texture = &v->Get<SpriteComponent>()->m_texture;
			states.shader = &v->Get<SpriteComponent>()->m_shader;

			target.draw(v->Get<SpriteComponent>()->m_vertices, states);

			states = old;
		}
	}
}