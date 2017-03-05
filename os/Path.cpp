//
//  Path.cpp
//  TouchCone
//
//  Created by zhuang yusong on 2017/3/5.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//
//#import <Foundation/Foundation.h>
#include <stdlib.h>
#include <mach-o/dyld.h>
#include <string.h>
#include "Path.h"

string Path::s_runDir;
string Path::s_resourceDir;

string Path::runDir(){
    if(Path::s_runDir.empty()){
        char buf[0];
        uint32_t size = 0;
        int res = _NSGetExecutablePath(buf,&size);
        
        char* path = (char*)malloc(size+1);
        path[size] = 0;
        res = _NSGetExecutablePath(path,&size);
        
        char* p = strrchr(path, '/');
        *p = 0;
         Path::s_runDir.append(path);
        free(path);  
       
    }
        
    return Path::s_runDir;
}

string Path::resouceDir(){
    if(Path::s_resourceDir.empty()){
        string runDir = Path::runDir();
        size_t index = runDir.find_last_of("/");
        Path::s_resourceDir = runDir.substr(0, index);
        Path::s_resourceDir.append("/Resources");
    }
    
    return Path::s_resourceDir;
}

string Path::joinResource(const string& subPath){
    string path = Path::resouceDir();
    path.append("/");
    path.append(subPath);
    return path;
}
