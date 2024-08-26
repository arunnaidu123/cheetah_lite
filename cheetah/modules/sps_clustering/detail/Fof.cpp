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
#include <boost/geometry/index/rtree.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/for_each.hpp>
#include <chrono>

namespace ska {
namespace cheetah {
namespace modules {
namespace sps_clustering {

namespace bmpl = boost::mpl;
namespace bgi = boost::geometry::index;

namespace
{
    // Calculate the square of the euclidian distance between two points

    template<size_t I, typename PointType>
    struct DCalc
    {
        inline static void calc_distance(size_t dim, const PointType& point1, const PointType& point2, double& d2)
        {
            d2 += pow( boost::geometry::get<I>(point1) - boost::geometry::get<I>(point2), 2);
            DCalc<I-1, PointType>::calc_distance(dim-1, point1, point2, d2);
        }
    };

    template<typename PointType>
    struct DCalc<0, PointType>
    {
        inline static void calc_distance(size_t, const PointType& point1, const PointType& point2, double& d2)
        {
            d2 += pow( boost::geometry::get<0>(point1) - boost::geometry::get<0>(point2), 2);
        }
    };


    template<typename PointType>
    void d2_calc(PointType const& point_1, PointType const& point_2, double& result)
    {
        static constexpr std::size_t Rank = boost::geometry::dimension<PointType>::value;
        DCalc<Rank-1, PointType>::calc_distance(Rank-1, point_1, point_2, result);
    }

    // Add a scaler to all the coordinates of a point

    template<size_t I, typename PointType>
    struct AddScalar
    {
        inline static void add_scalar(size_t dim, PointType& point, double const& c)
        {
            double new_coord = boost::geometry::get<I>(point) + c;
            boost::geometry::set<I>(point, new_coord);
            AddScalar<I-1, PointType>::add_scalar(dim-1, point,c);
        }
    };

    template<typename PointType>
    struct AddScalar<0, PointType>
    {
        inline static void add_scalar(size_t, PointType& point, double const& c)
        {
            double new_coord = boost::geometry::get<0>(point) + c;
            boost::geometry::set<0>(point, new_coord);
        }
    };


    template<typename PointType>
    void add_scalar_to_point(PointType& point, double const& value)
    {
        static constexpr std::size_t Rank = boost::geometry::dimension<PointType>::value;
        AddScalar<Rank-1, PointType>::add_scalar(Rank-1, point, value);
    }
} // namespace

template<typename NumRepType>
std::vector< std::vector<size_t>> Fof::operator()(data::SpCcl<NumRepType> const& cands)
{
    //auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<size_t>> groups;

    typedef std::pair<PointType, size_t> ValueType;
    typedef bgi::rtree<ValueType, bgi::linear<16>> Tree;

    std::vector< std::pair<PointType, size_t> > points;
    points.reserve(cands.size());

    typedef typename data::SpCcl<NumRepType>::Dm Dm;
    std::pair<Dm, Dm> dm_range = cands.dm_range();
    Dm dm_step=(dm_range.second - dm_range.first)/static_cast<typename Dm::value_type>(std::numeric_limits<std::size_t>::max());
    if (dm_step  == 0 * pss::astrotypes::units::parsecs_per_cube_cm)
        dm_step = (1.0 * pss::astrotypes::units::parsecs_per_cube_cm );

    if(cands.tf_blocks().size() == 0 || cands.size() == 0 ) return groups;

    auto tsamp = static_cast<boost::units::quantity<data::MilliSeconds,double>>(((*(cands.tf_blocks().begin()))->sample_interval())).value();
    // set up a point in the clustering search space for each candidate
    for(size_t i = 0 ; i < cands.size() ; ++i)
    {
        PointType point;
        point.template set<0>(static_cast<double>((cands[i].tstart()/tsamp)/(_config.time_tolerance()/tsamp)));

        //convert to appropriate scales for proper clustering
        point.template set<1>(static_cast<double>((cands[i].dm()).value())/((_config.dm_tolerance())).value());

        point.template set<2>(static_cast<double>(std::log2(cands[i].width().value()/tsamp)/std::log2(_config.pulse_width_tolerance().value()/tsamp)));

        //set_point(point, point.begin() + i*D);
        points.emplace_back(point, i);
    }

    Tree tree(points.begin(), points.end());

    while( !tree.empty())
    {
        std::vector<ValueType> to_add;
        // Grab a point from the tree.
        to_add.push_back( *tree.qbegin( bgi::satisfies([](ValueType const &){ return true; })) );
        tree.remove( to_add.begin(), to_add.end() );
        std::vector<ValueType>  added;

        for( std::size_t to_add_i = 0L; to_add_i < to_add.size(); ++to_add_i )
        {
            auto it = to_add.begin() + to_add_i;

            // Build box to query
            PointType lower = it->first;
            add_scalar_to_point(lower,  -_linking_length);
            PointType upper = it->first;
            add_scalar_to_point(upper,  _linking_length);

            boost::geometry::model::box<PointType> box( lower, upper );

            auto within_ball = [this, &it](ValueType const &v)
            {
                double d2 = 0.0;
                d2_calc(it->first, v.first, d2);
                return d2 < _linking_length_2;
            };

            // Find all points within a linking length of the current point.
            tree.query( bgi::within(box) && bgi::satisfies(within_ball), std::back_inserter(added) );

            to_add.reserve(to_add.size() + added.size());
            // Add the found points to the list so we can find their "friends" as well
            std::copy(added.begin(), added.end(), std::back_inserter(to_add));

            // Remove any points we find from the tree as they have been assigned.
            tree.remove( added.begin(), added.end() );

            added.clear();
            // Early exit when we have assigned all particles to a group
            if (tree.empty())
            {
                break;
            }
        }
        std::vector< size_t > group;
        for( auto p : to_add )
        {
            group.push_back(p.second);
        }
        groups.push_back(group);
    }
    //auto stop = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    //PANDA_LOG << "Time taken by clustering: " << duration.count() << " us";

    return groups;
}

} // namespace sps_clustering
} // namespace modules
} // namespace cheetah
} // namespace ska
