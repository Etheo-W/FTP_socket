//
//  main.cpp
//  client
//
//  Created by Etheo W on 2017/3/14.
//  Copyright © 2017年 Etheo W. All rights reserved.
//

#include "CommonHeader.h"
#include "SocketManger.h"

int main(int argc, char * argv[])
{
    cout << " ----------------------------- " << endl;
    cout << "|           用户操作手册        |" << endl;
    cout << "|             0.上传           |" << endl;
    cout << "|             1.下载           |" << endl;
    cout << " ----------------------------- " << endl;
    
    if (argc < 2)
    {
        argv[1] = (char*)malloc(16 * sizeof(char));
        cout << "请输入服务器IP地址:";
        for (int i = 0; i < 16; i++)
        {
            
            argv[1][i] = getchar();
            if (argv[1][i] == '\n') {
                argv[1][i] = '\0';
                break;
            }
        }
        
        printf("IPAddr:%s\n", argv[1]);
    }
    
    //int isEnd = 0;
    int operate = 0;
    
    SocketManger *socketManger = new SocketManger();
    
    if (!socketManger->init(argv[1])) return 0;
    
    //while (!isEnd)
    {
        cout << "请选择操作:";
        cin >> operate;
        
        socketManger->work(argv[1], (REQUEST_STYLE)operate);
        
        //cout << "是否结束(是1，否0):";
        //cin >> isEnd;
    }
    
    socketManger->stop();
    
    delete socketManger;
    
    free(argv[1]);
    
    return 0;
}
