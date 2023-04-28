#pragma once
#include "GameObject.h"

class ResourceManager;

class Gem : public GameObject
{
public:
	Gem(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string objType, bool isCollidable, RECT objRect = { 0, 0, 0 ,0 })
		: GameObject(dManager, texPath, position, scale, active, objSize, objType, isCollidable, objRect)
	{
		Init(dManager, texPath, position, scale, active, objSize, objType, isCollidable, objRect);
	}

	void Init(DeviceManager* dManager, std::wstring texPath, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, bool active, DirectX::SimpleMath::Vector2 objSize, std::string _objType, bool isCollidable, RECT objRect = { 0, 0, 0 ,0 }) override;
	void Update(DeviceManager* dManager, ResourceManager* rManager, float dTime) override;
	void Render(DeviceManager* dManager) override;

	bool GetActive() override { return isActive; }
	DirectX::SimpleMath::Vector2 GetObjectSize() override { return objSize; }
	std::string GetObjectType() override { return objType; }
	bool GetIsCollidable() override { return isCollidable; }
	RECT GetCollisionBounds() override { return collisionBounds; }
	DirectX::SimpleMath::Vector2 GetPosition() override { return mPos; }
	DirectX::SimpleMath::Vector2 GetScale() override { return mScale; }

	void SetActive(bool _isActive) override;
	void SetObjectSize(DirectX::SimpleMath::Vector2 _objSize) override;
	void SetIsCollidable(bool _isCollidable) override;
	void SetRect(RECT _objRect) override;
	void SetPosition(DirectX::SimpleMath::Vector2 _position) override;
	void SetScale(DirectX::SimpleMath::Vector2 _scale) override;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> objTex;
	bool isActive;
	DirectX::SimpleMath::Vector2 objSize;
	std::string objType;
	bool isCollidable;
	RECT objRect;

	DirectX::SimpleMath::Vector2 mPos;
	DirectX::SimpleMath::Vector2 mScale;
	DirectX::SimpleMath::Vector2 mOrigin = { 0,0 };

	void CheckCollision(DeviceManager* dManager, ResourceManager* rManager, float dTime);
	bool collidedTop = false;
	bool collidedBottom = false;
	bool collidedLeft = false;
	bool collidedRight = false;
	RECT collisionBounds;
};
