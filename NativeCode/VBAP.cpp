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
	create_triplets(triplets,speakers_angles,speakers_pos);
}

VBAP::~VBAP(){

}

void VBAP::pol2car(const Polar* p, Cartesian *c){
	double azimuth_rad = ((p->azimuth)-180.0)*deg2rad;
	c->x=(p->distance)*gsl_sf_sin(-azimuth_rad);
	c->y=(p->distance)*gsl_sf_sin((p->elevation)*deg2rad);
	c->z=-(p->distance)*gsl_sf_cos(azimuth_rad);
}

void VBAP::create_triplets(VBAP::Triplet* t,const VBAP::Polar* p,const VBAP::Cartesian* c){
	int t_idx;
	for (int side=0;side<4;++side){
		for(int n=0;n<6;++n){
			t_idx=side*6+n;
			t[t_idx].polar[0]=p[side*2];
			t[t_idx].cartesian[0]=c[side*2];
			//0=front,1=right,2=back,3=left 
				//front left up down
			if (n<2){
				if(side==0){
					t[t_idx].polar[1]=p[7];
					t[t_idx].polar[2]=p[4*n+11];
					t[t_idx].cartesian[1]=c[7];
					t[t_idx].cartesian[2]=c[4*n+11];

				}else{
					t[t_idx].polar[1]=p[(side-1)*2+1];
					t[t_idx].polar[2]=p[4*n+side+7];
					t[t_idx].cartesian[1]=c[(side-1)*2+1];
					t[t_idx].cartesian[2]=c[4*n+side+7];
				}
			//front right up down
			}else if(n<4){
				t[t_idx].polar[1]=p[side*2+1];
				t[t_idx].polar[2]=p[4*(n-2)+8+side];
				t[t_idx].cartesian[1]=c[side*2+1];
				t[t_idx].cartesian[2]=c[4*(n-2)+8+side];
	
			//front center up down
			}else{
				if(side ==0){
					t[t_idx].polar[1]=p[4*(n-4)+11];
					t[t_idx].polar[2]=p[4*(n-4)+8];
					t[t_idx].cartesian[1]=c[4*(n-4)+11];
					t[t_idx].cartesian[2]=c[4*(n-4)+8];

				}else{
					t[t_idx].polar[1]=p[4*(n-4)+side+7];
					t[t_idx].polar[2]=p[4*(n-4)+side+8];
					t[t_idx].cartesian[1]=c[4*(n-4)+side+7];
					t[t_idx].cartesian[2]=c[4*(n-4)+side+8];
				}
			}
		}
	}
}
		
