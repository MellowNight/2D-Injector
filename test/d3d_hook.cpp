#include "d3d_hook.h"
#include "menu.h"

LRESULT(CALLBACK* o_wndproc)(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	) = NULL;

typedef HRESULT(__fastcall* swapchain_present)(
	IDXGISwapChain* pChain,
	UINT SyncInterval,
	UINT Flags

);

ID3D11RenderTargetView* my_render_target_view = NULL;
ID3D11DeviceContext* device_context = NULL;
ID3D11Device* my_device = NULL;

BOOL CALLBACK EnumWindowCallback(HWND hwnd, LPARAM lp)
{
	DWORD proc_id;
	GetWindowThreadProcessId(hwnd, &proc_id);
	if (GetCurrentProcessId() != proc_id)
	{
		/*	return true, which means continue enumerating windows	*/
		return TRUE;
	}

	Global::main_window = hwnd;
	return FALSE;	// we found our process window
}

HRESULT GetDeviceAndCtxFromSwapchain(
	IDXGISwapChain* pSwapChain,
	ID3D11Device** ppDevice,
	ID3D11DeviceContext** ppContext
)
{
	HRESULT ret = pSwapChain->GetDevice(__uuidof(ID3D11Device), (PVOID*)ppDevice);
	if (SUCCEEDED(ret))
	{
		(*ppDevice)->GetImmediateContext(ppContext);
	}

	return ret;
}

HRESULT __fastcall hk_present(IDXGISwapChain* pChain, UINT SyncInterval, UINT Flags)
{
	if (!Global::d3dhook_inited) {
		Utils::log("hk_present called! \n");

		if (FAILED(GetDeviceAndCtxFromSwapchain(pChain, &my_device, &device_context)))
		{
			Utils::log("GetDeviceAndCtxFromSwapchain failed! \n");
			return static_cast<swapchain_present>(present_hk->original_bytes)(pChain, SyncInterval, Flags);
		}

		DXGI_SWAP_CHAIN_DESC sd;
		pChain->GetDesc(&sd);
		Global::main_window = sd.OutputWindow;

		GetWindowRect(Global::main_window, &Global::window_rect);
		Global::window_height = Global::window_rect.bottom - Global::window_rect.top;
		Global::window_width = Global::window_rect.right - Global::window_rect.left;

		/*	offset the width and height because the
		top window bar and the sides of the window have some area	*/
		Global::window_height -= 29;
		Global::window_width -= 5;

		ImGui::CreateContext();

		// Set OriginalWndProcHandler to the Address of the Original WndProc function
		o_wndproc = (WNDPROC)SetWindowLongPtr(Global::main_window,
			GWLP_WNDPROC, (LONG_PTR)hk_wndproc);

		ImGui_ImplWin32_Init(Global::main_window);
		ImGui_ImplDX11_Init(my_device, device_context);

		ID3D11Texture2D* pBackBuffer;

		pChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		my_device->CreateRenderTargetView(pBackBuffer, NULL, &my_render_target_view);
		pBackBuffer->Release();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		io.ImeWindowHandle = Global::main_window;

		Global::d3dhook_inited = true;
	}

	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();

	ShowMenu();

	ImGui::EndFrame();
	ImGui::Render();

	device_context->OMSetRenderTargets(1, &my_render_target_view, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return static_cast<swapchain_present>(present_hk->original_bytes)(pChain, SyncInterval, Flags);
}


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
);

LRESULT CALLBACK hk_wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	POINT mPos;
	GetCursorPos(&mPos);
	ScreenToClient(Global::main_window, &mPos);
	ImGui::GetIO().MousePos.x = mPos.x;
	ImGui::GetIO().MousePos.y = mPos.y;

	ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

	return CallWindowProc(o_wndproc, hWnd, uMsg, wParam, lParam);
}