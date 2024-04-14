#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <assert.h>


using namespace std;
#define DEBUG
int getstrlength(char*, int);
void initarray(char*, int);

void rs232c() {
	HANDLE comPort = CreateFileW(_T("COM3"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (comPort == INVALID_HANDLE_VALUE) {
		printf("failed\n");
	}
	DCB dcb;
	BOOL a = GetCommState(comPort, &dcb);
	if (a == FALSE) {
		SetLastError(GetLastError());
		printf("last function is getcommstate\n");
		printf("%d\n", GetLastError());
	}
	dcb.BaudRate = 38400;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.fDtrControl = FALSE;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.EofChar = '\n';
	dcb.EvtChar = NULL;
	BOOL f = SetCommState(comPort, &dcb);
	if (f == FALSE) {
		SetLastError(GetLastError());
		printf("last function is setcommstate\n");
		printf("%d\n", GetLastError());
	}
	int flg;
	while (1) {
		cout << "0:read, 1:write" << endl;
		cin >> flg;
		if (flg != 0 && flg != 1) {
			break;
		}
		if (flg == 0) {
			DWORD errors;//error���N�����ꍇ�ɃG���[�R�[�h����
			COMSTAT comStat;//�ʐM��ԃo�b�t�@�\����
			ClearCommError(comPort, &errors, &comStat);
			int lengthOfRecieved = comStat.cbInQue;//��M�������b�Z�[�W�����擾
			HANDLE hhp = HeapCreate(0, 0, 0);
			cout << "hhp" << endl;
			cout << hhp << endl;
			//��M�������b�Z�[�W��+�I�[�������̃f�[�^�o�b�t�@���q�[�v��Ɋm��
			char* recieveData = (char*)HeapAlloc(hhp, 0, sizeof(char) * (lengthOfRecieved + 1));
			DWORD numberOfGot;//��M�o�b�t�@����ǂݗ������o�C�g��
			//��M�o�b�t�@����f�[�^�o�b�t�@�ɓǂݍ���
			ReadFile(comPort, recieveData, lengthOfRecieved, &numberOfGot, NULL);
			/*����Ƃ�*/
			*(recieveData+1*lengthOfRecieved) = '\0';
			//recieveData[lengthOfRecieved] = '\0';
			
			printf("%s\n", recieveData);
		}
		else {
			char sentData[32];
			cout << "�p��œ���32�����ȉ�" << endl;
			cin >> sentData;
			DWORD lengthOfSent = getstrlength(sentData, 32);//���M����o�C�g��
			DWORD numberOfPut=0;//���M�����o�C�g��
			DWORD retval;
			bool check = WriteFile(comPort, sentData, lengthOfSent, &numberOfPut, NULL);//�|�[�g�֑��M
			if (check == FALSE) {
				SetLastError(GetLastError());
				printf("Last function is WriteFile\n");
				retval = GetLastError();
				printf("%d\n", retval);
			}
			retval = GetFileType(comPort);
			retval = GetLastError();
			if (retval != NO_ERROR) {
				SetLastError(GetLastError());
				printf("Last function is GerFileType\n");
				printf("error %d", retval);
			}
			initarray(sentData, sizeof(sentData));
		}
	}

	CloseHandle(comPort);
}

int main() {
	int buf[34];
	char uuu[53];
	rs232c();
	return 0;
}

int getstrlength(char* Sptr, int Max) {
	for (int i = 0; i < Max; i++) {
		if (*Sptr == '\0') {
			return i + 1;
		}
		Sptr++;
	}
	return -1;
}
void initarray(char* ptr, int size) {
	for (int i = 0; i < size; i++) {
		*ptr = 0;
		ptr++;
	}
}
