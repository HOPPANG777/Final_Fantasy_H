#pragma once

class WorldBuffer : public ConstantBuffer
{
	struct WorldStruct
	{
		DirectX::SimpleMath::Matrix _world;
	};

	WorldStruct _data;

public:
	WorldBuffer() : ConstantBuffer(&_data, sizeof(_data)) {}

	void SetWorld(const DirectX::SimpleMath::Matrix& world)
	{
		_data._world = DirectX::XMMatrixTranspose(world);
	}
};

class ViewProjectionBuffer : public ConstantBuffer
{
	struct ViewProjStruct
	{
		DirectX::SimpleMath::Matrix _view;
		DirectX::SimpleMath::Matrix _proj;
	};

	ViewProjStruct _data;

public:
	ViewProjectionBuffer() : ConstantBuffer(&_data, sizeof(_data)) {}

	void SetView(const DirectX::SimpleMath::Matrix& view)
	{
		_data._view = DirectX::XMMatrixTranspose(view);
	}

	void SetProjection(const DirectX::SimpleMath::Matrix& proj)
	{
		_data._proj = DirectX::XMMatrixTranspose(proj);
	}
};

class ColorBuffer : public ConstantBuffer
{
	struct ColorStruct
	{
		DirectX::SimpleMath::Color _color;
	};

	ColorStruct _data;

public:
	ColorBuffer() : ConstantBuffer(&_data, sizeof(_data))
	{
		_data._color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
	}

	void SetColor(const DirectX::SimpleMath::Color& color)
	{
		_data._color = color;
	}

	DirectX::SimpleMath::Color GetColor() const
	{
		return _data._color;
	}
};

class IndexNumBuffer : public ConstantBuffer
{
	struct IndexStruct
	{
		UINT _index = 0;
		DirectX::SimpleMath::Vector3 _dummy = Values::ZeroVector3;
	};

	IndexStruct _data;

public:
	IndexNumBuffer() : ConstantBuffer(&_data, sizeof(_data))
	{
		_data._index = 0;
	}

	void SetIndex(UINT index)
	{
		_data._index = index;
	}

};



