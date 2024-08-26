/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 The SKA organisation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef SKA_CHEETAH_EMULATOR_EMULATOR_H
#define SKA_CHEETAH_EMULATOR_EMULATOR_H

#include "cheetah/generators/DataGenerator.h"
#include "panda/Connection.h"
#include "panda/Server.h"
#include "panda/ServerSettings.h"
#include "panda/StreamerApp.h"
#include <memory>


namespace ska {
namespace cheetah {
namespace emulator {
class Config;

/**
 * @brief
 *    Emulate UDP stream using a data madel generator
 *
 * @details
 *
 */

class EmulatorBase
{
    protected:
        EmulatorBase() = default;

    public:
        virtual ~EmulatorBase() = default;
        virtual int run() = 0;
        virtual void stop() = 0;
        virtual bool is_running() const = 0;
};

template<typename StreamType, typename T>
class Emulator : public EmulatorBase
{
    public:
        typedef typename StreamType::ModelType ModelType;

    public:
        /**
         * @brief Construct an Emulator utilisiing the provided model
         * @param model is the model to be used, OWNERSHIP IS TRANSFERRED TO THIS CLASs
         */
        template<typename... StreamConstructorArgs>
        Emulator(Config const&, ModelType* model, StreamConstructorArgs&& ...);
        ~Emulator();

        int run() override;
        void stop() override;
        bool is_running() const override;

        /**
         * @brief the port to where the udp data stream is being directed
         */
        boost::asio::ip::udp::endpoint const end_point() const;

        /**
         * @brief the subscription service listening address
         * @details you can subscribe to the data stream by simply connecting (i.e send anything) to the returned address
         */
        boost::asio::ip::udp::endpoint const subscriber_end_point() const;

        /**
         * @brief return a reference to the model object
         */
        ModelType const& model() const;

    private:
        typedef ska::panda::TimedConnection<ska::panda::ConnectionTraits<ska::panda::Udp>> ConnectionType;

    private:
        //typedef io::producers::rcpt::PacketGenerator<ModelType> StreamType;

        ModelType* _model; // owner
        StreamType _stream;
        std::shared_ptr<ConnectionType> _connection;
        ska::panda::StreamerApp<StreamType> _emulator_app;
        ska::panda::Server<ska::panda::ServerTraits<ska::panda::Udp>, decltype(_emulator_app)> _server;
};


} // namespace emulator
} // namespace cheetah
} // namespace ska
#include "detail/Emulator.cpp"

#endif // SKA_CHEETAH_EMULATOR_EMULATOR_H
