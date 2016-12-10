//
//  RenderSystem.hpp
//  REngine3
//
//  Created by reworks on 6/08/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#ifndef RENGINE3_RENDERSYSTEM_HPP_
#define RENGINE3_RENDERSYSTEM_HPP_

#include "re/types/System.hpp"
#include "re/graphics/Group.hpp"

namespace re
{
	class Window;
	class World;

	class RenderSystem : public System
	{
	public:
		/*
		* IMPORTS: Number of groups. Think "layers". Each group is a layer of entitys to render.
		* EXPORTS: none
		* PURPOSE: Construct the system.
		*/
		RenderSystem(int numOfGroups);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the renderer.
		*/
		~RenderSystem() override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Automatically adds entitys to the system.
		*/
		void AutoSubmit(World* world) override;

		/*
		* IMPORTS: id of entity to add and its component list.
		* EXPORTS: none
		* PURPOSE: Add an entitys components from the system.
		*/
		void AddEntity(Entity* e) override;

		/*
		* IMPORTS: The name, the group and the generic data. Must inherit from both sf::Drawable and sf::Transformable.
		* EXPORTS: none
		* PURPOSE: Add a unique drawable to the system.
		*/
		void AddGenericDrawable(Entity* e, sf::Uint32 group, std::shared_ptr<sf::Drawable> d, std::shared_ptr<sf::Transformable> t);

		/*
		* IMPORTS: name (id) of entity to remove OR Object to remove. It does both.
		* EXPORTS: none
		* PURPOSE: Remove an entitys components from the system.
		*/
		void RemoveEntity(const std::string& name) override;

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean up the entitys.
		*/
		void Render(Window* window);

		/*
		* IMPORTS: none
		* EXPORTS: none
		* PURPOSE: Clean the system.
		*/
		void Clean() override;

	private:
		std::vector<Group> m_groups;
	};
}

#endif