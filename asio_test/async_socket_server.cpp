// asio����⣺�첽socket��server��

#include <conio.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
//using namespace std;
//using namespace boost;
//using namespace boost::asio;

void async_test1() {}

//�첽server
//�첽����Ĵ���������ͬ�����������ͬ��
//ֻ��Ҫ��ԭ�е�ͬ�����ú���������ǰ׺��async_���첽���ú�����
//�����ӻص��������ڻص�������������һ���첽���á�
class server
{
private:
	boost::asio::io_service& ios;
	boost::asio::ip::tcp::acceptor acceptor;
	typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_pt;
public:

	// acceptor��ָ���Ķ˿ںŽ�������
	server(boost::asio::io_service& io) :
		ios(io), 
		acceptor(ios, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6688))
	{
		start();
	}

	//start()�������������첽�������ӣ���Ҫ����acceptor��async_accept()������
	//Ϊ���ܹ���socket�ܹ����첽���ú���ʹ�ã����Ǳ���ʹ��shared_ptr������socket���������ָ�룬
	//�������ٳ�����������������д��ڣ�ֱ��û����ʹ����Ϊֹ��
	void start()
	{
		sock_pt sock(new boost::asio::ip::tcp::socket(ios));//����ָ��
		//�����첽��������
		acceptor.async_accept(*sock, bind(&server::accept_handler, this, boost::asio::placeholders::error, sock));
	}

	//����TCP���ӷ���ʱ��server::accept_handler()�����������ã���ʹ��socket���������ݡ�
	void accept_handler(const boost::system::error_code& ec, sock_pt sock)
	{
		if (ec)//��������
		{
			return;
		}
		//������ӵĿͻ�����Ϣ
		std::cout << "client:";
		std::cout << sock->remote_endpoint().address() << " port:" << sock->remote_endpoint().port() << std::endl;
		
		sock->async_write_some(boost::asio::buffer("hello asio"), bind(&server::write_handler, this, boost::asio::placeholders::error));
		
		//�ٴ������첽��������
		//������������asio���ݵ�error_code,��֤û�д�������
		//Ȼ�����socket�����async_write_some()�첽�������ݡ�
		//ͬ�������Ǳ�����Ϊ����첽���ñ�д�ص�����write_handler()��
		//�����������ݺ�Ҫ���˵���start()�ٴ������������������ӣ�����������ݷ��ͺ�io_service����Ϊû��ʱ�䴦����������С�
		start();
				
	}

	void write_handler(const boost::system::error_code&)
	{
		std::cout << "send msg complete." << std::endl;
	}
};
void async_test2()
{
	try
	{
		std::cout << "server start." << std::endl;
		boost::asio::io_service ios;
		server serv(ios);
		ios.run();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void async_test(char t)
{
	std::cout << "press key=====" << t << std::endl;
	switch (t)
	{
	case '1':async_test1(); break;
	case '2':async_test2(); break;
	case 27:
	case 'q':exit(0); break;
	default: std::cout << "default " << t << std::endl; break;
	}
}

void main()
{
	async_test2();
}