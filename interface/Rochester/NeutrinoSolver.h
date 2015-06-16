#ifndef HNSOLVER
#define HNSOLVER
#include <TMatrixD.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <iostream>

using namespace std;
using namespace TMath;

class NeutrinoSolver
{
	private:
		double Mt;
		double Mw;
		double Ml;
		double Mb;
		double Mn;

		bool ERROR;
		TMatrixD H;
		TMatrixD T;
		TMatrixD MET;
		TMatrixD VM;
		TMatrixD RotationX(double a);
		TMatrixD RotationY(double a);
		TMatrixD RotationZ(double a);

		void Solve(double t);
		TMatrixD GetPtSolution(double t);
		TLorentzVector GetSolution(double t);
		pair<double, double> Extrem(double t, bool MIN = true);
		double Chi2(double t);
	public:
		NeutrinoSolver(const TLorentzVector* lep, const TLorentzVector* bjet, double MW = 80, double MT = 173);
		TLorentzVector GetBest(double metx, double mety, double metxerr, double metyerr, double metxyrho, double& test);

};


#endif /* HNSOLVER */
