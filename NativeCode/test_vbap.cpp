#include "VBAP.h"
#include <iostream>

int main(){
	VBAP vbap = VBAP();
	/*for (int i=0;i<16;++i){
		VBAP::Cartesian& cat = vbap.speakers_pos[i];
		cout<<"Catesian " <<i<<" Polar "<<vbap.speakers_angles[i].azimuth<<" "<<vbap.speakers_angles[i].elevation<<endl;
		cout<<cat.x<<" "<<cat.y<<" "<<cat.z<<endl;
	}*/
	/*
	for(int i=0;i<24;++i){
		for(int j=0;j<3;++j){
			cout<<vbap.triplets[i].polar[j].azimuth<<" "<<vbap.triplets[i].polar[j].elevation<<" "<<i<<endl;
			cout<<vbap.triplets[i].cartesian[j].x<<" "<<vbap.triplets[i].cartesian[j].y<<" "<<vbap.triplets[i].cartesian[j].z<<" "<<i<<endl;
	
		}
	}a
	*/
	double g[3];
	VBAP::Triplet t;
	
	for (int a=0;a<360;a+=10){
		for(int e=-40;e<40;e+=10){
			cout<<"a "<<a<<"e "<<e<<endl;
			vbap.findVBAP_gain((double)a,(double)e,g,&t);
			cout<<"vbap "<<g[0]<<" "<<g[1]<<" "<<g[2]<<endl;
			for(int i=0;i<3;++i){
				cout<<"Triplet"<<i<<" "<<t.cartesian[i].x<<" "<<t.cartesian[i].y<<" "<<t.cartesian[i].z<<" "<<endl;
			}
		}
	}
}
