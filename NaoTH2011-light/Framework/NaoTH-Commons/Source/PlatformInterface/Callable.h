/* 
 * File:   Callable.h
 * Author: thomas
 *
 * Created on 10. August 2010, 17:07
 */

#ifndef CALLABLE_H
#define CALLABLE_H

// This file has been touched by the documentation effort
namespace naoth
{
  class PlatformBase;
  class ProcessInterface;

/**
* @brief Class included from Motion.h
*
* Class is an interface. ToDo: document where it is used and how.
*
*/
  class Callable
  {
  public:

    virtual void init(ProcessInterface& platformInterface, const PlatformBase& platform) = 0;

    virtual void call() = 0;

  };
}

#endif  /* CALLABLE_H */

