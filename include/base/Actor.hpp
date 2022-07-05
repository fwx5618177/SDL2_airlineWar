#ifndef Actor_hpp
#define Actor_hpp

class Actor {
    public:
        virtual void Update(float deltaTime);
        virtual void Draw();
};

#endif
