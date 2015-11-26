#ifndef BoxFactory_h
#define BoxFactory_h

#include <string>

namespace Box {
namespace Game {

class BoxFactory {
public:
    BoxFactory();
    ~BoxFactory();

    std::string getTitle();

private:
};

} // Game
} // BoxFactory

#endif
