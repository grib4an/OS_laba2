#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <iostream>
#include <process.h>
#include "SmartPointers.h"

using namespace std;
using namespace SmartPointers;

const LPCWSTR Log_File = L"log.txt";
const LPCWSTR Result_File = L"result.txt";

SYSTEMTIME time;
DWORD ByteWriter;
HANDLE event1, event2, event3, thread1, thread2, thread3, LogFile, ResultFile;
const int number = 5;

class Point {
	int x;
	int y;
	string addResultTime;
	string writeResultTime;

public:
	Point() {}

	Point(int x,int y) {
		this->x = x;
		this->y = y;
	}


	void setX(int x) {
		this->x = x;
	}

	void setY(int y) {
		this->y = y;
	}

	int getX() {
		return x;
	}

	int getY() {
		return y;
	}

	void setAddTime(string addResultTime) {
		this->addResultTime = addResultTime;
	}

	string getAddTime() {
		return addResultTime;
	}

	void setWriteTime(string writeResultTime) {
		this->writeResultTime = writeResultTime;
	}

	string getWriteTime() {
		return writeResultTime;
	}

};

void Function(SmartPointer<Point> &smartPoiner) {
	for (int i = 0; i < number; i++) {
		WaitForSingleObject(event1,INFINITE);
		cout << "method1" << endl;
		//y = x ^ 2 / 2
		int x = i;
		int y=(i*i)/2;
		GetLocalTime(&time);
		ResetEvent(event1);
		smartPoiner->setY(y);
		//smartPoiner.release();
		smartPoiner->setX(i);
		//smartPoiner.release();
		smartPoiner->setAddTime(to_string(int(time.wHour)) + ":" + to_string(int(time.wMinute)) + ":" + to_string(int(time.wSecond)));
		//smartPoiner.release();
		SetEvent(event2);

	}

}

void Write_in_File(SmartPointer<Point> &smartPoiner) {
	for (int i = 0; i < number; i++) {
		WaitForSingleObject(event2,INFINITE);
		cout << "method2" << endl;
		GetLocalTime(&time);
		string resultat = "x=" + to_string(smartPoiner->getX()) + "y=" + to_string(smartPoiner->getY())+"||";
		//smartPoiner.release();
		//smartPoiner.release();
		WriteFile(ResultFile,resultat.c_str(),resultat.size(),&ByteWriter,NULL);
		ResetEvent(event2);
		smartPoiner->setWriteTime(to_string(int(time.wHour)) + ":" + to_string(int(time.wMinute)) + ":" + to_string(int(time.wSecond)));
		//smartPoiner.release();

		SetEvent(event3);
	}
}

void Log_in_File(SmartPointer<Point> &smartPoiner) {
	for (int i = 0; i < number; i++) {
		WaitForSingleObject(event3, INFINITE);
		cout << "method3" << endl;
		string logi = "время получения значений " + to_string(smartPoiner->getX()) + " and " + to_string(smartPoiner->getY()) + ":" + smartPoiner->getAddTime() +
			" Время записи значения" + smartPoiner->getWriteTime() + "||";
		//smartPoiner.release();
		//smartPoiner.release();
		//smartPoiner.release();
		//smartPoiner.release();
		WriteFile(LogFile, logi.c_str(), logi.size(), &ByteWriter, NULL);
		ResetEvent(event3);
		SetEvent(event1);
	}	

}



int main()
{
	Point *point = new Point(0,0);
	SmartPointer<Point> smartPointer(point);

	int number = 5;

	event1 = CreateEvent(NULL, TRUE, TRUE, NULL);
	event2 = CreateEvent(NULL, TRUE, FALSE, NULL);
	event3 = CreateEvent(NULL, TRUE, FALSE, NULL);

	thread1 = (HANDLE)_beginthread((_beginthread_proc_type)Function, 0, (void*)&smartPointer);
	thread2 = (HANDLE)_beginthread((_beginthread_proc_type)Write_in_File, 0, (void*)&smartPointer);
	thread3 = (HANDLE)_beginthread((_beginthread_proc_type)Log_in_File, 0, (void*)&smartPointer);

	ResultFile = CreateFile(Result_File, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	LogFile = CreateFile(Log_File, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	WaitForSingleObject(thread1,INFINITE);
	WaitForSingleObject(thread2, INFINITE);
	WaitForSingleObject(thread3, INFINITE);

	system("Pause");
	return 0;
}
