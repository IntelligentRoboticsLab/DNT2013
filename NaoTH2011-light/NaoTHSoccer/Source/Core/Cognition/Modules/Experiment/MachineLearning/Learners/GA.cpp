#include "GA.h"


using namespace std;

GA::GA() {
}

bool GA::Test::isFinished() const
{
	return theMaxTime <= 0;
}

double GA::Test::getDistance() const
{
	return (theStopPos-theStartPos).abs();
}

GA::update(double fitness) {
	cout << "GAWalk: fitness = " << fitness << endl;
	GeneticAlgorithms::Individual& i = getIndividual();
	i.fitness = max(0.0, fitness);

	GeneticAlgorithms::Individual& j = getIndividual();

	// set the parameters
	map<string, string> args;
	for( std::map<std::string, Vector2<double> >::const_iterator g=genes.begin();
			g!=genes.end(); ++g)
	{
		j.gene[g->first] = Math::clamp(j.gene[g->first], g->second.x, g->second.y);
		if (g->first == "walk.useArm" || g->first== "walk.enableDynamicStabilizer")
			args[g->first] = naoth::DataConversion::toStr(j.gene[g->first]  > 0);
		else
			args[g->first] = naoth::DataConversion::toStr(j.gene[g->first]);
	}
	stringstream answer;
	DebugCommandManager::getInstance().handleCommand("ParameterList:IKParameters:set", args, answer);
}

