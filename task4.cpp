#include <cmath> 
#include <string>
#include <locale>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
using namespace std;

/*
	������ �������� � ��������� ������ ������ �� ������������� ������������������ �������� 
	�� ����������� '*' � ������������ ������ ���� ������������������ � ������ ������. ����
	������������� ������������������ �������� ��������� ������� ����� �������� ������ ������, 
	����� ������� ����������� ����� �� ������������ �����. � ��������� ������ ����� �������
	����������� ����� �� �� ��������������.
	
	�������������� ����������� ������� ������� ����������� ����� �� ������ ������� �������;
	��� ������������� ������������ ������ ����������� �������� ��� ������ ������.

	� ������, ����� �� ������ ������ ����������� �����������, ���������� ��������
	������ ����������� �������, ����� ������� ����������� ����� �� �� ������������
	���� ����������� ����� �� �� ��������������.

	� ��������� �������, ����� ������ ������ ���������� �� � ����������� �/���
	������������� �� �� �����������, ���������� �������� ��������� ����� �/��� ������
	���� �����, ����� ������� ����������� ����� � �������������� ����� (�� ����� ������������ 
	��� ������������ ������ �� ������������ �����). ���� � ���� ������� ������ �/��� �����
	���������, �� ��� ����������� �� ����������� ��������� ������ ��������.
		
	���������� ������, ����� ���� ��� ��� ������ �����.
*/

template <class Container>
//��������� ��������� ������ ������� ������
void SplitString(const std::string& str, Container& cont, const std::string& separators ="*")
{
	std::size_t current, previous = 0;
	current=str.find_first_of(separators);
	while(current!=std::string::npos)
	{
		if((str.substr(previous, current - previous)!="")&&(str.substr(previous, current - previous)!="\n")) cont.push_back(str.substr(previous, current - previous));
		previous=current+1;
		current = str.find_first_of(separators,previous);
	}
	if((str.substr(previous, current - previous)!="")&&(str.substr(previous, current - previous)!="\n")) cont.push_back(str.substr(previous, current - previous));
}

int main(int argc, char *argv[])
{
	setlocale(LC_ALL,"");
	if (argc==3)
	{
		string str1=argv[1], str2=argv[2]; //������� ������

		bool anystart=true; //��������������� ����: ������ ������ ���������� � �����������
		bool anyend=true; //��������������� ����: ������ ������ ������������ ������������
		bool resolved=false; //��������������� ���� ��� ��������� ����� ������ ��������
		
		if (str2[0]!='*') anystart=false;
		if (str2[str2.length()-1]!='*') anyend=false;
		
		int strstart=0, strend=str1.length()-1;//������ ������� � ���������� ��������� ����������� ����� ������ ������

		std::vector<std::string>words;//��������� ��������� ������ ������
		SplitString(str2,words);

		//����������� ������: ���� ��� ��� ������� ������ ������
		
		//������ ������ str1 ����� ���� ��������� ���� ������ ������ ���� ������, ��������� �� ����� ������������ 
		if (str1==""){
			if(words.size()==0)
			{
				cout<<"OK"<<endl;
			}
			else
			{
				cout<<"KO"<<endl;
			}
			return 0;
		}

		//���� ������ ������ �� ������, � ������ - ������, ������ �� ���������
		if (str2=="")
		{
			cout<<"KO"<<endl;;
			return 0;
		}

		//����������� ������: �� ������ ������ ��� ������������
		if((words.size()==1)&&!anystart&&!anyend)
		{
			if(str1==str2)
			{
				cout << "OK" << endl;
			}
			else
			{
				cout << "KO" << endl;
			}
			return 0;
		}

		//��������� ������: ������������� ������ ������ ������
		if(!anystart)
		{
			for(unsigned int i=0;i<words[0].size();++i)
			{
				if(words[0][i]!=str1[i])
				{
					cout<<"KO"<<endl;
					return 0;
				}
			}
			strstart+=words[0].size();
		}

		//��������� ������: ������������� ����� ������ ������
		if(!anyend){
			for(unsigned int i=0;i<words[words.size()-1].size();++i)
			{
				if(words[words.size()-1][i]!=str1[strend+i-words[words.size()-1].size()+1])
				{
					cout<<"KO"<<endl;
					return 0;
				}		
			}
			strend-=words[words.size()-1].size();
		}

		//����� ��� ������ ������
		int substr_number=0; //������� ����� ���������, ��� ������� �������������� �����
		int substr_number_cap=words.size()-1; //����� ��������� ���������, ��� ������� ������ �������������� �����
		
		//������ � ��������� ��������� ��� ����� ���� ������ � ��������� �������
		if (!anystart) substr_number++;
		if (!anyend) substr_number_cap--;
		
		//���� �� ��������� ����� ������ �������� ��� ��������� ��� �������, ������ ����������� ����� �� ������������ �����
		if(substr_number>substr_number_cap)
		{
			cout<<"OK"<<endl;
			return 0;
		}

		//�������� ���� ������
		for(unsigned int i=strstart;i<=strend;++i)
		{
			if(str1[i]==words[substr_number][0])
			{
				resolved=true;
				for(unsigned int j=1;j<words[substr_number].size();++j)
				{
					if((i+j)>strend)
					{
						resolved=false;
						break;
					}
					if(str1[i+j]!=words[substr_number][j])
					{
						resolved=false;
					}
				}
				if(resolved){
					i+=words[substr_number].size()-1;
					substr_number++;
				}
			}		
			if(substr_number>substr_number_cap){
				resolved=true;
				break;
			}
			resolved=false;
		}

		//������� �� ��� ��������� � ����� ������?
		if(resolved)
		{
			cout<<"OK"<<endl;
		}
		else
		{
			cout<<"KO"<<endl;
		}
		return 0; 
	}
	else
	{
		cout<<"Usage:\ntask4 string1 string2\n";
		return 1;
	}
	return 2;
}