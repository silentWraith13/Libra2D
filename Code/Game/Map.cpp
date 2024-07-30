#include "Game/Map.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Game/Bullets.hpp"



Map::Map(Game* game, IntVec2 dimensions)
	:m_game(game)
	,m_dimensions(dimensions)
	,m_distanceFieldFromStart(dimensions, maxMapDist)
{
	bool isDoneGenerating = false;
	while (!isDoneGenerating)
	{
		PopulateTiles();
		PopulateHeatMaps();
		isDoneGenerating = IsValid();
	}

	m_player = static_cast<Player*>(SpawnNewEntityOfType(ENTITY_TYPE_GOOD_PLAYER, Vec2(1.5f, 1.5f), 45.0f));
	m_scorpio = static_cast<Scorpio*>(SpawnNewEntityOfType(ENTITY_TYPE_EVIL_SCORPIO, Vec2(5.5f, 1.5f), 45.0f));
	m_leo = static_cast<Leo*>(SpawnNewEntityOfType(ENTITY_TYPE_EVIL_LEO, Vec2(7.5f, 1.5f), 45.0f));
	m_aries = static_cast<Aries*>(SpawnNewEntityOfType(ENTITY_TYPE_EVIL_ARIES, Vec2(9.5f, 1.5f), 45.0f));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

Map::~Map()
{
	if (m_player)
	{
		delete m_player;
		m_player = nullptr;
	}

	if (m_scorpio)
	{
		delete m_scorpio;
		m_scorpio = nullptr;
	}

	if (m_leo)
	{
		delete m_leo;
		m_leo = nullptr;
	}

	if (m_leo)
	{
		delete m_leo;
		m_leo = nullptr;
	}

	if (m_aries)
	{
		delete m_aries;
		m_aries = nullptr;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void Map::Update(float deltaSeconds)
{
	UpdateCamera();
	UpdateEntities(deltaSeconds);
	PushAllEntitiesOutOfWalls();
	DeleteGarbageEntities();
	DoEntityCollisions();
	CheckForBulletHits();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::Render() const
{
	Rgba8 clearColor{ 0,0,0,255 };
	g_theRenderer->ClearScreen(clearColor);

	g_theRenderer->BeginCamera(m_worldCamera);
	
	RenderTiles();
	RenderEntities();
	RenderDebugDrawEntities();
	
	if (m_game->m_debugHeatMap)
	{
		RenderDebugHeatMap();
	}

	g_theRenderer->EndCamera(m_worldCamera);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
int Map::GetTileIndexForTileCoords(int tileX, int tileY) const
{
	return tileX + (tileY * m_dimensions.x);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
AABB2 Map::GetTileBoundForTileCoords(int tileX, int tileY)
{
	return AABB2(Vec2( (float)tileX, (float)tileY ), Vec2( (float)(tileX + 1), (float)(tileY + 1 )) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::SetTileType(int tileIndex, TileType type)
{
	m_tiles[tileIndex].m_tileType = type;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::SetTileType(int tileX, int tileY, TileType type)
{
	int tileIndex = GetTileIndexForTileCoords(tileX, tileY);
	SetTileType(tileIndex, type);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::PopulateDistanceField(TileHeatMap& out_distanceField, IntVec2 referenceCoords, float maxCost)
{
// 	out_distanceField.SetAllValues(maxCost);  //Set tiles to be unreachably far away (ex: 99999) #ToDo How big should this be?
// 	out_distanceField.SetHeatValuesAt(referenceCoords, 0.f); // Put zero in start tile
// 	
// 	//Account for being outta bounds, not affecting lower number/ solid tiles when  hitting max tile distance
// 	for ( int passNum = 0; passNum < maxCost ; passNum++ )
// 	{
// 		float currentPassTileHeat = static_cast<float>(passNum);
// 		DoDistanceFieldPass(out_distanceField, currentPassTileHeat);
// 	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::DoDistanceFieldPass(TileHeatMap& distanceField, float currentPassTileHeat)
{
// 	for (int tileY = 0; tileY < distanceField.m_dimensions.y; tileY++)
// 	{
// 		for (int tileX = 0; tileX < distanceField.m_dimensions.x; tileX++ )
// 		{
// 			
// 			if (currentPassTileHeat == distanceField.GetHeatValuesAt(IntVec2(tileX,tileY) ) )
// 			{
// 				SpreadHeatToTile(distanceField,currentPassTileHeat + 1.f, IntVec2(tileX + 1, tileY));
// 				SpreadHeatToTile(distanceField, currentPassTileHeat + 1.f, IntVec2(tileX - 1, tileY));
// 				SpreadHeatToTile(distanceField, currentPassTileHeat + 1.f, IntVec2(tileX, tileY + 1));
// 				SpreadHeatToTile(distanceField, currentPassTileHeat + 1.f, IntVec2(tileX, tileY - 1));
// 			}
// 		}
// 		
// 	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::SpreadHeatToTile(TileHeatMap& distanceField, float nextTileHeat, IntVec2 currentTile)
{
// 	if (IsTileOutOfBounds(currentTile))
// 		return;
// 
// 
// 	if (IsTileSolid(currentTile))
// 		return;
// 
// 	if (distanceField.GetHeatValuesAt(currentTile) <= nextTileHeat)
// 		return;
// 
// 	distanceField.SetHeatValuesAt(currentTile, nextTileHeat);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::RenderDebugHeatMap() const
{
	std::vector<Vertex_PCU> heatMapTileVerts;
	for (int tileIndex = 0; tileIndex < m_tiles.size(); tileIndex++)
	{
		Tile const& currentTile = m_tiles[tileIndex];
		AABB2 currentTileBounds = currentTile.GetBounds();
		float heatValue = m_distanceFieldFromStart.m_values[tileIndex];
		float heatPercent = GetClampedZeroToOne(heatValue / 50.f);
		unsigned char colorByte = static_cast<unsigned char>(heatPercent * 255.f);
		Rgba8 heatColor = Rgba8(colorByte, colorByte, colorByte, 200);
		AddVertsForAABB2D(heatMapTileVerts, currentTileBounds, heatColor);
	}
	g_theRenderer->BindTexture(nullptr);
	g_theRenderer->SetBlendMode(BlendMode::ALPHA);
	g_theRenderer->DrawVertexArray((int)heatMapTileVerts.size(), heatMapTileVerts.data());
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::AddEntityToMap(Entity* entity)
{
	entity->m_map = this;
	AddEntityToList(entity, m_entityListsByType[entity->m_EntityType]);

	if (entity->IsActor())
	{
		AddEntityToList(entity, m_actorsByFaction[entity->m_faction]);
	}
	if (entity->IsProjectile())
	{
		AddEntityToList(entity, m_bulletsByFaction[entity->m_faction]);
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
IntVec2 Map::GetTileCoordsForWorldPos(Vec2 const& worldPos) const
{
	int tileX = RoundDownToInt(worldPos.x);
	int tileY = RoundDownToInt(worldPos.y);
	return IntVec2(tileX, tileY);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
AABB2 Map::GetTileBounds(IntVec2 const& tileCoords)
{
	AABB2 bounds;
	bounds.m_mins.x = static_cast<float>(tileCoords.x);
	bounds.m_mins.y = static_cast<float>(tileCoords.y);
	bounds.m_maxs.x = bounds.m_mins.x + 1.0f;
	bounds.m_maxs.y = bounds.m_mins.y + 1.0f;
	return bounds;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool Map::IsTileSolid(IntVec2 const& tileCoords) const
{
	int tileIndex = GetTileIndexForTileCoords(tileCoords.x, tileCoords.y);
	
	if(tileIndex < 0 || tileIndex > (m_dimensions.x * m_dimensions.y))
		return false;

	if (m_tiles[tileIndex].m_tileType == STONE_WALL)
	{
		return true;
	}
	
	else
		return false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool Map::IsPointInSolidTile(Vec2 const& worldPos) const
{
	IntVec2 tileCoords = GetTileCoordsForWorldPos(worldPos);
	return IsTileSolid(tileCoords);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool Map::IsTileOutOfBounds(IntVec2 const& tileCoords) const
{
	return (tileCoords.x < 0 || tileCoords.y < 0 || tileCoords.x >= m_dimensions.x || tileCoords.y >= m_dimensions.y);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool Map::HasLineOfSight(Vec2 const& posA, Vec2 const& posB) const
{
	Vec2 dispFromAToB = posB - posA;
	float dist = dispFromAToB.NormalizeAndGetPreviousLength();
	RaycastResult2D result = RaycastVsTiles(posA, dispFromAToB, dist);
	return !result.m_didImpact;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
Entity* Map::CreateNewEntityOfType(EntityType type, Vec2 const& position, float orientationDegrees)
{
	switch (type)
	{
		case ENTITY_TYPE_GOOD_PLAYER: return new Player(type, FACTION_GOOD, this, position, orientationDegrees);
		case ENTITY_TYPE_GOOD_BULLET: return new Bullets(type, FACTION_GOOD, this, position, orientationDegrees);
		case ENTITY_TYPE_EVIL_BULLET: return new Bullets(type, FACTION_EVIL, this, position, orientationDegrees);
		case ENTITY_TYPE_EVIL_LEO: return new Leo(type, FACTION_EVIL, this, position, orientationDegrees);
		case ENTITY_TYPE_EVIL_SCORPIO: return new Scorpio(type, FACTION_EVIL, this, position, orientationDegrees);
		case ENTITY_TYPE_EVIL_ARIES: return new Aries(type, FACTION_EVIL, this, position, orientationDegrees);
	}

	return nullptr;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
EntityList const& Map::GetEntitiesByType(EntityType type) const
{
	return m_entityListsByType[type];
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
Entity* Map::SpawnNewEntityOfType(EntityType entityType, Vec2 const& position, float orientationDegrees)
{
	Entity* newEntity = CreateNewEntityOfType(entityType, position, orientationDegrees);
	
	if (newEntity)
	{
		AddEntityToMap(newEntity);
	}
	
	return newEntity;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
RaycastResult2D Map::RaycastVsTiles_STEP_AND_SIMPLE(Vec2 startPos, Vec2 forwardDir, float maxDist) const
{
	RaycastResult2D result;
	constexpr int NUM_STEP_PER_TILE = 100;
	constexpr float FORWARD_STEP_LENGTH = 1.f / (float)NUM_STEP_PER_TILE;
	float distTravelledSoFar = 0.f;

	while (distTravelledSoFar < maxDist)
	{
		Vec2 currentStepPos = startPos + (forwardDir * distTravelledSoFar);
		if (IsPointInSolidTile(currentStepPos))
		{
			result.m_didImpact = true;
			result.m_impactDist = distTravelledSoFar;
			result.m_impactPos = currentStepPos;
			return result;
		}
		distTravelledSoFar += FORWARD_STEP_LENGTH;
	}
	result.m_didImpact = false;
	result.m_impactDist = maxDist;
	result.m_impactPos = startPos + (forwardDir * maxDist);
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
RaycastResult2D Map::RaycastVsTiles(Vec2 startPos, Vec2 forwardDir, float maxDist) const
{
	return RaycastVsTiles_STEP_AND_SIMPLE( startPos, forwardDir, maxDist);
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::PopulateTiles()
{
	int numOfTiles = m_dimensions.x * m_dimensions.y;
	m_tiles.resize(numOfTiles);
	//Give Tiles their coordinates
	for (int tileIndex = 0; tileIndex < numOfTiles; tileIndex++)
	{
		Tile& tile = m_tiles[tileIndex];
		int yCoord = tileIndex / m_dimensions.x;
		int xCoord = tileIndex % m_dimensions.x;

		tile.m_tileCoords = IntVec2(xCoord, yCoord);
	}

	//Set Border Tiles To Stone
	for (int tileX = 0; tileX < m_dimensions.x; tileX++)
	{
		SetTileType(tileX, 0, STONE_WALL);
		SetTileType(tileX, m_dimensions.y - 1, STONE_WALL);
	}

	for (int tileY = 0; tileY < m_dimensions.y; tileY++)
	{
		SetTileType(0, tileY, STONE_WALL);
		SetTileType(m_dimensions.x - 1, tileY, STONE_WALL);
	}

	//Sprinkle Random Stone throughout
	int numStoneSprinkle = numOfTiles / 15;
	for (int i = 0; i < numStoneSprinkle; i++)
	{
		int tileX = g_rng.RollRandomIntInRange(0, m_dimensions.x - 1);
		int tileY = g_rng.RollRandomIntInRange(0, m_dimensions.y - 1);
		SetTileType(tileX, tileY, STONE_WALL);
	}

	//Entry and exit
	SetTileType(1, 1, MAP_ENTRY);
	SetTileType(m_exitPos.x, m_exitPos.y, MAP_EXIT);
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::PopulateHeatMaps()
{
	PopulateDistanceField(m_distanceFieldFromStart, IntVec2(2, 2), maxMapDist);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool Map::IsValid()
{
// 	float disToExit = m_distanceFieldFromStart.GetHeatValuesAt(m_exitPos);
// 	if (disToExit == maxMapDist)
// 	{
// 		return false;
// 	}
 	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::AddEntityToList(Entity* entity, EntityList& list)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == nullptr)
		{
			list[i] = entity;
			return;
		}
	}
	
	list.push_back(entity);  // if there were no empty slots, grow the list.
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::UpdateEntities(float deltaSeconds)
{
	for (int entityType = 0; entityType < NUM_ENTITY_TYPES; entityType ++)
	{
		EntityList& entityList = m_entityListsByType[entityType];
		for (int i = 0; i < (int)entityList.size(); i++)
		{
			Entity* e = entityList[i];
			if (e)
			{
				e->Update(deltaSeconds);
			}
		}
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::PushAllEntitiesOutOfWalls()
{
	for (int entityType = 0; entityType < NUM_ENTITY_TYPES; entityType++)
	{
		EntityList& entityList = m_entityListsByType[entityType];
		for (int i = 0; i < (int)entityList.size(); i++)
		{
			Entity* e = entityList[i];
			if (e != nullptr)
			{
				PushEntityOutOfWalls(*e);
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void Map::PushEntityOutOfWalls(Entity& entity )
{
	if (!entity.m_isPushedByWalls)
		return;

	IntVec2 myTileCoords = GetTileCoordsForWorldPos(entity.m_position);

	if (m_game->g_noClip)
	{
		return;
	}

	PushEntityOutOfTileIfSolid(entity, myTileCoords + STEP_EAST);
	PushEntityOutOfTileIfSolid(entity, myTileCoords + STEP_WEST);
	PushEntityOutOfTileIfSolid(entity, myTileCoords + STEP_NORTH);
	PushEntityOutOfTileIfSolid(entity, myTileCoords + STEP_SOUTH);

	PushEntityOutOfTileIfSolid(entity, myTileCoords + STEP_NORTHEAST);
	PushEntityOutOfTileIfSolid(entity, myTileCoords + STEP_NORTHWEST);
	PushEntityOutOfTileIfSolid(entity, myTileCoords + STEP_SOUTHEAST);
	PushEntityOutOfTileIfSolid(entity, myTileCoords + STEP_SOUTHWEST);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::PushEntityOutOfTileIfSolid(Entity& entity, IntVec2 const& tileCoords)
{
	if (!IsTileSolid(tileCoords))
	{
		return;
	}
	AABB2 tileBounds = GetTileBounds(tileCoords);
	Vec2 nearestPointOnTile = tileBounds.GetNearestPoint(entity.m_position);
	PushDiscOutOfFixedPoint2D(entity.m_position, entity.m_physicsRadius, nearestPointOnTile);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::AddVertsForTile(std::vector<Vertex_PCU>& verts, int tileIndex) const
{
	Tile const& tile = m_tiles[tileIndex];
	AABB2 bounds = tile.GetBounds();
	Rgba8 color = tile.GetColor();
	TileDefinition const& tileDef = TileDefinition::GetDef(tile.m_tileType);
	AABB2 UVs = tileDef.m_UVs;
	AddVertsForAABB2D(verts, bounds, color, UVs );
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::RenderTiles() const
{
	Texture const& terrainSprite = g_terrainSprites->GetTexture();
	g_theRenderer->BindTexture(&terrainSprite);
	
	int initialVertexCount = 3 * 2 * m_dimensions.x * m_dimensions.y;
	std::vector<Vertex_PCU> tileVerts;
	tileVerts.reserve(initialVertexCount);

	for (int tileIndex = 0; tileIndex < (int)m_tiles.size(); tileIndex++)
	{
		AddVertsForTile(tileVerts, tileIndex);
	}
	g_theRenderer->DrawVertexArray((int)tileVerts.size(), tileVerts.data());
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::RenderEntities() const
{
	for (int entityType = 0; entityType < NUM_ENTITY_TYPES; entityType++)
	{
		EntityList const& entityList = m_entityListsByType[entityType];
		for (int i = 0; i < (int)entityList.size(); i++)
		{
			Entity* e = entityList[i];
			if (e)
			{
				e->Render();
			}
		}
	}

	RenderDebugDrawEntities();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::RenderDebugDrawEntities() const
{
	if (m_debugDraw)
	{
		for (int entityType = 0; entityType < NUM_ENTITY_TYPES; entityType++)
		{
			EntityList const& entityList = m_entityListsByType[entityType];
			for (int i = 0; i < (int)entityList.size(); i++)
			{
				Entity* e = entityList[i];
				if (e)
				{
					e->DebugRender();
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::PushEntitiesOutOfEachOther(Entity& a, Entity& b) const
{
	bool canAPushB = a.m_doesPushEntities && b.m_isPushedByEntites;
	bool canBPushA = b.m_doesPushEntities && a.m_isPushedByEntites;

	if (!canAPushB && !canBPushA)
	{
		return;
	}

	if (canAPushB && canBPushA)
	{
		PushDiscsOutOfEachOther2D(a.m_position, a.m_physicsRadius, b.m_position, b.m_physicsRadius);
	}

	else if (canAPushB)
	{
		PushDiscOutOfFixedDisc2D(b.m_position, b.m_physicsRadius, a.m_position, a.m_physicsRadius);
	}

	else if(canBPushA)
	{
		PushDiscOutOfFixedDisc2D(a.m_position, a.m_physicsRadius, b.m_position, b.m_physicsRadius);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::RemoveEntityFromMap(Entity* entity)
{
	entity->m_map = nullptr;
	RemoveEntityFromList(entity, m_entityListsByType[entity->m_EntityType]);

	if (entity->IsActor())
	{
		RemoveEntityFromList(entity, m_actorsByFaction[entity->m_faction]);
	}
	if (entity->IsProjectile())
	{
		RemoveEntityFromList(entity, m_bulletsByFaction[entity->m_faction]);
	}

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::DeleteGarbageEntities()
{
	for (int entityType = 0; entityType < NUM_ENTITY_TYPES; entityType++)
	{
		EntityList const& entityList = m_entityListsByType[entityType];
		for (int i = 0; i < (int)entityList.size(); i++)
		{
			Entity* e = entityList[i];
			if (e)
			{
				if (e->m_isGarbage)
				{
					RemoveEntityFromMap(e);
					delete e;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::RemoveEntityFromList(Entity* entity, EntityList& list)
{
	for (int i = 0; i < list.size(); i++)
	{
		if (list[i] == entity)
		{
			list[i] = nullptr;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::CheckBulletVsActor(Bullets& bullet, Entity& actor)
{
	if (bullet.m_faction == actor.m_faction)
		return;

	if (!DoEntitiesOverlap(bullet, actor))
		return;

	actor.ReactToBulletHit(bullet);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool Map::DoEntitiesOverlap(Entity& a, Entity& b)
{
	if (DoDiscsOverlap(a.m_position, a.m_physicsRadius, b.m_position, b.m_physicsRadius))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::DoEntityCollisions()
{
	for (int listIndexA = 0; listIndexA < NUM_ENTITY_TYPES; listIndexA++)
	{
		for (int listIndexB = 0; listIndexB < NUM_ENTITY_TYPES; listIndexB++)
		{
			CheckCollisionBetweenEntityLists(m_entityListsByType[listIndexA], m_entityListsByType[listIndexB]);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::CheckCollisionBetweenEntityLists(EntityList& listA, EntityList& listB)
{
	for (int listIndexA = 0; listIndexA < listA.size(); listIndexA++)
	{
		for (int listIndexB = 0; listIndexB < listB.size(); listIndexB++)
		{	
			if (listA[listIndexA] && listB[listIndexB])
			{
				if (listA[listIndexA] != listB[listIndexB])
				{
					Entity& a = *listA[listIndexA];
					Entity& b = *listB[listIndexB];
					PushEntitiesOutOfEachOther(a, b);
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::CheckForBulletHits()
{
	for (int bulletFaction = 0; bulletFaction < NUM_FACTIONS; bulletFaction++)
	{
		EntityList& bulletsList = m_bulletsByFaction[bulletFaction];
		for (int actorFaction = 0; actorFaction < NUM_FACTIONS; actorFaction++)
		{
			if (actorFaction == bulletFaction)
			{
				continue; //Bullets don't hurt actors of the same faction.
			}
			EntityList& actorList = m_actorsByFaction[actorFaction];
			CheckBulletsListVsActorList(bulletsList, actorList);
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Map::CheckBulletsListVsActorList(EntityList& bulletsList, EntityList& actorList)
{
 	for (int bulletIndex = 0; bulletIndex < bulletsList.size(); bulletIndex++)
 	{
 		Entity* bullet = bulletsList[bulletIndex];
		if (bullet == nullptr)
		{
			continue;
		}
 		
		for (int actorIndex = 0; actorIndex < actorList.size(); actorIndex++)
 		{
 			Entity* actor = actorList[actorIndex];
			if (actor == nullptr)
			{
				continue;
			}
 			
			if (DoEntitiesOverlap(*bullet,*actor))
 			{
				Bullets* asBullet = static_cast<Bullets*>(bullet);
				actor->ReactToBulletHit(*asBullet);
 			}
 		}
 	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
 Entity* Map::GetClosestVisibleEntityOfType(Vec2 const& refPos, EntityType type, float maxDist) 
  {
	 Entity* closestEntity = nullptr;
	 EntityList& list = m_entityListsByType[type];
	 for (int listIndex = 0; listIndex < list.size(); listIndex++)
	 {
		 Entity* e = list[listIndex];
		 if (e != nullptr)
		 {
			 float distanceToEntityPos = GetDistance2D(refPos, e->m_position);
			 float closestEntityDistance = maxDist;
			 if (distanceToEntityPos < closestEntityDistance)
			 {
				 if(HasLineOfSight(refPos, e->m_position))
				 {
					 closestEntity = e;
					 return closestEntity;
				 }
			 }
		 }
		 else
		 {
			 return closestEntity;
		 }
		 
	 }
	 return closestEntity;
  }

 //--------------------------------------------------------------------------------------------------------------------------------------------------------
 Entity* Map::GetClosestVisibleEntityOfFaction(Vec2 const& refPos, EntityFaction faction, float maxDist)
 {
	 Entity* closestEntity = nullptr;
	 EntityList& factionList = m_actorsByFaction[faction];
	 for (int listIndex = 0; listIndex < factionList.size(); listIndex++)
	 {
		 Entity* e = factionList[listIndex];
		 if (e != nullptr)
		 {
			 float distanceToEntityPos = GetDistance2D(refPos, e->m_position);
			 float closestEntityDistance = maxDist;
			 if (distanceToEntityPos < closestEntityDistance)
			 {
				 if (HasLineOfSight(refPos, e->m_position))
				 {
					 closestEntity = e;
					 return closestEntity;
				 }
			 }
		 }
		 else
		 {
			 return closestEntity;
		 }
	 }
	 return closestEntity;
 }
//--------------------------------------------------------------------------------------------------------------------------------------------------------
 void Map::UpdateCamera()
 {
	 if (m_debugCamera)
	 {
		 DebugCamera();
	 }

	 else
	 {
		 float camHeight = m_numTilesinViewVertically;
		 float camWidth = camHeight * 2.0f;

		 Vec2 camPos(camWidth * 0.5f, camHeight * 0.5f);

		 Vec2 topRight = m_player->m_position + camPos;
		 Vec2 bottomLeft = m_player->m_position - camPos;

		 IntVec2 mapDimensions = m_dimensions;

		 //clamp camera so it doesn't go out of bounds
		 if (topRight.y > mapDimensions.y)
		 {
			 topRight.y = (float)mapDimensions.y;
			 bottomLeft.y = mapDimensions.y - camHeight;
		 }

		 if (topRight.x > mapDimensions.x)
		 {
			 topRight.x = (float)mapDimensions.x;
			 bottomLeft.x = mapDimensions.x - camWidth;
		 }

		 if (bottomLeft.x < 0.f)
		 {
			 bottomLeft.x = 0.f;
			 topRight.x = camWidth;
		 }

		 if (bottomLeft.y < 0.f)
		 {
			 bottomLeft.y = 0.f;
			 topRight.y = camHeight;
		 }

		 m_worldCamera.m_mode = Camera::eMode_Orthographic;
		 m_worldCamera.SetOrthographicView(bottomLeft, topRight);
	 }
 }

 //--------------------------------------------------------------------------------------------------------------------------------------------------------
 void Map::DebugCamera()
 {
	 if (m_debugCamera)
	 {
		 if (m_dimensions.x > m_dimensions.y)
		 {
			 float camWidth = (float)m_dimensions.x;
			 float camHeight = camWidth / 2.0f;
			 m_worldCamera.m_mode = Camera::eMode_Orthographic;
			 m_worldCamera.SetOrthographicView(Vec2(0.f, 0.f), Vec2(camWidth, camHeight));
		 }

		 else
		 {
			 float camHeight = (float)m_dimensions.y;
			 float camWidth = camHeight * 2.0f;
			 m_worldCamera.m_mode = Camera::eMode_Orthographic;
			 m_worldCamera.SetOrthographicView(Vec2(0.f, 0.f), Vec2(camWidth, camHeight));
		 }
	 }
 }
 //--------------------------------------------------------------------------------------------------------------------------------------------------------
