#include "stdafx.h"
//#include "Control.h"
#include "Search.h"
#include "MFC21_Text_SearchExDlg.h"

// ������
CSearch::CSearch()
{
}

// �Ҹ���
CSearch::~CSearch()
{
}

char* CSearch::TextFind(char* _pszBuffer, int _iBufferSize, char* _pszText, int _iTextSize)
{

	// ���� �Ҵ��� �޸� �ּ�
	char* pszFirstBufferAddress = NULL;
	pszFirstBufferAddress = _pszBuffer;

	char* pszWordAddress = NULL;
	pszWordAddress = _pszText;

	// �ܾ� üũ
	bool bCheckWord = FALSE;

	// ��
	// �ؽ�Ʈ �ܾ�
	for (int i = 0; i < (_iBufferSize - _iTextSize); i++)
	{
		bCheckWord = WordCompare(pszFirstBufferAddress, _pszText, _iTextSize);	
		
		if (TRUE == bCheckWord)
		{
			return pszWordAddress;
		}
		else
		{
			pszFirstBufferAddress++;
		}
	}
	return NULL;
}

bool CSearch::WordCompare(char* _pszText, char* _pszWord, int _iWordSize)
{	
	for (int i = 0; i < _iWordSize; i++)
	{	
		// ������ �˻� �ܾ��� ���� �ε���
		if (*_pszWord == *_pszText)
		{
			_pszWord++;
			_pszText++;
		}
		// �ƴϸ� �˻� �ܾ� ó�� �ε���
		else
		{
			return FALSE;
		}	
	}
	return TRUE;
}




















//// �迭 �� �ֱ�
//void CSearch::InputValue(int* _piBuffer, int _iCount, int _iValue)
//{
//	int* piNum = NULL;
//	piNum = _piBuffer;
//
//	for (int i = 0; i < _iCount; i++)
//	{
//		*piNum = _iValue;
//		piNum++;
//	}
//}
//
//// ��������
//void CSearch::ValueAsc(int* _piBuffer, int _iCount)
//{
//	int* piNum = NULL;
//	piNum = _piBuffer;
//
//	for (int i = 0; i < _iCount; i++)
//	{
//		*piNum = i;
//		piNum++;
//	}
//}
//
//// 3�� ����� ���� ��
//int CSearch::SumResult(int* _piBuffer, int _iCount)
//{
//	int sum = 0;
//	int* piNum = NULL;
//	piNum = _piBuffer;
//
//	for (int i = 0; i < _iCount; i++)
//	{
//		if (*piNum % 3 == 0)
//			sum += i;
//		
//		piNum++;
//	}
//	return sum;
//}
//
//void CSearch::GetValue(int* _piBuffer, int _iCount, St_Test* _pstResult)
//{
//	int* piNum = NULL;
//	piNum = _piBuffer;
//	
//	for (int i = 0; i < _iCount; i++)
//	{
//		switch (i)
//		{
//		case 0:
//			_pstResult->test0 = *piNum;
//			break;
//
//		case 100:
//			_pstResult->test100 = *piNum;
//			break;
//
//		case 200:
//			_pstResult->test200 = *piNum;
//			break;
//
//		case 300:
//			_pstResult->test300 = *piNum;
//			break;
//
//		default:
//			break;
//		}
//		piNum++;
//	}
//
//}

//BOOL CSearch::IsStrFind(char* A_PtrFindStr, int A_FindStrSize, char* A_PtrTextStr, int A_TextSize)
//{
//#if 0
//	char* Tv_PtrTxt;
//
//	Tv_PtrTxt = A_PtrTextStr;
//	for (int i = 0; i < A_TextSize; i++)
//	{
//		*A_PtrTextStr = 10;
//
//		//next
//		A_PtrTextStr++;
//	}
//
//	Tv_PtrTxt = A_PtrTextStr;
//
//	Tv_PtrTxt = (char*)((unsigned int)A_PtrTextStr + 200);
//	for (int i = 0; i < 5; i++)
//	{
//		*Tv_PtrTxt = 10;
//		Tv_PtrTxt++;
//	}
//#endif
//		/*
//
//		Search_Word		= A_PtrFindStr;
//		Search_WordCnt	= A_FindStrSize;
//		Text			= A_PtrTextStr;
//		TextCnt			= A_TextSize;
//		
//		for (TextCnt = 0; Search_WordCnt < TextCnt; Search_WordCnt++)
//		{
//			IsStrMatch(Search_Word);
//
//
//			next
//			Search_WordCnt++;
//		}
//		
//		
//		*/
//	return true;
//}

//BOOL CSearch::IsStrMatch()
//{
//	/*
//	
//	if (Tv_PtrFindStr == *Tv_PtrTextStr)
//	{
//
//	}
//	
//	*/
//	return FALSE;
//}