#include <iostream>  //���������� ���������� iostream
using namespace std;  //���������� ������������ ����
void main()  //������� �������
{
	float cg;  //��������� ���������� cg � f
	setlocale(LC_ALL, "Russian");  //���������� ������� ����
	cout << "������� ����� ��������:"<<endl;  //������ ������������ ������ ����� ��������
	cin >> cg;  //������������ ������ ����� ��������
	cout << cg<<" " << "��������" <<" " << "=" <<" " << cg * 7.481<<" " << "���������� �����";  //��������� ��������� ����� ��������, �������� �������������, � ���������� ���� � ������� ���������
}