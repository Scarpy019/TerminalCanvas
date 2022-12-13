#include "./Kanva.h"

int main(){
    Kanva& kan=Kanva::instance;
    kan.reloadSize();
    for(int x=0;x<255;x++){
        for(int y=0;y<255;y++){
            kan.stroke.set(x,y,255-(x+y)/2);
            kan.drawPixel(x+1,y+1);
        }
    }
    
    
}