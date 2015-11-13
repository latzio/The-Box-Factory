#ifndef __AI_H_
#define __AI_H_

#include "Algebra.h"
#include "Macro.h"

#include <vector>

class AISubscriber;

// AI Class
//
// The philosophy here is that we will create an AI for each mob of
// enemies that we release. Once they are all dead, the AI will destroy itself
class AI {

  public:
    AI();
    virtual ~AI();

    NoMoveOrCopy(AI)

    // This method causes our AI to calculate the motions
    // for all of its subscribers and then instruct them
    // upon what action they should take. This basic AI
    // just moves the leader toward the player.
    virtual void tick();

    // Directions used in logic
    enum Direction{ DOWN, RIGHT, UP, LEFT };
    // Add an enemy to our processing list, the first one added
    // is our leader, naturally
    void addSubscriber(AISubscriber * subscriber);

      void remove(AISubscriber * subscriber);

    // Add an enemy to our processing list,
    void addTarget(AISubscriber * target) {
      m_pTarget = target;
    }

    // As long as we still have somebody
    bool isActive() {
      return (m_pSubscribers.size() > 0);
    }

     void setTicks(int tickTotal) {
         m_nProcessCount = tickTotal;
     }

    virtual bool isAuto() { return false; }

  protected:

    AISubscriber * m_pTarget; // This is the object this AI pursues
    typedef std::vector< AISubscriber* > SubscriberList;
    SubscriberList m_pSubscribers; // These are the mob


     int m_nProcessCount;
     int m_nProcessTicks;


};

class AutoAI : public AI {

  public:

    AutoAI(Direction d);
    virtual ~AutoAI();

    virtual void tick();
    virtual bool isAuto() { return true; }

  protected:

     Direction m_eDirection;
};

class AISubscriber {

  public:
    AISubscriber() : m_pAI(0) { }
    virtual ~AISubscriber() { }
    NoMoveOrCopy(AISubscriber)

    // Where is this unit
    virtual void getCentre(Point3D& p) = 0;

    // Move this unit, true if success, false if impeded
    virtual bool move(AI::Direction ePrimary, AI::Direction eSecondary) = 0;

    // Is this unit alive?
    virtual bool isActive() = 0;

    // Revert to general purpose ai
    virtual void revertToAI() = 0;

      void setAI(AI* ai) { m_pAI = ai; }
    AI* getAI() { return m_pAI; }

  protected:
       AI* m_pAI;

};


#endif
