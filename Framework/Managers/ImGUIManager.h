#pragma once

class ImGUIManager
{
	DECLARE_SINGLETON(ImGUIManager);
public:
	void Init();

	LRESULT MsgProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
	void Resize();

	void Update();
	void Render();

private:
	void ApplyStyle();

};

