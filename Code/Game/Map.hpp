#pragma once
#include "Game/GameCommon.hpp"
#include "Game/Tile.hpp"
#include "Game/Entities.hpp"
#include <vector>
#include "Game/Player.hpp"
#include "Game/Aries.hpp"
#include "Game/Leo.hpp"
#include "Game/Scorpio.hpp"
#include "Game/Game.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Game/TileDefintion.hpp"
#include "Engine/Core/HeatMaps.hpp"

class TileHeatMap;

constexpr float  maxMapDist = 999.f;

class Map
{
public:
	Map(Game*,IntVec2 dimensions);
	~Map();

	void				Update(float deltaSeconds);
	void				Render() const;
	
	//entity collisions
	void				PushAllEntitiesOutOfWalls();
	void				PushEntityOutOfWalls(Entity& entity);
	void				PushEntityOutOfTileIfSolid(Entity& entity, IntVec2 const& tileCoords);
	void				PushEntitiesOutOfEachOther(Entity& a, Entity& b) const;
	void				CheckBulletVsActor(Bullets& bullet, Entity& actor);
	bool				DoEntitiesOverlap(Entity& a, Entity& b);
	void				DoEntityCollisions();
	void				CheckCollisionBetweenEntityLists(EntityList& listA, EntityList& listB);
	void				CheckForBulletHits();
	void				CheckBulletsListVsActorList(EntityList& bulletsList, EntityList& actorList);

	//deleting of entities
	void				RemoveEntityFromMap(Entity* entity);
	void				DeleteGarbageEntities();
	void				RemoveEntityFromList(Entity* entity, EntityList& list);

	//Tile stuff
	void				PopulateTiles();
	void				PopulateHeatMaps();
	bool				IsValid();
	void				AddVertsForTile(std::vector<Vertex_PCU>& verts, int tileIndex) const;
	void				RenderTiles() const;
	int					GetTileIndexForTileCoords(int tileX, int tileY) const;
	void				SetTileType(int tileIndex, TileType type);
	void				SetTileType(int tileX, int tileY, TileType type);
	void				PopulateDistanceField(TileHeatMap& out_distanceField, IntVec2 referenceCoords, float maxCost);
	void				DoDistanceFieldPass(TileHeatMap& distanceField, float currentPassTileHeat);
	void				SpreadHeatToTile(TileHeatMap& distanceField, float nextTileHeat, IntVec2 currentTile);
	void				RenderDebugHeatMap() const;
	IntVec2				GetTileCoordsForWorldPos(Vec2 const& worldPos) const;	
	AABB2				GetTileBounds(IntVec2 const& tileCoords);
	AABB2				GetTileBoundForTileCoords(int tileX, int tileY);
	bool				IsTileSolid(IntVec2 const& tileCoords) const;
	bool				IsPointInSolidTile(Vec2 const& worldPos) const;
	bool				IsTileOutOfBounds(IntVec2 const& tileCoords) const;
	bool				HasLineOfSight(Vec2 const& posA, Vec2 const& posB) const;

	
	//Entity stuff
	Entity*				CreateNewEntityOfType(EntityType type, Vec2 const& position, float orientationDegrees);
	Entity*				SpawnNewEntityOfType(EntityType entityType, Vec2 const& position, float orientationDegrees);
	Entity*				GetClosestVisibleEntityOfType(Vec2 const& refPos, EntityType type, float maxDist);
	Entity*				GetClosestVisibleEntityOfFaction(Vec2 const& refPos, EntityFaction faction, float maxDist);
	EntityList const&	GetEntitiesByType(EntityType type) const;
	void				AddEntityToList(Entity* entity, EntityList& list);
	void				AddEntityToMap(Entity* entity);
	void				UpdateEntities(float deltaSeconds);
	void				RenderEntities() const;
	void				RenderDebugDrawEntities() const;

	//Raycast
	RaycastResult2D		RaycastVsTiles_STEP_AND_SIMPLE(Vec2 startPos, Vec2 forwardDir, float maxDist) const;
	RaycastResult2D		RaycastVsTiles(Vec2 startPos, Vec2 forwardDir, float maxDist) const;
	
	void				UpdateCamera();
	void				DebugCamera();

	Game*					m_game = nullptr;
	Player*					m_player = nullptr;
	bool					m_debugDraw = false;
	bool					m_debugCamera = false;
	float					m_numTilesinViewVertically = 8.0f;
	std::vector<Tile>		m_tiles;
	Camera					m_worldCamera;
	
	//Diff entity lists
	EntityList				m_entityListsByType[NUM_ENTITY_TYPES];
	EntityList				m_bulletsByFaction[NUM_FACTIONS];
	EntityList				m_actorsByFaction[NUM_FACTIONS];
	
	IntVec2					m_dimensions = IntVec2(0, 0);
	IntVec2					STEP_NORTH = IntVec2(0, 1);
	IntVec2					STEP_EAST = IntVec2(1, 0);
	IntVec2					STEP_WEST = IntVec2(-1, 0);
	IntVec2					STEP_SOUTH = IntVec2(0, -1);
	IntVec2					STEP_NORTHEAST = IntVec2(1, 1);
	IntVec2					STEP_NORTHWEST = IntVec2(-1, 1);
	IntVec2					STEP_SOUTHWEST = IntVec2(-1, -1);
	IntVec2					STEP_SOUTHEAST = IntVec2(1, -1);
	IntVec2					m_exitPos = IntVec2(18,28);
	
	TileHeatMap				m_distanceFieldFromStart;

	Scorpio* m_scorpio = nullptr;
	Leo* m_leo = nullptr;
	Aries* m_aries = nullptr;
};