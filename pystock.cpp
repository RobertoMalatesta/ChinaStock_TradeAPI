#include "stdafx.h"
#include "pystock.h"
char *strtok_r(char *s, const char *delim, char **save_ptr);
char* UnicodeToAnsi(const wchar_t* szStr);

Pystock::Pystock()
{
	h_mainLogin = NULL;
	h_mainTrade = NULL;
	h_adWindow = NULL;
	h_loginfailed = NULL;
}

Pystock::~Pystock()
{
	//CloseTrade();
	id_login = NULL;

}

int Pystock::OpenTrade(char *filename)
{
	char *tradefile2 = filename;

	WCHAR tradefile3[256];

	memset(tradefile3, 0, sizeof(tradefile3));

	MultiByteToWideChar(CP_ACP, 0, tradefile2, strlen(tradefile2) + 1, tradefile3, sizeof(tradefile3) / sizeof(tradefile3[0]));

	STARTUPINFO si;

	PROCESS_INFORMATION pi;

	ZeroMemory(&pi, sizeof(pi));

	ZeroMemory(&si, sizeof(si));

	si.cb = sizeof(si);

	if (CreateProcess(tradefile3, NULL, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
	{
		::CloseHandle(pi.hProcess);
		::CloseHandle(pi.hThread);
		id_login = pi.dwProcessId;
		cout << "�½���id = " << Pystock::id_login << endl;
		cout << "�½���thread" << pi.dwThreadId << endl;

		int zz = 0;
		while (h_mainLogin == NULL)
		{
			Sleep(200);
			h_mainLogin = GetWindowHd(id_login, appName1);
			zz++;
			if (h_mainLogin != NULL)
			{
				isopened = true;
				return 1;
			}
			if (zz >= 15)
			{
				isopened = false;
				return 0;
			}
		}
	}
	return 0;
}

int Pystock::LoginTrade(char *accName, char * secret, char * conSecret)
{
	HWND hwnd_account, hwnd_connect, hwnd_trade, hwnd_login;

	hwnd_account = GetDlgItem(h_mainLogin, 1100);
	hwnd_trade = GetDlgItem(h_mainLogin, 1104);
	hwnd_connect = GetDlgItem(h_mainLogin, 1117);
	hwnd_login = GetDlgItem(h_mainLogin, 1);

	//***������Ϣ
	SendMessageA(hwnd_account, WM_SETTEXT, 0, (LPARAM)accName);
	Sleep(100);
	SendMessageA(hwnd_trade, WM_SETTEXT, 0, (LPARAM)secret);
	Sleep(100);
	SendMessageA(hwnd_connect, WM_SETTEXT, 0, (LPARAM)conSecret);
	Sleep(100);
	SendMessage(hwnd_login, BM_CLICK, 0, 0);

	int zzz = 0;
	while (h_mainTrade == NULL)
	{
		Sleep(300);
		h_mainTrade = GetWindowHd(id_login, appName3);

		zzz++;
		if (h_mainTrade != NULL)
		{
			cout << "��¼�ɹ�" << endl;
			Sleep(200);
			firsthidetrade();
			CloseAdWindow();
			return 1;
		}
		if (zzz >= 15)
		{
			//cout << "��¼ʧ��" << endl;
			LoginFailed();
			return 0;
		}
	}
	return 0;
}

int Pystock::CloseAdWindow()
{
	if (h_mainTrade == NULL)
	{
		cout << "�޽��״���" << endl << endl;
		return 0;
	}

	char*  temp_adtext = "#32770";
	int temp_ad = 0;
	while (h_adWindow == NULL)
	{
		Sleep(200);
		h_adWindow = GetAdWindowHd(id_login, temp_adtext);
		temp_ad++;
		if (h_adWindow != NULL)
		{
			//cout << endl << "�����:  " << h_adWindow << endl;
			PostMessage(GetDlgItem(h_adWindow, 489), BM_CLICK, 0, 0);
			//cout << "���رհ�ť:  " << GetDlgItem(h_adWindow, 489) << endl << endl;
			cout << "���رճɹ�" << endl << endl;
			return 1;
		}
		if (temp_ad >= 5)
		{
			cout << "�ر����ʧ��" << endl << endl;
			return 0;
		}
	}
	return 0;
}

int Pystock::HideTrade()
{
	if (h_mainTrade == NULL)
	{
		return 0;
	}
	if (IsWindowVisible(h_mainTrade))
	{
		ShowWindow(h_mainTrade, SW_HIDE);
		return 1;
	}
	else
	{
		ShowWindow(h_mainTrade, SW_SHOW);
		return 2;
	}
	return 0;
}

int Pystock::LoginFailed()
{
	int temp_loginfailed = 0;
	HWND  h_failtext = NULL;
	HWND	h_failbtn = NULL;
	char		t_failtext[50];
	while (h_mainTrade == NULL)
	{
		h_loginfailed = GetWindowHd(id_login, appName2);
		temp_loginfailed++;

		if (h_loginfailed == NULL)
		{
			h_mainTrade = GetWindowHd(id_login, appName3);
			firsthidetrade();
			return 1;
		}
		else
		{
			h_failtext = GetDlgItem(h_loginfailed, 7013);
			GetWindowTextA(h_failtext, t_failtext, 50);
			cout << "�������: " << t_failtext << endl << endl;
			h_failbtn = GetDlgItem(h_loginfailed, 7015);
			PostMessage(h_failbtn, BM_CLICK, 0, 0);
			return 2;
		}
	}
	return 0;
}

int Pystock::PreHandle()
{
	getToolBar();
	Sleep(150);
	getBtnBar();
	getPosition();
	getAbsort();
	int temp_pre = 0;
	while(h_buybtn[0] == NULL || h_sellbtn[0] == NULL || h_Absortticket[0] == NULL || h_Hposition[0] == NULL || h_Dealticket[0] == NULL)
/*
		|| h_buybtn[0] == (HWND)0xCDCDCDCD || h_sellbtn[0] == (HWND)0xCDCDCDCD || h_Absortticket[0] == (HWND)0xCDCDCDCD 
		|| h_Hposition[0] == (HWND)0xCDCDCDCD || h_Dealticket[0] == (HWND)0xCDCDCDCD)*/
	{
		Sleep(150);
		getToolBar();
		Sleep(150);
		getBtnBar();
		getPosition();
		getAbsort();
		temp_pre++;
		if (temp_pre > 5)
		{
			break;
		}
	}
	return 0;
}

void Pystock::getToolBar()
{
	//ͨ������ľ����ȡ������������ջ�ȡ���������µ����������
	h_toolBar[1] = GetDlgItem(h_mainTrade, 59648);
	h_toolBar[2] = GetDlgItem(h_toolBar[1], 0);
	h_toolBar[3] = GetDlgItem(h_toolBar[2], 0);
	h_toolBar[4] = GetDlgItem(h_toolBar[3], 59648);
	h_toolBar[5] = GetDlgItem(h_toolBar[4], 59649);		//���� ���� ���� �ֲ� �ɽ��ĸ����
	h_toolBar[6] = GetDlgItem(h_toolBar[5], 59419);		//�������ľ��

	h_tradeBar = GetDlgItem(h_toolBar[6], 0);
	for (int i = 1; i < 7; i++)
	{
		cout << "toobar[" << i << "]:" << h_toolBar[i] << endl;
	}
	//SetForegroundWindow(mainT);
	PostMessage(h_mainTrade, WM_KEYDOWN, VK_F1, 0);
	PostMessage(h_mainTrade, WM_KEYUP, VK_F1, 0);
	Sleep(300);
	PostMessage(h_mainTrade, WM_KEYDOWN, VK_F2, 0);
	PostMessage(h_mainTrade, WM_KEYUP, VK_F2, 0);
	Sleep(300);
	PostMessage(h_mainTrade, WM_KEYDOWN, VK_F3, 0);
	PostMessage(h_mainTrade, WM_KEYUP, VK_F3, 0);
	Sleep(500);
	PostMessage(h_mainTrade, WM_KEYDOWN, VK_F4, 0);
	PostMessage(h_mainTrade, WM_KEYUP, VK_F4, 0); 
	Sleep(500); 
	PostMessage(h_mainTrade, WM_KEYDOWN, VK_F5, 0);
	PostMessage(h_mainTrade, WM_KEYUP, VK_F5, 0);
	Sleep(300);

}

void Pystock::getBtnBar()
{
	if (h_tradeBar == NULL)
	{
		this->getToolBar();
	}
	//---���Ȼ�ȡ4���Ӽ������F1 F2 F3 F4 F5
	HWND temp1 = NULL, temp2 = NULL, temp3 = NULL, temp4 = NULL, temp5 = NULL, temp6 = NULL, temp7 = NULL;
	char text1[50], text2[50], text3[50], text4[50];
	temp1 = FindWindow(NULL, NULL);
	int i = 0;
	while (temp1 != NULL)
	{
		i++;
		temp1 = FindWindowExA(h_toolBar[5], temp2, "#32770", NULL);
		cout << endl << "�����ҽ��׾�� " << i << "�Σ�" << endl << endl;

		temp2 = temp1;
		temp3 = GetDlgItem(temp1, 2010);
		//cout << ("�µ�����") << temp3 << endl;
		if (temp3 != NULL && GetDlgItem(temp1, 2010) != NULL)
		{
			GetWindowTextA(temp3, text1, 50);
			if (strstr(text1, "�����µ�"))
			{
				h_buybtn[0] = temp1;
				//cout << "��������" << temp3 << "����" << text1 << endl;
				continue;
			}
			if (strstr(text1, "�����µ�"))
			{
				h_sellbtn[0] = temp1;
				//cout << "���������" << temp3 << "����" << text1 << endl;
				continue;
			}
		}
		temp4 = GetDlgItem(temp1, 1136);
		//cout << "��������" << temp4 << endl;

		if (temp4 != NULL && (h_Absortticket[0] == NULL || h_Absortticket[0] == (HWND)0xCDCDCDCD))
		{
			GetWindowTextA(temp4, text2, 50);
			if (strstr(text2, "�� ��"))
			{
				h_Absortticket[0] = temp1;
				cout << ("���������") << temp4 << ("����") << text2 << endl;
				continue;
			}
		}

		temp5 = GetDlgItem(temp1, 1723);
		//cout << ("�ֲּ���") << temp5 << endl;
		if (temp5 != NULL && (h_Hposition[0] == NULL || h_Hposition[0] == (HWND)0xCDCDCDCD))
		{
			GetWindowTextA(temp5, text4, 50);
			if (strstr(text4, "�޸ĳɱ�"))
			{
				h_Hposition[0] = temp1;
				cout << ("�ֲ־����") << temp5 << endl << ("����") << text4 << endl;
				break;
			}
		}
	}

	temp7 = GetWindow(h_Hposition[0], GW_HWNDNEXT);
	while (temp7 != NULL)
	{
		temp7 = GetWindow(temp7, GW_HWNDLAST);
		GetClassNameA(temp7, text3, 50);
		if (strstr(text3, "#32770") && (temp7 != h_sellbtn[0] && temp7 != h_buybtn[0]
			&& temp7 != h_Absortticket[0] && temp7 != h_Hposition[0]))
		{
			h_Dealticket[0] = temp7;
			break;
		}
	}

	cout << "�����µ�: ------" << h_buybtn[0] << endl;
	cout << "�����µ�: ------" << h_sellbtn[0] << endl;
	cout << "����     : ------" << h_Absortticket[0] << endl;
	cout << "�ֲ�     : ------" << h_Hposition[0] << endl;
	cout << "�ɽ�     : ------" << h_Dealticket[0] << endl;
}

void Pystock::getBuy()
{
	h_buybtn[1] = GetDlgItem(h_buybtn[0], 12005);//�������봰��
	h_buybtn[2] = GetDlgItem(h_buybtn[0], 12006);//�۸����봰��
	h_buybtn[3] = GetDlgItem(h_buybtn[0], 12007);//������������
	h_buybtn[4] = GetDlgItem(h_buybtn[0], 2010); //�����µ�

	return;
}

void Pystock::getSell()
{
	h_sellbtn[1] = GetDlgItem(h_sellbtn[0], 12005);//�������봰��
	h_sellbtn[2] = GetDlgItem(h_sellbtn[0], 12006);//�۸����봰��
	h_sellbtn[3] = GetDlgItem(h_sellbtn[0], 12007);//������������
	h_sellbtn[4] = GetDlgItem(h_sellbtn[0], 2010); //�����µ�

	return;
}

void Pystock::getAbsort()
{
	h_Absortticket[1] = GetDlgItem(h_Absortticket[0], 9046);		//�������봰��
	h_Absortticket[2] = GetDlgItem(h_Absortticket[0], 1136);		//������ť
	h_Absortticket[3] = GetDlgItem(h_Absortticket[0], 14);			//ȫѡ��
	h_Absortticket[4] = GetDlgItem(h_Absortticket[0], 1140);		//ˢ�½���
	h_Absortticket[5] = GetDlgItem(h_Absortticket[0], 1163);		//ȫ��ѡ
	h_Absortticket[6] = GetDlgItem(h_Absortticket[0], 1567);		//�ֲ��б�
	h_Absortticket[7] = GetDlgItem(h_Absortticket[6], 0);			//�ֲ��б��ͷ

	for (int i = 0; i < 7;i++)
	{
	cout << "h_Absortticket[" << i  << "]:  "<< h_Absortticket[i] << endl;
	}
	return;
}

void Pystock::getDeal()
{
	h_Dealticket[1] = GetDlgItem(h_Dealticket[0], 1140);		//ˢ�½���
	h_Dealticket[2] = GetDlgItem(h_Dealticket[0], 1567);		//�ɽ��б�
	h_Dealticket[3] = GetDlgItem(h_Dealticket[2], 0);			//�ɽ��б��ͷ
	/*
	for (int i = 0; i < 4;i++)
	{
	cout << "h_Dealticket[" << i  << "]:  "<< h_Dealticket[i] << endl;
	}*/
	return;
}

void Pystock::getPosition()
{
	h_Hposition[1] = GetDlgItem(h_Hposition[0], 1576);			//�˻���Ϣ
	h_Hposition[2] = GetDlgItem(h_Hposition[0], 1567);			//�ֲ��б�
	h_Hposition[3] = GetDlgItem(h_Hposition[2], 0);					//�ֲ��б��ͷ
	h_Hposition[4] = GetDlgItem(h_Hposition[0], 1140);			//ˢ��
	h_Hposition[5] = GetDlgItem(h_Hposition[0], 1033);			//�ֲ������ݼ�
	h_Hposition[6] = GetDlgItem(h_Hposition[0], 1035);			//�ֲ�������ݼ�
	h_Hposition[7] = GetDlgItem(h_Hposition[0], 1243);			//�ֲ�����������

	return;
}

void Pystock::prePosition()
{
	getPosition();
	PostMessage(h_Hposition[4], BM_CLICK, 0, 0);
	Sleep(200);

	memset(positionTitle, 0x00, sizeof (char)* 200);	//ȫ������Ϊ0x00��\0�ַ�
	GetWindowTextA(h_Hposition[1], positionTitle, 200);

	//cout << "�ֲ�: " << positionTitle << endl;

	char* pos = positionTitle;
	char* outer_ptr = NULL;
	char* inner_ptr = NULL;
	int PosNum_temp = 0;
	while ((PosInfo[PosNum_temp] = strtok_r(pos, "  ", &outer_ptr)) != NULL)
	{
		pos = PosInfo[PosNum_temp];
		while ((PosInfo[PosNum_temp] = strtok_r(pos, ":", &inner_ptr)) != NULL)
		{
			PosNum_temp++;
			pos = NULL;
		}
		pos = NULL;
	}
	//cout << "�ֲ���Ϣ����: " << PosNum << endl;
	for (int j = 0; j < PosNum_temp; j++)
	{
		//cout << PosInfo[j] << endl;
	}
	PosNum = PosNum_temp / 2;
	return;
}

void Pystock::getAccountTicket(int &rows, int &cols)
{
	HANDLE    hProcess;
	LVITEM    *pointer;
	HWND    hListview;//hwnd,
	int headerhwnd; //listview�ؼ�����ͷ���
	//int rows, cols;  //listview�ؼ��е�������
	DWORD ProcessID = NULL;
	DWORD ThreadID = NULL;
	//���̽��洰�ڵľ��,ͨ��SPY��ȡ
	getPosition();
	hListview = h_Hposition[2];
	//listview����ͷ���
	headerhwnd = SendMessageA(hListview, LVM_GETHEADER, 0, 0);
	//������:���̵�����
	rows = SendMessage(hListview, LVM_GETITEMCOUNT, 0, 0);
	//�б�����
	cols = SendMessage((HWND)headerhwnd, HDM_GETITEMCOUNT, 0, 0);
	// 	cout << "������" << rows << "������" << cols << endl;

	ThreadID = GetWindowThreadProcessId(hListview, &ProcessID);
	//�򿪲��������
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	//���������ڴ���
	pointer = (LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			LVITEM vItem;
			vItem.mask = LVIF_TEXT;    //˵��pszText����Ч��
			vItem.iItem = i;        //�к�
			vItem.iSubItem = j;        //�к�
			vItem.cchTextMax = 512;    //���ܴ洢�������ı�Ϊ256�ֽ�

			LPSTR pItem = NULL;
			//�����ڴ�ռ�
			pItem = (LPSTR)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
			vItem.pszText = (LPWSTR)pItem;

			WriteProcessMemory(hProcess, pointer, &vItem, sizeof(LVITEM), NULL);
			SendMessage(hListview, LVM_GETITEM, (WPARAM)i, (LPARAM)pointer);

			WCHAR  ItemBuf[512];
			memset(ItemBuf, 0, 512);
			ReadProcessMemory(hProcess, (PVOID)pItem, ItemBuf, 512, NULL);

			//�ͷ��ڴ�ռ�
			VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);

			AccountTicket[i][j] = UnicodeToAnsi(ItemBuf);
			//cout << "*****��ȡ��������" << AccountTicket[i][j] << endl;
		}
	}
	//�ͷ��ڴ�ռ�
	VirtualFreeEx(hProcess, pointer, 0, MEM_RELEASE);//�������������ͷ�����������ڴ�ռ�,MEM_RELEASE��ʽ�ܳ���,��ȫ����
	CloseHandle(hProcess);//�رմ򿪵Ľ��̶���

}

