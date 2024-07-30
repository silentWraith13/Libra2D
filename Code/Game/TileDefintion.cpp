#include "Game/TileDefintion.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/EngineCommon.hpp"

void TileDefinition::InitializeTileDefs()
{
	s_definitions.resize((int)TileType::NUM_TYPES);
	DefineTileType( TileType::SHORT_GRASS, false, 0, 0,  Rgba8(180, 180, 180) );
	DefineTileType( TileType::LONG_GRASS,  false, 2, 0,  Rgba8(180, 180, 180) );
	DefineTileType( TileType::DARK_GRASS,  false, 3, 0,  Rgba8(180, 180, 180) );
	DefineTileType( TileType::DIRT,        false, 5, 2,  Rgba8(255, 255, 255) );
	DefineTileType( TileType::COBBLE,	   false, 6, 1,  Rgba8(255, 255, 255) );
	DefineTileType( TileType::PAVED,       false, 6, 2,  Rgba8(255, 255, 255) );
	DefineTileType( TileType::STONE_WALL,  false, 5, 4,  Rgba8(255, 255, 255) );
	DefineTileType( TileType::BRICK_WALL,  false, 0, 5,  Rgba8(255, 255, 255) );
	DefineTileType( TileType::IRON_WALL,   false, 5, 6,  Rgba8(255, 255, 255) );
	DefineTileType( TileType::MAP_ENTRY,   false, 0, 7,  Rgba8(255, 255, 255) );
	DefineTileType( TileType::MAP_EXIT,    false, 1, 7,  Rgba8(255, 255, 255) );

	for (int i = 0; i < (int)TileType::NUM_TYPES; i++)
	{
		if (s_definitions[i].m_UVs == AABB2(Vec2(0.f,0.f), Vec2(1.f,1.f)))
		{
			ERROR_RECOVERABLE(Stringf("TileDef for type #%i was not initialized",i));
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<TileDefinition> TileDefinition::s_definitions;

void TileDefinition::DefineTileType(TileType type, bool isSolid, int spriteX, int spriteY, Rgba8 const& tint)
{
	int SPRITE_PER_ROW = TERRAIN_SPRITE_LAYOUT.x;
	int spriteIndex = spriteX + (spriteY * SPRITE_PER_ROW);

	TileDefinition& tileDef = s_definitions[(int)type];
	tileDef.m_isSolid = isSolid;
	tileDef.m_UVs = g_terrainSprites->GetSpriteUVs(spriteIndex);
	tileDef.m_tint = tint;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
TileDefinition const& TileDefinition::GetDef(TileType type)
{
	TileDefinition const& tileDef = s_definitions[(int)type];
	return tileDef;
}

