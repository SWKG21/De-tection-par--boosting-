#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <string>

class Rectangle{

    public:
        Rectangle(int startX, int startY, int endX, int endY):
            startX_(startX), startY_(startY), endX_(endX), endY_(endY) {};
        ~Rectangle() {};

        int getStartX() const {return startX_;}
        int getStartY() const {return startY_;}
        int getEndX() const {return endX_;}
        int getEndY() const {return endY_;}
        int getWidth() const {return endX_-startX_;}
        int getHeight() const {return endY_-startY_;}

    private:
        int startX_;
        int startY_;
        int endX_;
        int endY_;

};

#endif // END_RECTANGLE_H_
