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

    // Initialize circle points for render
    for (unsigned int i = 0; i < 12; ++i) {
    	circ_points[i][0] = 0.5f*cos((float)i/12*6.2832);
    	circ_points[i][1] = 0.5f*sin((float)i/12*6.2832);
    }
}

void ParticleFilter::render() {
	float position[2] = {0, 0};
	float orientation = 0;
	float score_sum = 0;
	// Calculate filtered position
	for (int i = 0; i<nParticles; ++i) {
		if (render_all) particles.at(i).render(); // Render all particles if option enabled
		position[0] += particles.at(i).score*particles.at(i).position[0];
		position[1] += particles.at(i).score*particles.at(i).position[1];
		orientation += particles.at(i).score*particles.at(i).orientation;
		score_sum += particles.at(i).score;
	}
	position[0] /= (float)score_sum;
	position[1] /= (float)score_sum;
	orientation /= (float)score_sum;

	float spread = 0;
	for (int i = 0; i<nParticles; ++i) {
		float pdiff[3] = {position[0]-particles.at(i).position[0],
						  position[1]-particles.at(i).position[1],
						  orientation-particles.at(i).orientation};
		spread += particles.at(i).score*sqrt(pdiff[0]*pdiff[0]+pdiff[1]*pdiff[1]+pdiff[2]*pdiff[2]);
	}
	spread /= (float)score_sum;
	std::cout << "Filtered localization: (" <<
			position[0] << ", " << position[1] <<
			") with confidence = " << spread << std::endl;

	// Render filter result
	glPushMatrix();
    	glLineWidth(2);
		glScalef(1.0f/(float)gt_map->cols, 1.0f/(float)gt_map->rows, 1.0f);
		glTranslatef((float)position[1]+0.5f,
					 float(gt_map->rows - 1 - position[0])+0.5f, 0);
        glRotatef(-90.0f+orientation*90.0f, 0.0f, 0.0f, -1.0f);
        glScalef(spread, spread, 1);
		// Draw circle
		glBegin(GL_LINE_LOOP);
			glColor3f(0.0f, 0.0f, 0.0f);
			for (unsigned int i = 0; i < 12; ++i) {
				glVertex2f(circ_points[i][0], circ_points[i][1]);
			}
		glEnd();
		// Draw orientation triangle
		glBegin(GL_LINE_LOOP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2f(0.35f, 0.0f);
			glVertex2f(-0.3f, 0.25f);
			glVertex2f(-0.3f, -0.25f);
		glEnd();
    glPopMatrix();
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
    	float val = ((float)sense_acc.at(i)-min_acc)/max_acc; // Normalize new score
    	particles.at(i).score = (9.*particles.at(i).score + val)/10.0; // Update score "average"
    	particles.at(i).set_color(1.0f-particles.at(i).score, particles.at(i).score, 0.0f);

    	if (particles.at(i).score<0.3) {
    		// Discard particle and replace with new one
    		int row, col, ori;
    		if (rand()%10 >= 7) {
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

