//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "SpriteBatch.h"
#include <SpriteFont.h>


// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class NewD3D final : public DX::IDeviceNotify
{
public:

    NewD3D() noexcept(false);
    ~NewD3D();

    NewD3D(NewD3D&&) = default;
    NewD3D& operator= (NewD3D&&) = default;

    NewD3D(NewD3D const&) = delete;
    NewD3D& operator= (NewD3D const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize(int& width, int& height) const noexcept;
    DX::DeviceResources* GetDeviceResources() { return deviceResources.get(); }
    ID3D12Device* GetDevice() { return device; }
    DirectX::ResourceUploadBatch* GetResourceUpload() { return resourceUpload.get(); }
private:
    std::unique_ptr<DX::DeviceResources> deviceResources;
    ID3D12Device* device;
    std::unique_ptr<DirectX::ResourceUploadBatch> resourceUpload;
    std::unique_ptr<DirectX::DescriptorHeap> resourceDescriptors;

    DirectX::SpriteBatch* mpSB = nullptr;
    DirectX::SpriteFont* mpSF = nullptr;

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.

    // Rendering loop timer.
    DX::StepTimer                               m_timer;

    // If using the DirectX Tool Kit for DX12, uncomment this line:
    std::unique_ptr<DirectX::GraphicsMemory> m_graphicsMemory;

    using VertexType = DirectX::VertexPositionColor;

    std::unique_ptr<DirectX::BasicEffect> m_effect;
    std::unique_ptr<DirectX::PrimitiveBatch<VertexType>> m_batch;

    Microsoft::WRL::ComPtr<ID3D12Resource> m_texture;

    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
    DirectX::SimpleMath::Vector2 m_screenPos;
    DirectX::SimpleMath::Vector2 m_origin;

    RECT m_tileRect;
    std::unique_ptr<DirectX::CommonStates> m_states;

    RECT m_stretchRect;

    RECT m_fullscreenRect;
    Microsoft::WRL::ComPtr<ID3D12Resource> m_background;

    enum Descriptors
    {
        Cat,
        Background,
        Count
    };
};