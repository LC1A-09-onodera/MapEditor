#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "../BaseDirectX/BaseDirectX.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// パーティクル
/// </summary>
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	ParticleManager() = default;
	~ParticleManager() = delete;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	static const int vertexCount = 2048;
private: // 定数
	//std::forward_list<Particle> particles;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static bool StaticInitialize(ID3D12Device *device,  int window_width, int window_height, XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up);

	/// <summary>
	/// 視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3 &GetEye() { return eye; }

	/// <summary>
	/// 視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetEye(XMFLOAT3 eye);

	/// <summary>
	/// 注視点座標の取得
	/// </summary>
	/// <returns>座標</returns>
	static const XMFLOAT3 &GetTarget() { return target; }

	/// <summary>
	/// 注視点座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	static void SetTarget(XMFLOAT3 target);

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	static void CameraMoveVector(XMFLOAT3 move);

	/// <summary>
	/// ベクトルによる視点移動
	/// </summary>
	static void CameraMoveEyeVector(XMFLOAT3 move);
	
	// デバイス
	static ID3D12Device *device;

public: // 静的メンバ変数
	// ビュー行列
	static XMMATRIX matView;
	// 射影行列
	static XMMATRIX matProjection;
	// 視点座標
	static XMFLOAT3 eye;
	// 注視点座標
	static XMFLOAT3 target;
	// 上方向ベクトル
	static XMFLOAT3 up;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	//static VertexPos vertices[vertexCount];
	// 頂点インデックス配列
	//ビルボード
	static XMMATRIX matBillboard;
	static XMMATRIX matBillboardY;
private:// 静的メンバ関数

	/// <summary>
	/// カメラ初期化
	/// </summary>
	/// <param name="window_width">画面横幅</param>
	/// <param name="window_height">画面縦幅</param>
	static void InitializeCamera(int window_width, int window_height, XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up);

	/// <summary>
	/// カメラ初期化
	/// </summary>
	static void CameraUpdate();

public: // メンバ関数
	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	static void UpdateViewMatrix(XMFLOAT3 eye, XMFLOAT3 target, XMFLOAT3 up, bool isBillbord = true);
	// コマンドリスト
	static ID3D12GraphicsCommandList *cmdList;

private: // メンバ変数
	
	//でスクリプタの何番目のテクスチャを使うかを選択する
	int textureNum = 0;
};

class ParticleIndi
{
	struct VertexPos
	{
		DirectX::XMFLOAT3 pos;
		float scale;
	};

	struct Particle
	{
		using XMFLOAT3 = DirectX::XMFLOAT3;
		XMFLOAT3 position = {};
		XMFLOAT3 velocity = {};
		XMFLOAT3 accel = {};
		int frame = 0;
		int num_frame = 0;
		//スケール
		float scale = 1.0f;
		//スケール初期値
		float s_scale = 1.0f;
		//最終地
		float e_scale = 0.0f;
		
	};
	struct ConstBufferData
	{
		DirectX::XMMATRIX mat;
		DirectX::XMMATRIX matBillboard;
		float alpha;
	};
public:
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelinestate;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texbuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// 頂点データ配列
	VertexPos vertices[ParticleManager::vertexCount];
	//パーティクルのコンテナ
	std::forward_list<Particle> particles;
	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize = 0;

	//消滅しないためのフラグ
	bool isUI = false;
	//縦横を変更するなら仕様
	bool isSize = false;
	float alpha = 1.0f;
	ComPtr<ID3D12Resource> constBuff; // 定数バッファ
	// 色
	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	// ローカルスケール
	DirectX::XMFLOAT3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	DirectX::XMFLOAT3 rotation = { 0,0,0 };
	// ローカル座標
	DirectX::XMFLOAT3 position = { 0,0,0 };
	// ローカルワールド変換行列
	DirectX::XMMATRIX matWorld = {};
	// 親オブジェクト
	std::shared_ptr<ParticleManager> parent = nullptr;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	/// <summary>
	/// 座標の取得
	/// </summary>
	/// <returns>座標</returns>
	const DirectX::XMFLOAT3 &GetPosition() { return position; }
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(DirectX::XMFLOAT3 position) { this->position = position; }
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	ParticleIndi *Create(const wchar_t *texName);

	void CreateModel();
	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	bool InitializeDescriptorHeap();
	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	bool InitializeGraphicsPipeline();

