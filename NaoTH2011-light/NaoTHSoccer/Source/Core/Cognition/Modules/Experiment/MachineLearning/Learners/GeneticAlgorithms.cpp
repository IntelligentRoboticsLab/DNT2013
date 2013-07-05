/**
 * @file GeneticAlgorithms.h
 *
 * @author <a href="mailto:xu@informatik.hu-berlin.de">Xu, Yuan</a>
 */

#include <algorithm>
#include <fstream>
#include "GeneticAlgorithms.h"
#include "Tools/Math/Common.h"
#include "Tools/Debug/NaoTHAssert.h"
#include <glib/gstdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string>
#include <vector>

using namespace std;

double GeneticAlgorithms::transmit(double father, const double mother)
{
  return (Math::random(-1.0,1.0)>0)?father:mother;
}

double GeneticAlgorithms::crossover(double father, double mother)
{
  double k = Math::random(0.0, 1.0);
  return k * father + (1 - k) * mother;
}

double GeneticAlgorithms::mutation(double father, double mother)
{
  return (crossover(father, mother) + Math::random(-1.0f, 1.0f))*Math::random(0.0f, 2.0f);
}

double GeneticAlgorithms::coition(double father, double mother, double transmitRate, double crossoverRate, double mutationRate)
{
  double wayRate = Math::random(0.0, transmitRate + crossoverRate + mutationRate);
  if (wayRate < transmitRate)
  {
    return transmit(father, mother);
  } else if (wayRate < transmitRate + crossoverRate)
  {
    return crossover(father, mother);
  } else
  {
    return mutation(father, mother);
  }
}

GeneticAlgorithms::Individual::Individual(const Individual& father, const Individual& mother, double transmitRate, double crossoverRate, double mutationRate)
:fitness(-1)
{
  ASSERT(father.gene.size() == mother.gene.size() );
  
  for (map<string, double>::const_iterator g=father.gene.begin(); g!=father.gene.end(); ++g)
  {
    gene[g->first] = coition(g->second, mother.gene.find(g->first)->second, transmitRate, crossoverRate, mutationRate);
  }
}

GeneticAlgorithms::GeneticAlgorithms(int parentsNum,
                                     int populationSize,
                                     int surviveNum,
                                     int maxGeneration,
                                     double transmitRate,
                                     double crossoverRate,
                                     double mutationRate,
                                     string savedirName)
