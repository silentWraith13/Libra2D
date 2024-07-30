#include "Game/Scorpio.hpp"
#include "Game/Entities.hpp"
#include "Game/Map.hpp"
#include "Engine/Math/MathUtils.hpp"

Scorpio::Scorpio(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees)
	:Entity(type, faction, owner, initialPosition, orientationDegrees)
{
	m_position = initialPosition;
	m_physicsRadius = 0.3f;
	m_cosmeticRadius = 0.8f;
	type = ENTITY_TYPE_EVIL_SCORPIO;
	m_health = 3;
	faction = FACTION_EVIL;
	m_damage = 1;
	m_isPushedByEntites = false;
	m_doesPushEntities = true;
	m_isHitByBullets = true;
	m_isActor = true;
	m_isProjectile = false;
	m_isPushedByWalls = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
Scorpio::~Scorpio()
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Scorpio::Update(float deltaSeconds)
{
	m_secondsSinceFired += deltaSeconds;
	EntityFaction enemyFaction = GetOppositeFaction();
	Entity* visibleTarget = m_map->GetClosestVisibleEntityOfFaction(m_position, enemyFaction, m_visibleRange);
	if (visibleTarget)
	{
		Vec2 targetPos = visibleTarget->m_position;
		TurnTowardsPosition(targetPos, 100.0f * deltaSeconds);
		if (m_secondsSinceFired >= SCORPIO_SHOOT_COOLDOWN)
		{
			if (IsPointInsideOrientedSector2D(targetPos, m_position, m_orientationDegrees, 5.0f, m_visibleRange))
			{
				Shoot();
			}
		}
	}
	else
	{
		m_orientationDegrees += 90.f * deltaSeconds;
	}
	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Scorpio::Render() const
{
	Vec2 forwardNormal = GetForwardNormal();
 	RaycastResult2D laser = m_map->RaycastVsTiles(m_position, forwardNormal, 5.f);
 	g_theRenderer->BindTexture(nullptr);
 	
	float endAlphaFloat = 1.f - GetClampedZeroToOne(laser.m_impactDist / 5.f);
	unsigned char endAlpha = (unsigned char)GetClamped(255.f * endAlphaFloat, 0.f, 255.f);
 	DebugDrawLine(m_position, laser.m_impactPos, 0.09f,  Rgba8(255, 0, 0, endAlpha));

	//Draw turret and base
	AABB2 localBounds(-0.5f, -0.5f, 0.5f, 0.5f);
	std::vector<Vertex_PCU> tankVerts;
	std::vector<Vertex_PCU> turretVerts;

	AddVertsForAABB2D(tankVerts, localBounds, Rgba8(255, 255, 255));
	TransformVertexArrayXY3D((int)tankVerts.size(), tankVerts.data(), 1.f, 0.f, m_position);
	Texture* bodyTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/EnemyTurretBase.png");
	g_theRenderer->BindTexture(bodyTexture);
	g_theRenderer->DrawVertexArray((int)tankVerts.size(), tankVerts.data());
	
	AddVertsForAABB2D(turretVerts, localBounds, Rgba8(255, 255, 255));
	TransformVertexArrayXY3D((int)turretVerts.size(), turretVerts.data(), 1.f, m_orientationDegrees, m_position);
 	Texture* turretTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/EnemyCannon.png");
 	g_theRenderer->BindTexture(turretTexture);
	g_theRenderer->DrawVertexArray((int)turretVerts.size(), turretVerts.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Scorpio::DebugRender() const
{
	Entity::DebugRender();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Scorpio::Die()
{
	Entity::Die();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Scorpio::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Scorpio::TurnTowardsPosition(Vec2 const& targetPos, float maxDeltaDegrees)
{
	Entity::TurnTowardsPosition(targetPos, maxDeltaDegrees);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Scorpio::ReactToBulletHit(Bullets& bullet)
{
	Entity::ReactToBulletHit(bullet);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Scorpio::Shoot()
{
	float gunOrientation = m_orientationDegrees;
	Vec2 gunForwardNormal = Vec2::MakeFromPolarDegrees(gunOrientation);
	float spawnDistance = m_physicsRadius + 0.01f;
	Vec2 spawnPos = m_position + gunForwardNormal * spawnDistance;
	
	if (m_map->IsPointInSolidTile(spawnPos) && spawnDistance > 0.f)
	{
		spawnDistance -= 0.01f;
		spawnPos = m_position + gunForwardNormal * spawnDistance;
	}

	m_map->SpawnNewEntityOfType(ENTITY_TYPE_EVIL_BULLET, spawnPos, gunOrientation);
	m_secondsSinceFired = 0.f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
