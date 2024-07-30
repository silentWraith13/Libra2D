#include "Game/Tile.hpp"
#include "Game/TileDefintion.hpp"


AABB2 Tile::GetBounds() const
{
	AABB2 bounds;
	bounds.m_mins.x = static_cast<float>(m_tileCoords.x);
	bounds.m_mins.y = static_cast<float>(m_tileCoords.y);
	bounds.m_maxs.x = bounds.m_mins.x + 1.0f;
	bounds.m_maxs.y = bounds.m_mins.y + 1.0f;
	return bounds;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
Rgba8 Tile::GetColor() const
{
	Rgba8 color(255, 0, 255);
	
	if (m_tileType == SHORT_GRASS)
	{
		color = Rgba8( 255, 255 , 255);
	}

	else if( m_tileType == STONE_WALL)
	{
		color = Rgba8( 255, 255, 255);
	}
	return color;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool Tile::IsSolid() const
{
	if (m_tileType == STONE_WALL)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