bool  Pystock::getAccoutHead()
{
	//Ŀ�����ID����
	DWORD PID;
	HANDLE hProcess;
	int nBufferLength = 50; //��������С
	int nColCount = 0;        //����
	HWND hHeader = h_Hposition[3];//��ñ���ͷ���
	HWND h = h_Hposition[2];
	nColCount = (int)::SendMessage(hHeader, HDM_GETITEMCOUNT, 0, 0);      //��ȡListView����
	cout << "��ͷ������" << nColCount << endl;
	if (nColCount <= 0)
		return false;

	//Զ������ռ��ַ
	HDITEM  *pVirtualItem;
	wchar_t *pVirtualBuffer;

	GetWindowThreadProcessId(h, &PID);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);              //��ȡĿ����̾��ʧ��
	if (!hProcess)
	{
		/*AccountHeader[0] = "֤ȯ����");
		AccountHeader[1] = "֤ȯ����");
		AccountHeader[2] = "֤ȯ����");
		AccountHeader[4] = "��������");
		AccountHeader[7] = "�ɱ���");
		AccountHeader[5] = "��ǰ��");
		AccountHeader[6] = "������ֵ");
		AccountHeader[8] = "����ӯ��");
		AccountHeader[9] = "ӯ������(%)");*/
		return false;
	}

	//��Ŀ����̵�ַ�ռ�����ڴ�
	pVirtualItem = (HDITEM  *)VirtualAllocEx(hProcess, NULL, sizeof(HDITEM), MEM_COMMIT, PAGE_READWRITE);
	pVirtualBuffer = (wchar_t *)VirtualAllocEx(hProcess, NULL, nBufferLength, MEM_COMMIT, PAGE_READWRITE);
	if ((!pVirtualItem) || (!pVirtualBuffer))
	{
		cout  << "bad" <<endl ;
		return false;
	}

	for (int j = 0; j < nColCount; j++)
	{
		wchar_t *buffer = new wchar_t[nBufferLength];
		wmemset(buffer, 0, nBufferLength);

		HDITEM hdItem;

		hdItem.mask = HDI_TEXT;
		hdItem.fmt = 0;
		hdItem.cchTextMax = nBufferLength;
		hdItem.pszText = pVirtualBuffer;
		WriteProcessMemory(hProcess, pVirtualItem, &hdItem, sizeof(HDITEM), NULL);
		::SendMessage(hHeader, HDM_GETITEM, (WPARAM)j, (LPARAM)(LPHDITEM)pVirtualItem);
		ReadProcessMemory(hProcess, pVirtualBuffer, buffer, nBufferLength, NULL);
		ReadProcessMemory(hProcess, pVirtualItem, (LPVOID)&hdItem, sizeof(HDITEM), NULL);		//û����

		AccountHeader[j] = UnicodeToAnsi(buffer);
		cout << "////***////" << endl << AccountHeader[j] << endl;
		delete[]buffer;
	}
	//�ͷ�Ŀ������������ڴ�
	VirtualFreeEx(hProcess, pVirtualItem, sizeof(HDITEM), MEM_RELEASE);
	VirtualFreeEx(hProcess, pVirtualBuffer, nBufferLength, MEM_RELEASE);
	CloseHandle(hProcess);
	return true;
}

