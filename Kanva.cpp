#include "./Kanva.h"
#include <iostream>
#include <string>

Kanva Kanva::instance{};
Kanva::Kanva(){
    if(&Kanva::instance!=this)throw;
    
}
void Kanva::init(){
    std::cout<<char(0x1b)<<"[1;1H"<<char(0x1b)<<"[2J"<<char(0x1b)<<"[3J";
    reloadSize();
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
int Kanva::getXSize(){
    return xSize;
}
int Kanva::getYSize(){
    return ySize;
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

void Kanva::drawImage(std::string filename, int x, int y){
    std::ifstream f1 (filename,std::fstream::binary);
    std::string type;
    std::string outputString="";
    if(f1.peek()!=EOF){
        getline(f1, type);
        if(type!="P6")std::cout<<"unable to parse file.";
    }
    int imgXsize, imgYsize, scale;
    if(f1.peek()!=EOF){
        f1>>imgXsize>>imgYsize>>scale;
    }
    f1.get();//parse the newline
    outputString=outputString+(char)(0x1b)+"["+std::to_string(std::max(y,1))+";"+std::to_string(std::max(x,1))+"H";
    unsigned char pr=0,pg=0,pb=0;
    unsigned char r,g,b;
    outputString=outputString+char(0x1b)+"[48;2;0;0;0m";
    for(int yv=0;yv<imgYsize;yv++){
        bool xOutOfBounds=false;
        for(int xv=0;xv<imgXsize;xv++){
            r=f1.get();
            g=f1.get();
            b=f1.get();
            if(xv+x>(long)xSize)xOutOfBounds=true;
            if(xOutOfBounds || !inBounds(xv+x,yv+y))continue;
            if(!(r==pr && g==pg && b==pb)){
                outputString=outputString+char(0x1b)+"[48;2;"+
                    std::to_string((int)r)+";"+
                    std::to_string((int)g)+";"+
                    std::to_string((int)b)+"m";
                pr=r;
                pg=g;
                pb=b;
            }
            outputString+=" ";
        }
        outputString=outputString+char(0x1b)+"["+std::to_string(std::max(y+yv,1))+";"+std::to_string(std::max(x,1))+"H";
    }
    f1.close();
    outputString=outputString+char(0x1b)+"[0m";
    std::cout<<outputString;
    parkCursor();
}

void Kanva::strokeArc(int x, int y, int r, float startAngle, float endAngle){
    setColor(stroke);
    float step=1;//step in degrees
    for(float a=startAngle;a<endAngle;a+=step*M_PI/180){
        int xp=round(x+r*cos(a));
        int yp=round(y+r*sin(a));
        if(inBounds(xp,yp)){
            setPos(xp,yp);
            std::cout<<" ";
        }
    }
    std::cout<<char(0x1b)<<"[0m";
    parkCursor();
}
void Kanva::fillArc(int x, int y, int r, float startAngle, float endAngle){
    setColor(fill);
    float step=1;//step in degrees
    float rstep=0.4;//radius step
    for(float radius=r;radius>=0;radius-=rstep){
        for(float a=startAngle;a<endAngle;a+=step*M_PI/180){
            int xp=round(x+radius*cos(a));
            int yp=round(y+radius*sin(a));
            if(inBounds(xp,yp)){
                setPos(xp,yp);
                std::cout<<" ";
            }
        }
    }
    
    std::cout<<char(0x1b)<<"[0m";
    parkCursor();
}

void Kanva::clear(){
    std::cout<<char(0x1b)<<"[0m"<<char(0x1b)<<"[1;1H"<<char(0x1b)<<"[2J"<<char(0x1b)<<"[3J";
    parkCursor();
}