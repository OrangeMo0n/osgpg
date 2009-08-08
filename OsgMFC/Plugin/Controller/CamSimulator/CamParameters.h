#pragma once

#include "CamCalibrator.h"
#include <osg/Geometry>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

namespace FC {
#define MAX_CORRESPONDENT_NUM 20
	struct CamParameters {
		Uint correspondentNum;

		//unit: meter
		WorldPoint world[MAX_CORRESPONDENT_NUM];
		ImagePoint image[MAX_CORRESPONDENT_NUM];

		K_Matrix k;
		R_Matrix r;
		C_Matrix c;
		PrincipleAxis pa;
		P_Matrix p;

		//unit: meter
		double f;				//valid camera focus length
		double dx,dy;		//pixel size

		//unit: pixel
		int u0,v0;		//principle point
		int pw,ph;		//photo size

		std::string photoPath;

		bool needReComputeParameters;

		//camera file format:
		//f
		//dx dy
		//u0 v0
		//pw ph
		//correspondentNum
		//WorldPoint ImagePoint
		//...
		//photoPath
		CamParameters(std::ifstream& camFile);

		CamParameters();

		CamParameters(const CamParameters& rhs);
		CamParameters& operator=(const CamParameters& rhs);

		inline void computeParameters() {
			if(needReComputeParameters) {
				FC::calibrate(k, correspondentNum, image, world, r, c, pa, p);
				needReComputeParameters = false;
			}
		}

		inline void computeLookAt(osg::Vec3& eye, osg::Vec3& look, osg::Vec3& up) {
			eye = osg::Vec3(c[0], c[1], c[2]);
			look = osg::Vec3(pa[0], pa[1], pa[2]);
			up = osg::Vec3(-r[1][0], -r[1][1], -r[1][2]);
		}
		
		//frustum[6]: near, far, left, right, top, bottom
		//near = f, far = f + depth
#define Near 0
#define Far 1
#define Left 2
#define Right 3
#define Top 4
#define Bottom 5
		inline void computerDefaultFrustum(double depth, double frustum[6]) {
			frustum[Near]		= f;
			frustum[Far]		= frustum[Near] + depth;
			frustum[Left]		= -(u0   )*dx*frustum[Near]/f;
			frustum[Right]	=  (pw-u0)*dx*frustum[Near]/f;
			frustum[Top]		=  (ph-v0)*dy*frustum[Near]/f;
			frustum[Bottom]	= -(v0   )*dy*frustum[Near]/f;
		}
	};
}