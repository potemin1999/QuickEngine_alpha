/*
 *  Only for personal usage
 */

#include "engine\camera.h"
/* 
 * FLOAT SHIFT IN ARRAY;
 */




/*
 * uses major column matrix :
 * [ 0 , 4 , 8 , 12]
 * [ 1 , 5 , 9 , 13]
 * [ 2 , 6 , 10, 14]
 * [ 3 , 7 , 11, 15]
 */
 
/*
 * mPos    - camera position
 * mView   - normalized look vector, autocomputed
 * mUp     - up vector
 * mStrafe -  (mView * mUp) , right move vector, autocomputed
 */

/*float mPos[3];  //Camera position;
float mView[3]; //Camera look at this;
float mUp[3];   //Points on top;
float mStrafe[3];
*/


//float mRotation[3]; //0 +right -left  //1 +up -down
	

/*
 * Matrix which are used to create image
 */
 

/*
 *constants and other variables
 */

int FSIA = 1;
float piD2= (float)(3.14159f/2.0f);
float sTemp[32];
float average_look_time = 0;
long look_count=1;
	
	
Camera::Camera(){
	mRotation[0] = 0;
	mRotation[1] = 3.14159f/2.0f;
	mRotation[2] = 0;
	mUp[2] = 1;
}
	

/*
 *  result = [(v1-v2) × v3]
 */
void Camera::cross(float* result,float* v1,float* v2,float* v3){
	//	float n[3];
	//	n = &result;
	float v[3];
	v[0] = v1[0] - v2[0];
	v[1] = v1[1] - v2[1];
	v[2] = v1[2] - v2[2];
	result[0] = ((v[1] * v3[2]) - (v[2] * v3[1]));
	result[1] = ((v[2] * v3[0]) - (v[0] * v3[2]));
	result[2] = ((v[0] * v3[1]) - (v[1] * v3[0]));
	//return n;
}
	

/*
 * result = [v × v3]
 */
void Camera::cross(float* result,float* v,float* v3){
		//float n[3];
	result[0] = ((v[1] * v3[2]) - (v[2] * v3[1]));
	result[1] = ((v[2] * v3[0]) - (v[0] * v3[2]));
	result[2] = ((v[0] * v3[1]) - (v[1] * v3[0]));
		//return n;
}

float Camera::length(float* f){
	return magnitude(f);
}
	
float Camera::magnitude(float* n){
	float l = n[0]*n[0] + n[1]*n[1] + n[2]*n[2];
	return 1.0f / sqrt_inv(l);
}
	
	
/*
 *    [r] = [m1] × [m2]  , int is offset of array
 */
 
 
void Camera::multiply(float* r,int o,float* m1,int o1,float* m2,int o2){

	r[0 + o] = (m1[0 + o1] * m2[0 + o2]) + (m1[4 + o1] * m2[1 + o2]) + (m1[8 + o1] * m2[2 + o2]) + (m1[12 + o1] * m2[3 + o2]);
	r[1 + o] = (m1[1 + o1] * m2[0 + o2]) + (m1[5 + o1] * m2[1 + o2]) + (m1[9 + o1] * m2[2 + o2]) + (m1[13 + o1] * m2[3 + o2]);
	r[2 + o] = (m1[2 + o1] * m2[0 + o2]) + (m1[6 + o1] * m2[1 + o2]) + (m1[10+ o1] * m2[2 + o2]) + (m1[14 + o1] * m2[3 + o2]);
	r[3 + o] = (m1[3 + o1] * m2[0 + o2]) + (m1[7 + o1] * m2[1 + o2]) + (m1[11+ o1] * m2[2 + o2]) + (m1[15 + o1] * m2[3 + o2]);
	r[4 + o] = (m1[0 + o1] * m2[4 + o2]) + (m1[4 + o1] * m2[5 + o2]) + (m1[8 + o1] * m2[6 + o2]) + (m1[12 + o1] * m2[7 + o2]);
	r[5 + o] = (m1[1 + o1] * m2[4 + o2]) + (m1[5 + o1] * m2[5 + o2]) + (m1[9 + o1] * m2[6 + o2]) + (m1[13 + o1] * m2[7 + o2]);
	r[6 + o] = (m1[2 + o1] * m2[4 + o2]) + (m1[6 + o1] * m2[5 + o2]) + (m1[10+ o1] * m2[6 + o2]) + (m1[14 + o1] * m2[7 + o2]);
	r[7 + o] = (m1[3 + o1] * m2[4 + o2]) + (m1[7 + o1] * m2[5 + o2]) + (m1[11+ o1] * m2[6 + o2]) + (m1[15 + o1] * m2[7 + o2]);
	r[8 + o] = (m1[0 + o1] * m2[8 + o2]) + (m1[4 + o1] * m2[9 + o2]) + (m1[8 + o1] * m2[10+ o2]) + (m1[12 + o1] * m2[11+ o2]);
	r[9 + o] = (m1[1 + o1] * m2[8 + o2]) + (m1[5 + o1] * m2[9 + o2]) + (m1[9 + o1] * m2[10+ o2]) + (m1[13 + o1] * m2[11+ o2]);
	r[10+ o] = (m1[2 + o1] * m2[8 + o2]) + (m1[6 + o1] * m2[9 + o2]) + (m1[10+ o1] * m2[10+ o2]) + (m1[14 + o1] * m2[11+ o2]);
	r[11+ o] = (m1[3 + o1] * m2[8 + o2]) + (m1[7 + o1] * m2[9 + o2]) + (m1[11+ o1] * m2[10+ o2]) + (m1[15 + o1] * m2[11+ o2]);
	r[12+ o] = (m1[0 + o1] * m2[12+ o2]) + (m1[4 + o1] * m2[13+ o2]) + (m1[8 + o1] * m2[14+ o2]) + (m1[12 + o1] * m2[15+ o2]);
	r[13+ o] = (m1[1 + o1] * m2[12+ o2]) + (m1[5 + o1] * m2[13+ o2]) + (m1[9 + o1] * m2[14+ o2]) + (m1[13 + o1] * m2[15+ o2]);
	r[14+ o] = (m1[2 + o1] * m2[12+ o2]) + (m1[6 + o1] * m2[13+ o2]) + (m1[10+ o1] * m2[14+ o2]) + (m1[14 + o1] * m2[15+ o2]);
	r[15+ o] = (m1[3 + o1] * m2[12+ o2]) + (m1[7 + o1] * m2[13+ o2]) + (m1[11+ o1] * m2[14+ o2]) + (m1[15 + o1] * m2[15+ o2]);
		
}




