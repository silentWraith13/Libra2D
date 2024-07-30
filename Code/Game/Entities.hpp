#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Game/GameCommon.hpp"
#include <vector>

//--------------------------------------------------------------------------------------------------------------------------------------------------------
class Game;
class Map;
class Entity;
class Bullets;
//--------------------------------------------------------------------------------------------------------------------------------------------------------
typedef std::vector<Entity*> EntityList;
//--------------------------------------------------------------------------------------------------------------------------------------------------------
 enum EntityFaction
{
	 FACTION_INVALID = -1,
	 FACTION_GOOD,
	 FACTION_NEUTRAL,
	 FACTION_EVIL,

	 NUM_FACTIONS
};
 //--------------------------------------------------------------------------------------------------------------------------------------------------------
 enum EntityType
 {
	 ENTITY_TYPE_GOOD_PLAYER,
	 ENTITY_TYPE_EVIL_SCORPIO,
	 ENTITY_TYPE_EVIL_LEO,
	 ENTITY_TYPE_EVIL_ARIES,
	 ENTITY_TYPE_GOOD_BULLET,
	 ENTITY_TYPE_EVIL_BULLET,
	
	 NUM_ENTITY_TYPES
 };
 //--------------------------------------------------------------------------------------------------------------------------------------------------------
class Entity
{
public:
	Entity( EntityType type, EntityFaction faction, Map* owner,Vec2 const& initialPosition, float orientationDegrees);
	virtual ~Entity();
	
	virtual void     Update(float deltaSeconds) = 0;
	virtual void     Render() const = 0;
	virtual void     DebugRender() const ;
	virtual void     Die() ;
	virtual void     TakeDamage(int damage);
	virtual void	 TurnTowardsPosition(Vec2 const& targetPos, float maxDeltaDegrees);
	virtual void	 ReactToBulletHit(Bullets& bullet);
	
	bool			 IsActor() const;
	bool             IsProjectile() const;
	
	EntityFaction    GetOppositeFaction() const;
	
	Vec2             GetForwardNormal() const;
	Vec2             m_position;
	Vec2             m_velocity;
	
	float            m_orientationDegrees = 0.f;
	float            m_angularVelocity = 0.f;
	float            m_cosmeticRadius = 0.f;
	float            m_physicsRadius = 0.f;
	float			 m_visibleRange = 10.0f;
	
	int              m_health = 0;
	
	bool             m_isGarbage = false;
	bool			 m_isDead = false;
	bool             m_doesPushEntities;
	bool			 m_isPushedByEntites;
	bool			 m_isPushedByWalls = true;
	bool			 m_isHitByBullets;
	bool			 m_isProjectile = false;
	bool			 m_isActor = true;
	int				 m_damage = 0;
	float		     m_secondsSinceFired = 100.f;
	EntityFaction    m_faction;
	EntityType		 m_EntityType;

	Map* m_map = nullptr;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------