void Pystock::rangeAccount_Head()
{
	for (int i = 0; i < 12; i++)
	{
		AccHeaderRange[i] = 0;
	}
	if (getAccoutHead())
	{
		for (int i = 0; i < 12; i++)
		{
			cout << AccountHeader[i];
			if (AccountHeader[i] == "֤ȯ����")
			{
				AccHeaderRange[0] = i;
			}
			else if (AccountHeader[i] == "֤ȯ����")
			{
				AccHeaderRange[1] = i;
			}
			else if (AccountHeader[i] == "֤ȯ����" ||		//���뺣֤ͨȯ
				AccountHeader[i] == "�ֲ���" ||			//���붫ݸ֤ȯ
				AccountHeader[i] == "�ɷ����")		//����ƽ��֤ȯ
			{
				AccHeaderRange[2] = i;
			}
			else 	if (AccountHeader[i] == "��������" ||
				AccountHeader[i] == "���ùɷ�")
			{
				AccHeaderRange[3] = i;
			}
			else if (AccountHeader[i] == "�ɱ���")
			{
				AccHeaderRange[4] = i;
			}
			else if (AccountHeader[i] == "����ӯ��")
			{
				AccHeaderRange[5] = i;
			}
			else if (AccountHeader[i] == "ӯ������(%)")
			{
				AccHeaderRange[6] = i;
			}
			else if (AccountHeader[i] == "������ֵ")
			{
				AccHeaderRange[7] = i;
			}
			else if (AccountHeader[i] == "��ǰ��")
			{
				AccHeaderRange[8] = i;
			}
			cout << "����˳���"<< i << AccHeaderRange[i] << endl;
		}

		for (int i = 0; i < 8; i++)
		{
			cout << "<<<<<>>>>>" << AccHeaderRange[i];
		}
	}
	return;
}

