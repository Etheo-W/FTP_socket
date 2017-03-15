//
//  main.cpp
//  server
//
//  Created by Etheo W on 2017/3/14.
//  Copyright © 2017年 Etheo W. All rights reserved.
//

#include "CommonHeader.h"
#include "SocketManger.h"

int main(int argc, const char * argv[])
{
    SocketManger *socketManger = new SocketManger();
    
    socketManger->init();
    
    while (1)
    {
        socketManger->work();
        //socketManger->hThread = CreateThread(NULL, 0,
        //	(LPTHREAD_START_ROUTINE)(socketManger->work()), NULL, 0, 0 );
        
        //Sleep(1000);
    }
    
    // delete socketManger;
    
    return 0;
}
