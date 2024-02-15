#pragma once
#include <EngineBase/EngineMath.h>

// 전역으로 사용할 상수를 정의한다.
class Global
{
public:
	static const float CHARACTER_WALK_SPEED;
	static const float CHARACTER_JUMP_SPEED;
	static const float CHARACTER_WALK_ANIMATION_FRAME_INTERVAL;
	static const float CHARACTER_JUMP_ANIMATION_FRAME_INTERVAL;

	static const int PIXEL_SIZE;
	static const int MAP_RUNTIME_SCALE_FACTOR;
	static const int SCREEN_X;
	static const int SCREEN_Y;
	static const int IMAGE_TILE_SIZE;
	static const int TILE_SIZE;
	static const int HALF_SCREEN_X;
	static const int HALF_SCREEN_Y;

	static const float F_PIXEL_SIZE;
	static const float F_MAP_RUNTIME_SCALE_FACTOR;
	static const float F_SCREEN_X;
	static const float F_SCREEN_Y;
	static const float F_IMAGE_TILE_SIZE;
	static const float F_TILE_SIZE;
	static const float F_HALF_SCREEN_X;
	static const float F_HALF_SCREEN_Y;

	static const int MONITOR_X;
	static const int MONITOR_Y;
	static const int HALF_MONITOR_X;
	static const int HALF_MONITOR_Y;

	static const FVector SCREEN;
	static const FVector HALF_SCREEN;
	static const Color8Bit GLYPH_BACKGROUND_COLOR;

	static const std::string ANIMATION_NAME_SUFFIX_UPPER_BODY;
	static const std::string ANIMATION_NAME_SUFFIX_LOWER_BODY;

	// delete Function
	Global(const Global& _Other) = delete;
	Global(Global&& _Other) noexcept = delete;
	Global& operator=(const Global& _Other) = delete;
	Global& operator=(Global&& _Other) noexcept = delete;

protected:

private:
	Global() {}
	~Global() {}
};

enum ERenderingOrder
{
	None,
	Background,
	Lower,
	Upper,
	Foreground,
	LowerUI,
	UpperUI,
	Super
};

enum ECollisionOrder
{
	Player,
	NPC,
};