// asio����⣺ͬ��socket��Client��

#include <conio.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/function.hpp> 
#include <boost/bind.hpp>
//using namespace std;
//using namespace boost;
//using namespace boost::asio;

//IP��ַ�Ͷ˵�      
//IP��ַ������TCP��UDP��ͨ��Э�飬asio��ʹ����ip::address����ʾIP��ַ������ͬʱ֧��ipv4��ipv6���ֵ�ַ��
void test1()
{
	boost::asio::ip::address addr = addr.from_string("127.0.0.1");
	assert(addr.is_v4());
	std::cout << addr.to_string() << std::endl;
	addr = addr.from_string("ab::12:34:56");
	assert(addr.is_v6());
	//����ip��ַ���ټ���ͨ���õĶ˿ھ͹�����һ��socket�˵㣬��asio������ip::tcp::endpoint������ʾ��
	boost::asio::ip::address addr1 = addr1.from_string("127.0.0.1");
	boost::asio::ip::tcp::endpoint ep(addr1, 6688);//����һ���˵���󣬶˿�Ϊ6688
	assert(ep.address() == addr1);
	assert(ep.port() == 6688);
}

//���Զ�ʱִ�����⺯���Ķ�ʱ�� a_timer(asyc timer)��
//������һ��asio��ʱ�������һ��������������һ��function������������ص�����
class a_timer
{
private:
	int count, cout_max; //��������Ա����  
	boost::function<void()> f; //function���󣬳����޲����޷��صĿɵ�����  
	boost::asio::deadline_timer t;//asio��ʱ��  
public:
	//���캯����ʼ����Ա���������������������ü����������ޣ������洢�ص�������������������ʱ��  
	//֮����Ҫ"����"����������Ϊ���Ǳ����װ��io_service.run()֮ǰ������һ���첽������ִ�У�
	//����io_service.run()����Ϊû���¼�������������ȴ����ء�  
	template<typename F>a_timer(boost::asio::io_service& ios, int x, F func) :f(func), cout_max(x), count(0), t(ios, boost::posix_time::microsec(500))//ģ�����ͣ��ɽ�������ɵ�����  
	{
		//�����ռ���asio::placeholders��һ��ռλ��error����������������bind���ռλ��_1,_2,���ڴ���errror_codeֵ��
		t.async_wait(boost::bind(&a_timer::call_func, this, boost::asio::placeholders::error));
	}

	//call_func()�ڲ��ۼӼ����������������δ�ﵽ���������function����f
	//Ȼ���������ö�ʱ������ֹʱ�䣬�ٴ��첽�ȴ������ã��Ӷ��ﵽ����ִ�е�Ŀ�ġ�  
	void call_func(const boost::system::error_code &)
	{
		if (count >= cout_max)
		{
			return;
		}
		++count;
		f();
		t.expires_at(t.expires_at() + boost::posix_time::millisec(500));//���ö�ʱ������ֹʱ��Ϊ0.5��֮��  
		t.async_wait(boost::bind(&a_timer::call_func, this, boost::asio::placeholders::error));
	}
};

void client(boost::asio::io_service& ios)
{
	try
	{
		std::cout << "client start." << std::endl;
		boost::asio::ip::tcp::socket sock(ios);
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 6688);//�������Ӷ˵�
		sock.connect(ep);//socket���ӵ��˵�
		std::vector<char> str(100, 0);//����һ��vector������
		sock.read_some(boost::asio::buffer(str));//ʹ��buffer()��װ��������������
		std::cout << "recive from: ip:" << sock.remote_endpoint().address() << " port:" << sock.remote_endpoint().port() << std::endl;
		std::cout << "recv:" << &str[0] << std::endl;//������յ�������
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}
//ͬ���ͻ���
void test2()
{
	boost::asio::io_service ios;
	a_timer at(ios, 5, boost::bind(client, boost::ref(ios)));
	ios.run();
}
void test3()
{
}
void test(char t)
{
	std::cout << "press key=====" << t << std::endl;
	switch (t)
	{
	case '1':test1(); break;
	case '2':test2(); break;
	case '3':test3(); break;
	case 27:
	case 'q':exit(0); break;
	default: std::cout << "default " << t << std::endl; break;
	}
}
void main()
{
	while (1)
		test(getch());
}