#pragma once

#include <GLEngine.h>

class OrbitCalculator
{
public:
    
    static glm::vec2 compute(double angle, double a, double b, double theta, const glm::vec2 &p, int pertN, double pertAmp);
};
