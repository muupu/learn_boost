
#include <iostream>  
#include <boost/asio.hpp>  
#include <boost/date_time/posix_time/posix_time.hpp>  
#include <boost/function.hpp>   
#include <boost/bind.hpp>  

//ͬ����ʱ��
//���԰�����thread���sleep()�����Ա��о�һ��
//������Ȼ���ǵȴ������ڲ�������ȫ��ͬ��thread���sleep()ʹ���˻��������������������߳��еȴ���
//��asio���ǵ����˲���ϵͳ���첽���ƣ���select��epool����ɡ�  
//ͬ����ʱ�����÷��ܼ򵥣�������ʾ��asio����Ļ����ṹ�����̣�
//һ��asio��������Ҫ����һ��io_service��������ǰ����ģʽ�����ص�proactor��ɫ��
//Ȼ����������һ��IO����(�����Ƕ�ʱ��),�������ҽ���io_service�ϣ���Ȼ��Ϳ���ִ�к����� ͬ�����첽������  
void timer_test1()
{
	boost::asio::io_service ios; //���е�asio�������Ҫ��һ��io_service����  

	boost::asio::deadline_timer t(ios, boost::posix_time::seconds(5)); //5���Ӻ�ʱ����ֹ  
	std::cout << t.expires_at() << std::endl;//�鿴��ֹ�ľ���ʱ��  
	t.wait();//����wait()ͬ���ȴ�

	std::cout << "Hello, world!\n";
}

void myprint(boost::system::error_code ec)
{
	std::cout << "hello asio" << std::endl;
}

//�첽��ʱ��
//���������ͬ����ʱ����ͬ�������˻ص���������ʹ��io_service_run()�Ͷ�ʱ����async_wait()
//��������Ҫ����ص�������asio��Ҫ��ص�����ֻ����һ�������������������������const asio::error_code&����
void timer_test2()
{
	boost::asio::io_service ios;
	boost::asio::deadline_timer t(ios, boost::posix_time::seconds(3));

	//�첽�ȴ�������ص���������������  
	//��֪ͨio_service�첽��ִ��IO����������ע���˻ص�������������IO�������ʱ���¼���·���������ɷ���ֵ(error_code)���á�  
	t.async_wait(myprint);
	std::cout << "it show before t expired." << std::endl;

	//������Ǳ������io_service��run()��Ա������������ǰ�������¼�����ѭ���������ȴ����еĲ�����ɲ������¼���
	//���������run()��ô��Ȼ���첽ִ���ˣ���û��һ���ȴ�����ɵĻ��ƣ��ص��������ò���ִ�еĻ��ᡣ 
	ios.run();

	//����hello asioһ�������˵��run()����������  
	std::cout << "runned" << std::endl;
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

void print1(int index)
{
	std::cout << "hello asio:" << index << std::endl;
}

void print2()
{
	std::cout << "hello boost" << std::endl;
}

void timer_test3()
{
	boost::asio::io_service ios;
	a_timer a(ios, 10, boost::bind(print1, 2));//���õ�һ����ʱ��  
	a_timer at(ios, 5, print2);//���õڶ�����ʱ��  
	ios.run();//ios�ȴ��첽���ý���  
}

//int main()  
//{  
//	//timer_test1();
//	//timer_test2();
//	timer_test3();
//	return 0;  
//}  