void Pystock::getAbsortTicket(int &rows, int &cols)
{
	PostMessage(h_mainTrade, WM_KEYDOWN, VK_F3, 0);
	PostMessage(h_mainTrade, WM_KEYUP, VK_F3, 0);
	Sleep(500);

	HANDLE    hProcess;
	LVITEM    *pointer;
	HWND   hListview; //hwnd,
	int headerhwnd;
	DWORD ProcessID = NULL;
	DWORD ThreadID = NULL;
	//���̽��洰�ڵľ��,ͨ��SPY��ȡ
	getAbsort();
	hListview = h_Absortticket[6];
	//listview����ͷ���
	headerhwnd = SendMessageA(hListview, LVM_GETHEADER, 0, 0);
	//������:���̵�����
	rows = SendMessage(hListview, LVM_GETITEMCOUNT, 0, 0);
	//�б�����
	cols = SendMessage((HWND)headerhwnd, HDM_GETITEMCOUNT, 0, 0);
	// 	cout << "������" << rows << "������" << cols << endl;

	ThreadID = GetWindowThreadProcessId(hListview, &ProcessID);

	//�򿪲��������
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	//���������ڴ���
	pointer = (LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			LVITEM vItem;
			vItem.mask = LVIF_TEXT;    //˵��pszText����Ч��
			vItem.iItem = i;        //�к�
			vItem.iSubItem = j;        //�к�
			vItem.cchTextMax = 512;    //���ܴ洢�������ı�Ϊ256�ֽ�
			LPSTR pItem = NULL;
			//�����ڴ�ռ�
			pItem = (LPSTR)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
			vItem.pszText = (LPWSTR)pItem;

			WriteProcessMemory(hProcess, pointer, &vItem, sizeof(LVITEM), NULL);
			::SendMessage(hListview, LVM_GETITEM, (WPARAM)i, (LPARAM)pointer);

			WCHAR  ItemBuf[512];
			memset(ItemBuf, 0, 512);
			ReadProcessMemory(hProcess, (PVOID)pItem, ItemBuf, 512, NULL);
			VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
			AbsortTicket[i][j] = UnicodeToAnsi(ItemBuf);
			//cout << ">>>>>" << AbsortTicket[i][j] << endl;
		}
	}
	//�ͷ��ڴ�ռ�
	VirtualFreeEx(hProcess, pointer, 0, MEM_RELEASE);//�������������ͷ�����������ڴ�ռ�,MEM_RELEASE��ʽ�ܳ���,��ȫ����
	CloseHandle(hProcess);//�رմ򿪵Ľ��̶���
	// 	qDebug() << QString::fromLocal8Bit("test����") << AccountTicket[1][1] << endl;
}

