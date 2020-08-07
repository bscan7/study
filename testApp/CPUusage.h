#pragma once
#include <Windows.h>   
//ԭ������GetProcessTimes()�������ϴε��õõ��Ľ����������õ�ĳ��ʱ����CPU��ʹ��ʱ��  
//C++ ��ȡ�ض����̹涨CPUʹ����  ԭ�ģ�http://blog.csdn.net/liuqx97bb/article/details/52058657  
class CPUusage {
private:
	//typedef long long          DWORD;
	//typedef unsigned long long DWORD;
	HANDLE _hProcess;
	int _processor;    //cpu����    
	DWORD _last_time;         //��һ�ε�ʱ��    
	DWORD _last_system_time;


	// ʱ��ת��    
	DWORD file_time_2_utc(const FILETIME* ftime);

	// ���CPU�ĺ���    
	int get_processor_number();

	//��ʼ��  
	void init()
	{
		_last_system_time = 0;
		_last_time = 0;
		_hProcess = 0;
	}

	//�رս��̾��  
	void clear()
	{
		if (_hProcess) {
			CloseHandle(_hProcess);
			_hProcess = 0;
		}
	}

public:
	CPUusage(DWORD ProcessID) {
		init();
		_processor = get_processor_number();
		setpid(ProcessID);
	}
	CPUusage() { init(); _processor = get_processor_number(); }
	~CPUusage() { clear(); }

	//����ֵΪ���̾�������ж�OpenProcess�Ƿ�ɹ�  
	HANDLE setpid(DWORD ProcessID) {
		clear();    //���֮ǰ���ӹ���һ�����̣����ȹر����ľ��  
		init();
		return _hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, ProcessID);
	}

	//-1 ��Ϊʧ�ܻ�������˳��� ����ɹ����״ε��û᷵��-2����;��setpid������PID���״ε���Ҳ�᷵��-2��  
	float get_cpu_usage();
};