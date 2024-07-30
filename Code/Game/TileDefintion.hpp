#pragma once
#include <vector>
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/Vec2.hpp"

#include "Game/Tile.hpp"

struct  AABB2;
struct  Vec2;
class Tile;

class TileDefinition
{
public:
	static void InitializeTileDefs();
	static void DefineTileType(TileType type, bool isSolid, int spriteX, int spriteY, Rgba8 const& tint);
	static TileDefinition const& GetDef(TileType type);
		
	static std::vector<TileDefinition> s_definitions;

	bool m_isSolid = false;
	AABB2 m_UVs = AABB2( Vec2(0.f, 0.f), Vec2(1.f, 1.f) );
	Rgba8 m_tint = Rgba8(255, 255, 255);
	TileType m_tileType;
};
