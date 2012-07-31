#include "blackboard.h"
#include <iostream>

class Data
{
public:
	Data(int i):m_i(i)
	{
		std::cout << "construct, i:" << m_i << std::endl;
	}
	Data(Data& data)
	{
		m_i = data.m_i;
		std::cout << "copy construct, i:" << m_i << std::endl;
	}
	~Data()
	{
		std::cout << "destruct, i:" << m_i << std::endl;
	}
	int Get(){return m_i;}
	int m_i;
};

Board g_board;

int main()
{
	/*
	 *	����new��BoardVar�����ݶ�����Ҫdelete����
	 *  board���ٵ�ʱ����Զ�����
	 */
	g_board.Add("test", new BoardVar(new Data(1)));
	g_board.Set("test", new BoardVar(new Data(2)));
	BoardVar* var = g_board.Get("test");
	if(var)
		std::cout << "test:" << var->Get<Data>()->Get() << std::endl;

	g_board.Del("test");
	var = g_board.Get("test");
	if(var)
		std::cout << "test:" << var->Get<Data>()->Get() << std::endl;

	g_board.Add("test", new BoardVar(new int(1)));
	g_board.Set("test", new BoardVar(new int(2)));
	var = g_board.Get("test");
	if(var)
		std::cout << "test:" << *(var->Get<int>()) << std::endl;

	g_board.Del("test");
	var = g_board.Get("test");
	if(var)
		std::cout << "test:" << *(var->Get<int>()) << std::endl;

	return 0;
}