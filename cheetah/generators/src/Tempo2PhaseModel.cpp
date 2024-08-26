/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 The SKA organisation (except where explicity noted below)
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
#include "Tempo2PhaseModel.h"
#include "panda/Error.h"
#include "panda/CaseInsensitiveString.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <cmath>
#include <limits>

namespace {

/* 
 * All functions in this anonymous namespace are complete rewrites of the 
 * tempo2 predictor code. 
 * copyright details of the original code can be found in:
 *
 *    https://bitbucket.org/psrsoft/tempo2/overview
 */

/* Basic Chebyshev polynomial */
class Cheby2D
{
    public:
        Cheby2D() 
            : _coeff(nullptr)
            , _nx(0U)
            , _ny(0U)
        {
        }

        ~Cheby2D()
        {
            free(_coeff);
        }

        void resize(unsigned x, unsigned y);

        // return the x, y coefficient
        inline
        long double& coeff(unsigned x, unsigned y) {
            return _coeff[y*_nx+x];
        }

        // evaluate the chebyshev polynomial
        long double evaluate(long double x, long double y) const;

        // calculate the derived Cheby2D
        void construct_x_derivative(Cheby2D& result) const;

    protected:
        long double evaluate_1d(long double x, int index) const;

    private:
        long double* _coeff;
        unsigned _nx, _ny;

};

void Cheby2D::resize(unsigned x, unsigned y)
{
    if(_coeff) {
        free(_coeff);
        _coeff = nullptr;
    }
    _nx = x;
    _ny = y;
    if(_nx && _ny)
        _coeff = static_cast<long double*>(malloc(_nx*_ny*sizeof(long double))); 
}

long double Cheby2D::evaluate_1d(long double x, int index) const
{
    // Basic 1D Clenshaw's formula applied to a row of coeffs
    long double di = 0.0L;
    long double di1 = 0.0L;
    long double di2;
    long double* coeffcurr = _coeff + (index + 1) * _nx - 1;
    for(std::size_t i = _nx - 1; i > 0; --i) {
        di2 = di1;
        di1 = di;
        di = 2.0L * x * di1 - di2 + *(coeffcurr--); // i.e. + c_ij
    }
    // compute di0, special case
    return x * di - di1 + 0.5L * (*coeffcurr--);
}

long double Cheby2D::evaluate(long double x, long double y) const
{
    // Clenshaw's formula applied twice. The cheby polynomials
    // in x are evaluated to provide the coefficients for the 
    // cheby polynomial in y

    long double y_coeff, dj, dj1, dj2;

    dj = dj1 = 0.0L;
    for (int j = _ny - 1; j > 0; --j) {
        y_coeff = evaluate_1d(x, j);
        dj2 = dj1;
        dj1 = dj;
        dj = 2.0L * y * dj1 - dj2 + y_coeff;
    } 
    return y * dj - dj1 + 0.5L * evaluate_1d(x, 0);
}

void Cheby2D::construct_x_derivative(Cheby2D& derived_cheby) const
{
    unsigned i=0;
    assert(derived_cheby._nx == _nx && derived_cheby._ny == _ny);
    assert(_nx>2);

    for (unsigned iy=0; iy < _ny; ++iy)
    {
        unsigned ioff = i+_nx;
        derived_cheby._coeff[ioff-1] = 0.0;
        derived_cheby._coeff[ioff-2] = 2 * (_nx-1) * _coeff[ioff-1];    
        unsigned ix=_nx-1;
        do
        {
            derived_cheby._coeff[i+ix]= derived_cheby._coeff[i+ix+2] + 2 * (ix+1) * _coeff[i+ix+1];
        } while(ix-- != 0);
        i+=_nx;
    }
}

/* Chebyshev predictive phase model */
struct ChebyModel
{
    public:
        void load(boost::filesystem::ifstream& is, unsigned& line_count)
        {
            std::string line;
            std::istringstream iss;
            unsigned ix=0;
            // lambda to skip whitespace/comments
            auto next_line = [&]() -> bool {
                while( std::getline(is, line) ) {
                    ++line_count;
                    boost::trim_left(line);
                    if(line.empty() || line.front() == '#') {
                        continue; // ignore comments
                    }
                    iss.str(line);
                    iss.clear();
                    return true;
                }
                return false;
            };
            auto parse_error = [&](std::string const& msg)
            {
                ska::panda::Error e(msg);
                e << " on line " << line_count;
                throw e;
            };
            unsigned nx=0;
            unsigned ny=0;
            ska::panda::CaseInsensitiveString keyword;
            if(!next_line()) return;
            iss >> keyword;
            if(keyword == "ChebyModel") {
                iss >> keyword;
                if(keyword != "BEGIN") {
                    parse_error("expecting ChebyModel BEGIN");
                }
            }
            while( next_line() ) {
                iss >> keyword;
                if(keyword == "ChebyModel") {
                    iss >> keyword;
                    if(keyword == "BEGIN") {
                        parse_error("unexpected ChebyModel BEGIN");
                        continue;
                    }
                    if(keyword == "END") {
                        cheby.construct_x_derivative(frequency_cheby);
                        return;
                    }
                }
                if(keyword == "PSRNAME") {
                    iss >> psrname;
                    continue;
                }
                if(keyword == "SITENAME") {
                    iss >> sitename;
                    continue;
                }
                if(keyword == "TIME_RANGE")
                {
                    iss >> mjd_start;
                    iss >> mjd_end;
                    continue;
                }
                if(keyword == "FREQ_RANGE")
                {
                    iss >> freq_start;
                    iss >> freq_end;
                    continue;
                }
                if(keyword == "DISPERSION_CONSTANT")
                {
                    iss >> dispersion_constant;
                    continue;
                }
                if(keyword == "NCOEFF_TIME")
                {
                    iss >> nx;
                    continue;
                }
                if (keyword == "NCOEFF_FREQ")
                {
                    iss >> ny;
                    continue;
                }
                if (keyword == "COEFFS")
                {
                    if(ix==0) {
                        if(!(nx&&ny))
                            parse_error("unexpected COEFFS keyword before NCOEFF_TIME or NCOEFF_FREQ");
                        cheby.resize(nx,ny);
                        frequency_cheby.resize(nx,ny);
                    }
                    unsigned iy=0;
                    while(iy<ny) {
                        if(iss.eof()) {
                            next_line();
                        }
                        iss >> cheby.coeff(ix, iy);
                        ++iy;
                    }
                    ++ix;
                    continue;
                }
                parse_error(std::string("unrecognised keyword :") + keyword.c_str());
            } // while
        }

