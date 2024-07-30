#pragma once
#include "Game/Entities.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/vertexUtils.hpp"
#include "Game/Map.hpp"

struct Vec2;
class Map;
class Game;

constexpr int		NUM_TANK_TRIANGLES = 2;
constexpr int       NUM_TANKS_VERTS = NUM_TANK_TRIANGLES * 3;

constexpr float     TANK_TURN_SPEED = 180.0f;
constexpr float     PLAYER_MOVEMENT_SPEED = 1.5f;
constexpr float		PLAYER_PHYSICS_RADIUS = 0.3f;
constexpr float		PLAYER_COSMETIC_RADIUS = 0.8f;
const     Rgba8     TANK_BODY_COLOR(255, 255, 255, 255);

constexpr int       NUM_TURRET_TRIANGLES = 2;
constexpr int       NUM_TURRET_VERTS = NUM_TURRET_TRIANGLES * 3;
constexpr float     TURRET_TURN_SPEED = 360.0f;
const     Rgba8     TURRET_COLOR(255, 255, 255, 255);

class Player : public Entity
{

public:
	Player(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees);
	~Player();
	virtual void     Update(float deltaSeconds) override;
	virtual void     Render() const override;
	void			 InitializeLocalVertsTankBody();
	void			 InitializeLocalVertsTurret();
	void             UpdateFromKeyboard(float deltaSeconds);
	void			 UpdateFromController(float deltaSeconds);
	void			 RenderTank() const;
	void		     RenderTurret() const;
	virtual void     DebugRender() const override;
	virtual void     Die() override;
	virtual void     TakeDamage(int damage) override;
	void			 Shoot();
	virtual void	 ReactToBulletHit(Bullets& bullet) override;

	Vertex_PCU		m_tankLocalVerts[NUM_TANKS_VERTS];
	Vertex_PCU		m_turretLocalverts[NUM_TURRET_VERTS];
	
	Vec2            m_tankMoveIntention;
	Vec2			m_turretMoveIntention;
	
	float			m_goalOrientationDegrees;
	float			m_moveFraction;
	float           m_turretRelativeGoalDegrees;
	float			m_turretRelativeDegrees;
	float			PLAYER_SHOOT_COOLDOWN = 0.1f;
	
	bool            g_debugInvincible = false;

	Game* m_game = nullptr;
	
};