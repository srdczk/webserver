//
// Created by Admin on 2020/5/14.
//

#ifndef WEBSERVER_CHANNEL_H
#define WEBSERVER_CHANNEL_H

#include <functional>
#include <memory>
#include <sys/time.h>
#include "Buffer.h"


class EventLoop;

class HttpMessage;

class Channel {
public:
    typedef std::function<void()> CallbackType;

    Channel(EventLoop *loop, int fd);

    void SetREvents(int revents) { revents_ = revents; }
    void SetEvents(int events) { events_ = events; }
    bool UpdateLastEvents();
    int Fd() { return fd_; }
    int Events() { return events_; }
    int LastEvents() { return lastEvents_; }

    int32_t Index() const { return index_; }
    void SetIndex(int32_t index) { index_ = index; }

    uint64_t ExpiredTime() const { return expiredTime_; }
    void SetExpiredTime(uint64_t expiredTime) { expiredTime_ = expiredTime; }

    void SetReadCallback(CallbackType &&readCb) { readCb_ = readCb; }
    void SetWriteCallback(CallbackType &&writeCb) { writeCb_ = writeCb; }
    void SetUpdateCallback(CallbackType &&updateCb) { updateCb_ = updateCb; }
    void SetErrorCallback(CallbackType &&errorCb) { errorCb_ = errorCb; }

    void EchoRead();

    void HandleRead();
    void HandleWrite();
    void HandleUpdate();
    void HandleEvents();

    std::shared_ptr<HttpMessage> Holder();
    void SetHolder(std::shared_ptr<HttpMessage> http) { http_ = http; }

private:
    EventLoop *loop_;
    int fd_;
    int revents_;
    int events_;
    int lastEvents_;
    // Channel, index in Heap
    int32_t index_;
    uint64_t expiredTime_;
    CallbackType readCb_;
    CallbackType writeCb_;
    CallbackType updateCb_;
    CallbackType errorCb_;
    std::weak_ptr<HttpMessage> http_;
};

#endif //WEBSERVER_CHANNEL_H
