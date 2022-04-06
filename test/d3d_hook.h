#pragma once
#include "includes.h"
#include "global.h"
#include "hooks.h"
#include "utils.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

extern LRESULT(CALLBACK* o_wndproc)(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	);

extern ID3D11Device* my_device;
extern IDXGISwapChain* my_swapchain;
extern ID3D11RenderTargetView* my_render_target_view;
extern ID3D11DeviceContext* device_context;

bool SetPresentHook();

extern Hooks::JmpRipCode* present_hk;

HRESULT __fastcall hk_present(
	IDXGISwapChain* pChain,
	UINT SyncInterval,
	UINT Flags
);

LRESULT CALLBACK hk_wndproc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

#define VMT_PRESENT_IDX 8
