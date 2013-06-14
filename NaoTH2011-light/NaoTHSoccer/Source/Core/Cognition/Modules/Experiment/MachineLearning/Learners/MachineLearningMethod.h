#ifndef MACHINELEARNINGMETHOD_H
#define MACHINELEARNINGMETHOD_H

class MachineLearningMethod
{
public:
    virtual void update(double fitness) = 0;
    virtual bool isFinished() const = 0;
};

#endif // MACHINELEARNINGMETHOD_H
