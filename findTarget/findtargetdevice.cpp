#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <poll.h>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define SYS_PATH_PRE "/sys/class/input/event"
#define SYS_PATH_TAIL "/device/name"
#define DEV_PATH_PRE "/dev/input/event"
#define SCREEN_DEVICE_NAME "Weida Hi-Tech CoolTouch® System"
#define KEY_DEVICE_NAME "qpnp_pon"

using namespace std;

std::stringstream inputFileName;

string::size_type idx;
string contentStr;
bool hasGetKey = false, hasGetScreen = false;
int index = 0;
int screenId = 0, keyId = 0;

struct input_event *event;
struct pollfd *pfds;
int key_fd = 0;
int abs_fd = 0;
int fds[] = {0, 0};
int pollres = 0;

void findTarget()
{
	while (!(hasGetKey & hasGetScreen))
	{
		cout << "dww---start" << endl;
		inputFileName.str("");
		inputFileName << SYS_PATH_PRE << index << SYS_PATH_TAIL;
		std::ifstream inputStream(inputFileName.str());
		//获取文件的大小
		inputStream.seekg(0, std::ios::end);
		int length = inputStream.tellg();
		inputStream.seekg(0, std::ios::beg);
		char *content = new char[length];

		if (inputStream.is_open())
		{
			cout << "dww--Now reading..." << endl;
			// 读取文件内容
			inputStream.read(content, length);
			contentStr = content;

			// 将读取的内容和目标文件内容相比较,如果相同,则返回
			idx = contentStr.find(KEY_DEVICE_NAME); // 在contentStr中查找XXXX_DEVICE_NAME.
			if (idx == string::npos)
			{ 	// 不存在。
				cout << "dww--not found targetKey" << endl;
				if (contentStr.find(SCREEN_DEVICE_NAME) != string::npos)
				{
					hasGetScreen = true;
					cout << "dww--getScreen = " << hasGetScreen << endl;
					screenId = index;
					index++;
					continue;
				}
			} else { // 存在。

				hasGetKey = true;
				cout << "dww--getKey = " << hasGetKey << endl;
				keyId = index;
				index++;
				continue;
			}
		}
		index++;
	}
	cout << "dww--path scan is over" << "  screenId=" << screenId << " keyId=" << keyId << endl;
}

void readTouchEvent(int *fds, int length){

	if ((pfds = (struct pollfd *)calloc(length, sizeof(struct pollfd))) == NULL)
	{
		cout << "dww--calloc error" << endl;
		exit(1);
	}

	if ((event = (struct input_event *)calloc(length, sizeof(struct input_event))) == NULL) {
		cout << "dww--calloc error" << endl;
		exit(1);
    }

	for (int i = 0; i < 2; i++)
	{
		(pfds + i)->fd = fds[i];
		(pfds + i)->events = POLLIN | POLLPRI;
	}

	while(1) { 	// 循环处理
		// 获取可用描述符的个数
		pollres = poll(pfds, 2, 500);
		if (pollres < 0)
		{
			exit(1);
		} else if (pollres == 0) {
			continue;
		} else {
			cout << "dww--pollres =" << pollres << endl;
		}

		for (int i = 0; i < length; i++) {
    		if ((pfds + i)->revents){
				ssize_t readSize = read(fds[i], (event+i), sizeof(struct input_event));
				if (readSize <= 0) {
					continue;
				}
				//  对于物理按键,暂不知道具体按键信息,先用物理音量键代替
				if ((event+i)->type == EV_KEY &&(event+i)->code == KEY_VOLUMEDOWN) {
					cout << "dww---------------kk物理按键kk" << endl;
					break;
				}
				// 对于屏幕点击事件确认,需要获取event.type(EVS_ABS和EV_KEY, EV_KEY和物理按键中的event.type冲突)
				if ((event+i)->type == EV_KEY | (event+i)->type == EV_ABS) {
					cout << "dww---------------abs屏幕点击abs" << endl;
				}
			}
		}	
	}
}


int main()
{
	// find the target device file	
	findTarget();
	// use variable : screenId, keyId;
	string pathScreen = DEV_PATH_PRE + std::to_string(screenId);
	string pathKey = DEV_PATH_PRE + std::to_string(keyId);

	abs_fd = open(pathScreen.c_str(), O_RDONLY | O_NONBLOCK); // "/dev/input/event2"
	key_fd = open(pathKey.c_str(), O_RDONLY | O_NONBLOCK);	// "/dev/input/event0"
    if (key_fd < 0 && abs_fd < 0) {
        cout<<"Open input touch device failed."<<endl;
        return 0;
    }
	fds[0] = abs_fd;
	fds[1] = key_fd;

    int length = sizeof( fds ) / sizeof( fds[0] ); //计算数组的长度.
 	readTouchEvent(fds, length);
	close(abs_fd);
	close(key_fd);
	return 0;
}
