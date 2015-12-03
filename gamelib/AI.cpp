#include "AI.h"
#include <algorithm>

#define MAX_MOB_SIZE 16

AI::AI()
    : m_pTarget(0)
    , m_pSubscribers()
    , m_nProcessCount(15)
    , m_nProcessTicks(-1)
{
    m_pSubscribers.reserve(MAX_MOB_SIZE);
}

AI::~AI()
{
}

void AI::tick()
{

    if (m_nProcessTicks-- > 0) {
        return;
    }

    m_nProcessTicks = m_nProcessCount;

    // If we're all dead or have no target
    if (m_pSubscribers.size() == 0 || !m_pTarget) {
        return;
    }

    // Select our leader, first man on scene (who's still alive!)
    AISubscriber* pLeader = m_pSubscribers[0];

    // Get source and destination
    glm::vec3 s, d;
    pLeader->getCentre(s);
    m_pTarget->getCentre(d);

    float xDiff = d[0] - s[0];
    float zDiff = d[2] - s[2];

    // Directions to move
    Direction ePrimary, eSecondary;
    (xDiff > 0) ? ePrimary = RIGHT : ePrimary = LEFT;
    (zDiff > 0) ? eSecondary = DOWN : eSecondary = UP;

    // Further in z, then swap
    if (abs(xDiff) < abs(zDiff)) {
        Direction temp = ePrimary;
        ePrimary = eSecondary;
        eSecondary = temp;
    }

    // Issue primary and secondary commands
    for (SubscriberList::iterator it = m_pSubscribers.begin();
            it != m_pSubscribers.end(); it++) {
        if ((*it)->isActive()) {
            (*it)->move(ePrimary , eSecondary);
        }
    }

}

void AI::remove(AISubscriber* subscriber)
{
    for (SubscriberList::iterator it = m_pSubscribers.begin();
            it != m_pSubscribers.end(); it++) {
        if ((*it) == subscriber) {
            m_pSubscribers.erase(it);
            return;
        }
    }
}

void AI::addSubscriber(AISubscriber* subscriber)
{
    AI* pFormerAI = subscriber->getAI();
    if (pFormerAI) {
        pFormerAI->remove(subscriber);
    }

    m_pSubscribers.push_back(subscriber);
    subscriber->setAI(this);
}

AutoAI::AutoAI(Direction d)
    : AI()
    , m_eDirection(d)
{
    m_nProcessCount = 15;
}

AutoAI::~AutoAI()
{
}

void AutoAI::tick()
{

    // Dont act every time
    if (m_nProcessTicks-- > 0) {
        return;
    }

    m_nProcessTicks = m_nProcessCount;

    // Issue primary and secondary commands
    for (SubscriberList::iterator it = m_pSubscribers.begin();
            it != m_pSubscribers.end(); it++) {
        if ((*it)->isActive()) {
            (*it)->move(m_eDirection , m_eDirection);

        }
    }

    // Check to see if we should turn this over to a general AI.

}
