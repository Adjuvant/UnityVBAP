#include "VBAP.h"

VBAP::VBAP(){
	
	for (int i=0;i<16;++i){
		if (i<8){
			speakers_angles[i].azimuth=i*45;
			speakers_angles[i].elevation=0;
		}else if(i<12){
			speakers_angles[i].azimuth=((i-7)*90.0)-45;
			speakers_angles[i].elevation=40;
		}else{
			speakers_angles[i].azimuth=((i-11)*90.0)-45;
			speakers_angles[i].elevation=-40;
		}
		speakers_angles[i].distance=1;
		pol2car(&speakers_angles[i],&speakers_pos[i]);
	}
}

VBAP::~VBAP(){

}

void VBAP::pol2car(const Polar* p, Cartesian *c){
	double azimuth_rad = ((p->azimuth)-180.0)*deg2rad;
	c->x=(p->distance)*gsl_sf_sin(-azimuth_rad);
	c->y=(p->distance)*gsl_sf_sin((p->elevation)*deg2rad);
	c->z=-(p->distance)*gsl_sf_cos(azimuth_rad);
}

void VBAP::findVBAP_gain(double azimuth, double elevation, double* vbap_gain, VBAP::Polar* triplet_angles){
	gsl_vector* dist_vec = gsl_vector_alloc(16);
	for (int i=0;i<16;++i){
		gsl_vector_set(dist_vec,i,abs(azimuth-speakers_angles[i].azimuth)+abs(elevation-speakers_angles[i].elevation));
	}
	size_t spk_idx[3];
	gsl_sort_vector_smallest_index(spk_idx,3,dist_vec);
	
	for (int i=0;i<3;++i){
		triplet_angles[i]=speakers_angles[spk_idx[i]];
	}
}

int main(){
	VBAP vbap = VBAP ();
	//VBAP::Polar pol ={0,0,1};
	//pol.distance=1;pol.elevation=30;pol.azimuth=80;
	//VBAP::Cartesian cat;
	//vbap.pol2car(&pol,&cat);
	/*
	for (int i=0;i<16;++i){
		VBAP::Cartesian& cat = vbap.speakers_pos[i];
		cout<<"Catesian " <<i<<" Polar "<<vbap.speakers_angles[i].azimuth<<" "<<vbap.speakers_angles[i].elevation<<endl;
		cout<<cat.x<<" "<<cat.y<<" "<<cat.z<<endl;
	}
	*/
	double a = 35;
	double e = 45;
	double v[3];
	VBAP::Polar tri[3];
	vbap.findVBAP_gain(a,e,v,tri);
	for(int i=0;i<3;++i){
		cout<<tri[i].azimuth<<" "<<tri[i].elevation<<endl;
	}

}
