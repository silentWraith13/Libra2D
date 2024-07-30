#pragma once

#include "Game/Entities.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/vertexUtils.hpp"

struct Vec2;


class Scorpio : public Entity
{

public:
	Scorpio(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees);
	~Scorpio();
	virtual void     Update(float deltaSeconds) override;
	virtual void     Render() const override;
	virtual void     DebugRender() const override;
	virtual void     Die() override;
	virtual void     TakeDamage(int damage) override;
	virtual void	 TurnTowardsPosition(Vec2 const& targetPos, float maxDeltaDegrees) override;
	virtual void	 ReactToBulletHit(Bullets& bullet) override;
	void			 Shoot();

	float			SCORPIO_SHOOT_COOLDOWN = 0.3f;
};