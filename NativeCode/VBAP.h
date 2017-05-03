#ifndef VBAP_H_INCLUDED
#define VBAP_H_INCLUDED
#include<gsl/gsl_sf_trig.h>
#include<gsl/gsl_math.h>
#include<gsl/gsl_vector.h>
#include<gsl/gsl_matrix.h>
#include<gsl/gsl_linalg.h>
#include<gsl/gsl_permutation.h>
#include<math.h>
using namespace std;

class VBAP{
public:
	struct Polar{
		double azimuth;
		double elevation;
		double distance;
	};

	struct Cartesian{
		double x;
		double y;
		double z;
	};

	struct Triplet{
		Polar polar[3];
		Cartesian cartesian[3];
	};

	Cartesian speakers_pos[16];
	Polar speakers_angles[16];
	Triplet triplets[24];

	VBAP();
	~VBAP();
	void pol2car(const Polar* p, Cartesian* c);
	void findVBAP_gain(double azimuth, double elevation, double* gain_vector, Triplet* t);
	const double deg2rad = M_PI/180;
private:
	void create_triplets(Triplet* t, const Polar* p, const Cartesian* c);

};
#endif
