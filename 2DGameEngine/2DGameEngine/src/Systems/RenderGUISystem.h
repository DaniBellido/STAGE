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
			// Transform Component
			static int enemyXPos = 0;
			static int enemyYPos = 0;
			static float enemyXScale = 1.0;
			static float enemyYScale = 1.0;
			static float enemyRotation = 0.0;
			ImGui::InputInt("Enemy X position", &enemyXPos);
			ImGui::InputInt("Enemy Y position", &enemyYPos);
			ImGui::InputFloat("Enemy X Scale", &enemyXScale);
			ImGui::InputFloat("Enemy Y Scale", &enemyYScale);
			ImGui::SliderFloat("Rotation", &enemyRotation, 0.0, 359.999);

			// Rigid Body Component
			static float enemyXVelocity = 0.0;
			static float enemyYVelocity = 0.0;
			ImGui::InputFloat("Enemy X velocity", &enemyXVelocity);
			ImGui::InputFloat("Enemy Y velocity", &enemyYVelocity);

			// Sprite Component
			const char* items[] = { "tank-image", "truck-image"};
			static int item_current = 0;
			ImGui::Combo("Sprite", &item_current, items, IM_ARRAYSIZE(items));

			// Health Component
			static float enemyHealth = 100.0;
			ImGui::SliderFloat("Health", &enemyHealth, 0.0, 100.0);

			// Projectile Component
			static float projectileVelocityX = 0.0;
			static float projectileVelocityY = 0.0;
			static int repeatFrecuency = 1;
			static int projectileDuration = 1;
			static int hitPercentDamage = 10;
			ImGui::InputFloat("Projectile X velocity", &projectileVelocityX);
			ImGui::InputFloat("Projectile Y velocity", &projectileVelocityY);
			ImGui::InputInt("Frecuency", &repeatFrecuency);
			ImGui::InputInt("Duration (in seconds)", &projectileDuration);
			ImGui::SliderInt("Damage %", &hitPercentDamage, 0, 100);
			


			if (ImGui::Button("Create new enemy")) 
			{
				Entity enemy = registry->CreateEntity();
				enemy.Group("enemies");
				enemy.AddComponent<TransformComponent>(glm::vec2(enemyXPos, enemyYPos), glm::vec2(enemyXScale, enemyYScale), enemyRotation);
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(enemyXVelocity, enemyYVelocity));
				enemy.AddComponent<SpriteComponent>(items[item_current], 32, 32, 1);
				enemy.AddComponent<BoxColliderComponent>(32, 32);
				enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projectileVelocityX, projectileVelocityY), repeatFrecuency * 1000, projectileDuration * 1000, hitPercentDamage, false);
				enemy.AddComponent<HealthComponent>(enemyHealth);

			}
		}
		ImGui::End();



		// ##############################################################

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer); 

	}
};