/*
 * Histogram.cpp
 *
 *  Created on: 28 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/HistHelpers/Histogram.h"

namespace BAT {

Histogram::Histogram(std::string name, std::string title, unsigned int nbinsX, double xmin, double xmax) :
		name_(name), //
		title_(title), //
		nbinsX_(nbinsX), //
		nbinsY_(0), //
		nbinsZ_(0), //
		xmin_(xmin), //
		xmax_(xmax), //
		ymin_(0), //
		ymax_(0), //
		zmin_(0), //
		zmax_(0), //
		binContent_(boost::extents[nbinsX_ + 2][nbinsY_][nbinsZ_]), //
		numberOfEntries_(0), //
		underflow_(0), //
		overflow_(0) {

}

Histogram::Histogram(std::string name, std::string title, unsigned int nbinsX, double xmin, double xmax,
		unsigned int nbinsY, double ymin, double ymax) :
		name_(name), //
		title_(title), //
		nbinsX_(nbinsX), //
		nbinsY_(nbinsY), //
		nbinsZ_(0), //
		xmin_(xmin), //
		xmax_(xmax), //
		ymin_(ymin), //
		ymax_(ymax), //
		zmin_(0), //
		zmax_(0), //
		binContent_(boost::extents[nbinsX_][nbinsY_][nbinsZ_]), //
		numberOfEntries_(0), //
		underflow_(0), //
		overflow_(0) {

}

Histogram::Histogram(std::string name, std::string title, unsigned int nbinsX, double xmin, double xmax,
		unsigned int nbinsY, double ymin, double ymax, unsigned int nbinsZ, double zmin, double zmax) :
		name_(name), //
		title_(title), //
		nbinsX_(nbinsX), //
		nbinsY_(nbinsY), //
		nbinsZ_(nbinsZ), //
		xmin_(xmin), //
		xmax_(xmax), //
		ymin_(ymin), //
		ymax_(ymax), //
		zmin_(zmin), //
		zmax_(zmax), //
		binContent_(boost::extents[nbinsX_][nbinsY_][nbinsZ_]), //
		numberOfEntries_(0), //
		underflow_(0), //
		overflow_(0) {

}

Histogram::~Histogram() {
}

unsigned int Histogram::GetNumberOfXBins() const {
	return nbinsX_;
}

unsigned int Histogram::GetNumberOfYBins() const {
	return nbinsY_;
}

unsigned int Histogram::GetNumberOfZBins() const {
	return nbinsZ_;
}

double Histogram::GetMinX() const {
	return xmin_;
}

double Histogram::GetMinY() const {
	return ymin_;
}

double Histogram::GetMinZ() const {
	return zmin_;
}

double Histogram::GetMaxX() const {
	return xmax_;
}

double Histogram::GetMaxY() const {
	return ymax_;
}

double Histogram::GetMaxZ() const {
	return zmax_;
}

std::string Histogram::GetName() const {
	return name_;
}

std::string Histogram::GetTitle() const {
	return title_;
}

unsigned long Histogram::GetEntries() const {
	return numberOfEntries_;
}

void Histogram::Fill(double x, double weight){
	unsigned int bin = FindBin(x);
	binContent_[bin][0][0] += weight;
}

unsigned int Histogram::FindBin(double x) const{
	return 0;
}

double Histogram::GetBinContent(unsigned int bin) const{
	return binContent_[bin][0][0];
}

} /* namespace BAT */
