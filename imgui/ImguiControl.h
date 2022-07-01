#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
using namespace Microsoft::WRL;
using namespace DirectX;

class ImguiEnumElement
{
public:
    int number;
    std::string name;
};

class ImguiEnum
{
public:
    ImguiEnum(int count, ...);
    ~ImguiEnum();
    std::list<ImguiEnumElement> enums;
    std::string nameString;
    int enumCount = 0;
};

class Imgui
{
private:
    static ComPtr<ID3D12DescriptorHeap> imguiDescHeap;//imguiï€éùópÉÅÉìÉo
    static ComPtr<ID3D12DescriptorHeap> heapForImgui;
public:
    
    static ComPtr<ID3D12DescriptorHeap> CreateDescrriptorHeapForImgui();
    static ComPtr<ID3D12DescriptorHeap> GetHeapForImgui();
    static void DrawImGui();
    static void Init();
    static void GetAndDrawInfo(const int &intOriginal, int& imguiInfo);
    static void GetAndDrawInfo(const float &floatOriginal, float& imguiInfo);
    static void GetAndDrawInfo(const XMFLOAT3 &xmfloat3Original, XMFLOAT3& imguiInfo);
    static void ChangeInfo(int &intOriginal, int& imguiInfo);
    static void ChangeInfo(float &floatOriginal, float& imguiInfo);
    static void ChangeInfo(XMFLOAT3 &xmfloat3Original, XMFLOAT3& imguiInfo);

    static void CreateMenuBar();
    static void EachInfo();
    static void DebugUpdate();

    static bool isActive;
    static void SetWindowActive(bool isActive);

    static int effectType;
    enum class ImguiType
    {
        Status,
        CameraInfo,
        Debug,
        PostEffect,

        ImguiTypeEnd,
    };
    static ImguiType tab;

    enum class DebugType
    {
        Player,
        Water,

        DebugTypeEnd,
    };
    static DebugType debugType;

    static int playerCombo;
    static ImguiEnum iEnum;

    static float volume;


    static float CameraR;
    static float CameraRotation;
    static float CameraHigh;
    static bool CameraControl;

    static int useWaterNum;

    static int mouseWheel;

    static bool touchedImgui;
};
