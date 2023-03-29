#include "PlayerAnimation.h"
#include "D3D.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

using namespace rapidjson;
using namespace DirectX;
using namespace DirectX::SimpleMath;

void Animation::Init(std::string jsonPath, GameObject& Sprite)
{
	jsonPath = "Data/" + jsonPath;
	if (jsonPath == "Data/TestSheet.json")
	{
		LoadAnimation(jsonPath);
		//LoadAnimationData("Data/PlayerAnimData.json") For Full spritesheet implementation
		SwitchTex(Sprite, Zero, InitState);
	}
	else
	{
		assert("Animation does not exist");
	}
}
void Animation::Update(float dTime, GameObject& Sprite, int animState)
{
		//calculate elapsed time
		FramesTemp = Zero;//Temporary magic numbers unil animation data loading is complete
		elapsedTime += dTime;
		if (animState == 1 || animState == 2)
		{
			PlayIdle = false;
			FramesTemp = PlayerFrames;
			if (elapsedTime >= frameDuration)
			{
				++currentFrame;
				if (currentFrame >= FramesTemp)
				{
					currentFrame = Zero;
				}
				elapsedTime -= frameDuration;
			}
		}
		if (animState == 0)
		{
			//FramesTemp = IdleFrames + IdleOffset; For Full spritesheet implementation
			FramesTemp = IdleFrames;
			if (elapsedTime >= delayTime)
			{
				PlayIdle = true;
			}
			if (PlayIdle == true)
			{
				if (elapsedTime >= frameDuration)
				{
					++currentFrame;
					if (currentFrame >= FramesTemp)
					{
						//currentFrame = IdleOffset;For Full spritesheet implementation
						currentFrame = Zero;
					}
					elapsedTime -= frameDuration;
				}
			}
		}
		//advance to the next frame if enough time has passed (Constantly changing frames)
		SwitchTex(Sprite, currentFrame, animState);
	
}

void Animation::SwitchTex(GameObject& Sprite, int currentFrame, int animState)
{
		if (animState == 0)
		{
			Sprite.SetScale(Vector2(kSizeUp, kSizeUp));
			Sprite.SetRect(walkspriteSheet[currentFrame]);
		}
		if (animState == 1)
		{
			Sprite.SetScale(Vector2(kSizeUp, kSizeUp));
			Sprite.SetRect(walkspriteSheet[currentFrame]);
		}
		if (animState == 2)
		{
			Sprite.SetScale(Vector2(-kSizeUp, kSizeUp));
			Sprite.SetRect(-walkspriteSheet[currentFrame]);
		}
		if (animState == 3)
		{
			Sprite.SetScale(Vector2(-kSizeUp, kSizeUp));
			//Sprite.SetRect(-walkspriteSheet[Zero]);
		}
		else
		{
			assert("Animation state invalid");
		}
}

void Animation::LoadAnimation(std::string jsonPath)
{
	//From Joshua Moxon project 2
	FILE* Animation;
	errno_t levelsStatus = fopen_s(&Animation, jsonPath.c_str(), "rb");
	if (levelsStatus != 0)
	{
		printf("ERROR: Could not open file!");
		assert(false);
	}
	char readBuffer[bufferMemory];
	FileReadStream is(Animation, readBuffer, sizeof(readBuffer));
	Document AnimationDoc;
	AnimationDoc.ParseStream(is);
	int i = 0;
https://stackoverflow.com/questions/41857273/rapidjson-get-member-name-of-value
	GenericArray fullArray = AnimationDoc["frames"].GetArray();
	for (Value::ConstValueIterator itr = fullArray.Begin(); itr != fullArray.End(); ++itr)
	{
		auto obj = itr->GetObj();
		if (obj.HasMember("frame"))
		{
			auto oneFrame = obj.FindMember("frame");
			auto const& FrameData = oneFrame->value;
			float FrameWidth = FrameData["x"].GetInt();
			float FrameHeight = FrameData["y"].GetInt();
			float FrameOffsetW = FrameData["w"].GetInt();
			float FrameOffsetH = FrameData["h"].GetInt();
			RECTF TempRect = { FrameWidth, FrameHeight,FrameOffsetW,FrameOffsetH };
			walkspriteSheet[i] = TempRect;
			++i;
		}
	}
}

void Animation::LoadAnimationData(std::string jsonPath)
{
	//Frameworks for any additional animation data that isnt created when creating a texture packer animation
	FILE* Animation;
	errno_t levelsStatus = fopen_s(&Animation, jsonPath.c_str(), "rb");
	if (levelsStatus != 0)
	{
		printf("ERROR: Could not open file!");
		assert(false);
	}
	char readBuffer[bufferMemory];
	FileReadStream is(Animation, readBuffer, sizeof(readBuffer));
	Document AnimationDoc;
	AnimationDoc.ParseStream(is);
	frameDuration = AnimationDoc["Duration"].GetFloat();
	Frames = AnimationDoc["FrameCount"].GetInt();
	delayTime = AnimationDoc["IdleDelay"].GetFloat();
	IdleOffset = AnimationDoc["IdleOffset"].GetFloat();
	JumpOffset = AnimationDoc["JumpOffset"].GetFloat();
}