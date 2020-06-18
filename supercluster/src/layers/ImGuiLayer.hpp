///
/// ImGuiLayer.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_IMGUILAYER_HPP_
#define SUPERCLUSTER_IMGUILAYER_HPP_

#include <memory>
#include <filesystem>

#include <TextEditor.h>
#include <qs/core/Shader.hpp>
#include <galaxy/layer/Layer.hpp>
#include <galaxy/ui/LuaConsole.hpp>
#include <qs/graphics/TextureAtlas.hpp>

#include <imgui_stdlib.h>
#include <nlohmann/json.hpp>
#include <imgui_impl_glfw.h>
#include <qs/core/Window.hpp>
#include <imgui_impl_opengl3.h>
#include <galaxy/core/World.hpp>
#include <qs/shaders/Sprites.hpp>
#include <solar/entity/Manager.hpp>
#include <pfd/portable-file-dialogs.h>
#include <qs/shaders/RenderToTexture.hpp>
#include <galaxy/core/ServiceLocator.hpp>
#include <galaxy/systems/RenderSystem.hpp>
#include <galaxy/scripting/JSONDefinition.hpp>

#include <galaxy/components/SpriteComponent.hpp>
#include <galaxy/components/TransformComponent.hpp>
#include <galaxy/components/SpriteBatchComponent.hpp>
#include <galaxy/components/PlaylistComponent.hpp>
#include <galaxy/components/AudioComponent.hpp>

namespace pfd
{
	class open_file;
	class save_file;
}

namespace sc
{
	class ImGuiLayer final : public galaxy::Layer
	{
	public:
		ImGuiLayer() noexcept;
		virtual ~ImGuiLayer() noexcept;

		void events() noexcept override;
		void update(protostar::ProtectedDouble * deltaTime) noexcept override;
		void render() noexcept override;

	private:
		void entityUI() noexcept;
		void componentUI(sr::Entity active) noexcept;
		void scriptEditorUI() noexcept;
		void textureAtlasEditor() noexcept;

		void start() noexcept;
		void end() noexcept;

		std::filesystem::path openFilePath() noexcept;
		std::filesystem::path saveFilePath() noexcept;

	private:
		bool m_showEUI;
		bool m_showTEUI;
		bool m_showTAEUI;
		bool m_newlyAdded;
		bool m_drawConsole;

		std::string m_name;
		galaxy::World* m_world;

		qs::Window* m_window;
		qs::Shader m_spriteShader;
		qs::Shader m_atlasShader;
		std::unique_ptr<qs::TextureAtlas> m_textureAtlas;

		TextEditor m_editor;
		galaxy::LuaConsole m_console;
		std::unique_ptr<pfd::open_file> m_fileToOpen;
		std::unique_ptr<pfd::save_file> m_fileToSave;

		galaxy::SpriteComponent* sc = nullptr;
		galaxy::TransformComponent* tc = nullptr;
		galaxy::SpriteBatchComponent* sbc = nullptr;
		galaxy::PlaylistComponent* pc = nullptr;
		galaxy::AudioComponent* ac = nullptr;

		std::vector<std::string> m_listOfEntitys;
	};
}

#endif