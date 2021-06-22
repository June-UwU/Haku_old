#pragma once
#include <wrl.h>
#include <dxgidebug.h>
#include <vector>

class DXGIInfoQueue
{
public:
	DXGIInfoQueue();
	DXGIInfoQueue(DXGIInfoQueue& rhs) = delete;
	DXGIInfoQueue& operator=(DXGIInfoQueue& rhs) = delete;
	void log_message() noexcept;
	std::vector<std::string>& ExceptionMessgeDumb() noexcept;
	std::vector<std::string> MessageVector;
private:
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> _InfoQueue;
};

