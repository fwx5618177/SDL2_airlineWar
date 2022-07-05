#ifndef PcMan_hpp
#define PcMan_hpp

#include "Actor.hpp"

class PcMan : public Actor {
    public:
        void Update(float deltaTime) override;
        void Draw() override;
};

#endif