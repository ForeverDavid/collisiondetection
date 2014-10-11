#include "PenaltyGroup.h"
#include "PenaltyPotential.h"
#include <iostream>

using namespace Eigen;
using namespace std;

PenaltyGroup::PenaltyGroup(double dt, double eta, double stiffness) : nextstep_(0), dt_(dt), eta_(eta), stiffness_(stiffness)
{	
}

PenaltyGroup::~PenaltyGroup()
{
	for(vector<VertexFacePenaltyPotential *>::iterator it = vfforces_.begin(); it != vfforces_.end(); ++it)
		delete *it;
	for(vector<EdgeEdgePenaltyPotential *>::iterator it = eeforces_.begin(); it != eeforces_.end(); ++it)
		delete *it;
}

void PenaltyGroup::addVFStencil(VertexFaceStencil vfstencil)
{
	vfforces_.push_back(new VertexFacePenaltyPotential(vfstencil, eta_, stiffness_));
}

void PenaltyGroup::addEEStencil(EdgeEdgeStencil eestencil)
{
	eeforces_.push_back(new EdgeEdgePenaltyPotential(eestencil, eta_, stiffness_));
}

void PenaltyGroup::addForce(const Eigen::VectorXd &q, Eigen::VectorXd &F)
{
	VectorXd groupforce(q.size());
	groupforce.setZero();
	for(vector<VertexFacePenaltyPotential *>::iterator it = vfforces_.begin(); it != vfforces_.end(); ++it)
		(*it)->addForce(q,groupforce);
	for(vector<EdgeEdgePenaltyPotential *>::iterator it = eeforces_.begin(); it != eeforces_.end(); ++it)		
		(*it)->addForce(q,groupforce);

	F += groupforce * dt_;
}

void PenaltyGroup::incrementTimeStep()
{
	nextstep_++;
}

double PenaltyGroup::nextFireTime() const
{
	return nextstep_ * dt_;
}

void PenaltyGroup::rollback()
{
	nextstep_ = 0;
}