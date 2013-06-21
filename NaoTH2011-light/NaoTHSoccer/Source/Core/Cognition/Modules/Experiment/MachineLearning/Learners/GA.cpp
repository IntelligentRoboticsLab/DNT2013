#include "GA.h"

GA::GA(MachineLearningParameters::Evolution params,
       std::map<std::string, double > initialGene,
       std::map<std::string, Vector2<double> > genes ) :
    GeneticAlgorithms(params.parentsNum,
                      params.populationSize,
                      params.surviveNum,
                      params.maxGeneration,
                      params.transmitRate,
                      params.crossoverRate,
                      params.mutationRate,
                      params.savedirname),
    genes(genes)
{
    std::vector<Individual> initialGeneration;

    Individual i;
    i.gene =  initialGene ;

    initialGeneration.push_back(i);
    initialGeneration.push_back(i); // to make sure inital value is tested correctly (?! ok)
    init(initialGeneration);

    this->name = "Evolution";
}

void GA::update(double fitness) {
    std::cout << "Fitness = " << fitness << std::endl;

    GeneticAlgorithms::Individual& i = getIndividual();
    i.fitness = std::max(0.0, fitness);

    if (fittest < i) {
        fittest = i;
    }

    GeneticAlgorithms::Individual& j = getIndividual();

    // set the parameters
    std::map<std::string, std::string> args;
    for( std::map<std::string, Vector2<double> >::const_iterator g=genes.begin();
                        g!=genes.end(); ++g) {
        j.gene[g->first] = Math::clamp(j.gene[g->first], g->second.x, g->second.y);
          if (g->first == "walk.useArm" || g->first== "walk.enableDynamicStabilizer")
            args[g->first] = naoth::DataConversion::toStr(j.gene[g->first]  > 0);
          else
            args[g->first] = naoth::DataConversion::toStr(j.gene[g->first]);
    }
    std::stringstream answer;
    DebugCommandManager::getInstance().handleCommand("ParameterList:IKParameters:set", args, answer);
}

std::string GA::getInfo()
{
   std::stringstream outputstream;
   outputstream << "Current generation-nr: " << generations.size() << "." << std::endl;
   outputstream << "Current individual-nr: " << childNr << "." << std::endl;

   outputstream << "Highest fitness: " << fittest.fitness << "." << std::endl;
   outputstream << "Fittest per generation:" << std::endl;

   //for (int generationNum=0; generationNum != fitnesses.size(); generationNum++)
   //{
   //  std::vector<Individual> generation = generations.at(generationNum);
   //  outputstream << "Generation " << generationNum << ":" << std::max_element(generation)->fitness << std::endl;
   //}

   return outputstream.str();
}