:parentsNum(parentsNum),
  populationSize(populationSize),
  surviveNum(surviveNum),
  maxGeneration(maxGeneration),
  transmitRate(transmitRate),
  crossoverRate(crossoverRate),
  mutationRate(mutationRate),
  dirName(savedirName)
{
    // TODO specify folder ourselves?
 /* GDateTime* dateTime = g_date_time_new_now_local();
  dataDir = "ga"+//string( g_date_time_format(dateTime, "%Y-%m-%d-%H-%M-%S") );
  g_date_time_unref(dateTime); */

  dataDir = "ga/"+savedirName;

  lastGenIndex = dirCount();

#ifdef WIN32
  g_mkdir(dataDir.c_str(), 0); // mode arguments are ignored in windows
#else
  g_mkdir(dataDir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

vector<GeneticAlgorithms::Individual> GeneticAlgorithms::newGeneration(const vector<GeneticAlgorithms::Individual>& old)
{
  vector<Individual> children;
  childNr = 0;
  // survive indivisuals
  for( size_t i=0; i<surviveNum&&i<old.size(); i++)
  {
    children.push_back(old[old.size()-1-i]);
    childNr++;
  }

  // generate children
  while (children.size() < populationSize)
  {
    // selected parents
    const Individual& father = selectParent(old);
    const Individual& mother = selectParent(old);
    children.push_back( Individual(father, mother, transmitRate, crossoverRate, mutationRate) );
    childNr++;
  }
  
  return children;
}

const GeneticAlgorithms::Individual& GeneticAlgorithms::selectParent(const std::vector<Individual>& old)
{
  //@note: all the parents have the same possibility to be seleceted
  double maxId = min(old.size(), parentsNum);
  double r = Math::random(0.0, maxId);
  return old[old.size() -1 - (size_t)floor(r)];
}

void GeneticAlgorithms::init(const vector<Individual>& initial)
{
  generations.clear();
  generations.push_back( initial );
}

bool GeneticAlgorithms::converged() const
{
  return generations.size() > maxGeneration;
}

GeneticAlgorithms::Individual& GeneticAlgorithms::getIndividual()
{
  vector<Individual>& lastGeneration = generations.back();
  vector<Individual>::iterator iter = lastGeneration.begin();
  while( iter != lastGeneration.end() && iter->fitness >= 0 )
  {
    iter++;
  }
  if ( iter != lastGeneration.end() )
  {
    return *iter;
  }
  else
  {
    sort(lastGeneration.begin(), lastGeneration.end());
    stringstream filename;

    filename << dataDir << "/" << generations.size() <<".txt";
    saveGeneration( lastGeneration, filename.str() );
    generations.push_back( newGeneration(lastGeneration) );
    return generations.back().front();
  }
}

void GeneticAlgorithms::saveGeneration(const std::vector<Individual>& gen, const std::string& file)
{
  ofstream os(file.c_str());
  if ( os )
  {
    for ( std::vector<Individual>::const_iterator iter=gen.begin(); iter!=gen.end(); ++iter )
    {
      os << *iter << endl;
    }
  }
}

void GeneticAlgorithms::loadGeneration() {
  std::cout << "=== LOADING GENERATION ===" << std::endl;

  this->generations.clear();
  std::vector<Individual>* lastGen = new std::vector<Individual>();

  vector<string> files = vector<string>();
  getdir(dataDir,files);
  int x;
  if (files.size()==2) x = 1;
  else x = files.size() - 2;

  string line;
  stringstream sstm;
  sstm << dataDir << "/" << x << ".txt";
  cout << "SaveDIR: " << sstm.str() << endl;
  ifstream myfile(sstm.str().c_str());
  int i = -1;
  string extractValue= "";
  string extractName="";
  Individual* ind = new Individual();
  if (myfile.is_open())
  {
    while ( myfile.good() )
    {
      getline (myfile,line);

      if(strcmp(line.c_str(),"") == 0) continue;

      size_t position = line.find("=");
      if (string::npos == position) {
        position = line.find("-");
        if (string::npos != position){
          i++;
          if(i!=0) lastGen->push_back(*ind);
          ind = new Individual();

          extractValue = (string::npos == position)? line : line.substr(0, position);

          ind->fitness = atof(extractValue.c_str());

          std::cout<< "Loaded individual with fitness " << ind->fitness << "." << std::endl;
          continue;
        }
      }

      extractName = (string::npos == position)? line : line.substr(0, position - 1);
      extractValue = (string::npos == position)? line : line.substr(position + 2, line.size());
      position = extractValue.find(";");
      extractValue = (string::npos == position)? extractValue : extractValue.substr(0, position);
      ind->gene[extractName]= atof(extractValue.c_str());
      i++;
    }
    lastGen->push_back(*ind);
    myfile.close();
  }
  init(*lastGen);
  std::cout << "=== DONE ===" << std::endl;
}

std::ostream& operator <<(std::ostream& ost, const GeneticAlgorithms::Individual& v)
{
  ost<<v.fitness<<"------------------------------------\n";
  for ( map<string, double>::const_iterator iter=v.gene.begin(); iter!=v.gene.end(); ++iter)
  {
    ost<<iter->first<<" = "<<iter->second<<";\n";
  }
  ost<<endl;
  return ost;
}

int GeneticAlgorithms::getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << "." << endl;
        return -1;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int GeneticAlgorithms::dirCount(){
    vector<string> files = vector<string>();
    if ((getdir(dataDir,files)) < 0) return 0;
    if (files.size() == 2) return 1;
    return files.size() - 2 ;
}