bool Pystock::getAbsortHead()
{
	//Ŀ�����ID����
	DWORD PID;
	HANDLE hProcess;
	int nBufferLength = 50; //��������С
	int nColCount = 0;        //����
	//  	cout << "1:" << h_Absortticket[7] << endl << "2:" << h_Absortticket[6] << endl;
	HWND hHeader = h_Absortticket[7];//��ñ���ͷ���
	HWND h = h_Absortticket[6];
	nColCount = (int)::SendMessage(hHeader, HDM_GETITEMCOUNT, 0, 0);      //��ȡListView����
	//cout << "��ͷ������" << nColCount << endl;
	if (nColCount <= 0)
		return false;

	//Զ������ռ��ַ
	HDITEM  *pVirtualItem;
	wchar_t *pVirtualBuffer;

	GetWindowThreadProcessId(h, &PID);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);              //��ȡĿ����̾��ʧ��
	if (!hProcess)
	{
		/*AccountHeader[0] = "֤ȯ����");
		AccountHeader[1] = "֤ȯ����");
		AccountHeader[2] = "֤ȯ����");
		AccountHeader[4] = "��������");
		AccountHeader[7] = "�ɱ���");
		AccountHeader[5] = "��ǰ��");
		AccountHeader[6] = "������ֵ");
		AccountHeader[8] = "����ӯ��");
		AccountHeader[9] = "ӯ������(%)");*/
		return false;
	}

	//��Ŀ����̵�ַ�ռ�����ڴ�
	pVirtualItem = (HDITEM  *)VirtualAllocEx(hProcess, NULL, sizeof(HDITEM), MEM_COMMIT, PAGE_READWRITE);
	pVirtualBuffer = (wchar_t *)VirtualAllocEx(hProcess, NULL, nBufferLength, MEM_COMMIT, PAGE_READWRITE);
	if ((!pVirtualItem) || (!pVirtualBuffer))
	{
		//cout  << "sapdagsd" <<endl ;
		return false;
	}

	for (int j = 0; j < nColCount; j++)
	{
		wchar_t *buffer = new wchar_t[nBufferLength];
		wmemset(buffer, 0, nBufferLength);

		HDITEM hdItem;

		hdItem.mask = HDI_TEXT;
		hdItem.fmt = 0;
		hdItem.cchTextMax = nBufferLength;
		hdItem.pszText = pVirtualBuffer;
		WriteProcessMemory(hProcess, pVirtualItem, &hdItem, sizeof(HDITEM), NULL);
		::SendMessage(hHeader, HDM_GETITEM, (WPARAM)j, (LPARAM)(LPHDITEM)pVirtualItem);
		ReadProcessMemory(hProcess, pVirtualBuffer, buffer, nBufferLength, NULL);
		ReadProcessMemory(hProcess, pVirtualItem, (LPVOID)&hdItem, sizeof(HDITEM), NULL);		//û����

		AbsortHeader[j] = UnicodeToAnsi(buffer);
		//cout << "////***////" << endl << AbsortHeader[j] << endl;
		delete[]buffer;
	}
	//�ͷ�Ŀ������������ڴ�
	VirtualFreeEx(hProcess, pVirtualItem, sizeof(HDITEM), MEM_RELEASE);
	VirtualFreeEx(hProcess, pVirtualBuffer, nBufferLength, MEM_RELEASE);
	CloseHandle(hProcess);
	return true;
}

void Pystock::rangeAbsort_Head()
{
	for (int i = 0; i < 12; i++)
	{
		AbsHeaderRange[i] = 0;
	}
	if (getAbsortHead())
	{
		for (int i = 0; i < 12; i++)
		{
			if (AbsortHeader[i] == "֤ȯ����")
			{
				AbsHeaderRange[0] = i;
			}
			else if (AbsortHeader[i] == "֤ȯ����")
			{
				AbsHeaderRange[1] = i;
			}
			else if (AbsortHeader[i] == "������־")
			{
				AbsHeaderRange[2] = i;
			}
			else 	if (AbsortHeader[i] == "ί�м۸�")
			{
				AbsHeaderRange[3] = i;
			}
			else if (AbsortHeader[i] == "ί������")
			{
				AbsHeaderRange[4] = i;
			}
			else if (AbsortHeader[i] == "�ɽ��۸�")
			{
				AbsHeaderRange[5] = i;
			}
			else if (AbsortHeader[i] == "�ɽ�����")
			{
				AbsHeaderRange[6] = i;
			}
			else if (AbsortHeader[i] == "״̬˵��")
			{
				AbsHeaderRange[7] = i;
			}
			else if (AbsortHeader[i] == "ί��ʱ��")
			{
				AbsHeaderRange[8] = i;
			}
		}

		for (int i = 0; i < 8; i++)
		{
			//cout << "<<<<<>>>>>"<< AbsortHeader[i] <<endl;
		}
	}
	return;
}

