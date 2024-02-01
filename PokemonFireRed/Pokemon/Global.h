#pragma once

// �������� ����� ����� �����Ѵ�.
class Global
{
public:
	static const int PIXEL_SIZE;
	static const int SCREEN_X;
	static const int SCREEN_Y;
	static const int IMAGE_TILE_SIZE;
	static const int TILE_SIZE;
	static const int HALF_SCREEN_X;
	static const int HALF_SCREEN_Y;

	static const float F_PIXEL_SIZE;
	static const float F_SCREEN_X;
	static const float F_SCREEN_Y;
	static const float F_IMAGE_TILE_SIZE;
	static const float F_TILE_SIZE;
	static const float F_HALF_SCREEN_X;
	static const float F_HALF_SCREEN_Y;

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
