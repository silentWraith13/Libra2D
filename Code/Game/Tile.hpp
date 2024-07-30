#pragma once
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Core/Rgba8.hpp"
#include "Engine/Math/AABB2.hpp"

class TileDefinition;

enum TileType
{
	SHORT_GRASS,
	LONG_GRASS,
	DARK_GRASS,
	DIRT,
	COBBLE,
	PAVED,
	STONE_WALL,
	BRICK_WALL,
	IRON_WALL,
	MAP_ENTRY,
	MAP_EXIT,

	NUM_TYPES
};

class Tile
{
public:
	IntVec2 m_tileCoords=IntVec2(-1,-1);
	TileType m_tileType = SHORT_GRASS;

	AABB2 GetBounds() const;
	Rgba8 GetColor() const;
	
	bool  IsSolid() const;
};