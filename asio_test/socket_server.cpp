// asio����⣺ͬ��socket��server��

#include <conio.h>
#include <iostream>
#include <boost/asio.hpp>
//using namespace std;
//using namespace boost;
//using namespace boost::asio;

//server��
//����һ��acceptor������6688�˿ڽ������ӣ���������ʱʹ��socket������һ���ַ���
void socket_server()
{
	try
	{
		boost::asio::io_service ios;

		//acceptor����ڵ�socket API��accept()�������ܣ������ڷ������ˣ���ָ���Ķ˿ںŽ������ӣ��������socket��������ͨ�š�
		boost::asio::ip::tcp::acceptor acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6688));
		std::cout << acceptor.local_endpoint().address() << std::endl;
		while (true)
		{
			boost::asio::ip::tcp::socket sock(ios);
			acceptor.accept(sock);//�����ȴ�socket����
			std::cout << "client:ip:" << sock.remote_endpoint().address() << "   port:" << sock.remote_endpoint().port() << std::endl;
			
			//�������˳�����Ҫע��������ɺ���buffer(),�����ð�װ�ܶ������������Ϊasio������õĻ��������͡�
			//ͨ�����ǲ���ֱ�Ӱ����飬vector����������asio�Ķ�д����������ʹ��buffer()������װ��
			sock.write_some(boost::asio::buffer("hello asio"));
			
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

//void main()
//{
//	socket_server();
//}