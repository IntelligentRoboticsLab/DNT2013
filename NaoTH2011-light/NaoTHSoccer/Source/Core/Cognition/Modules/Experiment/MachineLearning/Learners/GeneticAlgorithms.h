/**
 * @file GeneticAlgorithms.h
 *
 * @author <a href="mailto:xu@informatik.hu-berlin.de">Xu, Yuan</a>
 */

#ifndef _GENETICALGORITHMS_H
#define  _GENETICALGORITHMS_H

#include <map>
#include <string>
#include <math.h>
#include <iostream>
#include <vector>
#include "Tools/Math/Common.h"
#include "Tools/Math/Vector2.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>



class GeneticAlgorithms
{
public:
  ////////////reproduction functions///////////////////
  // three different ways:1.transmit 2.crossover 3.mutation
  static double transmit(double father, double mother);
  static double crossover(double father, double mother);
  static double mutation(double father, double mother);
  static double coition(double father, double mother, double transmitRate, double crossoverRate, double mutationRate);

  class Individual
  {
  public:

    Individual() : fitness(-1)
    {
    }

    Individual(const Individual& father, const Individual& mother, double transmitRate, double crossoverRate, double mutationRate);

    bool operator<(const Individual& other) const
    {
      return fitness < other.fitness;
    }

    double fitness;
    std::map<std::string, double> gene;
  };


  int getdir (std::string dir, std::vector<std::string> &files);

  GeneticAlgorithms(int parentsNum,
                    int populationSize,
                    int surviveNum,
                    int maxGeneration,
                    double transmitRate,
                    double crossoverRate,
                    double mutationRate);

  virtual ~GeneticAlgorithms(){}

  void loadGeneration();

  void init(const std::vector<Individual>& initial);

  Individual& getIndividual();

  bool converged() const;
  int childNr;

private:
  std::vector<Individual> newGeneration(const std::vector<Individual>& old);

  const Individual& selectParent(const std::vector<Individual>& old);

  void saveGeneration(const std::vector<Individual>& gen, const std::string& file);

protected:
  std::string dataDir;

  size_t parentsNum;
  size_t populationSize;
  size_t surviveNum;
  size_t maxGeneration;
  double transmitRate, crossoverRate, mutationRate;

  std::vector<std::vector<Individual> > generations;
};

std::ostream& operator <<(std::ostream& ost, const GeneticAlgorithms::Individual& v);

#endif  /* _GENETICALGORITHMS_H */

