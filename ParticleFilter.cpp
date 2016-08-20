/*
 * ParticleFilter.cpp
 *
 *  Created on: Aug 19, 2016
 *      Author: lothas
 */

#include "ParticleFilter.h"

ParticleFilter::ParticleFilter(int _nParticles) {
	nParticles = _nParticles;
}

ParticleFilter::ParticleFilter(int _nParticles, Matrix2D* _gt_map) {
	nParticles = _nParticles;
	gt_map = _gt_map;
}

ParticleFilter::~ParticleFilter() {
}

void ParticleFilter::initialize() {
    for (int i = 0; i<nParticles; ++i) {
    	int col = rand()%gt_map->cols;
    	int row = rand()%gt_map->rows;
    	int ori = rand()%4;
    	MobileRobot particle(col, row, ori);
    	particle.set_map(gt_map);
    	particle.set_color(0.0f, 0.8f, 0.3f);

    	particles.push_back(particle);
    }

    // Set-up kernel
    kernel.data[1][1] = 2;
    // [[1,1,1]
    //  [1,2,1]
    //  [1,1,1]]
}

void ParticleFilter::render() {
    for (int i = 0; i<nParticles; ++i) {
    	particles.at(i).render();
    }
}

void ParticleFilter::actuate(act_type action) {
    for (int i = 0; i<nParticles; ++i) {
    	particles.at(i).actuate(action);
    }
    sense();
}

void ParticleFilter::sense() {
    Matrix2D* robot_sense_mat = robot->rect_sense();

    vector<int> sense_acc;
    int max_id = 0;
    float max_acc = 0;
    float min_acc = robot->sense_dist*(2*robot->sense_dist-1);
    for (int i = 0; i<nParticles; ++i) {
    	Matrix2D* particle_sense_mat = particles.at(i).rect_sense();
    	// Compare sense from robot and particle
		int val = 0;
		for (unsigned int i = 0; i<robot_sense_mat->rows; ++i) {
			for (unsigned int j = 0; j<robot_sense_mat->cols; ++j) {
				if (robot_sense_mat->data[i][j] == particle_sense_mat->data[i][j]) ++val;
			}
		}

		sense_acc.push_back(val);
		if (val > max_acc) { max_acc = val; max_id = i; }
		if (val < min_acc) min_acc = val;
//    	Matrix2D* particle_sense_mat_gauss = particle_sense_mat->convolute(&kernel);
//
//    	int val = 0;
//        for (unsigned int i = 0; i<robot_sense_mat->rows; ++i) {
//            for (unsigned int j = 0; j<robot_sense_mat->cols; ++j) {
//				val += robot_sense_mat->data[i][j]*particle_sense_mat_gauss->data[i][j];
//            }
//        }
    }

    // Update particle color based on sense accuracy
    for (int i = 0; i<nParticles; ++i) {
    	float val = ((float)sense_acc.at(i)-min_acc)/max_acc;
    	particles.at(i).set_color(1.0f-val, val, 0.0f);

    	if (val<0.5 && max_acc-min_acc > 2) {
    		// Discard particle and replace with new one
    		int row, col, ori;
    		if (rand()%10 >= 9) {
    			// Replace with random particle
    			col = rand()%gt_map->cols;
    			row = rand()%gt_map->rows;
    			ori = rand()%4;
    		}
    		else {
    			// Replace with best particle + random
    			col = particles.at(max_id).position[1] + rand()%2;
    			row = particles.at(max_id).position[0] + rand()%2;
    			ori = particles.at(max_id).orientation + rand()%2;
    		}
        	particles.at(i).position[0] = row;
        	particles.at(i).position[1] = col;
        	particles.at(i).orientation = ori;
    		particles.at(i).set_color(0.5f, 0.5f, 0.5f);
    	}
    }
}

