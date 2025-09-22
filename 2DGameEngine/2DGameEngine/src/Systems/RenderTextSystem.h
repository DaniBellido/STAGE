#pragma once

#include "../ECS/ECS.h"
#include <SDL.h>
#include "../Components/TextLabelComponent.h"
#include "../AssetStore/AssetStore.h"

class RenderTextSystem : public System 
{
public:
	RenderTextSystem() 
	{
		RequireComponent<TextLabelComponent>();
	}

	void Update(std::unique_ptr<AssetStore>& assetStore, SDL_Renderer* renderer, const SDL_Rect& camera) 
	{
		for (auto entity : GetSystemEntities()) 
		{
			const auto textlabel = entity.GetComponent<TextLabelComponent>();

			SDL_Surface* surface = TTF_RenderText_Blended(assetStore->GetFont(textlabel.assetId), textlabel.text.c_str(), textlabel.color);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
			SDL_FreeSurface(surface);

			int labelWidth = 0;
			int labelHeight = 0;

			SDL_QueryTexture(texture, NULL, NULL, &labelWidth, &labelHeight);

			SDL_Rect desRect = {

				static_cast<int>(textlabel.position.x - (textlabel.isFixed ? 0 : camera.x)),
				static_cast<int>(textlabel.position.y - (textlabel.isFixed ? 0 : camera.y)),
				labelWidth,
				labelHeight

			};

			SDL_RenderCopy(renderer, texture, NULL, &desRect);

			SDL_DestroyTexture(texture);

		}
	}

};