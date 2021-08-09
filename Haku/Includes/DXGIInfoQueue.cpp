#include "HakuLog.h"
#include "DXGIInfoQueue.h"
#include "ExceptionMacros.h"
#include <dxgidebug.h>

#pragma comment(lib, "dxguid.lib")

DXGIInfoQueue::DXGIInfoQueue()
{
	typedef HRESULT(WINAPI * LPDXGIGETDEBUGINTERFACE)(REFIID, void**);

	HMODULE dxgidebug = LoadLibraryExA("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (dxgidebug)
	{
		auto dxgiGetDebugInterface = reinterpret_cast<LPDXGIGETDEBUGINTERFACE>(
			reinterpret_cast<void*>(GetProcAddress(dxgidebug, "DXGIGetDebugInterface")));

		if (SUCCEEDED(dxgiGetDebugInterface(IID_PPV_ARGS(_InfoQueue.GetAddressOf()))))
		{
			_InfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
			_InfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
		}
	}
}

void DXGIInfoQueue::log_message() noexcept
{
	size_t MessageNumber = _InfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (size_t i = 0; i < MessageNumber; i++)
	{
		size_t MessageSize{};
		_InfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, nullptr, &MessageSize);
		// does it append or reassign..?point of failure maybe
		DXGI_INFO_QUEUE_MESSAGE* Message = static_cast<DXGI_INFO_QUEUE_MESSAGE*>(malloc(MessageSize));
		_InfoQueue->GetMessageA(DXGI_DEBUG_ALL, i, Message, &MessageSize);
		switch (Message->Severity)
		{
		case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
		{
			HAKU_LOG_INFO(Message->pDescription);
		}
		break;
		case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
		{
			HAKU_LOG_INFO(Message->pDescription);
		}
		break;
		case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
		{
			HAKU_LOG_WARN(Message->pDescription);
		}
		break;
		case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
		{
			HAKU_LOG_ERR(Message->pDescription);
		}
		break;
		}
		free(Message);
	}
	_InfoQueue->ClearStoredMessages(DXGI_DEBUG_ALL);
}
