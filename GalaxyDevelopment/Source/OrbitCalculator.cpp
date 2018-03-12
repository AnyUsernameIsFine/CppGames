#include "OrbitCalculator.h"
#include <cmath>

using namespace std;


glm::vec2 OrbitCalculator::compute(double angle, double a, double b, double theta, const glm::vec2 &p, int pertN, double pertAmp)
{
	double beta = -angle,
		alpha = theta * M_PI / 180.0;

	// temporaries to save cpu time
	double  cosalpha = cos(alpha),
		sinalpha = sin(alpha),
		cosbeta = cos(beta),
		sinbeta = sin(beta);

	glm::vec2 pos = {
		p.x + (a * cosalpha * cosbeta - b * sinalpha * sinbeta),
		p.y + (a * cosalpha * sinbeta + b * sinalpha * cosbeta)
	};

	// Add small perturbations to create more spiral arms
	if (pertAmp > 0 && pertN > 0) {
		pos.x += (a / pertAmp) * sin(alpha * 2 * pertN);
		pos.y += (a / pertAmp) * cos(alpha * 2 * pertN);
	}

	return pos;
}