    public:
      std::string psrname;
      std::string sitename;
      long double mjd_start, mjd_end;
      long double freq_start, freq_end;
      long double dispersion_constant; // phase = polyco + d_c/f^2
      Cheby2D cheby;
      Cheby2D frequency_cheby;
};


class ChebyModelSet {
    public:
        ~ChebyModelSet() {
            for(auto ptr : _models) {
                delete ptr;
            }
        }
        std::vector<ChebyModel*>& models() { return _models; };

        // the number of models is the set
        std::size_t size() const { return _models.size(); }

        // return the evaluation of phase from the closest model mathcing the params
        // N.B. ignoring DM in the model
        long double phase(long double mjd, long double freq) const
        {
            ChebyModel const* model = get_nearest_model(mjd);
            if(model) {
                // linear fit across the model range
                return model->cheby.evaluate( 
                    -1.0L + 2.0L * (mjd - model->mjd_start)/(model->mjd_end - model->mjd_start)
                    ,
                    -1.0L + 2.0L * (freq - model->freq_start)/(model->freq_end - model->freq_start));
                    //+ model->dispersion_constant/(freq * freq);
            }
            return -1.0;
        }

        void load(boost::filesystem::ifstream& is)
        {
            std::string line;
            unsigned line_number=0;
            ska::panda::CaseInsensitiveString keyword;
            unsigned model_number;
            while( std::getline(is, line) ) {
                ++line_number;
                boost::trim_left(line);
                if(line.empty() || line.front() == '#') {
                    continue; // ignore comments
                }
                std::istringstream iss(line);
                iss >> keyword >> model_number;
                if(keyword != "ChebyModelSet") {
                    throw ska::panda::Error("expecting a ChebyModelSet format");
                } 
                _models.reserve(model_number);
                for(unsigned num =0; num < model_number; ++num)
                {
                    ChebyModel* model = new ChebyModel();
                    _models.push_back(model);
                    model->load(is, line_number);
                }
            }
        }

    protected:
        /// return the model closest to the mjd requested
        ChebyModel const* get_nearest_model(long double mjd) const
        {
            ChebyModel const* rv_model = nullptr;
            long double min_offset=std::numeric_limits<long double>::max(), offset;
            for(ChebyModel const* model : _models) {
                // check model for matching times
                if(mjd < model->mjd_start || mjd > model->mjd_end) {
                    continue;
                }
                offset = std::abs((model->mjd_start+model->mjd_end)/2 - mjd);
                if( offset < min_offset ) {
                    min_offset = offset;
                    rv_model = model;
                }
            }
            return rv_model;
        }

    private:
        std::vector<ChebyModel*> _models;
};

} // namespace

namespace ska {
namespace cheetah {
namespace generators {

struct T2Predictor {
    public:
        T2Predictor()
            : _modelset(nullptr)
        {
        }
        ~T2Predictor() {
            delete _modelset;
        }

        long double phase(long double mjd, long double freq) // freq in MHz
        {
            return _modelset->phase(mjd, freq);
        }

        // load in model from file
        void load(boost::filesystem::path const& filename) {
            boost::filesystem::ifstream is(filename);
            if(!is.is_open()) {
                panda::Error e("unable to open file: ");
                e << filename;
                throw e;
            }
            if(_modelset == nullptr) _modelset = new ChebyModelSet;
            _modelset->load(is);
        }

    private:
        ChebyModelSet* _modelset;
     
};

Tempo2PhaseModel::Tempo2PhaseModel(Tempo2PhaseModelConfig const& config)
    : _pred(new T2Predictor)
{
    load(config.filename());
}

void Tempo2PhaseModel::load(boost::filesystem::path const& tempo2_predictor_file)
{
    // check the file exists
    if(!boost::filesystem::is_regular_file(tempo2_predictor_file))
    {
        panda::Error e("file does not exist:");
        e << tempo2_predictor_file;
        throw e;
    }
    PANDA_LOG << "tempo2 predictor model loading from file " << tempo2_predictor_file;

    _pred->load(tempo2_predictor_file);
    
}

Tempo2PhaseModel::~Tempo2PhaseModel()
{
}

double Tempo2PhaseModel::operator()(utils::ModifiedJulianClock::time_point const& mjd, boost::units::quantity<data::MegaHertz, double> const mhz) const
{
    long double const mjd_dbl = mjd.time_since_epoch().count();
    return _pred->phase(mjd_dbl, mhz.value());
}

} // namespace generators
} // namespace cheetah
} // namespace ska