void VBAP::findVBAP_gain(double azimuth, double elevation, double* gain_array, Triplet* t){
	if(elevation >40){
		elevation=40;
	}else if(elevation <-40){
		elevation=-40;
	}
	
	double e_bound;
	//find the right triplet
	if (azimuth<=45){
		e_bound=40.0*azimuth/45.0;
		if (elevation>0){
			*t=elevation > e_bound?triplets[4]:triplets[2];
		}else{
			*t=elevation<(-e_bound)?triplets[5]:triplets[3];
		}
	}else if(azimuth<=90){
		e_bound=40.0*(90-azimuth)/45;
		if (elevation>0){
			*t=elevation>e_bound?triplets[10]:triplets[6];
		}else{
			*t=elevation<(-e_bound)?triplets[11]:triplets[7];
		}
	}else if(azimuth<=135){
		e_bound=40.0*(azimuth-90)/45;
		if (elevation>0){
			*t=elevation>e_bound?triplets[10]:triplets[8];
		}else{
			*t=elevation<(-e_bound)?triplets[11]:triplets[9];
		}
	}else if(azimuth<=180){
		e_bound=40.0*(180-azimuth)/45;
		if (elevation>0){
			*t=elevation>e_bound?triplets[16]:triplets[12];
		}else{
			*t=elevation<(-e_bound)?triplets[17]:triplets[13];
		}
	}else if(azimuth<=235){
		e_bound=40.0*(azimuth-235)/45;
		if (elevation>0){
			*t=elevation>e_bound?triplets[16]:triplets[14];
		}else{
			*t=elevation<(-e_bound)?triplets[17]:triplets[15];
		}
	}else if(azimuth<=270){
		e_bound=40.0*(270-azimuth)/45;
		if (elevation>0){
			*t=elevation>e_bound?triplets[22]:triplets[18];
		}else{
			*t=elevation<(-e_bound)?triplets[23]:triplets[19];
		}
	}else if(azimuth<=315){
		e_bound=40.0*(azimuth-315)/45;
		if (elevation>0){
			*t=elevation>e_bound?triplets[22]:triplets[20];
		}else{
			*t=elevation<(-e_bound)?triplets[23]:triplets[21];
		}
	}else{
		e_bound=40.0*(315-azimuth)/45;
		if (elevation>0){
			*t=elevation>e_bound?triplets[4]:triplets[0];
		}else{
			*t=elevation<(-e_bound)?triplets[5]:triplets[1];
		}
	}
	
	gsl_matrix* triplets_matrix = gsl_matrix_alloc(3,3);
	for(int i=0;i<3;++i){
		gsl_matrix_set(triplets_matrix,0,i,t->cartesian[i].x);
		gsl_matrix_set(triplets_matrix,1,i,t->cartesian[i].y);
		gsl_matrix_set(triplets_matrix,2,i,t->cartesian[i].z);
	}	

	Cartesian source_pos;
	Polar source_angle = {azimuth,elevation,1};
	//source_angle.azimuth=azimuth;
	//source_angle.elevation=elevation;
	//source_angle.distance=1;
	pol2car(&source_angle,&source_pos);
	gsl_vector* source_vector = gsl_vector_alloc(3);
	double* sv_ptr=gsl_vector_ptr(source_vector,0);
	*(++sv_ptr)=source_pos.x;
	*(++sv_ptr)=source_pos.y;
	*(sv_ptr)=source_pos.z;

	gsl_permutation* p = gsl_permutation_alloc(3);
	int s;
	gsl_linalg_LU_decomp(triplets_matrix,p,&s);
	gsl_vector* gain_vector = gsl_vector_alloc(3);
	gsl_linalg_LU_solve(triplets_matrix,p,source_vector,gain_vector);
	double* temp=gsl_vector_ptr(gain_vector,0);
	double norm = 0;
	for(int i=0;i<3;++i){
		norm+=gsl_pow_2(temp[i]);
	}
	norm=sqrt(norm);
	for(int i=0;i<3;++i){
		gain_array[i]=temp[i]/norm;
	}

	gsl_permutation_free(p);
	gsl_matrix_free(triplets_matrix);
	gsl_vector_free(source_vector);
	gsl_vector_free(gain_vector);
	

}
/*
int main(){
	VBAP vbap = VBAP ();
	//VBAP::Polar pol ={0,0,1};
	//pol.distance=1;pol.elevation=30;pol.azimuth=80;
	//VBAP::Cartesian cat;
	//vbap.pol2car(&pol,&cat);
	
	for (int i=0;i<16;++i){
		VBAP::Cartesian& cat = vbap.speakers_pos[i];
		cout<<"Catesian " <<i<<" Polar "<<vbap.speakers_angles[i].azimuth<<" "<<vbap.speakers_angles[i].elevation<<endl;
		cout<<cat.x<<" "<<cat.y<<" "<<cat.z<<endl;
	}
	*/
/*
	double a = 35;
	double e = 45;
	double v[3];
	VBAP::Polar tri[3];
	vbap.findVBAP_gain(a,e,v,tri);
	for(int i=0;i<3;++i){
		cout<<tri[i].azimuth<<" "<<tri[i].elevation<<endl;
	}

}
*/
