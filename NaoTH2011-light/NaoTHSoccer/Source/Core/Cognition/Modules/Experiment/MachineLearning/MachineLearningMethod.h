#ifndef MACHINELEARNINGMETHOD_H
#define MACHINELEARNINGMETHOD_H

class MachineLearningMethod
{
public:
    MachineLearningMethod();
    virtual ~MachineLearningMethod();

    virtual void update(double fitness);
};

#endif // MACHINELEARNINGMETHOD_H
