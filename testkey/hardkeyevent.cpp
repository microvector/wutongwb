#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

using namespace std;

int main()
{        
    struct input_event keyEvent;
    int key_fd = 0;
    
    key_fd = open("/dev/input/event0", O_RDONLY);  //"/dev/input/event3"
    if (key_fd <=0 )
    {
         cout<<"open error "<<endl;
    }
    while (1)
    {
	if (read(key_fd, &keyEvent, sizeof(keyEvent)))
        {
	    if (keyEvent.type == EV_KEY)  // 时间类型为按键(物理按键和屏幕点击按键)
 	    {
	        if (keyEvent.code == KEY_VOLUMEDOWN)  // 暂时使用音量按键
		{
		    // start do something...
		    cout<<"dww--dedededed"<<endl;		
		}
	    }
	}
    }
    return 0;
}
