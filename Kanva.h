#include <iostream>
#include "termios_control.h"
#include <cmath>

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

    Color fill{255,255,255};
    Color stroke{255,255,255};
    


    //drawing functions
    void drawPixel(int x, int y);
    void fillRect(int x,int y, int w, int h);
    void strokeRect(int x, int y, int w, int h);
    void drawLine(int x, int y, int x2, int y2);
    
    
    void reloadSize();




};

Kanva Kanva::instance{};
Kanva::Kanva(){
    if(&Kanva::instance!=this)throw;
    std::cout<<char(0x1b)<<"[1;1H"<<char(0x1b)<<"[2J"<<char(0x1b)<<"[3J";
    parkCursor();
}
Kanva::~Kanva(){
    std::cout<<char(0x1b)<<"[0m";
}
void Kanva::setPos(int x, int y){
    std::cout<<char(0x1b)<<"["<<y<<";"<<x<<"H";
}
void Kanva::setColor(Color& c){
    std::cout<<char(0x1b)<<"[48;2;"<<int(c.r)<<";"<<int(c.g)<<";"<<int(c.b)<<"m";
}
void Kanva::parkCursor(){
    std::cout<<char(0x1b)<<"["<<ySize+1<<";1H";
}
bool Kanva::inBounds(int x, int y){
    return x>=1 && x<=xSize && y>=1 && y<=ySize;
}

void Kanva::reloadSize(){
    BufferToggle::off();
    setPos(2000,2000);
    std::cout<<char(0x1b)<<"[1A"<<char(0x1b)<<"[6n"<<"\n";
    std::string response="";
    while (std::cin.peek()!='R')
    {
        char x;
        std::cin>>x;
        response+=x;
    }
    std::cin.get();
    BufferToggle::on();
    size_t st=response.find('[');
    size_t mid=response.find(';');
    ySize=atoi(response.substr(st+1,mid-st-1).c_str());
    xSize=atoi(response.substr(mid+1).c_str());
    std::cout<<char(0x1b)<<"[0m"<<char(0x1b)<<"[2K";
    parkCursor();
    return;
}

void Kanva::drawPixel(int x,int y){
    //reloadSize();
    if(x<1 || x>xSize || y<1 || y>ySize)return;

    //set stroke color
    setColor(stroke);

    //draw pixel
    setPos(x,y);
    std::cout<<" ";
    //reset cursor
    parkCursor();
}

void Kanva::drawLine(int x, int y, int x2, int y2){
    //reloadSize();

    setColor(stroke);

    float xf=x;
    float yf=y;
    int dx=x2-x;
    int dy=y2-y;
    float d=sqrtf(dx*dx+dy*dy);
    float xStep=0.5*dx/d;
    float yStep=0.5*dy/d;
    
    while(abs(dx)>abs(xf-x) || abs(dy)>abs(yf-y)){
        xf+=xStep;
        yf+=yStep;
        int roundx=round(xf);
        int roundy=round(yf);
        if(inBounds(roundx,roundy)){
            setPos(roundx,roundy);
            std::cout<<" ";
        }
    }

    parkCursor();
}

void Kanva::strokeRect(int x, int y, int w, int h){
    //reloadSize();

    setColor(stroke);

    int xsign=w>0?1:-1;
    int ysign=h>0?1:-1;
    //top line
    for(int i=w;i!=-xsign;i-=xsign){
        int X=x+i;
        int Y=y;
        if(inBounds(X,Y)){
            setPos(X,Y);
            std::cout<<" ";
        }
    }
    //bottom line
    for(int i=w;i!=-xsign;i-=xsign){
        int X=x+i;
        int Y=y+h;
        if(inBounds(X,Y)){
            setPos(X,Y);
            std::cout<<" ";
        }
    }
    //left line
    for(int i=h-1;i!=0;i-=ysign){
        int X=x;
        int Y=y+i;
        if(inBounds(X,Y)){
            setPos(X,Y);
            std::cout<<" ";
        }
    }
    //right line
    for(int i=h-1;i!=0;i-=ysign){
        int X=x+w;
        int Y=y+i;
        if(inBounds(X,Y)){
            setPos(X,Y);
            std::cout<<" ";
        }
    }
    parkCursor();
}

void Kanva::fillRect(int x, int y, int w, int h){
    //reloadSize();

    setColor(fill);

    int xsign=w>0?1:-1;
    int ysign=h>0?1:-1;
    for(int xp=x;xp!=x+w+xsign;xp+=xsign){
        for(int yp=y;yp!=y+h+ysign;yp+=ysign){
            if(inBounds(xp,yp)){
                setPos(xp,yp);
                std::cout<<" ";
            }
        }
    }
    parkCursor();
}