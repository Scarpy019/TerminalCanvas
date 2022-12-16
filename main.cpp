#include "./Kanva.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

int main(){
    /*Kanva& kan=Kanva::instance;
    kan.init();
    for(int x=1;x<512;x++){
        for(int y=1;y<512;y++){
            if(x%3!=0 && y%3!=0)continue;
            float at=atan2(256-y,256-x);
            kan.stroke.set(
                255*(sin(at)+1)/2,
                255*(sin(at+M_PI*2/3)+1)/2,
                255*(sin(at+M_PI*4/3)+1)/2);
            kan.drawPixel(x/3,y/3);
        }
    }*/
    std::ios_base::sync_with_stdio(false);
    Kanva& kan=Kanva::instance;
    kan.init();
    kan.drawImage("./1.ppm",1,1);
    std::cin.get();
    kan.clear();
    
    //kan.strokeArc(50,50,25,0,M_PI*2);
    //kan.strokeArc(100,50,15,45*M_PI/180,M_PI);
    //kan.fillArc(150,100,25,0,270*M_PI/180);

    //kan.drawImage("./troll.ppm",1,1);
    /*
    long long tc=0;
    long long tt=0;
    for(int x=-220;x<kan.getXSize();x++){
        auto t=std::chrono::high_resolution_clock::now();
        auto t2=t+std::chrono::milliseconds(1000/60);
        kan.drawImage("./troll.ppm",x,-4);
        std::cout<<std::flush;
        std::this_thread::sleep_until(t2);
        tt+=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-t).count();
        tc++;
    }
    std::cout<<1000/(tt/tc);
    */
    
}