
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
  
int main()  
{  
	timer_test1();
  
	return 0;  
}  