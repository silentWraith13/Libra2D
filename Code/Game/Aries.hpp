#pragma once
#include "Game/Entities.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/vertexUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------------------
struct Vec2;
class Map;
//--------------------------------------------------------------------------------------------------------------------------------------------------------
constexpr float     ARIES_MOVEMENT_SPEED = 1.0f;
//--------------------------------------------------------------------------------------------------------------------------------------------------------
class Aries : public Entity
{
public:
	Aries(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees);
	~Aries();
	virtual void     Update(float deltaSeconds) override;
	virtual void     Render() const override;
	virtual void     DebugRender() const override;
	virtual void     Die() override;
	virtual void     TakeDamage(int damage) override;
	virtual void	 TurnTowardsPosition(Vec2 const& targetPos, float maxDeltaDegrees) override;
	virtual void	 ReactToBulletHit(Bullets& bullet) override;

	Map* m_currentMap = nullptr;
	float randomizeTick = 0.f;
	float m_goalOrientationDegrees = 0.f;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------