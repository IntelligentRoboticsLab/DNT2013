#include "GA.h"

GA::GA() {
}

void GA::update(double fitness) {
    std::cout << "GA: fitness = " << fitness << std::endl;
	GeneticAlgorithms::Individual& i = getIndividual();
    i.fitness = std::max(0.0, fitness);

	GeneticAlgorithms::Individual& j = getIndividual();

	// set the parameters
    std::map<std::string, std::string> args;
    for( std::map<std::string, Vector2<double> >::const_iterator g=genes.begin();
			g!=genes.end(); ++g)
	{
        j.gene[g->first] = Math::clamp(j.gene[g->first], g->second.x, g->second.y);
		if (g->first == "walk.useArm" || g->first== "walk.enableDynamicStabilizer")
			args[g->first] = naoth::DataConversion::toStr(j.gene[g->first]  > 0);
		else
			args[g->first] = naoth::DataConversion::toStr(j.gene[g->first]);
	}
    std::stringstream answer;
	DebugCommandManager::getInstance().handleCommand("ParameterList:IKParameters:set", args, answer);
}

