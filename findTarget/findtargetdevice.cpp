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

#define PATH "/sys/class/input/event"

using namespace std;

std::stringstream inputFileName;
string targetScrean = "Weida Hi-Tech CoolTouch® System";
string targetKey = "qpnp_pon";

string::size_type idx;
string contentStr;
bool getKey = false, getScreen = false;
int index = 0;
int screenId = 0, keyId = 0;

struct input_event abs_event;
struct input_event key_event;

struct pollfd *pfds;
int key_fd = 0;
int abs_fd = 0;
int fds[] = {0, 0};
int pollres = 0;

void findTarget()
{

	while (!(getKey & getScreen))
	{
		cout << "dww---start" << endl;
		inputFileName.str("");
		inputFileName << PATH << index << "/device/name";
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
			cout << "dww-length:" << length << endl;
			inputStream.read(content, length);
			cout << "dww--content.length=" << strlen(content) << endl;
			cout << "dww--content:" << content << endl;
			contentStr = content;

			// 将读取的内容和目标文件内容相比较,如果相同,则返回
			idx = contentStr.find(targetKey); // 在contentStr中查找targetXXX.
			if (idx == string::npos)
			{ 	// 不存在。
				cout << "dww--not found targetKey" << endl;
				if (contentStr.find(targetScrean) != string::npos)
				{
					cout << "dww--found targetKey" << endl;
					cout << "dww--targetScrean event" << index << endl;
					getScreen = true;
					cout << "dww--getScreen = " << getScreen << endl;
					screenId = index;
					index++;
					continue;
				}
			} else { // 存在。
				cout << "dww--found targetKey" << endl;
				cout << "dww--targetKey event" << index << endl;
				getKey = true;
				cout << "dww--getKey = " << getKey << endl;
				keyId = index;
				index++;
				continue;
			}
		}
		index++;
	}
	cout << "dww--path scan is over"
		 << "  screenId=" << screenId << " keyId=" << keyId << endl;
}

//void readTouchEvent(int fds[2]){
// }

int main()
{

	findTarget();
	// use variable : screenId, keyId;

	if (!(getKey && getScreen))
	{
		cout << "dww--We didn`t find screen`s path or key`s path " << endl;
	}

	string pathScreen = "/dev/input/event" + std::to_string(screenId);
	string pathKey = "/dev/input/event" + std::to_string(keyId);

	abs_fd = open(pathScreen.c_str(), O_RDONLY | O_NONBLOCK); // "/dev/input/event2"
	key_fd = open(pathKey.c_str(), O_RDONLY | O_NONBLOCK);	// "/dev/input/event0"
	fds[0] = abs_fd;
	fds[1] = key_fd;

	cout << "dww--pathScreen=" << pathScreen.c_str() << "  pathKey =" << pathKey.c_str() << endl;
	cout << "dww--abs_fd=" << abs_fd << " key_fd=" << key_fd << endl;

	// memset(pfds, 0, sizeof(struct pollfd));

	if ((pfds = (struct pollfd *)calloc(2, sizeof(struct pollfd))) == NULL)
	{
		cout << "dww--calloc error" << endl;
		exit(1);
	}

	memset(&abs_event, 0, sizeof(struct input_event));
	memset(&key_event, 0, sizeof(struct input_event));

	if (abs_fd <= 0 && key_fd <= 0)
	{
		printf("open error \n");
	}
	for (int i = 0; i < 2; i++)
	{
		(pfds + i)->fd = fds[i];
		(pfds + i)->events = POLLIN | POLLPRI;
	}
	//    pfds[0].fd = abs_fd;
	//    pfds[1].fd = key_fd;
	//    pfds[1].events = POLLIN | POLLPRI;
	//    pfds[1].events = POLLIN | POLLPRI;
	for (;;)
	{ 	// 循环处理
		// 获取可用描述符的个数
		pollres = poll(pfds, 2, 500);
		if (pollres < 0)
		{
			cout << "dww--poll input device failed." << endl;
			exit(1);
		} else if (pollres == 0) {
			cout << "dww--pollres == 0 ; poll timeout" << endl;
			continue;
		} else {
			cout << "dww--pollres =" << pollres << endl;
		}
		cout << "dww--pfds->revents = " << pfds->revents << endl;
		if (pfds->revents)
		{ // 是否有事件发生,数据可读 key
			cout << "dww----------+0-" << endl;
			ssize_t readSize = read(fds[0], &abs_event, sizeof(abs_event));
			if (readSize <= 0)
			{
				cout << "dww--readSize=" << readSize << endl;
				continue;
			}
			cout << "dww--abs--readSize2=" << readSize << " abs_event.type=" << abs_event.type << endl;
			if (abs_event.type == EV_KEY | abs_event.type == EV_ABS)
			{
				cout << "dww---------------abs屏幕点击abs" << endl;
			}
		}
		cout << "dww--(pfds+1)->revents = " << (pfds + 1)->revents << endl;
		if ((pfds + 1)->revents)
		{   // 是否有事件发生,数据可读 abs
			cout << "dww----------+1-" << endl;
			ssize_t readSize = read(fds[1], &key_event, sizeof(key_event));
			if (readSize <= 0)
			{
				cout << "dww--readSize=" << readSize << endl;
				continue;
			}
			cout << "dww--key--readSize2=" << readSize << " key_event.type=" << key_event.type << endl;
			if (key_event.type == EV_KEY)
			{
				cout << "dww---------------kk物理按键kk" << endl;
			}
		}
	}
	close(abs_fd);
	close(key_fd);
	return 0;
}
