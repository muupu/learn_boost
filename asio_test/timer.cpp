
#include <iostream>  
#include <boost/asio.hpp>  
#include <boost/date_time/posix_time/posix_time.hpp>  

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
  
int main()  
{  
	//timer_test1();
	timer_test2();
	return 0;  
}  