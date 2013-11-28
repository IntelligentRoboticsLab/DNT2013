/**
 * @author <a href="mailto:mellmann@informatik.hu-berlin.de">Mellmann, Heinrich</a>
 */

#ifndef _ActionList_H
#define _ActionList_H

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <cassert>
#include <typeinfo>

// This file has been touched by the documentation effort
namespace naoth
{
/**
* @brief AbstractAction defines an interface for an action.
*
* It is used to wrap the call of a get/set method to read/write a representation.
* 
*/
class AbstractAction
{
public:
  virtual ~AbstractAction(){} //ACHTUNG: needed, for the desructor of the derived classes to be called
  virtual void execute() = 0;
};

/**
* @brief Represents a list of \c AbstractAction.
*/
class ActionList
{
protected:
/**
* @brief List of pointers to \c AbstractAction.
*/
  typedef std::list<AbstractAction*> ActionListT;

  ActionListT actions;

public:

/**
* @brief Destructor 
*
* Deletes all \c AbstractAction contained in the list.
*/
  virtual ~ActionList()
  {
    std::cout << "destruct ActionList" << std::endl;
    // delete all actions
    for(ActionListT::iterator iter = actions.begin(); iter != actions.end(); ++iter)
    {
      delete (*iter);
    } 
  }

/**
* @brief Execute all \c AbstractAction in the list.
*/
  void execute() const
  {
    for(ActionListT::const_iterator iter = actions.begin(); iter != actions.end(); ++iter)
    {
      (*iter)->execute();
    }
  }

/**
* @brief Adds an \c AbstractAction to the list.
*
* @param action A non null \c AbstractAction pointer.
*/
  void push_back(AbstractAction* action)
  {
    assert(action != NULL);
    actions.push_back(action);
  }
};


/**
* @brief Hack class for void pointers.
*
* ToDO: Check dependencies and if possible remove class.
*/
class Void
{
public:
  virtual ~Void(){}
};

/**
* @brief TypedActionCreator
* 
* Abstract interface for a class which creates an \c AbstractAction
* based on given data and its type.
*/
template<class T>
class TypedActionCreator : public Void
{
public:
  virtual ~TypedActionCreator(){} 

/**
* @brief Allocates a new \c AbstractAction. 
*
* @param data Data used to create the \c AbstractAction.
*
* @return A pointer to the newly created \c AbstractAction.
*/
  virtual AbstractAction* createAction(T& data) = 0;
};


/**
* @brief Implements a creator for \c AbstractAction. 
*
* An action is created using a TypedActionCreatorMap 
* if avaliable. All avaliable creators are stored in a map.
*/

class TypedActionCreatorMap
{
private:
/**
* @brief A map for \c TypedActionCreator.
*
* The \c Void class is used to bypass type limitations for 
* the templated \c TypedActionCreator.
* The key of the map is a string containing the name of the
* type of the corresponding \c TypedActionCreator.
*/
  typedef std::map<std::string, Void*> TypedActionCreatorMapT;
 
  TypedActionCreatorMapT registeredActions;

/**
* @brief Returns a pointer to a stored \c TypedActionCreator of a particular type in the map. 
*
* Based on the template type of the function it searches the map using as 
* key a string containing the name of the type.
*
* @return Null if no corresponding \c TypedActionCreator could be found. 
*	  A pointer to the appropriate stored \c TypedActionCreator. 
*/
  template<class T>
  TypedActionCreator<T>* getActionCreator() const
  {
    TypedActionCreatorMapT::const_iterator iter = registeredActions.find(typeid(T).name());
    if(iter == registeredActions.end())
    {
      return NULL;
    } 

    return static_cast<TypedActionCreator<T>*>(iter->second);
  } 

public:

/**
* @brief Deletes all stored \c TypedActionCreator.
*/
  virtual ~TypedActionCreatorMap()
  {
    std::cout << "destruct TypedActionCreatorMap " << std::endl;

    for(TypedActionCreatorMapT::iterator iter = registeredActions.begin(); iter != registeredActions.end(); ++iter)
    {
      delete iter->second;
    }
  }


/**
* @brief Creates an \c AbstractAction for the type T if an appropriate \c TypedActionCreator is available.
*/
  template<class T>
  AbstractAction* createAction(T& data) const
  {
    TypedActionCreator<T>* creator = getActionCreator<T>();
      
    if(creator == NULL)
      return NULL;

    return creator->createAction(data);
  } 

  /**
  * adds a new creator or overwrites an existing one
  */
/**
* @brief Adds a new \c TypedActionCreator or overwrites an existing one.
*
* @tparam T Type of the data used to create an \c AbstractAction.
* @param actionCreator A pointer to the \c TypedActionCreator.
*/
  template<class T>
  void add(TypedActionCreator<T>* actionCreator)
  {
    registeredActions[typeid(T).name()] = actionCreator;
  }
};


}// end namespace naoth

#endif // _ActionList_H
