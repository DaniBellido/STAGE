#pragma once
#include "../ECS/ECS.h"
#include <SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdlrenderer2.h>

#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/HealthComponent.h"


class RenderGUISystem : public System 
{
public:

	RenderGUISystem() = default;

	void Update(const std::unique_ptr<Registry>& registry, SDL_Renderer* renderer)
	{
		// Draw all the ImGui objects on the screen
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// ########## Draw all your ImGui Objects/Windows here ##########

		if (ImGui::Begin("Spawn enemies")) 
		{
			static int enemyXPos = 0;
			static int enemyYPos = 0;
			ImGui::InputInt("Enemy X position", &enemyXPos);
			ImGui::InputInt("Enemy Y position", &enemyYPos);

			if (ImGui::Button("Create new enemy")) 
			{
				Entity enemy = registry->CreateEntity();
				enemy.Group("enemies");
				enemy.AddComponent<TransformComponent>(glm::vec2(enemyXPos, enemyYPos), glm::vec2(1.0, 1.0), 0.0);
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
				enemy.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
				enemy.AddComponent<BoxColliderComponent>(32, 32);
				enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000, 3000, 10, false);
				enemy.AddComponent<HealthComponent>(100);
			}
		}
		ImGui::End();



		// ##############################################################

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer); 

	}
};