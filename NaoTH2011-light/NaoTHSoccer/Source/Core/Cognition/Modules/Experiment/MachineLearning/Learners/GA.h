#ifndef _GA_H
#define _GA_H

class GA : MachineLearningMethod
{
public:
    GA();
    virtual ~GA();

    virtual void update(double fitness);
};

#endif // _GA_H