void Pystock::getDealTicket(int &rows, int &cols)
{
	PostMessage(h_mainTrade, WM_KEYDOWN, VK_F5, 0);
	PostMessage(h_mainTrade, WM_KEYUP, VK_F5, 0);
	Sleep(500);

	HANDLE    hProcess;
	LVITEM    *pointer;
	HWND   hListview; //hwnd,
	int headerhwnd;
	DWORD ProcessID = NULL;
	DWORD ThreadID = NULL;
	//���̽��洰�ڵľ��,ͨ��SPY��ȡ
	getDeal();
	hListview = h_Dealticket[2];
	//listview����ͷ���
	headerhwnd = SendMessageA(hListview, LVM_GETHEADER, 0, 0);
	//������:���̵�����
	rows = SendMessage(hListview, LVM_GETITEMCOUNT, 0, 0);
	//�б�����
	cols = SendMessage((HWND)headerhwnd, HDM_GETITEMCOUNT, 0, 0);
	//cout << "������" << rows << "������" << cols << endl;

	ThreadID = GetWindowThreadProcessId(hListview, &ProcessID);

	//�򿪲��������
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessID);
	//���������ڴ���
	pointer = (LVITEM*)VirtualAllocEx(hProcess, NULL, sizeof(LVITEM), MEM_COMMIT, PAGE_READWRITE);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			LVITEM vItem;
			vItem.mask = LVIF_TEXT;    //˵��pszText����Ч��
			vItem.iItem = i;        //�к�
			vItem.iSubItem = j;        //�к�
			vItem.cchTextMax = 512;    //���ܴ洢�������ı�Ϊ256�ֽ�
			LPSTR pItem = NULL;
			//�����ڴ�ռ�
			pItem = (LPSTR)VirtualAllocEx(hProcess, NULL, 512, MEM_COMMIT, PAGE_READWRITE);
			vItem.pszText = (LPWSTR)pItem;

			WriteProcessMemory(hProcess, pointer, &vItem, sizeof(LVITEM), NULL);
			::SendMessage(hListview, LVM_GETITEM, (WPARAM)i, (LPARAM)pointer);

			WCHAR  ItemBuf[512];
			memset(ItemBuf, 0, 512);
			ReadProcessMemory(hProcess, (PVOID)pItem, ItemBuf, 512, NULL);
			VirtualFreeEx(hProcess, pItem, 0, MEM_RELEASE);
			DealTicket[i][j] = UnicodeToAnsi(ItemBuf);
			//cout << "�ɽ��б�: ------" << DealTicket[i][j] << endl;
		}
	}
	//�ͷ��ڴ�ռ�
	VirtualFreeEx(hProcess, pointer, 0, MEM_RELEASE);//�������������ͷ�����������ڴ�ռ�,MEM_RELEASE��ʽ�ܳ���,��ȫ����
	CloseHandle(hProcess);//�رմ򿪵Ľ��̶���
	// 	qDebug() << QString::fromLocal8Bit("test����") << AccountTicket[1][1] << endl;

}

bool Pystock::getDealHead()
{
	//Ŀ�����ID����
	DWORD PID;
	HANDLE hProcess;
	int nBufferLength = 50; //��������С
	int nColCount = 0;        //����
	// 	cout << "1:" << h_Dealticket[3] << endl << "2:" << h_Dealticket[2] << endl;
	HWND hHeader = h_Dealticket[3];//��ñ���ͷ���
	HWND h = h_Dealticket[2];
	nColCount = (int)::SendMessage(hHeader, HDM_GETITEMCOUNT, 0, 0);      //��ȡListView����
	//cout << "��ͷ������" << nColCount << endl;
	if (nColCount <= 0)
		return false;

	//Զ������ռ��ַ
	HDITEM  *pVirtualItem;
	wchar_t *pVirtualBuffer;

	GetWindowThreadProcessId(h, &PID);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, PID);              //��ȡĿ����̾��ʧ��
	if (!hProcess)
	{
		/*AccountHeader[0] = "֤ȯ����");
		AccountHeader[1] = "֤ȯ����");
		AccountHeader[2] = "֤ȯ����");
		AccountHeader[4] = "��������");
		AccountHeader[7] = "�ɱ���");
		AccountHeader[5] = "��ǰ��");
		AccountHeader[6] = "������ֵ");
		AccountHeader[8] = "����ӯ��");
		AccountHeader[9] = "ӯ������(%)");*/
		return false;
	}

	//��Ŀ����̵�ַ�ռ�����ڴ�
	pVirtualItem = (HDITEM  *)VirtualAllocEx(hProcess, NULL, sizeof(HDITEM), MEM_COMMIT, PAGE_READWRITE);
	pVirtualBuffer = (wchar_t *)VirtualAllocEx(hProcess, NULL, nBufferLength, MEM_COMMIT, PAGE_READWRITE);
	if ((!pVirtualItem) || (!pVirtualBuffer))
	{
		//cout  << "sapdagsd" <<endl ;
		return false;
	}

	for (int j = 0; j < nColCount; j++)
	{
		wchar_t *buffer = new wchar_t[nBufferLength];
		wmemset(buffer, 0, nBufferLength);

		HDITEM hdItem;

		hdItem.mask = HDI_TEXT;
		hdItem.fmt = 0;
		hdItem.cchTextMax = nBufferLength;
		hdItem.pszText = pVirtualBuffer;
		WriteProcessMemory(hProcess, pVirtualItem, &hdItem, sizeof(HDITEM), NULL);
		::SendMessage(hHeader, HDM_GETITEM, (WPARAM)j, (LPARAM)(LPHDITEM)pVirtualItem);
		ReadProcessMemory(hProcess, pVirtualBuffer, buffer, nBufferLength, NULL);
		ReadProcessMemory(hProcess, pVirtualItem, (LPVOID)&hdItem, sizeof(HDITEM), NULL);		//û����

		DealHeader[j] = UnicodeToAnsi(buffer);
		//cout << "�ɽ��б��ͷ: ------" << endl << DealHeader[j] << endl;
		delete[]buffer;
	}
	//�ͷ�Ŀ������������ڴ�
	VirtualFreeEx(hProcess, pVirtualItem, sizeof(HDITEM), MEM_RELEASE);
	VirtualFreeEx(hProcess, pVirtualBuffer, nBufferLength, MEM_RELEASE);
	CloseHandle(hProcess);
	return true;
}

