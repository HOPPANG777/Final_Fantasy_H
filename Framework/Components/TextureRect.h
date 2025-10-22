#pragma once

class TextureRect : public Drawable, public IColorable
{
public:
	// Component을(를) 통해 상속됨
	void Update() override;
	void Render() override;

};

