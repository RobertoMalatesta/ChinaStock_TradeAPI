#pragma once
#define DLL_API   __declspec(dllexport)
/*#include "stdafx.h"*/
#include <windows.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <commctrl.h>
#define GP 50                               //����Ʊ������

using namespace std;

typedef struct tagWNDINFO
{
	DWORD dwProcessId;
	HWND hWnd;
	char*    tag_title;
} WNDINFO, *LPWNDINFO;

struct   PositionItem
{
	char*  accountequity;                   //��ֵ
	char*  accountfree;                     //�����ʽ�
	char*  marketvalue;                     //��ֵ
	char*  opsitionloss;                    //ӯ��
	char*  opsitionrange;                //��λ
	char*  profitratio;                     //ӯ������
};

class  DLL_API  Pystock
{
public:
	Pystock();
	~Pystock();
	//****�ӿں���/* 1.�������  2. ��¼����   3. �ر����*/
	int			OpenTrade(char *filename);
	HWND	GetWindowHd(DWORD  target, char* target_text);
	HWND	GetAdWindowHd(DWORD  target, char* target_text);
	int			LoginTrade(char *accName, char * secret, char * conSecret);
	int			CloseAdWindow();				//�رչ�洰��
	int			CloseTrade();
	int			HideTrade();							//������غ���
	int			LoginFailed();						//��¼ʧ�ܴ�����
	void		firsthidetrade();
	bool		IsVisble();								//���ش���״̬

	//****Ԥ������
	int			PreHandle();							//����Ԥ������
	//***********��ȡh_toolBar���������ֲֳ����ɽ�ˢ�¾��
	void		getToolBar();							//��ȡtoolbar�ľ��
	void		getBtnBar();							//��ȡ�µ���ť�ľ��
	void		getBuy();								//��ȡ���밴ť�ľ��
	void		getSell();								//��ȡ������ť�ľ��
	void		getAbsort();							//��ȡ������ť�ľ��
	void		getDeal();								//��ȡ�ɽ��б�ľ��

	//****
	void		getPosition();						//��ȡ�ֲֽ���ľ��
	void		prePosition();
	//****��ȡ�ֲֽ����ʵ�ֺ���
	void		getAccountTicket(int &rows, int &cols);
	bool		getAccoutHead();
	void		rangeAccount_Head();

	//****��ȡ���������ʵ�ֺ���
	void		getAbsortTicket(int &rows, int &cols);
	bool		getAbsortHead();
	void		rangeAbsort_Head();

	//****��ȡ�ɽ������ʵ�ֺ���
	void		getDealTicket(int &rows, int &cols);
	bool		getDealHead();
	void		rangeDeal_Head();

	//****���̺�
	DWORD		id_login;
	//****�µ�����
	bool        iBuy(char* _stockcode,
		char* _orderprice,
		char* _orderlots);     //�����Ʊ
	bool        iSell(char* _stockcode,
		char* _orderprice,
		char* _orderlots);     //������Ʊ
	bool        iAbsort(char* _stockcode, bool AbsortAll);  //ȡ������
	PositionItem		iPosition();
	bool		iTradeConfirm(int temp_icon);

private:                                    //----���Ա����
	//****�����������¼����֤


private:                                    //----���Ա
	char* appName1 = "ͨ�������Ͻ���V6.01";
	char* appName2 = "��ʾ";
	char* appName3 = "ͨ�������Ͻ���V6V5.78";
	char* appName4 = "���뽻��ȷ��";
	char* appName5 = "��������ȷ��";
	//****�˻���Ϣ
	char*       accountName;                //�˻�����
	char*       ibName;                     //ȯ������
	char*       tradeSecret;                //��������
	char*       connetSecret;               //ͨѶ����
	//****���
	HWND        h_mainLogin;                //��¼����
	HWND        h_mainTrade;                //���״���
	HWND		h_loginfailed;				//��¼ʧ�ܴ���
	HWND        h_adWindow;                //��洰��
	HWND        h_tradeBar;                    //�µ��������ľ��(����|����|����|�ֲ�)
	HWND        h_toolBar[8];                  //��ȡ�µ����������ϲ�����h_toolBar���ϲ㣩
	HWND        h_buybtn[30];               //�����µ��ľ�����飨���룬�۸��������µ���ť��
	HWND        h_sellbtn[6];                  //�����µ��ľ�����飨�������۸��������µ���ť��
	HWND        h_Hposition[10];          //�ֲ�״̬�µľ��
	HWND        h_Absortticket[10];     //����״̬�ľ��
	HWND		h_Dealticket[10];			//�ɽ��б�ľ��
	HWND		h_tradeConfirm;			//�ɽ�ȷ��
	//****��
	PositionItem item;

	//****�����������
	bool		isopened = false;				//����Ƿ��
	bool		islogined = false;				//����Ƿ��¼

	char*      appFile;								 //�������Ŀ¼
	char        loginTitle[50];					 //��¼������
	char        failTitle[50];						 //��¼ʧ��ʱ��������
	char        tradeTitle[50];					 //���״�����
	char        positionTitle[200];			 //�˻����

	char*      AccountTicket[GP][25];        //�ֲ��б�
	char*      AccountHeader[25];             //�ֲ��б��ͷ
	int           AccHeaderRange[25];         //��ͷ˳��
	int           TicketNumber;						//�ֲ�����

	char*      AbsortTicket[GP][25];			//�����б�
	char*		AbsortHeader[25];				//�����б��ͷ
	int			AbsHeaderRange[25];		//��ͷ˳��

	char*      DealTicket[GP][25];			//�ɽ��б�
	char*		DealHeader[25];
	int			DealHeaderRange[25];

	int			PosNum;						    //�ֲ���Ϣ����
	char*		PosInfo[20];						//�ֲ���ϸ��Ϣ
	//****���ײ���
	const char* stockCode;                  //���׹�Ʊ����
	const char* orderPrice;                 //���׹�Ʊ�۸�
	const char* orderLots;                  //���׹�Ʊ����
};
