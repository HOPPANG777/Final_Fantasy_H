#include "Framework.h"
#include "TileMap.h"

TileMap::TileMap(UINT width, UINT height, UINT spacing, bool editNow)
	: width(width), height(height), spacing(spacing), editNow(editNow)
{
	//TILESET->Init();

	GenerateTileMap();

	//vertices
	{
		//vertices.resize(4);
		vertices.assign(4, VertexTile());

		vertices[0].position = Values::ZeroVec3;
		vertices[1].position = Vector3(0.0f, (float)spacing, 0.0f);
		vertices[2].position = Vector3((float)spacing, 0.0f, 0.0f);
		vertices[3].position = Vector3((float)spacing, (float)spacing, 0.0f);
	}

	//indices
	{
		indices = { 0, 1, 2, 2, 1, 3 };
	}

	//vertexBuffer
	{
		vertexBuffer = make_unique<VertexBuffer>();
		vertexBuffer->Create(vertices, D3D11_USAGE_DYNAMIC);
	}

	//indexBuffer
	{
		indexBuffer = make_unique<IndexBuffer>();
		indexBuffer->Create(indices, D3D11_USAGE_IMMUTABLE);
	}

	//vertexBuffer
	{
		vertexShader = make_unique<VertexShader>();
		vertexShader->Create(ShaderPath + L"VertexTile.hlsl", "VS");
	}

	//pixelShader
	{
		pixelShader = make_unique<PixelShader>();
		pixelShader->Create(ShaderPath + L"VertexTile.hlsl", "PS");
	}

	//inputLayout
	{
		inputLayout = make_unique<InputLayout>();
		inputLayout->Create(VertexTile::isDescs.data(), VertexTile::isDescs.size(), vertexShader->GetBlob());
	}

	//worldBuffer
	{
		worldBuffer = make_unique<WorldBuffer>();
		worldBuffer->SetWorld(world);
	}

	//indexNumBuffer
	{
		indexNumBuffer = make_unique<IndexNumBuffer>();
		indexNumBuffer->SetIndex(0);
	}

	// Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

		desc.CullMode = D3D11_CULL_BACK;
		//desc.CullMode = D3D11_CULL_NONE;

		desc.FillMode = D3D11_FILL_SOLID;

		desc.FrontCounterClockwise = false;

		HRESULT hr = DEVICE->CreateRasterizerState(&desc, rs.GetAddressOf());
		CHECK(hr);
	}

	// Sampler
	{
		D3D11_SAMPLER_DESC desc;
		RenderStates::GetSamplerDesc(&desc);
		RenderStates::CreateSampler(&desc, sampler[0].GetAddressOf());

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		RenderStates::CreateSampler(&desc, sampler[1].GetAddressOf());
	}

	// Blend
	{
		D3D11_BLEND_DESC desc;
		RenderStates::GetBlendDesc(&desc);
		RenderStates::CreateBlend(&desc, blend[0].GetAddressOf());

		desc.RenderTarget[0].BlendEnable = true;
		RenderStates::CreateBlend(&desc, blend[1].GetAddressOf());
	}
}

TileMap::~TileMap()
{
	for (auto& tile : tiles)
	{
		for (auto& t : tile)
		{
			t.reset();
		}
	}
}

void TileMap::GenerateTileMap()
{
	if (width == 0 || height == 0 || spacing == 0) assert(false);

	//vector<vector<shared_ptr<Tile>>> tiles;
	tiles.resize(height);

	int index = 0;
	for (UINT y = 0; y < height; y++)
	{
		tiles[y].resize(width);
		for (UINT x = 0; x < width; x++)
		{
			tiles[y][x] = make_shared<Tile>();
			tiles[y][x]->SetPosition(Vector3(static_cast<float>(x * spacing), static_cast<float>(y * spacing), 0.0f));
			tiles[y][x]->SetIndex(index++);
		}
	}
}

