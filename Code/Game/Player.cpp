#include "Game/Player.hpp"
#include "Game/Game.hpp"



Player::Player(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees)
	:Entity(type,faction,owner, initialPosition,orientationDegrees)
{
	m_physicsRadius = PLAYER_PHYSICS_RADIUS;
	m_cosmeticRadius = PLAYER_COSMETIC_RADIUS;
	m_position = initialPosition;
	InitializeLocalVertsTankBody();
	InitializeLocalVertsTurret();
	m_isPushedByEntites = true;
	m_doesPushEntities = true;
	m_isHitByBullets = true;
	m_isActor = true;
	m_isProjectile = false;
	m_isPushedByWalls = true;
	m_health = 100;
	type = ENTITY_TYPE_GOOD_PLAYER;
	faction = FACTION_GOOD;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
Player::~Player()
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::Update(float deltaSeconds)
{
	if (m_isDead)
	{
		if (g_theInput->WasKeyJustPressed('N'))
		{
			m_isDead = false;
			m_health = 100;
		}
		return;
	}

	UpdateFromKeyboard(deltaSeconds);
	UpdateFromController(deltaSeconds);

	if (g_theInput->WasKeyJustPressed(KEYCODE_F9))
	{
		g_debugInvincible = !g_debugInvincible;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::Render() const
{
	if (m_isDead)
		return;

	RenderTank();
	RenderTurret();
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::UpdateFromKeyboard(float deltaSeconds)
{
	m_velocity = Vec2(0, 0);
	

	//Tank Input
	m_tankMoveIntention = Vec2(0.f, 0.f);
	
	if (g_theInput->IsKeyDown('W'))
	{
		m_tankMoveIntention += Vec2(0.f, 1.f);
	}

	if (g_theInput->IsKeyDown('A'))
	{
		m_tankMoveIntention += Vec2(-1.f, 0.f);
	}

	if (g_theInput->IsKeyDown('S'))
	{
		m_tankMoveIntention += Vec2(0.f, -1.f);
	}

	if (g_theInput->IsKeyDown('D'))
	{
		m_tankMoveIntention += Vec2(1.f, 0.f);
	}

	m_tankMoveIntention.ClampLength(1.0f);

	//Move Tank Body
	if (m_tankMoveIntention.GetLength() > 0)
	{
		m_goalOrientationDegrees = m_tankMoveIntention.GetOrientationDegrees();
		m_orientationDegrees = GetTurnedTowardDegrees(m_orientationDegrees, m_goalOrientationDegrees, TANK_TURN_SPEED * deltaSeconds);
		m_velocity = GetForwardNormal() * (PLAYER_MOVEMENT_SPEED * m_tankMoveIntention.GetLength());
	}

	//Turret Input
	m_turretMoveIntention = Vec2(0.f, 0.f);
	
	if (g_theInput->IsKeyDown('I'))
	{
		m_turretMoveIntention += Vec2(0.f, 1.f);
	}

	if (g_theInput->IsKeyDown('J'))
	{
		m_turretMoveIntention += Vec2(-1.f, 0.f);
	}

	if (g_theInput->IsKeyDown('K'))
	{
		m_turretMoveIntention += Vec2(0.f, -1.f);
	}

	if (g_theInput->IsKeyDown('L'))
	{
		m_turretMoveIntention += Vec2(1.f, 0.f);
	}

	if (g_theInput->IsKeyDown(' '))
	{
 		if (m_secondsSinceFired >= PLAYER_SHOOT_COOLDOWN)
 		{
			Shoot();
		}
	}

	m_turretMoveIntention.ClampLength(1.0f);

	//Move turret
	if (m_turretMoveIntention.GetLength() > 0.0f)
	{
		float absoluteTurretGoal = m_turretMoveIntention.GetOrientationDegrees();
		m_turretRelativeGoalDegrees = absoluteTurretGoal - m_orientationDegrees;
		m_turretRelativeDegrees = GetTurnedTowardDegrees(m_turretRelativeDegrees, m_turretRelativeGoalDegrees, TURRET_TURN_SPEED * deltaSeconds);
	}

	if (g_theInput->IsKeyDown(KEYCODE_SHIFT))
	{
		m_velocity *= 5.f;
	}


	m_secondsSinceFired += deltaSeconds;
	Entity::Update(deltaSeconds);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::UpdateFromController(float deltaSeconds)
{
	XboxController const& controller = g_theInput->GetController(0);
	
	//Tank Movement
	AnalogJoystick const& leftStick = controller.GetLeftStick();
	if ( leftStick.GetMagnitude() > 0.0f)
	{
		m_moveFraction = leftStick.GetMagnitude();
		m_goalOrientationDegrees = leftStick.GetOrientationDegrees();
		m_orientationDegrees = GetTurnedTowardDegrees(m_orientationDegrees, m_goalOrientationDegrees, TANK_TURN_SPEED * deltaSeconds);
		m_velocity = GetForwardNormal() * (PLAYER_MOVEMENT_SPEED * m_moveFraction);
	}

	//Turret Movement
	AnalogJoystick const& rightStick = controller.GetRightStick();
	if ( rightStick.GetMagnitude() > 0.0f)
	{
		float absoluteTurretGoal = rightStick.GetOrientationDegrees();
		m_turretRelativeGoalDegrees = absoluteTurretGoal - m_orientationDegrees;
		m_turretRelativeDegrees = GetTurnedTowardDegrees(m_turretRelativeDegrees, m_turretRelativeGoalDegrees, TURRET_TURN_SPEED * deltaSeconds);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::RenderTank() const
{
	Vertex_PCU tempWorldVerts[NUM_TANKS_VERTS];

	for (int i = 0; i < NUM_TANKS_VERTS; i++)
	{
		tempWorldVerts[i] = m_tankLocalVerts[i];
	}

 	Texture* tankBaseTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/PlayerTankBase.png");
 	g_theRenderer->BindTexture(tankBaseTexture);
	
	TransformVertexArrayXY3D(NUM_TANKS_VERTS, tempWorldVerts, 0.5f, m_orientationDegrees, m_position);
	g_theRenderer->DrawVertexArray(NUM_TANKS_VERTS, tempWorldVerts);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::RenderTurret() const
{
	Vertex_PCU tempWorldVerts[NUM_TURRET_VERTS];

	for (int i = 0; i < NUM_TURRET_VERTS; i++)
	{
		tempWorldVerts[i] = m_turretLocalverts[i];
	}
	
	Texture* tankBaseTexture = g_theRenderer->CreateOrGetTextureFromFile("Data/Images/PlayerTankTop.png");
	g_theRenderer->BindTexture(tankBaseTexture);

	TransformVertexArrayXY3D(NUM_TURRET_VERTS, tempWorldVerts, 0.5f,m_orientationDegrees + m_turretRelativeDegrees, m_position);
	g_theRenderer->DrawVertexArray(NUM_TURRET_VERTS, tempWorldVerts);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::DebugRender() const
{
 	Rgba8 turretColor(60, 60, 255);
 	Vec2 tankGoalForward	= m_cosmeticRadius * 1.0f * Vec2::MakeFromPolarDegrees(m_goalOrientationDegrees);
 	Vec2 turretGoalForward  = m_cosmeticRadius * 1.0f * Vec2::MakeFromPolarDegrees(m_orientationDegrees + m_turretRelativeGoalDegrees);
 	Vec2 turretForward	    = m_cosmeticRadius * 1.0f * Vec2::MakeFromPolarDegrees(m_orientationDegrees + m_turretRelativeDegrees);

	Entity::DebugRender();
	
	if (g_debugInvincible == true)
	{
		DebugDrawRing(m_position, m_physicsRadius * 1.1f, 0.1f, Rgba8(255,255,255));
	}

	//TO-DO:Add no clip ring
	
// 	if (m_game->g_noClip)
// 	{
// 		DebugDrawRing(m_position, m_physicsRadius , 0.1f, Rgba8(0, 0, 0));
// 	}
	
 	DebugDrawLine(m_position + 1.0f * tankGoalForward, m_position + 1.1f * tankGoalForward, 0.1f, turretColor);
 	DebugDrawLine(m_position + 1.1f * turretGoalForward, m_position + 1.2f * turretGoalForward, 0.1f, turretColor);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::Die()
{
	m_isDead = true;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void Player::TakeDamage(int damage)
{
	if (!g_debugInvincible)
	{
		Entity::TakeDamage(damage);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::Shoot()
{
	float gunOrientation = m_orientationDegrees + m_turretRelativeDegrees;
	Vec2 gunForwardNormal = Vec2::MakeFromPolarDegrees(gunOrientation);
	float spawnDistance = m_physicsRadius + 0.01f;
	Vec2 spawnPos = m_position + gunForwardNormal * spawnDistance;
	if (m_map->IsPointInSolidTile(spawnPos) && spawnDistance > 0.f)
	{
		spawnDistance -= 0.01f;
		spawnPos = m_position + gunForwardNormal * spawnDistance;
	}

	Entity* bullet = m_map->SpawnNewEntityOfType(ENTITY_TYPE_GOOD_BULLET, spawnPos, gunOrientation);
	bullet->m_health = 3;	
	m_secondsSinceFired = 0.f;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::ReactToBulletHit(Bullets& bullet)
{
	Entity::ReactToBulletHit(bullet);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::InitializeLocalVertsTankBody()
{
	m_tankLocalVerts[0].m_position = Vec3(-1.f, -1.f, 0.f);
	m_tankLocalVerts[1].m_position = Vec3(1.f, -1.f, 0.f);
	m_tankLocalVerts[2].m_position = Vec3(1.f, 1.f, 0.f);

	m_tankLocalVerts[3].m_position = Vec3(-1.f, -1.f, 0.f);
	m_tankLocalVerts[4].m_position = Vec3(1.f, 1.f, 0.f);
	m_tankLocalVerts[5].m_position = Vec3(-1.f, 1.f, 0.f);

	m_tankLocalVerts[0].m_uvTexCoords = Vec2(0.f,0.f);
	m_tankLocalVerts[1].m_uvTexCoords = Vec2(1.0f, 0.f);
	m_tankLocalVerts[2].m_uvTexCoords = Vec2(1.f,1.f);
										   	
	m_tankLocalVerts[3].m_uvTexCoords = Vec2(0.f, 0.f);
	m_tankLocalVerts[4].m_uvTexCoords = Vec2(1.f, 1.f);
	m_tankLocalVerts[5].m_uvTexCoords = Vec2(0.f,1.f);

	for (int vertIndex = 0; vertIndex < NUM_TANKS_VERTS; vertIndex++)
	{
		m_tankLocalVerts[vertIndex].m_color = TANK_BODY_COLOR;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void Player::InitializeLocalVertsTurret()
{
	m_turretLocalverts[0].m_position = Vec3(-1.f, -1.f, 0.f);
	m_turretLocalverts[1].m_position = Vec3(1.f, -1.f, 0.f);
	m_turretLocalverts[2].m_position = Vec3(1.f, 1.f, 0.f);

	m_turretLocalverts[3].m_position = Vec3(-1.f, -1.f, 0.f);
	m_turretLocalverts[4].m_position = Vec3(1.f, 1.f, 0.f);
	m_turretLocalverts[5].m_position = Vec3(-1.f, 1.f, 0.f);

	m_turretLocalverts[0].m_uvTexCoords = Vec2(0.f, 0.f);
	m_turretLocalverts[1].m_uvTexCoords = Vec2(1.0f, 0.f);
	m_turretLocalverts[2].m_uvTexCoords = Vec2(1.f, 1.f);

	m_turretLocalverts[3].m_uvTexCoords = Vec2(0.f, 0.f);
	m_turretLocalverts[4].m_uvTexCoords = Vec2(1.f, 1.f);
	m_turretLocalverts[5].m_uvTexCoords = Vec2(0.f, 1.f);

	for (int vertIndex = 0; vertIndex < NUM_TURRET_VERTS; vertIndex++)
	{
		m_turretLocalverts[vertIndex].m_color = TURRET_COLOR;
	}
}