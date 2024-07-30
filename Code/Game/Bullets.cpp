#include "Game/Bullets.hpp"
#include "Game/Map.hpp"

Bullets::Bullets(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees)
	:Entity(type, faction, owner, initialPosition, orientationDegrees)
{
	m_velocity = Vec2::MakeFromPolarDegrees(orientationDegrees) * 5.0f;
	m_physicsRadius = 0.1f;
	m_isPushedByEntites = false;
	m_doesPushEntities = false;
	m_isHitByBullets = false;
	m_isPushedByWalls = false;
	m_isActor = false;
	m_isProjectile = true;
	m_health = 1;
	m_damage = 1;

	switch (m_EntityType)
	{
		case ENTITY_TYPE_GOOD_BULLET:
		{
			m_bulletTexure = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/FriendlyBullet.png");
			m_health = 3;
			
			break;
		}

		case ENTITY_TYPE_EVIL_BULLET:
		{
			m_bulletTexure = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/EnemyShell.png");
			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
Bullets::~Bullets()
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Bullets::Update(float deltaSeconds)
{
	Vec2 prevPos = m_position;
	m_position += m_velocity * deltaSeconds;

	IntVec2 prevTileCoords = m_map->GetTileCoordsForWorldPos(prevPos);
	IntVec2 nextTileCoords = m_map->GetTileCoordsForWorldPos(m_position);
	
  	if ( m_map->IsPointInSolidTile(m_position) )
  	{
 		m_position = prevPos;
 		Vec2 bounceNormal = Vec2(prevTileCoords - nextTileCoords).GetNormalized();
 		BounceOff(bounceNormal);
 	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Bullets::Render() const
{
	AABB2 localBounds(-0.5f, -0.5f, 0.5f, 0.5f);
	std::vector<Vertex_PCU> verts;
	AddVertsForAABB2D(verts, localBounds, Rgba8(255, 255, 255));	
	TransformVertexArrayXY3D((int)verts.size(), verts.data(), 0.1f, m_orientationDegrees, m_position);
	g_theRenderer->BindTexture(m_bulletTexure);
	g_theRenderer->DrawVertexArray((int)verts.size(), verts.data());
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Bullets::DebugRender() const
{
	DebugDrawRing(m_position, m_physicsRadius, 0.1f, Rgba8(0,255,255));
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Bullets::Die()
{
	Entity:: Die();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Bullets::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Bullets::BounceOff(Vec2 const& bounceNormal)
{
	TakeDamage(1);
	
	if (m_isDead)
	{
		return;
	}

	m_velocity.Reflect(bounceNormal);
	m_orientationDegrees = m_velocity.GetOrientationDegrees();
}

