//
//  main.cpp
//  HelloArrow
//
//  Created by zhuang yusong on 2017/3/4.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#include <iostream>
#include "SimpleWireframe.h"

int main(void)
{
	SimpleWireframe app;
	app.initGLApp("Simple Wireframe");
	app.init();
	app.run();
	return 0;
}
