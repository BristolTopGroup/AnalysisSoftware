/*
 * Histogram.h
 *
 *  Created on: 28 Mar 2012
 *      Author: kreczko
 */

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_
#include <string>
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>
namespace BAT {

class Histogram {
public:
	/**
	 * Contructor for 1D histogram
	 */
	Histogram(std::string name, std::string title, unsigned int nbinsX, double xmin, double xmax);
	/**
	 * Contructor for 2D histogram
	 */
	Histogram(std::string name, std::string title, unsigned int nbinsX, double xmin, double xmax, unsigned int nbinsY,
			double ymin, double ymax);
	/**
	 * Contructor for 3D histogram
	 */
	Histogram(std::string name, std::string title, unsigned int nbinsX, double xmin, double xmax, unsigned int nbinsY,
			double ymin, double ymax, unsigned int nbinsZ, double zmin, double zmax);
	virtual ~Histogram();

	/**
	 * Method to fill a 1D histogram
	 */
	void Fill(double xvalue, double weight = 1.);
	void Fill2D(double xvalue, double yvalue, double weight = 1.);
	void Fill3D(double xvalue, double yvalue, double zvalue, double weight = 1.);

	double GetBinContent(unsigned int bin) const;
	double GetBinContent(unsigned int binX, unsigned int binY) const;
	double GetBinContent(unsigned int binX, unsigned int binY, unsigned int binZ) const;

	std::string GetName() const;
	std::string GetTitle() const;

	unsigned int GetNumberOfXBins() const;
	unsigned int GetNumberOfYBins() const;
	unsigned int GetNumberOfZBins() const;

	double GetMinX() const;
	double GetMinY() const;
	double GetMinZ() const;

	double GetMaxX() const;
	double GetMaxY() const;
	double GetMaxZ() const;

	unsigned long GetEntries() const;
	unsigned int FindBin(double x) const;
	unsigned int GetDimension() const;

private:
	std::string name_, title_;
	unsigned int nbinsX_, nbinsY_, nbinsZ_;
	double xmin_, xmax_;
	double ymin_, ymax_;
	double zmin_, zmax_;
	//for non-equal bins
//	boost::array<double, 2> binBoundaries_;
	// 3 dimensional array
	boost::multi_array<double, 3> binContent_;
	unsigned long numberOfEntries_;
	double underflow_, overflow_;
};

typedef boost::shared_ptr<Histogram> HistogramPtr;

} /* namespace BAT */
#endif /* HISTOGRAM_H_ */
