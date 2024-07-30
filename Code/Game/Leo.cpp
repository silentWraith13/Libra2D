#include "Game/Leo.hpp"
#include "Game/Map.hpp"
#include "Game/Bullets.hpp"

Leo::Leo(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees)
	:Entity(type, faction, owner, initialPosition, orientationDegrees)
{
	faction = FACTION_EVIL;
	m_position = initialPosition;
	type = ENTITY_TYPE_EVIL_LEO;
	m_health = 3;
	m_damage = 1;
	m_isPushedByEntites = true;
	m_doesPushEntities = true;
	m_isHitByBullets = true;
	m_isActor = true;
	m_isProjectile = false;
	m_isPushedByWalls = true;
	m_physicsRadius = 0.3f;
	m_cosmeticRadius = 0.8f;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
Leo::~Leo()
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Leo::Update(float deltaSeconds)
{
	EntityFaction enemyFaction = GetOppositeFaction();
	Entity* visibleTarget = m_map->GetClosestVisibleEntityOfFaction(m_position, enemyFaction, m_visibleRange);
	m_velocity = Vec2(0.f, 0.f);

	//Attack mode
	if (visibleTarget)
	{
		m_secondsSinceFired += deltaSeconds;
		Vec2 targetPos = visibleTarget->m_position;
		TurnTowardsPosition(targetPos, 100.0f * deltaSeconds);
		if (m_secondsSinceFired >= LEO_SHOOT_COOLDOWN)
		{
			if (IsPointInsideOrientedSector2D(targetPos, m_position, m_orientationDegrees, 45.0f, m_visibleRange))
			{
				//Drive towards enemy
				m_velocity = GetForwardNormal() * LEO_MOVEMENT_SPEED;
				//Shoot if in +/-5
				if (IsPointInsideOrientedSector2D(targetPos, m_position, m_orientationDegrees, 5.0f, m_visibleRange))
				{
					Shoot();
				}
			}

		}
	}

	//Wander mode
	if (visibleTarget == nullptr)
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
void Leo::Render() const
{
	AABB2 localSpaceBounds(-0.5f, -0.5f, 0.5f, 0.5f);
	std::vector<Vertex_PCU> verts;
	AddVertsForAABB2D(verts, localSpaceBounds, Rgba8(255, 255, 255));
	TransformVertexArrayXY3D((int)verts.size(), verts.data(), 1.f, m_orientationDegrees, m_position);
	Texture* leoTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/EnemyTank4.png");
	g_theRenderer->BindTexture(leoTexture);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Leo::DebugRender() const
{
	Entity::DebugRender();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Leo::Die()
{
	Entity::Die();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Leo::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Leo::TurnTowardsPosition(Vec2 const& targetPos, float maxDeltaDegrees)
{
	Entity::TurnTowardsPosition(targetPos, maxDeltaDegrees);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Leo::ReactToBulletHit(Bullets& bullet)
{
	Entity::ReactToBulletHit(bullet);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Leo::Shoot()
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
