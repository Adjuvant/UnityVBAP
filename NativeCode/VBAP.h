#ifndef VBAP_H_INCLUDED
#define VBAP_H_INCLUDED

#include<gsl/gsl_sf_trig.h>
#include<gsl/gsl_math.h>
#include<gsl/gsl_vector.h>
#include<gsl/gsl_sort_vector.h>
#include<iostream>
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

	Cartesian speakers_pos[16];
	Polar speakers_angles[16];

	VBAP();
	~VBAP();
	void pol2car(const Polar* p, Cartesian* c);
	void findVBAP_gain(double azimuth, double elevation, double* vbap_gains, Polar* triplet_angles);
	static const double deg2rad = M_PI/180;
private:

};
#endif
