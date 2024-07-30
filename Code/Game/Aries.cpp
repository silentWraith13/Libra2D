#include "Game/Aries.hpp"
#include "Game/Bullets.hpp"
#include "Game/Map.hpp"

Aries::Aries(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees)
	:Entity(type, faction, owner, initialPosition, orientationDegrees)
{
	faction = FACTION_EVIL;
	m_position = initialPosition;
	type = ENTITY_TYPE_EVIL_ARIES;
	m_isActor = true;
	m_isProjectile = false;
	m_health = 3;
	m_damage = 1;
	m_isPushedByEntites = true;
	m_doesPushEntities = true;
	m_isHitByBullets = true;
	m_isPushedByWalls = true;
	m_physicsRadius = 0.3f;
	m_cosmeticRadius = 0.8f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
Aries::~Aries()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Aries::Update(float deltaSeconds)
{
	EntityFaction enemyFaction = GetOppositeFaction();
	Entity* visibleTarget = m_map->GetClosestVisibleEntityOfFaction(m_position, enemyFaction, m_visibleRange);
 	m_velocity = Vec2(0.f, 0.f);
   	
	//Attack mode
	if (visibleTarget)
	{
		Vec2 targetPos = visibleTarget->m_position;
		TurnTowardsPosition(targetPos, 100.0f * deltaSeconds);
		if (IsPointInsideOrientedSector2D(targetPos, m_position, m_orientationDegrees, 45.0f, m_visibleRange))
		{
			//Drive towards enemy
			m_velocity = GetForwardNormal() * ARIES_MOVEMENT_SPEED;
		}
	}
   	
	//Wander mode
   	if( visibleTarget == nullptr ) 
    {
		randomizeTick += deltaSeconds;
		if (randomizeTick >= 1.0f)
		{		
			m_goalOrientationDegrees = g_rng.RollRandomFloatInRange(0.f, 360.f);
			randomizeTick = 0.f;
 		}
		m_orientationDegrees = GetTurnedTowardDegrees(m_orientationDegrees, m_goalOrientationDegrees, TANK_TURN_SPEED * deltaSeconds);
		m_velocity = GetForwardNormal() * ARIES_MOVEMENT_SPEED;
	}

	Entity::Update(deltaSeconds);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Aries::Render() const
{
	AABB2 localSpaceBounds(-0.5f, -0.5f, 0.5f, 0.5f);
	std::vector<Vertex_PCU> verts;
	AddVertsForAABB2D(verts, localSpaceBounds, Rgba8(255, 255, 255));
	TransformVertexArrayXY3D((int)verts.size(), verts.data(), 1.f, m_orientationDegrees, m_position);
	Texture* ariesTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/EnemyAries.png");
	g_theRenderer->BindTexture(ariesTexture);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void Aries::DebugRender() const
{
	Entity::DebugRender();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Aries::Die()
{
	Entity::Die();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Aries::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Aries::TurnTowardsPosition(Vec2 const& targetPos, float maxDeltaDegrees)
{
	Entity::TurnTowardsPosition(targetPos, maxDeltaDegrees);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Aries::ReactToBulletHit(Bullets& bullet)
{
	Vec2 fwdNormal = GetForwardNormal();
	Vec2 dispToBullet = bullet.m_position - m_position;
	if (GetAngleDegreesBetweenVectors2D(dispToBullet, fwdNormal) < 45.0f)
	{
		//Deflect if infront of shield
		PushDiscOutOfFixedDisc2D(bullet.m_position, bullet.m_physicsRadius, m_position, m_physicsRadius);
		Vec2 bounceNormal = dispToBullet.GetNormalized();
		bullet.BounceOff(bounceNormal);
	}
	else
	{
		Entity::ReactToBulletHit(bullet);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

