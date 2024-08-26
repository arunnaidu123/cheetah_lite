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
#include "cheetah/emulator/Emulator.h"
#include "cheetah/emulator/Config.h"
#include "panda/Log.h"
#include <cassert>


namespace ska {
namespace cheetah {
namespace emulator {


template<typename StreamType, typename T>
template<typename... StreamConstructorArgs>
Emulator<StreamType, T>::Emulator(Config const& config
                                 , ModelType* model
                                 , StreamConstructorArgs&&... args)
    : _model(model)
    , _stream(*_model, std::forward<StreamConstructorArgs>(args)...)
    , _connection(new ConnectionType(config.engine()))
    , _emulator_app(_stream, config.engine())
    , _server(config.server_config(), _emulator_app)
{
    assert(_model != nullptr);
    _connection->set_remote_end_point(config.fixed_end_point());
    _emulator_app.add_fixed_client(_connection);
}

template<typename StreamType, typename T>
Emulator<StreamType, T>::~Emulator()
{
    PANDA_LOG << "mean send rate: " << _connection->monitor().mean();
    delete _model;
}

template<typename StreamType, typename T>
int Emulator<StreamType, T>::run()
{
    _connection->socket().set_option(boost::asio::ip::udp::socket::reuse_address(true));
    return _server.run();
}

template<typename StreamType, typename T>
void Emulator<StreamType, T>::stop()
{
    _server.stop();
}

template<typename StreamType, typename T>
bool Emulator<StreamType, T>::is_running() const
{
    return _server.is_running();
}

template<typename StreamType, typename T>
boost::asio::ip::udp::endpoint const Emulator<StreamType, T>::end_point() const
{
    return _connection->remote_end_point();
}

template<typename StreamType, typename T>
boost::asio::ip::udp::endpoint const Emulator<StreamType, T>::subscriber_end_point() const
{
    return _server.end_point();
}

template<typename StreamType, typename T>
typename Emulator<StreamType,T>::ModelType const& Emulator<StreamType, T>::model() const
{
    return *_model;
}

} // namespace emulator
} // namespace cheetah
} // namespace ska