void TileMap::MapVertexBuffer()
{
	DC->Map(vertexBuffer->GetResource(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
}

void TileMap::UnmapVertexBuffer()
{
	memcpy(subResource.pData, vertices.data(), sizeof(vertices[0]) * vertices.size());
	DC->Unmap(vertexBuffer->GetResource(), 0);
}

void TileMap::ResizeMap(UINT newWidth, UINT newHeight)
{
    vector<vector<shared_ptr<Tile>>> newTiles;
    newTiles.resize(newHeight);

    int index = 0;

    for (UINT y = 0; y < newHeight; y++)
    {
        newTiles[y].resize(newWidth);

        for (UINT x = 0; x < newWidth; x++)
        {
            if (x < width && y < height)
            {
                newTiles[y][x] = tiles[y][x];
            }
            else
            {
				const auto& tile = make_shared<Tile>();
                tile->SetPosition(Vector3((float)x * spacing, (float)y * spacing, 0.0f));
                tile->SetIndex(index);
                tile->SetColor(Values::Black);
                newTiles[y][x] = tile;
            }
            index++;
        }
    }

    tiles = move(newTiles);
    // width/height 
    width = newWidth;
    height = newHeight;
}

void TileMap::Update()
{
	if (editNow == true)
	{
		Vector3 mPos = INPUT->GetMousePos();

		mPos += CAMERA->GetPosition();

		Tile* tile = GetTile(mPos);

		if (tile) indexNumBuffer->SetIndex(tile->GetIndex());

		if (INPUT->MousePressed(0x0000))
		{
			if (tile)
			{
				Vector2 startUV = TILESET->selectedStartUV;
				Vector2 endUV = startUV + TILESET->texelTileSize;
				tile->SetStartUV(startUV);
				tile->SetEndUV(endUV);
			}
		}
	}
	else
	{
		indexNumBuffer->SetIndex(0xFFFFFFFF);
	}

}

void TileMap::Render()
{
	vertexBuffer->SetIA();
	indexBuffer->SetIA();
	inputLayout->SetIA();
	DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	vertexShader->SetShader();
	pixelShader->SetShader();

	DC->PSSetSamplers(0, 1, sampler[1].GetAddressOf());

	DC->PSSetShaderResources(0, 1, TILESET->GetSRV().GetAddressOf());

	DC->RSSetState(rs.Get());

	DC->OMSetBlendState(blend[1].Get(), nullptr, (UINT)0xFFFFFFFF);


	for (UINT y = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			shared_ptr<Tile> tile = tiles[y][x];
			if (!tile) continue;

			MapVertexBuffer();
			{
				for (VertexTile& v : vertices)
				{
					v.color = tile->GetColor();
					v.index = tile->GetIndex();
				}

				vertices[0].uv = Vector2(tile->GetStartUV().x, tile->GetEndUV().y);
				vertices[1].uv = tile->GetStartUV();
				vertices[2].uv = tile->GetEndUV();
				vertices[3].uv = Vector2(tile->GetEndUV().x, tile->GetStartUV().y);

				vertices[0].uv2 = Vector2(0, 1);
				vertices[1].uv2 = Vector2(0, 0);
				vertices[2].uv2 = Vector2(1, 1);
				vertices[3].uv2 = Vector2(1, 0);
			}
			UnmapVertexBuffer();

			world = Matrix::CreateTranslation(tile->GetPosition());
			worldBuffer->SetWorld(world);
			worldBuffer->SetVSBuffer(0);
			indexNumBuffer->SetPSBuffer(2);
			DC->DrawIndexed(indexBuffer->GetCount(), 0, 0);
		}
	}
}

void TileMap::ImGUI()
{
	TILESET->ImGUI();
}

void TileMap::Save(const string& path)
{
	ofstream fout(path);
	fout << to_string(width) << ' ' << to_string(height) << ' ' << to_string(spacing) << endl;

	string imagePath = "";
	wstring tsPath = TILESET->GetTilePath();
	imagePath.assign(tsPath.begin(), tsPath.end());
	string xCount = to_string(TILESET->GetXCount());
	string yCount = to_string(TILESET->GetYCount());
	fout << imagePath << ' ' << xCount << ' ' << yCount << endl;

	for (UINT y = 0; y < height; y++)
	{
		for (UINT x = 0; x < width; x++)
		{
			std::shared_ptr<Tile>& tile = tiles[y][x];
			bool isEmpty = (tile->GetStartUV() == Vector2(0, 0) && tile->GetEndUV() == Vector2(0, 0));
			fout << (isEmpty ? "0 " : "1 ");
			fout << to_string(tile->GetStartUV().x) << ' '
				<< to_string(tile->GetStartUV().y) << ' '
				<< to_string(tile->GetEndUV().x) << ' '
				<< to_string(tile->GetEndUV().y) << endl;
		}
	}
}

void TileMap::Load(const string& path)
{
	ifstream fin(path);
	if (fin.fail())
	{
		cout << "  ʽϴ. ̸ Ȯ ּ." << endl;
		return;
	}

	tiles.clear();
	char temp[256];

	{
		fin.getline(temp, 256);
		istringstream iss(temp);
		iss >> width >> height >> spacing;
		if (width == 0 || height == 0 || spacing == 0) assert(false);
	}

	{
		fin.getline(temp, 256);
		istringstream iss(temp);
		string imagePath;
		UINT xCount = 0, yCount = 0;
		iss >> imagePath >> xCount >> yCount;
		wstring tsPath(imagePath.begin(), imagePath.end());
		TILESET->ChangeTileMap(tsPath, xCount, yCount);
	}

	tiles.resize(height);
	int index = 0;
	for (UINT y = 0; y < height; y++)
	{
		tiles[y].resize(width);
		for (UINT x = 0; x < width; x++)
		{
			const std::shared_ptr<Tile>& tile = make_shared<Tile>();
			Vector3 position(static_cast<float>(x * spacing), static_cast<float>(y * spacing), 0.0f);
			tile->SetPosition(position);
			tile->SetIndex(index++);

			fin.getline(temp, 256);
			istringstream uvStream(temp);
			int activeFlag;
			float sx, sy, ex, ey;
			uvStream >> activeFlag >> sx >> sy >> ex >> ey;

			if (activeFlag == 1)
			{
				tile->SetStartUV(Vector2(sx, sy));
				tile->SetEndUV(Vector2(ex, ey));
				tile->SetColor(Values::White);
			}
			else
			{
				tile->SetColor(Values::Transparent);
			}
			tiles[y][x] = tile;
		}
	}

	Save(BuildTilePath + "After.txt");
	cout << "Ÿϸ ҷ Ϸ!" << endl;
}

Tile* TileMap::GetTile(Vector3 mPos)
{
	UINT x = (int)(mPos.x) / spacing;
	UINT y = (int)(mPos.y) / spacing;
	if (x < 0 || x >= width || y < 0 || y >= height) return nullptr;
	return tiles[y][x].get();
}

