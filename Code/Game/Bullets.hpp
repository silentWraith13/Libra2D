#pragma once

#include "Game/Entities.hpp"
#include "Engine/Math/Vec2.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/vertexUtils.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------------------
struct Vec2;
//--------------------------------------------------------------------------------------------------------------------------------------------------------
class Bullets : public Entity
{

public:
	Bullets(EntityType type, EntityFaction faction, Map* owner, Vec2 const& initialPosition, float orientationDegrees);
	~Bullets();
	virtual void     Update(float deltaSeconds) override;
	virtual void     Render() const override;
	virtual void     DebugRender() const override;
	virtual void     Die() override;
	virtual void     TakeDamage(int damage) override;
	void			 BounceOff(Vec2 const& bounceNormal);

	Texture* m_bulletTexure = nullptr;
	float  BULLET_SPEED = 1.0f;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------