/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2020 Metrological
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/ 

#include "Module.h"
#include <vector>

#include "avmonitor.h"
#include <interfaces/IAVMonitor.h>

using namespace WPEFramework;
namespace {

WPEFramework::Exchange::IAVMonitor::EventInfo ConvertEventInfo(const avmonitor_event_t& other) 
{
    WPEFramework::Exchange::IAVMonitor::EventInfo result;
    result.timeStamp = other.timeStamp;
    result.id = other.id;
    result.type = static_cast<WPEFramework::Exchange::IAVMonitor::EventInfo::EventType>(other.type);
    result.stateChange = static_cast<WPEFramework::Exchange::IAVMonitor::EventInfo::GstStateChange>(other.stateChange);
    result.windowWidth = other.windowWidth;
    result.windowHeight = other.windowHeight;
    result.visible = other.visible;
    result.zorder = other.zorder;
    result.startPTS = other.startPTS;
    result.firstPTS = other.firstPTS;
    result.currentPTS = other.currentPTS;
    result.position = other.position;
    result.frameRenderCount = other.frameRenderCount;
    result.frameDropCount = other.frameDropCount;
    result.fps = other.fps;
    result.fpsMean = other.fpsMean;
    //caps
    return result;
}

static string Callsign()
{
    static constexpr const TCHAR Default[] = _T("AVMonitor");
    return (Default);
}

class AVMonitorLink : public WPEFramework::RPC::SmartInterfaceType<WPEFramework::Exchange::IAVMonitor> {
private:
    using BaseClass = WPEFramework::RPC::SmartInterfaceType<WPEFramework::Exchange::IAVMonitor>;

    AVMonitorLink()
        : BaseClass()
        , _lock()
    {
        ASSERT(_singleton==nullptr);

        _singleton = this;

        BaseClass::Open(RPC::CommunicationTimeOut, BaseClass::Connector(), Callsign());
    }

public:
    AVMonitorLink(const AVMonitorLink&) = delete;
    AVMonitorLink& operator=(const AVMonitorLink&) = delete;
    ~AVMonitorLink() override
    {
        BaseClass::Close(WPEFramework::Core::infinite);
        _singleton = nullptr;
    }

    static AVMonitorLink& Instance()
    {
        static AVMonitorLink *instance = new AVMonitorLink;
        ASSERT(instance!=nullptr);
        return *instance;
    }

    static void Dispose()
    {
        ASSERT(_singleton != nullptr);

        if (_singleton != nullptr) {
            delete _singleton;
        }
    }

    uint32_t AVMonitorRegisterEvent(const avmonitor_event_t& event)
    {
        uint32_t errorCode = Core::ERROR_UNAVAILABLE;
        _lock.Lock();

        if (_avMonitorInterface != nullptr) {
            auto convertedEvent = ConvertEventInfo(event);
            errorCode = _avMonitorInterface->RegisterEvent(convertedEvent);
        }

        _lock.Unlock();
        return errorCode;
    }

private:
    void Operational(const bool upAndRunning) override
    {
        _lock.Lock();
        if (upAndRunning) {
            if (_avMonitorInterface == nullptr) {
                _avMonitorInterface = BaseClass::Interface();
            }
        } else {
            if (_avMonitorInterface != nullptr) {
                _avMonitorInterface->Release();
                _avMonitorInterface = nullptr;
            }
        }
        _lock.Unlock();
    }


private:
    Core::CriticalSection _lock;
    static AVMonitorLink* _singleton;
    Exchange::IAVMonitor* _avMonitorInterface;
};

AVMonitorLink* AVMonitorLink::_singleton = nullptr;


}// nameless namespace

extern "C" {

uint32_t avmonitor_register_event(const avmonitor_event_t event) {
    return AVMonitorLink::Instance().AVMonitorRegisterEvent(event);
}

void avmonitor_dispose() {
    AVMonitorLink::Dispose();
}
} // extern "C"