void Pystock::rangeDeal_Head()
{
	for (int i = 0; i < 12; i++)
	{
		DealHeaderRange[i] = 0;
	}
	if (getDealHead())
	{
		for (int i = 0; i < 12; i++)
		{
			if (DealHeader[i] == "֤ȯ����")
			{
				DealHeaderRange[0] = i;
			}
			else if (DealHeader[i] == "֤ȯ����")
			{
				DealHeaderRange[1] = i;
			}
			else if (DealHeader[i] == "������־")
			{
				DealHeaderRange[2] = i;
			}
			else 	if (DealHeader[i] == "�ɽ��۸�")
			{
				DealHeaderRange[3] = i;
			}
			else if (DealHeader[i] == "�ɽ�����")
			{
				DealHeaderRange[4] = i;
			}
			else if (DealHeader[i] == "�ɽ����")
			{
				DealHeaderRange[5] = i;
			}
			else if (DealHeader[i] == "�ɽ�ʱ��")
			{
				DealHeaderRange[6] = i;
			}
		}

		for (int i = 0; i < 8; i++)
		{
			cout << "<<<<<>>>>>" << DealHeader[i] << endl;
		}
	}
	return;

}

int	 Pystock::CloseTrade()
{
	if (h_mainTrade != NULL)
	{
		PostMessage(h_mainTrade, WM_CLOSE, 0, 0);

		HWND exitbtn = NULL, exitbtn2 = NULL;
		char*	temp_close = "�˳�ȷ��";

		int temp_exit = 0;
		while (exitbtn == NULL)
		{
			Sleep(200);
			exitbtn = GetWindowHd(id_login, temp_close);
			temp_exit++;
			if (exitbtn != NULL)
			{
				cout << endl << "�˳����:  " << exitbtn << endl << endl;
				int zz = PostMessage(GetDlgItem(exitbtn, 1001), BM_CLICK, 0, 0);
				cout << "�˳���ť:  " << GetDlgItem(exitbtn, 1001) << endl << endl;
				cout << "���״��ھ��: " << h_mainTrade << endl << endl;
				h_mainTrade = NULL;
				return 1;
			}
			if (temp_exit >= 5)
			{
				cout << "�ر����ʧ��" << endl << endl;
				return 0;
			}
		}
	}
	else if (h_mainLogin != NULL)
	{
		PostMessage(h_mainLogin, WM_QUIT, 0, 0);
		h_mainLogin = NULL;
		return 2;
	}
	return 0;
}

void Pystock::firsthidetrade()
{
	if (h_mainTrade == NULL)
	{
		return;
	}
	/*if (IsWindowVisible(h_mainTrade))
	{
	ShowWindow(h_mainTrade, SW_HIDE);
	}*/
	return;
}

