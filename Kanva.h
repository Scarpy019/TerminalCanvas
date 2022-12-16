
#pragma once
#include <iostream>
#include "termios_control.h"
#include <cmath>
#include <fstream>

class Kanva{
private:
    unsigned int xSize;
    unsigned int ySize;
    Kanva();
    ~Kanva();
    void parkCursor();
    struct Color{
        unsigned char r;
        unsigned char g;
        unsigned char b;
        void set(unsigned char r, unsigned char g, unsigned char b){
            this->r=r;
            this->b=b;
            this->g=g;
        }
    };
    void setColor(Color&);
    void setPos(int x, int y);
    bool inBounds(int x, int y);
public:
    static Kanva instance;
    //delete all constructors and assignment operators to maintain singleton
    Kanva(const Kanva&) = delete;
    Kanva(Kanva&&) = delete;
    Kanva& operator=(const Kanva&) = delete;
    Kanva& operator=(Kanva&&) = delete;
    void init();
    Color fill{255,255,255};
    Color stroke{255,255,255};
    
    int getXSize();
    int getYSize();

    //drawing functions
    void drawPixel(int x, int y);
    void fillRect(int x,int y, int w, int h);
    void strokeRect(int x, int y, int w, int h);
    void drawLine(int x, int y, int x2, int y2);
    void drawImage(std::string filename, int x, int y);
    void strokeArc(int x, int y, int r, float startingAngle, float endAngle);
    void fillArc(int x, int y, int r, float startingAngle, float endAngle);

    void clear();

    void reloadSize();




};

