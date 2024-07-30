#include "Game/Entities.hpp"
#include "Game/Bullets.hpp"



Entity::Entity(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees)
	:m_map(owner)
	, m_position(initialPosition)
	,m_orientationDegrees(orientationDegrees)
	,m_faction(faction)
	,m_EntityType(type)
{
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
Entity::~Entity()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Entity::Update(float deltaSeconds)
{
	m_position += (m_velocity * deltaSeconds);
	m_orientationDegrees += (m_angularVelocity * deltaSeconds);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Entity::DebugRender() const
{
		float lineThickness = 0.01f;
		DebugDrawRing(m_position, m_physicsRadius, lineThickness, Rgba8(0, 255, 255));
		DebugDrawRing(m_position, m_cosmeticRadius, lineThickness, Rgba8(255, 0, 255));
		DebugDrawLine(m_position, m_position + GetForwardNormal() * m_physicsRadius, lineThickness, Rgba8(255, 0, 0));
		DebugDrawLine(m_position, m_position + GetForwardNormal().GetRotated90Degrees() * m_physicsRadius, lineThickness, Rgba8(0, 255, 0));
		DebugDrawLine(m_position, m_velocity + m_position, lineThickness * 0.5f, Rgba8(255, 255, 0));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Entity::Die()
{
	m_isDead = true;
	m_isGarbage = true;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Entity::TakeDamage(int damage)
{
	if (damage > 0)
	{
		m_health -= damage;
		if (m_health <= 0)
		{
			Die();
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Entity::TurnTowardsPosition(Vec2 const& targetPos, float maxDeltaDegrees)
{
	Vec2 dispToTarget = targetPos - m_position;
	float orientationToTarget = dispToTarget.GetOrientationDegrees();
	m_orientationDegrees = GetTurnedTowardDegrees(m_orientationDegrees, orientationToTarget, maxDeltaDegrees);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Entity::ReactToBulletHit(Bullets& bullet)
{
   	TakeDamage(bullet.m_damage);
	bullet.Die();
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool Entity::IsActor() const
{
	if (m_EntityType == ENTITY_TYPE_EVIL_ARIES || m_EntityType == ENTITY_TYPE_GOOD_PLAYER || m_EntityType == ENTITY_TYPE_EVIL_LEO || m_EntityType == ENTITY_TYPE_EVIL_SCORPIO)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
bool Entity::IsProjectile() const
{
	if (m_EntityType == ENTITY_TYPE_GOOD_BULLET || m_EntityType == ENTITY_TYPE_EVIL_BULLET)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
EntityFaction Entity::GetOppositeFaction() const
{
	if (m_faction == FACTION_GOOD)
	{
		return FACTION_EVIL;
	}

	if (m_faction == FACTION_EVIL)
	{
		return FACTION_GOOD;
	}

	return m_faction;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
Vec2 Entity::GetForwardNormal() const
{
	return Vec2(CosDegrees(m_orientationDegrees), SinDegrees(m_orientationDegrees));
}