bool Pystock::IsVisble()
{
	if (h_mainTrade == NULL)
	{
		return false;
	}
	if (IsWindowVisible(h_mainTrade))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Pystock::iBuy(char* _stockcode, char* _orderprice, char* _orderlots)
{
	getBuy();
	cout << "�����µ�>>>>" << endl;
	//cout << "��Ʊ�۸�����" << h_buybtn[2] << endl;

	//SendMessageA(buybtn[1], WM_SETFOCUS, 0, 0);
	int z = SendMessageA(h_buybtn[1], WM_SETTEXT, 0, LPARAM(_stockcode));
	//cout << z << endl;

	//SendMessageA(buybtn[2], WM_SETFOCUS, 0, 0);
	int y = SendMessageA(h_buybtn[2], WM_SETTEXT, 0, LPARAM(_orderprice));
	//cout << y << endl;

	//SendMessageA(buybtn[3], WM_SETFOCUS, 0, 0);
	int m = SendMessageA(h_buybtn[3], WM_SETTEXT, 0, LPARAM(_orderlots));
	//cout << m << endl;
	Sleep(70);
	PostMessage(h_buybtn[4], BM_CLICK, 0, 0);
	Sleep(70);
	iTradeConfirm(1);

	return 0;
}

bool Pystock::iSell(char* _stockcode, char* _orderprice, char* _orderlots)
{
	getSell();
	cout << "�����µ�>>>>" << endl;
	//cout << "��Ʊ�۸�����" << h_sellbtn[2] << endl;

	//SendMessageA(buybtn[1], WM_SETFOCUS, 0, 0);
	int z = SendMessageA(h_sellbtn[1], WM_SETTEXT, 0, LPARAM(_stockcode));
	//cout << z << endl;

	//SendMessageA(buybtn[2], WM_SETFOCUS, 0, 0);
	int y = SendMessageA(h_sellbtn[2], WM_SETTEXT, 0, LPARAM(_orderprice));
	//cout << y << endl;

	//SendMessageA(buybtn[3], WM_SETFOCUS, 0, 0);
	int m = SendMessageA(h_sellbtn[3], WM_SETTEXT, 0, LPARAM(_orderlots));
	//cout << m << endl;
	Sleep(70);
	PostMessage(h_sellbtn[4], BM_CLICK, 0, 0);
	Sleep(70);
	iTradeConfirm(2);

	return 0;
}

bool Pystock::iAbsort(char* _stockcode, bool AbsortAll)
{
	//Sleep(100);
	//SetForegroundWindow(h_mainTrade);
	PostMessage(h_mainTrade, WM_KEYDOWN, VK_F3, 0);
	PostMessage(h_mainTrade, WM_KEYUP, VK_F3, 0);
	cout << "���볷������" << endl;

	Sleep(200);
	getAbsort();
	Sleep(300);

	if (AbsortAll)					//ȫ����
	{
		PostMessage(h_Absortticket[3], BM_CLICK, 0, 0);
		Sleep(200);
		PostMessage(h_Absortticket[2], BM_CLICK, 0, 0);
	}
	else									//����ָ����
	{
		SendMessageA(h_Absortticket[1], WM_SETTEXT, 0, (LPARAM)_stockcode);
		cout << "����  " << _stockcode << "�ҵ�" << endl;
		PostMessage(h_Absortticket[2], BM_CLICK, 0, 0);
	}

	Sleep(200);
	iTradeConfirm(0);
	Sleep(900);
	PostMessage(h_Absortticket[4], BM_CLICK, 0, 0);
	return true;
}

PositionItem Pystock::iPosition()
{
	prePosition();
	//***********���ݴ���
	for (int i = 0; i < PosNum * 2; i = i + 2)
	{
		if (strstr(PosInfo[i], "�ʲ�"))
		{
			item.accountequity = PosInfo[i + 1];
		}
		else if (strstr(PosInfo[i], "����"))
		{
			item.accountfree = PosInfo[i + 1];
		}
		else if (strstr(PosInfo[i], "����ӯ��"))
		{
			item.opsitionloss = PosInfo[i + 1];
		}
		else if (strstr(PosInfo[i], "ӯ������"))
		{
			item.profitratio = PosInfo[i + 1];
		}
	}

	return item;
}

bool Pystock::iTradeConfirm(int temp_icon)
{
	h_tradeConfirm = NULL;
	int temp_tradecon = 0;
	char* temp_itradeconfirm;

	switch (temp_icon)
	{
	case 1:
		temp_itradeconfirm = appName4;
		break;
	case 2:
		temp_itradeconfirm = appName5;
		break;
	case 0:
		temp_itradeconfirm = appName2;
		break;
	default:
		temp_itradeconfirm = appName2;
		break;
	}

	while (temp_tradecon < 3)
	{
		if (temp_tradecon == 2)
		{
			h_tradeConfirm = GetWindowHd(id_login, appName2);
			PostMessage(GetDlgItem(h_tradeConfirm, 7015), BM_CLICK, 0, 0);
			cout << "���׳ɹ�" << endl << endl;
			return true;
		}

		else if (temp_tradecon == 0)
		{
			h_tradeConfirm = GetWindowHd(id_login, temp_itradeconfirm);
		}
		else if (temp_tradecon == 1)
		{
			h_tradeConfirm = GetWindowHd(id_login, appName2);
		}

		if (GetDlgItem(h_tradeConfirm, 7015) != NULL)
		{
			PostMessage(GetDlgItem(h_tradeConfirm, 7015), BM_CLICK, 0, 0);
			switch (temp_icon)
			{
			case 1:
				Sleep(100);
				break;
			case 2:
				Sleep(100);
				break;
			case 0:
				Sleep(150);
				break;
			default:
				Sleep(200);
				break;
			}
		}
		temp_tradecon++;
	}
	return true;
}

BOOL CALLBACK findWindow(HWND hWnd, LPARAM lParam);

HWND	Pystock::GetWindowHd(DWORD  target, char* target_text)
{
	WNDINFO wi;
	wi.dwProcessId = target;
	wi.hWnd = NULL;
	wi.tag_title = target_text;
	::EnumWindows(&findWindow, (LPARAM)&wi);
	return wi.hWnd;
}

BOOL CALLBACK findWindow(HWND hWnd, LPARAM lParam)
{
	char class_name[80];
	char title[50];
	DWORD  processId;
	::GetWindowThreadProcessId(hWnd, &processId);
	::GetClassNameA(hWnd, class_name, sizeof(class_name));
	::GetWindowTextA(hWnd, title, 50);
	LPWNDINFO pinfo = (LPWNDINFO)lParam;

	/*	cout << "processId:  " << processId << endl
	<< "class_name:  " << class_name
	<< endl << hWnd
	<< endl << title
	<< endl << endl;*/

	if (processId == pinfo->dwProcessId && strstr(title, pinfo->tag_title))
	{
		//cout << "Get the Process Id is :" << processId << endl;
		pinfo->hWnd = ::GetParent(hWnd);
		pinfo->hWnd = hWnd;
		//cout << "main window: " << pinfo->hWnd << endl;
		return false;
	}
	return true;
}

BOOL CALLBACK findAdWindow(HWND hWnd, LPARAM lParam);

HWND	Pystock::GetAdWindowHd(DWORD  target, char* target_text)
{
	WNDINFO wi;
	wi.dwProcessId = target;
	wi.hWnd = NULL;
	wi.tag_title = target_text;
	::EnumWindows(&findAdWindow, (LPARAM)&wi);
	return wi.hWnd;
}

BOOL CALLBACK findAdWindow(HWND hWnd, LPARAM lParam)
{
	char class_name[80];
	char title[50];
	DWORD  processId;
	::GetWindowThreadProcessId(hWnd, &processId);
	::GetClassNameA(hWnd, class_name, sizeof(class_name));
	::GetWindowTextA(hWnd, title, 50);
	LPWNDINFO pinfo = (LPWNDINFO)lParam;

	/*	cout << "processId:  " << processId << endl
	<< "class_name:  " << class_name
	<< endl << hWnd
	<< endl << title
	<< endl << endl;*/

	if (processId == pinfo->dwProcessId && strstr(class_name, pinfo->tag_title) && IsWindowVisible(hWnd))
	{
		//cout << "Get the Process Id is :" << processId << endl;
		pinfo->hWnd = ::GetParent(hWnd);
		pinfo->hWnd = hWnd;
		//cout << "ad window: " << pinfo->hWnd << endl;
		return false;
	}
	return true;
}

char *strtok_r(char *s, const char *delim, char **save_ptr) {
	char *token;

	if (s == NULL) s = *save_ptr;

	/* Scan leading delimiters.  */
	s += strspn(s, delim);
	if (*s == '\0')
		return NULL;

	/* Find the end of the token.  */
	token = s;
	s = strpbrk(token, delim);
	if (s == NULL)
		/* This token finishes the string.  */
		*save_ptr = strchr(token, '\0');
	else {
		/* Terminate the token and make *SAVE_PTR point past it.  */
		*s = '\0';
		*save_ptr = s + 1;
	}

	return token;
}

char* UnicodeToAnsi(const wchar_t* szStr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
	return pResult;
}