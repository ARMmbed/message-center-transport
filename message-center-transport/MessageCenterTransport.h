/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __MESSAGE_CENTER_TRANSPORT_H__
#define __MESSAGE_CENTER_TRANSPORT_H__

#include "mbed-drivers/mbed.h"
#include "core-util/SharedPointer.h"
#include "mbed-block/BlockStatic.h"

using namespace mbed::util;


class MessageCenterTransport
{
public:
    typedef enum {
        MinimumIRQDelay = 10,
        TimeoutInMs     = 1000
    } constants_t;

    bool sendTask(uint16_t port, BlockStatic& block, void (*callback)(void))
    {
        bool result = internalSendTask(port, block);

        if (result)
        {
            callbackSend.attach(callback);
        }

        return result;
    }

    bool sendTask(BlockStatic& block, void (*callback)(void))
    {
        return sendTask(0, block, callback);
    }

    template <typename T>
    bool sendTask(uint16_t port, BlockStatic& block, T* object, void (T::*member)(void))
    {
        bool result = internalSendTask(port, block);

        if (result)
        {
            callbackSend.attach(object, member);
        }

        return result;
    }

    template <typename T>
    bool sendTask(BlockStatic& block, T* object, void (T::*member)(void))
    {
        return sendTask(0, block, object, member);
    }

    /*  Register receive callback. */
    void onReceiveTask(void (*callback)(uint16_t port, SharedPointer<BlockStatic> block))
    {
        callbackReceive.attach(callback);
    }

    template <typename T>
    void onReceiveTask(T* object, void (T::*callback)(uint16_t port, SharedPointer<BlockStatic> block))
    {
        callbackReceive.attach(object, callback);
    }

protected:
    MessageCenterTransport()
        :   callbackSend(),
            callbackReceive()
    {}

    virtual bool internalSendTask(uint16_t port, BlockStatic& block) = 0;

    FunctionPointer0<void>                                        callbackSend;
    FunctionPointer2<void, uint16_t, SharedPointer<BlockStatic> > callbackReceive;
};

#endif // __MESSAGE_CENTER_TRANSPORT_H__
