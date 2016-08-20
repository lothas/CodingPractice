/*
 * ParticleFilter.h
 *
 *  Created on: Aug 19, 2016
 *      Author: lothas
 */

#ifndef PARTICLEFILTER_H_
#define PARTICLEFILTER_H_

#include <iostream>
#include <vector>
#include "MobileRobot.h"
#include "Matrix2D.h"

using namespace std;

class ParticleFilter {
public:
	ParticleFilter(int _nParticles);
	ParticleFilter(int _nParticles, Matrix2D* _gt_map);

	void set_robot(MobileRobot* _robot) { robot = _robot; }
	void set_gt_map(Matrix2D* _gt_map) { gt_map = _gt_map; }
	void initialize();
	void render();

	void actuate(act_type action);
	void sense();

	virtual ~ParticleFilter();

private:
	int nParticles;
	vector<MobileRobot> particles;

	MobileRobot* robot;
	Matrix2D* gt_map = nullptr;
	Matrix2D kernel = Matrix2D(3,3,1);
};

#endif /* PARTICLEFILTER_H_ */
