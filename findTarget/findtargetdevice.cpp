#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#define PATH "/sys/class/input/event"

using namespace std;

// struct pollfd *ufds;

int main(){
    std::stringstream inputFileName;
    string targetScrean="Weida Hi-Tech CoolTouch® System";
    string targetKey="qpnp_pon";

    string::size_type idx;
    string contentStr;
    bool getKey = false, getScreen = false;

    int index =0;
   int screenId= 0, keyId = 0;

    while( !(getKey & getScreen)){
    	cout<<"dww---start"<<endl;
    	inputFileName.str("");
    	inputFileName << PATH << index <<"/device/name";
   	std::ifstream inputStream(inputFileName.str());
   	//获取文件的大小
    	inputStream.seekg(0, std::ios::end);
    	int length = inputStream.tellg();
    	inputStream.seekg(0, std::ios::beg);
    	char* content = new char[length];在

    	if (inputStream.is_open()) {
	    cout << "dww--Now reading..." << endl;
	    // 读取文件内容
	    cout << "dww-length:" << length << endl;
	    inputStream.read(content, length);
	    cout<< "dww--content.length="<<strlen(content)<<endl;
	    cout<<"dww--content:"<<content<<endl;	
   	    contentStr = content;

            // 将读取的内容和目标文件内容相比较,如果相同,则返回
            idx=contentStr.find(targetKey);  // 在contentStr中查找targetXXX.
            if(idx == string::npos ) {  // 不存在。
	        cout << "dww--not found targetKey"<<endl;
		if (contentStr.find(targetScrean) !=string::npos) {
	            cout<<"dww--found targetKey"<<endl; 
    	  	    cout<<"dww--targetScrean event"<<index<<endl;
		    getScreen = true;
		    screenId = index;
	       	    index++;
		    continue;			
		}
            } else {   // 存在。
         	cout<<"dww--found targetKey"<<endl; 
		cout<<"dww--targetKey event"<<index<<endl; 
		getKey = true;
		keyId = index;
		index++;
		continue;
            }
        }
	index++;
    }
    cout<<"dww--once scan is over"<<"  screenId="<<screenId<<" keyId="<<keyId <<endl;
    return 0;  
}


















