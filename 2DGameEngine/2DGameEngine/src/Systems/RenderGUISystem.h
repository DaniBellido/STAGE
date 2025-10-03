#pragma once
#include "../ECS/ECS.h"
#include <SDL.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include "../Game/LevelLoader.h"

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

	void Update(const std::unique_ptr<Registry>& registry, SDL_Renderer* renderer, const SDL_Rect& camera)
	{
		// Draw all the ImGui objects on the screen
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// ########## Draw all your ImGui Objects/Windows here ##########

		if (ImGui::Begin("Enemy Spawner")) 
		{

			// Sprite Component
			const char* sprites[] = { "tank-image", "truck-image" };
			static int sprite_current = 0;
			if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Texture ID");
				ImGui::SameLine();
				ImGui::Combo("##Texture ID", &sprite_current, sprites, IM_ARRAYSIZE(sprites));

			}
			ImGui::Spacing();

			// Transform Component
			static int posX = 0;
			static int posY = 0;
			static int scaleX = 1;
			static int scaleY = 1;
			static float rotation = 0.0;

			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				ImGui::Text("Position X         ");
				ImGui::SameLine();
				ImGui::InputInt("##Position X", &posX);

				ImGui::Text("Position Y         ");
				ImGui::SameLine();
				ImGui::InputInt("##Position Y", &posY);

				ImGui::Text("Scale X            ");
				ImGui::SameLine();
				ImGui::SliderInt("##Scale X", &scaleX, 1, 10);

				ImGui::Text("Scale Y            ");
				ImGui::SameLine();
				ImGui::SliderInt("##Scale Y", &scaleY, 1, 10);

				ImGui::Text("Rotation           ");
				ImGui::SameLine();
				ImGui::SliderAngle("##Rotation", &rotation, 0.0, 359.999);
			}
			ImGui::Spacing();

			// Rigid Body Component
			static int velocityX = 0;
			static int velocityY = 0;
			if (ImGui::CollapsingHeader("Rigid Body", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				ImGui::Text("Velocity X         ");
				ImGui::SameLine();
				ImGui::InputInt("##Velocity X", &velocityX);

				ImGui::Text("Velocity Y         ");
				ImGui::SameLine();
				ImGui::InputInt("##Velocity Y", &velocityY);
			}
			ImGui::Spacing();

			// Health Component
			static int health = 100.0;
			if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				ImGui::Text("HP                 ");
				ImGui::SameLine();
				ImGui::SliderInt("##HP", &health, 0, 100);
			}
			ImGui::Spacing();

			// Projectile Component
			static float projectileAngle = 0.0;
			static float projectileSpeed = 100.0;
			static int repeatFrecuency = 1;
			static int projectileDuration = 1;
			static int hitPercentDamage = 10;
			if (ImGui::CollapsingHeader("Projectile Emitter", ImGuiTreeNodeFlags_DefaultOpen)) 
			{
				ImGui::Text("Angle (deg)       ");
				ImGui::SameLine();
				ImGui::SliderAngle("##Angle (deg)", &projectileAngle, 0, 360);
				ImGui::Text("Speed (px/sec)    ");
				ImGui::SameLine();
				ImGui::SliderFloat("##Speed (px/sec)", &projectileSpeed, 10, 500);
				ImGui::Text("Frecuency         ");
				ImGui::SameLine();
				ImGui::InputInt("##Frecuency", &repeatFrecuency);
				ImGui::Text("Duration (in secs)");
				ImGui::SameLine();
				ImGui::InputInt("##Duration (in seconds)", &projectileDuration);
				ImGui::Text("Damage %          ");
				ImGui::SameLine();
				ImGui::SliderInt("##Damage %", &hitPercentDamage, 0, 100);
			}
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			
			// Button to instanciate an enemy
			if (ImGui::Button("Create new enemy")) 
			{
				Entity enemy = registry->CreateEntity();
				enemy.Group("enemies");
				enemy.AddComponent<TransformComponent>(glm::vec2(posX, posY), glm::vec2(scaleX, scaleY), glm::degrees(rotation));
				enemy.AddComponent<RigidBodyComponent>(glm::vec2(velocityX, velocityY));
				enemy.AddComponent<SpriteComponent>(sprites[sprite_current], 32, 32, 5);
				enemy.AddComponent<BoxColliderComponent>(25, 20, glm::vec2(5, 5));
				double projectileVelocityX = cos(projectileAngle) * projectileSpeed;
				double projectileVelocityY = sin(projectileAngle) * projectileSpeed;
				enemy.AddComponent<ProjectileEmitterComponent>(glm::vec2(projectileVelocityX, projectileVelocityY), repeatFrecuency * 1000, projectileDuration * 1000, hitPercentDamage, false);
				enemy.AddComponent<HealthComponent>(health);

			}
		}
		ImGui::End();

		// Display a small window showing map coordinates using the mouse
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav;
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
		ImGui::SetNextWindowBgAlpha(0.9f);
		if (ImGui::Begin("Map coordinates", NULL, windowFlags))
		{
			ImGui::Text("Position: x = %.1f, y = %.1f", ImGui::GetIO().MousePos.x + camera.x, ImGui::GetIO().MousePos.y + camera.y);

		}
		ImGui::End();

		// ##############################################################

		ImGui::Render();
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer); 

	}
};