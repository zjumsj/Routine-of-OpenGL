
#include "GNU.h"

POINT Mo::mousepoint;


void Mo::Mode(){
	
	
	if (!State){//调试状态转到游戏状态
		
		State=1;
	}
	else{//游戏状态切换到调试状态
		
		State=0;
	}
	

}



void object::appear() {

}



