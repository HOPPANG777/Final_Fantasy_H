#pragma once

class Program
{
public:
	Program() = default;
	~Program() = default;

public:
	void Awake();
	void Start();

	void Update();

	void Render();
	void PostRender();

	void ImGUI();

private:
	

};

