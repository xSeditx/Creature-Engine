#include<cmath>
#include"Easing.h"

#pragma warning(push)
#pragma warning(disable: 4244 4305)

#define _PI  3.14159265359

namespace Math
{

    float easeLinear(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseLinear(inputvalue,outputmin,outputmax,inputmax)
        return _outputmax * _input / _inputmax + _outputmin;
    }

    float easeInQuad(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInQuad(inputvalue,outputmin,outputmax,inputmax)
        _input /= _inputmax;
        return _outputmax * _input * _input + _outputmin;
    }
    float easeOutQuad(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutQuad(inputvalue,outputmin,outputmax,inputmax)
        _input /= _inputmax;
        return -_outputmax * _input * (_input - 2) + _outputmin;
    }
    float easeInOutQuad(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutQuad(inputvalue,outputmin,outputmax,inputmax)

        _input /= _inputmax * 0.5;

        if (_input < 1)
        {
            return _outputmax * 0.5 * _input * _input + _outputmin;
        }

        return _outputmax * -0.5 * (--_input * (_input - 2) - 1) + _outputmin;
    }


    float easeInCubic(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInCubic(inputvalue,outputmin,outputmax,inputmax)
        return _outputmax * pow(_input / _inputmax, 3) + _outputmin;
    }
    float easeOutCubic(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutCubic(inputvalue,outputmin,outputmax,inputmax)
        return _outputmax * (pow(_input / _inputmax - 1, 3) + 1) + _outputmin;
    }
    float easeInOutCubic(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutCubic(inputvalue,outputmin,outputmax,inputmax)

        _input /= _inputmax * 0.5;

        if (_input < 1)
        {
            return _outputmax * 0.5 * pow(_input, 3) + _outputmin;
        }

        return _outputmax * 0.5 * (pow(_input - 2, 3) + 2) + _outputmin;
    }


    float easeInQuart(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInQuart(inputvalue,outputmin,outputmax,inputmax)
        return _outputmax * pow(_input / _inputmax, 4) + _outputmax;
    }
    float easeOutQuart(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutQuart(inputvalue,outputmin,outputmax,inputmax)
        return -_outputmax * (pow(_input / _inputmax - 1, 4) - 1) + _outputmin;
    }
    float easeInOutQuart(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutQuart(inputvalue,outputmin,outputmax,inputmax)

        _input /= _inputmax * 0.5;

        if (_input < 1)
        {
            return _outputmax * 0.5 * pow(_input, 4) + _outputmin;
        }

        return _outputmax * -0.5 * (pow(_input - 2, 4) - 2) + _outputmin;
    }


    float easeInQuint(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInQuint(inputvalue,outputmin,outputmax,inputmax)

        return _outputmax * pow(_input / _inputmax, 5) + _outputmin;
    }
    float easeOutQuint(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutQuint(inputvalue,outputmin,outputmax,inputmax)

        return _outputmax * (pow(_input / _inputmax - 1, 5) + 1) + _outputmin;
    }
    float easeInOutQuint(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutQuint(inputvalue,outputmin,outputmax,inputmax)

        _input /= _inputmax * 0.5;

        if (_input < 1)
        {
            return _outputmax * 0.5 * pow(_input, 5) + _outputmin;
        }

        return _outputmax * 0.5 * (pow(_input - 2, 5) + 2) + _outputmin;
    }


    float easeInSine(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInSine(inputvalue,outputmin,outputmax,inputmax)

        return _outputmax * (1 - cos(_input / _inputmax * (_PI / 2))) + _outputmin;
    }
    float easeOutSine(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutSine(inputvalue,outputmin,outputmax,inputmax)

        return _outputmax * sin(_input / _inputmax * (_PI / 2)) + _outputmin;
    }
    float easeInOutSine(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutSine(inputvalue,outputmin,outputmax,inputmax)

        return _outputmax * 0.5 * (1 - cos(_PI * _input / _inputmax)) + _outputmin;
    }


    float easeInCirc(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInCirc(inputvalue,outputmin,outputmax,inputmax)

        _input /= _inputmax;
        return _outputmax * (1 - sqrt(1 - _input * _input)) + _outputmin;
    }
    float easeOutCirc(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutCirc(inputvalue,outputmin,outputmax,inputmax)

        _input = _input / _inputmax - 1;
        return _outputmax * sqrt(1 - _input * _input) + _outputmin;
    }
    float easeInOutCirc(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutCirc(inputvalue,outputmin,outputmax,inputmax)

        _input /= _inputmax * 0.5;

        if (_input < 1)
        {
            return _outputmax * 0.5 * (1 - sqrt(1 - _input * _input)) + _outputmin;
        }

        _input -= 2;
        return _outputmax * 0.5 * (sqrt(1 - _input * _input) + 1) + _outputmin;
    }


    float easeInExpo(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInExpo(inputvalue,outputmin,outputmax,inputmax)

        return _outputmax * pow(2, 10 * (_input / _inputmax - 1)) + _outputmin;
    }
    float easeOutExpo(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutExpo(inputvalue,outputmin,outputmax,inputmax)

        return _outputmax * (-pow(2, -10 * _input / _inputmax) + 1) + _outputmin;
    }
    float easeInOutExpo(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutExpo(inputvalue,outputmin,outputmax,inputmax)

        _input /= _inputmax * 0.5;

        if (_input < 1)
        {
            return _outputmax * 0.5 * pow(2, 10 * --_input) + _outputmin;
        }

        return _outputmax * 0.5 * (-pow(2, -10 * --_input) + 2) + _outputmin;
    }