	bool Initialize(const wchar_t *texName);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update(DirectX::XMFLOAT3 eye, DirectX::XMFLOAT3 target, DirectX::XMFLOAT3 up , DirectX::XMFLOAT3 *pos = nullptr, bool isBilbord = true);

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <returns>成否</returns>
	bool LoadTexture(const wchar_t *texName);

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	void Add(int life, DirectX::XMFLOAT3 &position, DirectX::XMFLOAT3 &velocity, DirectX::XMFLOAT3 &accel, float start_scale = 1.0f, float end_scale = 0.0f);

	/// <summary>
	/// パーティクルの生成
	/// </summary>
	void StartParticle(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 1.0f, float endSize = 0.0f, int life = 60);

	/// <summary>
	/// 円形パーティクル
	/// </summary>
	void CiycleParticle(const DirectX::XMFLOAT3 emitterPosition = {0, 0, 0}, float startSize = 1.0f, float endSize = 0.0f, int life = 60);

	void ContinueParticle(const DirectX::XMFLOAT3 emitterPosition = {0, 0, 0}, float startSize = 1.0f, float endSize = 0.0f, int life = 60);

	/// <summary>
	/// 炎のようなパーティクル
	/// </summary>
	void FrameParticle(const DirectX::XMFLOAT3 emitterPosition = {0, 0, 0}, float startSize = 1.0f, float endSize = 0.0f, int life = 60, int count = 2);

	/// <summary>
	/// ジャンプ時用パーティクル
	/// </summary>
	void JumpParticle(const DirectX::XMFLOAT3 emitterPosition = {0, 0, 0}, float startSize = 1.0f, float endSize = 0.0f, int life = 60);

	/// <summary>
	/// ポジティブなパーティクル
	/// </summary>
	void LuckParticle(const DirectX::XMFLOAT3 emitterPosition = {0, 0, 0}, float startSize = 1.0f, float endSize = 0.0f, int life = 60);

	/// <summary>
	/// ネガティブなパーティクル
	/// </summary>
	void DownParticle(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 1.0f, float endSize = 0.0f, int life = 60);

	/// <summary>
	/// 板ポリ表示
	/// </summary>
	void UI(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 5.0f, float endSize = 5.0f, int life = 1);
	void UI2(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 130.0f, float endSize = 130.0f);
	/// <summary>
	/// パーティクルが収束してくる
	/// </summary>
	void UpParticle(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 0.0f, float endSize = 1.0f, int life = 30);

	/// <summary>
	/// 雨particle
	/// </summary>
	void RainParticle(const DirectX::XMFLOAT3 emitterPosition = { 0, 100, 0 }, float startSize = 0.0f, float endSize = 1.0f, int life = 60, int count = 30);

	/// <summary>
	/// 増強particle
	/// </summary>
	void BaffParticle(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 4.0f, float endSize = 0.0f, int life = 60, int count = 20);


	/// <summary>
	/// ロックオン
	/// </summary>
	void RockOn(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 5.0f, float endSize = 5.0f);

	/// <summary>
	/// 
	/// </summary>
	void StarParticle(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 6.0f, float endSize = 3.0f, int life = 60, int count = 20);

	void BackParticle(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 2.0f, float endSize = 2.0f, int life = 180);

	void FlashParticle(const DirectX::XMFLOAT3 emitterPosition = { 0, 0, 0 }, float startSize = 2.0f, float endSize = 2.0f, int life = 60);
};
/// <summary>
	/// 描画
	/// </summary>
void ParticleDraw(ID3D12GraphicsCommandList *cmdList, const ParticleIndi *particle);
/// <summary>
/// 描画後処理
/// </summary>
void PostDraw();

class ParticleControl
{
private:
	ParticleControl();
	~ParticleControl();
public:
	ParticleControl(const ParticleControl &obj) = delete;
	ParticleControl &operator=(const ParticleControl &obj) = delete;
	static std::shared_ptr<ParticleIndi> attackEffect;
	static std::shared_ptr<ParticleIndi> expEffect;
	static std::shared_ptr<ParticleIndi> flashEffect;
	static std::shared_ptr<ParticleIndi> rockOnEffect;
	static std::shared_ptr<ParticleIndi> numbers[10];
	static void Update();
	static void Init();
	static void Draw();
};
