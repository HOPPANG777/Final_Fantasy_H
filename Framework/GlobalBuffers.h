#pragma once

class WorldBuffer : public ConstantBuffer
{
	// 16바이트 단위로 데이터 패딩을 해줘야 한다.
	struct WorldStruct
	{
		DirectX::SimpleMath::Matrix _world;
	};

	WorldStruct _data;

public:
	WorldBuffer() : ConstantBuffer(&_data, sizeof(_data)) {}

	void SetWorld(const DirectX::SimpleMath::Matrix& world)
	{
		_data._world = XMMatrixTranspose(world);
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
		_data._view = XMMatrixTranspose(view);
	}

	void SetProjection(const DirectX::SimpleMath::Matrix& proj)
	{
		_data._proj = XMMatrixTranspose(proj);
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
		// 기본 변환 색상은 흰색으로 설정 (즉, 색상 변화 없음)
		_data._color = DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 색상 변환 값 설정 (예: 텍스처 색상에 곱해질 값)
	void SetColor(const DirectX::SimpleMath::Color& color)
	{
		_data._color = color;
	}

	// 현재 설정된 색상 변환 값을 반환 (읽기 전용)
	DirectX::SimpleMath::Color GetColor() const
	{
		return _data._color;
	}
};

class IndexNumBuffer : public ConstantBuffer
{
	struct IndexStruct
	{
		UINT _index;
		DirectX::SimpleMath::Vector3 _dummy;
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