    float easeInElastic(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInElastic(inputvalue,outputmin,outputmax,inputmax)

        float _s = 1.70158;
        float _p = 0;
        float _a = _outputmax;

        if (_input == 0 || _a == 0)
        {
            return _outputmin;
        }

        _input /= _inputmax;

        if (_input == 1)
        {
            return _outputmin + _outputmax;
        }

        if (_p == 0)
        {
            _p = _inputmax * 0.3;
        }

        if (_a < abs(_outputmax))
        {
            _a = _outputmax;
            _s = _p * 0.25;
        }
        else
        {
            _s = _p / (2 * _PI) * asin(_outputmax / _a);
        }

        return -(_a * pow(2, 10 * (--_input)) * sin((_input * _inputmax - _s) * (2 * _PI) / _p)) + _outputmin;
    }
    float easeOutElastic(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutElastic(inputvalue,outputmin,outputmax,inputmax)

        float _s = 1.70158;
        float _p = 0;
        float _a = _outputmax;

        if (_input == 0 || _a == 0)
        {
            return _outputmin;
        }

        _input /= _inputmax;

        if (_input == 1)
        {
            return _outputmin + _outputmax;
        }

        if (_p == 0)
        {
            _p = _inputmax * 0.3;
        }

        if (_a < abs(_outputmax))
        {
            _a = _outputmax;
            _s = _p * 0.25;
        }
        else
        {
            _s = _p / (2 * _PI) * asin(_outputmax / _a);
        }

        return _a * pow(2, -10 * _input) * sin((_input * _inputmax - _s) * (2 * _PI) / _p) + _outputmax + _outputmin;
    }
    float easeInOutElastic(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutElastic(inputvalue,outputmin,outputmax,inputmax)

        float _s = 1.70158;
        float _p = 0;
        float _a = _outputmax;

        if (_input == 0 || _a == 0)
        {
            return _outputmin;
        }

        _input /= _inputmax * 0.5;

        if (_input == 2)
        {
            return _outputmin + _outputmax;
        }

        if (_p == 0)
        {
            _p = _inputmax * (0.3 * 1.5);
        }

        if (_a < abs(_outputmax))
        {
            _a = _outputmax;
            _s = _p * 0.25;
        }
        else
        {
            _s = _p / (2 * _PI) * asin(_outputmax / _a);
        }

        if (_input < 1)
        {
            return -0.5 * (_a * pow(2, 10 * (--_input)) * sin((_input * _inputmax - _s) * (2 * _PI) / _p)) + _outputmin;
        }

        return _a * pow(2, -10 * (--_input)) * sin((_input * _inputmax - _s) * (2 * _PI) / _p) * 0.5 + _outputmax + _outputmin;
    }


    float easeInBack(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInBack(inputvalue,outputmin,outputmax,inputmax)

        float _s = 1.70158;

        _input /= _inputmax;
        return _outputmax * _input * _input * ((_s + 1) * _input - _s) + _outputmin;
    }
    float easeOutBack(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutBack(inputvalue,outputmin,outputmax,inputmax)

        float _s = 1.70158;

        _input = _input / _inputmax - 1;
        return _outputmax * (_input * _input * ((_s + 1) * _input + _s) + 1) + _outputmin;
    }
    float easeInOutBack(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutBack(inputvalue,outputmin,outputmax,inputmax)

        float _s = 1.70158;

        _input = _input / _inputmax * 2;

        if (_input < 1)
        {
            _s *= 1.525;
            return _outputmax * 0.5 * (_input * _input * ((_s + 1) * _input - _s)) + _outputmin;
        }

        _input -= 2;
        _s *= 1.525;

        return _outputmax * 0.5 * (_input * _input * ((_s + 1) * _input + _s) + 2) + _outputmin;
    }


    float easeInBounce(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInBounce(inputvalue,outputmin,outputmax,inputmax)
        return _outputmax - easeOutBounce(_inputmax - _input, 0, _outputmax, _inputmax) + _outputmin;
    }
    float easeOutBounce(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseOutBounce(inputvalue,outputmin,outputmax,inputmax)

        _input /= _inputmax;

        if (_input < 1 / 2.75)
        {
            return _outputmax * 7.5625 * _input * _input + _outputmin;
        }
        else
            if (_input < 2 / 2.75)
            {
                _input -= 1.5 / 2.75;
                return _outputmax * (7.5625 * _input * _input + 0.75) + _outputmin;
            }
            else
                if (_input < 2.5 / 2.75)
                {
                    _input -= 2.25 / 2.75;
                    return _outputmax * (7.5625 * _input * _input + 0.9375) + _outputmin;
                }
                else
                {
                    _input -= 2.625 / 2.75;
                    return _outputmax * (7.5625 * _input * _input + 0.984375) + _outputmin;
                }
    }
    float easeInOutBounce(float _input, float _outputmin, float _outputmax, float _inputmax)
    {/// EaseInOutBounce(inputvalue,outputmin,outputmax,inputmax)

        if (_input < _inputmax*0.5)
        {
            return (easeInBounce(_input * 2, 0, _outputmax, _inputmax)*0.5 + _outputmin);
        }

        return (easeOutBounce(_input * 2 - _inputmax, 0, _outputmax, _inputmax)*0.5 + _outputmax * 0.5 + _outputmin);
    }


}//NS Math


#pragma warning(pop)