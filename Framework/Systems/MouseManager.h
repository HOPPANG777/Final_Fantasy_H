#pragma once

constexpr size_t MAX_INPUT_MOUSE = 8;

class MouseManager
{
	DECLARE_SINGLETON(MouseManager);

	enum
	{
		MOUSE_ROTATION_RIGHT = 0,
		MOUSE_ROTATION_LEFT,
		MOUSE_ROTATION_WHEEL
	};

	enum
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};

public:
	void Init();

	void Update();
	LRESULT MouseProc(UINT message, WPARAM wParam, LPARAM lParam);

public:
	DirectX::SimpleMath::Vector3 GetMousePos() const { return mousePos; }
	DirectX::SimpleMath::Vector3 GetMoveValue() const { return wheelMoveValue; }

public:
	bool Down(const DWORD button) const { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN; }
	bool Up(const DWORD button) const { return buttonMap[button] == BUTTON_INPUT_STATUS_UP; }
	bool Press(const DWORD button) const { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS; }


private:
	HWND handle;
	DirectX::SimpleMath::Vector3 mousePos;

private:
	std::array<unsigned char, MAX_INPUT_MOUSE> buttonStatus = { 0 };
	std::array<unsigned char, MAX_INPUT_MOUSE> buttonOldStatus = { 0 };
	std::array<unsigned char, MAX_INPUT_MOUSE> buttonMap = { 0 };

private:
	DirectX::SimpleMath::Vector3 wheelStatus = DirectHelper::ZeroVector3;
	DirectX::SimpleMath::Vector3 wheelOldStatus = DirectHelper::ZeroVector3;
	DirectX::SimpleMath::Vector3 wheelMoveValue = DirectHelper::ZeroVector3;

private:
	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

};