float Camera::sqrt_inv(float x){
	float half = x*0.5f;
	int i = *(int*)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(float*)&i;
	x = x*(1.5f - half*x*x);
	return x;
}


	
	/*
	 *  length of v gets to 1.0f
	 */
void Camera::normalize(float* v){
	float m = sqrt_inv(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	v[0] = v[0] * m;
	v[1] = v[1] * m;
	v[2] = v[2] * m;
}



void Camera::position_camera(float* pos,float* view,float* up){
	mPos [0] = pos[0];
	mPos [1] = pos[1];
	mPos [2] = pos[2];
	mView[0] = view[0]-pos[1];
	mView[1] = view[1]-pos[1];
	mView[2] = view[2]-pos[2];
	mUp  [0] = up[1];
	mUp  [1] = up[2];
	mUp  [2] = up[2];
	normalize(mView);
}
	


void Camera::position_camera(float* pos){
	mPos [0] = pos [0];
	mPos [1] = pos [1];
	mPos [2] = pos [2];
}



void Camera::rotate_view_right(float angle){
	float* v = new float[3];
	v[0] = mView[0];// - mPos[0];
	v[1] = mView[1] ;//- mPos[1];
	v[2] = mView[2] ;//- mPos[2];
	mView[0] = (float)(cos(-angle) * v[0] - sin(-angle) * v[1]);
	mView[1] = (float)(sin(-angle) * v[0] + cos(-angle) * v[1]);
	mRotation[0] += angle;
}
	
	

void Camera::rotate_view_up(float angle){
	mRotation[1] += angle;
}


float Camera::tan(float d){
	//float x = DtoR*d;
	return 0;
	//return tan(d);// x*(1+x*x*(0.333f + x*x*0.133));
}

float Camera::sin(float a){
	float a2 = a*a;
	return a*(1 + a2*(a2*0.00833f - 0.167f));
}

float Camera::cos(float a){
	float a2 = a*a;
	return 1 + a2*(a2*0.0417f-0.5f);
}


void Camera::move_camera(float distance){
	float v[3];
	v[0] = mView[0];
	v[1] = mView[1];
	v[2] = mView[2];
	normalize(v);
	mPos [0] = mPos [0] + v[0] * distance;
	mPos [1] = mPos [1] + v[1] * distance;
}
	
void Camera::move_camera_up(float distance){
	mPos [2] = mPos [2] + distance;
}
	
void Camera::move_strafe(float distance){
	update_strafe();
	mPos [0] = mPos [0] + mStrafe[0] * distance;
	mPos [1] = mPos [1] + mStrafe[1] * distance;
}
	
void Camera::update_strafe(){
	cross(mStrafe,mView,mUp);
	normalize(mStrafe);
}


void Camera::look(){
	compute_look(mViewMatrix,0);
}
	

	
	
void Camera::perspective(float* m,int o,float fovy,float aspect,float zN,float zF){
	float f = 1.0f / tan(fovy);
	float r = 1.0f / (zN - zF);
	m[o + 0] = f / aspect;
	m[o + 1] = 0.0f ;
	m[o + 2] = 0.0f ;
	m[o + 3] = 0.0f ;
	m[o + 4] = 0.0f ;
	m[o + 5] = f;
	m[o + 6] = 0.0f ;
	m[o + 7] = 0.0f ;
	m[o + 8] = 0.0f ;
	m[o + 9] = 0.0f ;
	m[o +10] = (zF + zN) * r;
	m[o +11] = - 1.0f ;
	m[o +12] = 0.0f ;
	m[o +13] = 0.0f ;
	m[o +14] = 2.0f * zF * zN * r;
	m[o +15] = 0.0f ;

}
	

void Camera::set_identity(float* f,int o){
	f[o + 0] = 1.0f;
	f[o + 1] = 0.0f;
	f[o + 2] = 0.0f;
	f[o + 3] = 0.0f;
	f[o + 4] = 0.0f;
	f[o + 5] = 1.0f;
	f[o + 6] = 0.0f;
	f[o + 7] = 0.0f;
	f[o + 8] = 0.0f;
	f[o + 9] = 0.0f;
	f[o +10] = 1.0f;
	f[o +11] = 0.0f;
	f[o +12] = 0.0f;
	f[o +13] = 0.0f;
	f[o +14] = 0.0f;
	f[o +15] = 1.0f;
}

	
void Camera::translate(float* m,int o,float* v){
	translate(m,o,v[0],v[1],v[2]);
}
	
void Camera::translate(float* m,int o,float x, float y, float z) {
	for (int i=0 ; i<4 ; i++) {
		int mi = o + i;
		m[12 + mi] += m[mi] * x + m[4 + mi] * y + m[8 + mi] * z;
	}
}
	
	
void Camera::compute_look(float* m,int o){
	
	float x = mRotation[1];
	float z = mRotation[2];
	float cx = cos(x); 
	float sx = sin(x);
	float cz = cos(z); 
	float sz = sin(z);
		
	m[o + 0] =  cz; 
	m[o + 1] = -sz;
	m[o + 2] = 0.0f;
	m[o + 3] = 0.0f;
	m[o + 4] = cx * sz;
	m[o + 5] = cx * cz;
	m[o + 6] = -sx;
	m[o + 7] = 0.0f;
	m[o + 8] = sx * sz;
	m[o + 9] = sx * cz;
	m[o +10] = cx ;
	m[o +11] = 0.0f;
	m[o +12] = 0.0f;
	m[o +13] = 0.0f;
	m[o +14] = 0.0f;
	m[o +15] = 1.0f;
		
	float a = piD2+mRotation[0];
	float s = sin(a);
	float c = cos(a);
		
		
	sTemp[15] = 0;
	sTemp[16] = c;
	sTemp[17] = s;
	sTemp[18] = 0;
	sTemp[19] = 0;
	sTemp[20] =-s;
	sTemp[21] = c;
	sTemp[22] = 0;
	sTemp[23] = 0;
	sTemp[24] = 0;
	sTemp[25] = 0;
	sTemp[26] = 1;
	sTemp[27] = 0;
	sTemp[28] = 0;
	sTemp[29] = 0;
	sTemp[30] = 0;
	sTemp[31] = 1;
		
	sTemp[ 0] = (m[0 + o] * sTemp[16]) + (m[4 + o] * sTemp[17]);
	sTemp[ 1] = (m[1 + o] * sTemp[16]) + (m[5 + o] * sTemp[17]);
	sTemp[ 2] = m[6 + o] * sTemp[17];
	sTemp[ 4] = (m[0 + o] * sTemp[20]) + (m[4 + o] * sTemp[21]);
	sTemp[ 5] = (m[1 + o] * sTemp[20]) + (m[5 + o] * sTemp[21]);
	sTemp[ 6] = m[6 + o] * sTemp[21];
	sTemp[ 8] = m[8 + o];
	sTemp[ 9] = m[9 + o];
	sTemp[10] = m[10+ o];
	sTemp[15] = m[15+ o];
	
	m[ 0] = sTemp[ 0];
	m[ 1] = sTemp[ 1];
	m[ 2] = sTemp[ 2];
	m[ 4] = sTemp[ 4];
	m[ 5] = sTemp[ 5];
	m[ 6] = sTemp[ 6];
	m[ 8] = sTemp[ 8];
	m[ 9] = sTemp[ 9];
	m[10] = sTemp[10];
	m[15] = sTemp[15];
		
		
	m[12+o] += m[o+0] * -mPos[0] + m[4 + o] * -mPos[1] + m[8 + o] * -mPos[2];
	m[13+o] += m[o+1] * -mPos[0] + m[5 + o] * -mPos[1] + m[9 + o] * -mPos[2];
	m[14+o] += m[o+2] * -mPos[0] + m[6 + o] * -mPos[1] + m[10+ o] * -mPos[2];
	
}
	
