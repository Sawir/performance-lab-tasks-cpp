#include <cmath> 
#include <string>
#include <algorithm>
#include <iterator>
#include <vector>
#include <locale>
#include <fstream>
#include <iostream>
using namespace std;

/*
	�������� �������� � �������������, ��� ������� ���-����
	������������ � ������� ����������� ���� � �������.
*/

template <class Container>
//��������� ��������� ������ ������� ������
void SplitString(const std::string& str, Container& cont, const std::string& separators =" -|:.")
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

//��� ������� ���������� ��� ���� � ������ ����� � ���, ����� �� ������ ����, ��� ������
bool date_comp(unsigned int date1[6], unsigned int date2[6])
{
	if (date1[0]!=date2[0]) return (date1[0]>date2[0]); //��������� �� �����
	if (date1[1]!=date2[1]) return (date1[1]>date2[1]); //�� �������
	if (date1[2]!=date2[2]) return (date1[2]>date2[2]); //�� ����
	if (date1[3]!=date2[3]) return (date1[3]>date2[3]); //�� �����
	if (date1[4]!=date2[4]) return (date1[4]>date2[4]); //�� �������
	if (date1[5]!=date2[5]) return (date1[5]>date2[5]); //�� ��������
	return false; //� ������ ������� ���������� ��� ���������� ����
}

int main(int argc, char *argv[]) 
{ 
	if (argc==5)
	{
	unsigned int Volume,MaxVolume,cVolume; //������� � ������������ ����� �����, � ����� ��������������� �����
	unsigned int startDate[6], endDate[6], cDate[6]; //������ � ����� �������� ���������� �������, � ����� ��������������� ����
	
	//���������� ����������
	unsigned int add_tries=0,add_failed_tries=0,add_volume=0,add_failed_volume=0;
	double add_fail_percentage;
	unsigned int scoop_tries=0,scoop_failed_tries=0,scoop_volume=0,scoop_failed_volume=0;
	double scoop_fail_percentage;
	unsigned int startVolume=0,endVolume=0;

	bool addscoop, cstate, reached=false;//��������������� �����

	string strDate1=argv[3], strDate2=argv[4]; //������� ������ � ������
	std::vector<std::string>words;//��������� �������� �������� ������� �����
	
	//��������� ������� ���
	SplitString(strDate1,words);
	
	for(int i=0;i<6;++i)
	{
		startDate[i]=stoi(words[i]);
	}
	words.clear();
	
	SplitString(strDate2,words);
	for(int i=0;i<6;++i)
	{
		endDate[i]=stoi(words[i]);
	}
	words.clear();

	if(date_comp(startDate,endDate))
	{
		setlocale(LC_ALL,"");
		cout<<"�������� ����������� ���."<<endl;
		return 1;
	}

	string infilePath=argv[1]; //���� � �������� �����
	ifstream infile(infilePath); //���� ������� ������
	
	string outfilePath=argv[2]; //���� � ��������� �����
	ofstream outfile; //���� �������� ������

	if(!infile){
			setlocale(LC_ALL,"");
			cout<<"������� ���� �� ������."<<endl;
			return 1;
	}

	string strInput; //������ ������� ������	
	
	//���������� ������������� � �������� ������
	getline(infile, strInput);
	getline(infile, strInput);
	MaxVolume=stoi(strInput);
	getline(infile, strInput);
	Volume=stoi(strInput);

	while(!infile.eof()){
		getline(infile, strInput);
		SplitString(strInput,words);	

		for(int i=0;i<6;++i){
			cDate[i]=stoi(words[i]);
		}

		//���� ����� �� ������� ��������� �������, �������� �������� ����������
		if(!reached){
			if(date_comp(cDate,startDate)){
				startVolume=Volume;
				reached=true;
			}
		}

		//���� ����� �� ���� ����� ��������� �������, ���������� ����������
		if(date_comp(cDate,endDate)){
			break;
		}

		if(words[9]=="add")
		{
			addscoop=true;
		}
		else if(words[9]=="scoop")
		{
			addscoop=false;
		}
		
		cVolume=stoi(words[10]);
		
		if(words[11]=="success")
		{
			cstate=true;
		}
		else if(words[11]=="failure")
		{
			cstate=false;
		}

		//���� ��������� ������ ���� � ����� ������� � ����� ������
		if(cstate){
			if(addscoop)
			{
				Volume+=cVolume;
			}
			else
			{
				Volume-=cVolume;
			}
		}
		
		//���������� ����������, ������ ���� ����� �� ���������� �������
		if(reached){
			if(cstate){
				if(addscoop)
				{
					add_tries++;
					add_volume+=cVolume;
				}
				else
				{
					scoop_tries++;
					scoop_volume+=cVolume;
				}
			}
			else
			{
				if(addscoop)
				{
					add_tries++;
					add_failed_tries++;
					add_failed_volume+=cVolume;
				}
				else
				{
					scoop_tries++;
					scoop_failed_tries++;
					scoop_failed_volume+=cVolume;
				}
			}
		}
		words.clear();
	}
	endVolume=Volume;
	if(!reached)
	{
		startVolume=endVolume;
	}
	
	if(add_tries>0)
	{
		add_fail_percentage=double(add_failed_tries)/add_tries;
	}
	else
	{
		add_fail_percentage=0;
	}
	
	if(scoop_tries>0)
	{
		scoop_fail_percentage=double(scoop_failed_tries)/scoop_tries;
	}
	else
	{
		scoop_fail_percentage=0;
	}

	//������ ���������� � ����
	outfile.open(outfilePath);
	setlocale(LC_ALL,"");
	outfile<<"������� ���������"<<";";
	outfile<<"���� �������� ���������"<<";";
	outfile<<"����� ��������"<<";";
	outfile<<"����� ����������"<<";";
	outfile<<"������� ���������"<<";";
	outfile<<"���� �������� ���������"<<";";
	outfile<<"����� ��������"<<";";
	outfile<<"����� ����������"<<";";
	outfile<<"��������� �����"<<";";
	outfile<<"�������� �����"<<";";
	outfile<<endl;
	outfile<<add_tries<<";";
	outfile<<add_fail_percentage<<";";
	outfile<<add_volume<<";";
	outfile<<add_failed_volume<<";";
	outfile<<scoop_tries<<";";
	outfile<<scoop_fail_percentage<<";";
	outfile<<scoop_volume<<";";
	outfile<<scoop_failed_volume<<";";
	outfile<<endVolume<<";";
	outfile<<startVolume<<";";
	outfile.close();
    return 0; 
	}
	else
	{
		//����� usage
		setlocale(LC_ALL,"");
		cout<<"Usage: task3 full_in_file_path full_out_file_path date1 date2"<<endl;
		cout<<"������ ����: yyyy:mm:dd-hh:mm:ss"<<endl;
		cout<<"������ ���� ������ ���� ����� ������"<<endl;
		return 1;
	}
	return 2;
}