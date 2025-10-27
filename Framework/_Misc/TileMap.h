#pragma once

class TileMap
{
private:
    vector<VertexTile> vertices;
    vector<UINT> indices;

    unique_ptr<VertexBuffer> vertexBuffer;
    unique_ptr<IndexBuffer> indexBuffer;

    unique_ptr<VertexShader> vertexShader;

    unique_ptr<InputLayout> inputLayout;
    
    unique_ptr<PixelShader> pixelShader;

    unique_ptr<WorldBuffer> worldBuffer;

    unique_ptr<IndexNumBuffer> indexNumBuffer;

    ComPtr<ID3D11RasterizerState> rs = nullptr;

    ComPtr<ID3D11BlendState> blend[2];

    ComPtr<ID3D11SamplerState> sampler[2];

    Matrix world = Values::Identity;

    UINT width = 0, height = 0, spacing = 0;

    D3D11_MAPPED_SUBRESOURCE subResource;

    vector<vector<shared_ptr<Tile>>> tiles;
    
    UINT totalVertexCount = 0;
    UINT totalIndexCount = 0;

    bool needUpdateVB = true;

    bool editNow = false;

public:
    TileMap(UINT width, UINT height, UINT spacing, bool editNow = false);
    ~TileMap();

public:
    void Update();
    void Render();

    void ImGUI();

public:
    void GenerateTileMap();

    void MapVertexBuffer();
    void UnmapVertexBuffer();
    
    void ResizeMap(UINT newWidth, UINT newHeight);

public:
    void Save(const string& path);
    void Load(const string& path);

public:
    Tile* GetTile(Vector3 mPos);

};