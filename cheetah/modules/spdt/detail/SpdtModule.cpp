/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 The SKA organisation
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


namespace ska {
namespace cheetah {
namespace modules {
namespace spdt {

namespace detail {

template<typename T, typename SpdtTraits>
struct CreateHelper
{
    template<typename SpdtAlgoFactory>
    static inline T create(SpdtAlgoFactory& algo_factory)
    {
        return T(algo_factory._config);
    }
};

template<typename SpdtTraits>
struct CreateHelper<cpu::Spdt<SpdtTraits>, SpdtTraits>
{
    template<typename SpdtAlgoFactory>
    static inline cpu::Spdt<SpdtTraits> create(SpdtAlgoFactory& algo_factory)
    {
        return cpu::Spdt<SpdtTraits>(algo_factory._config);
    }
};
} // namespace detail

template<class SpdtTraits>
class SpdtAlgoFactory
{
    private:
        template<typename, typename>
        friend struct detail::CreateHelper;
        typedef typename SpdtTraits::Config ConfigType;

    public:
        SpdtAlgoFactory(ConfigType const& config)
            : _config(config)
        {}

        void none_selected() const {
            PANDA_LOG_WARN << "spdt:: no algorithm has been activated. Please set at least one spdt algorithm to active.";
        }

        template<typename T> T create() const
        {
            return detail::CreateHelper<T, SpdtTraits>::create(*this);
        }

        template<typename Algo>
        bool active() const
        {
            return _config.template config<typename Algo::Config>().active();
        }
    private:
        ConfigType const& _config;
};

template<typename SpdtTraits, template<typename> class... SpdtAlgorithms>
SpdtModule<SpdtTraits, SpdtAlgorithms...>::SpdtModule(Config const& config, SpdtHandler const& spdt_handler)
    : _spdt_handler(spdt_handler)
    , _task(config.pool(), _spdt_handler)
{
    SpdtAlgoFactory<SpdtTraits> algo_factory(config);
    utils::TaskConfigurationSetter<SpdtAlgorithms<SpdtTraits>...>::configure(_task, algo_factory);
}

template<typename SpdtTraits, template<typename> class... SpdtAlgorithms>
inline void SpdtModule<SpdtTraits, SpdtAlgorithms...>::operator()(std::shared_ptr<DmTrialsType> data)
{
    std::cout<<"submitting spdt task \n";
    _task.submit(data);
}

} // namespace spdt
} // namespace modules
} // namespace cheetah
} // namespace ska
