/*
 * EventWeights.h
 *
 *  Created on: 24 Mar 2012
 *      Author: kreczko
 */

#ifndef EVENTWEIGHTS_H_
#define EVENTWEIGHTS_H_
#include "Event.h"
#include "DataTypes.h"
#include "Python/DatasetInformation.h"
#include "GlobalVariables.h"

#include <string>
#include <vector>

#include "TH1D.h"

#include <boost/shared_ptr.hpp>
#include <boost/multi_array.hpp>

typedef boost::shared_ptr<TH1D> TH1DPtr;

#define NVERTEX 50

namespace BAT {

class EventWeights {
public:
	/**
	 * @param datasetInfo Path to Python file containing the number of produced events and cross-sections for all samples.
	 * @param DataPUFile ROOT file containing the histogram with the distribution of number of vertices in data
	 * @param MCPUFile ROOT file containing the histogram with the distribution of number of vertices in MC
	 */
	EventWeights(std::string datasetInfo, std::string DataPUFile, std::string MCPUFile);
	/**
	 * @param datasetInfo Path to Python file containing the number of produced events and cross-sections for all samples.
	 * @param DataPUHistogram ROOT histogram with the distribution of number of vertices in data
	 * @param MCPUHistogram ROOT histogram with the distribution of number of vertices in MC
	 */
	EventWeights(std::string datasetInfo, TH1DPtr DataPUHistogram, TH1DPtr MCPUHistogram);
	/**
	 * @param datasetInfo Path to Python file containing the number of produced events and cross-sections for all samples.
	 * @param DataPUDistribution The distribution of number of vertices in data
	 * @param MCPUDistribution The distribution of number of vertices in MC
	 */
	EventWeights(std::string datasetInfo, std::vector<double> DataPUDistribution, std::vector<double> MCPUDistribution);
	virtual ~EventWeights();

	/**
	 * @brief Return event weight for a given event.
	 *
	 * Returns 1 for real data, scales MC events to the expected number of events for the given luminosity.
	 * By default luminosity is scaled to the global set luminosity.
	 * The weight depends on the number of produced/processed(grid) events of the corresponding MC process and its cross-section:
	 * weight = cross-section * luminosity /number of produced events
	 *
	 */
	double getEventWeight(const EventPtr, double luminosity = Globals::luminosity) const;
	/**
	 * @brief: Scales the MC event to the Pile-Up distribution of data defined in the constructor.
	 *
	 * Scales the MC event to the Pile-Up distribution of data defined in the constructor.
	 * Returns 1 for data.
	 */
	double getPUWeight(const EventPtr) const;
	/**
	 * @brief: Scales the MC event to the Pile-Up distribution of data defined in the constructor.
	 *
	 * Scales the MC event to the Pile-Up distribution of data defined in the constructor for in-time collisions only.
	 * Returns 1 for data.
	 */
	double getPUWeightInTimeOnly(const EventPtr) const;
	/**
	 * @brief: Scales the MC event to the Pile-Up distribution of data defined in the constructor.
	 *
	 * Scales the MC event to the Pile-Up distribution of data defined in the constructor for the average number of
	 * vertices for +-1(out of time) and 0 bunch crossing.
	 *
	 * Returns 1 for data.
	 */
	double getPUWeightAverage3BX(const EventPtr) const;
	/**
	 * @brief: Scales the MC event to the Pile-Up distribution of data defined in the constructor.
	 *
	 * Scales the MC event to the Pile-Up distribution of data defined in the constructor for vertices for +-1(out of time) and 0 bunch crossing
	 * weighted by a poisson distribution.
	 *
	 * Returns 1 for data.
	 */
	double getPUWeight3D(const EventPtr) const;
	/**
	 * @brief: Scales the MC event to the Pile-Up distribution of data defined in the constructor.
	 *
	 * Scales the MC event to the Pile-Up distribution of data defined in the constructor for the true(before reconstruction) number of generated
	 * vertices.
	 *
	 * Returns 1 for data.
	 */
	double getPUWeightTrueInteractions(const EventPtr) const;

	/**
	 * @brief Retrieves the Pile-up histogram from a ROOT file.
	 */
	static const TH1DPtr getPUHistogramFromFile(const std::string file);
	/**
	 * @brief Retrieves the Pile-up distribution from a ROOT histogram.
	 */
	static const std::vector<double> getPUDistributionFromHistogram(const TH1DPtr);

private:
	void generate3DWeights();
	void generateWeights();
private:
	DatasetInformation datasetInfo_;
	boost::array<double, DataType::NUMBER_OF_DATA_TYPES> xsection;
	boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> numberOfProcessedEvents;
	std::vector<double> dataPUDistribution_, mcPUDistribution_;
	boost::array<double, NVERTEX> pileUpWeights_;
	boost::multi_array<double, 3> pileUp3DWeights_;

};

} /* namespace BAT */
#endif /* EVENTWEIGHTS_H_ */
