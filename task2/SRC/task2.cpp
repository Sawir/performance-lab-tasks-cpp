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
	��������� ���������� ������� ����� ������������� � ��������� �����, � ��������� ������,
	�� ����� ��� ���������� �����, ��������� ��������� ����� � �������; ����������� �����
	������� �� ���� �������� ��������� ��� ���� ��������� � ������ ����������� ��������� 
	��� ������� ����������. ����� ������ ����������� ��������� ����� ����� ����� ��������.

	��������� � ������ ������� ����������� ������������ ��������� ������ � ������������,
	����� ��������� �������� � �������� �� ���� ��� ����������� ����������� ���������
	������������ ����� ��� ����, �� �� ���� ���� ���������. ��� ������ � ������� wmain.
*/

double output1[3],output2[3]; //���������� ����� ��������

//�������������� �������: ��������� ���������� ����� ��������, ���������� �� � output, � ���������� ����� ����� ��������
int CalculatePoints(double p0[3], double r, double p1[3], double p2[3]){

	double a1=(p2[1]-p1[1])/(p2[0]-p1[0]);
	double b1=-p1[0]*a1+p1[1]-p0[1];
	double c1=(p2[2]-p1[2])/(p2[0]-p1[0]);
	double d1=-p1[0]*c1+p1[2]-p0[2];

	double A=1+a1*a1+c1*c1;
	double B=2*(-p0[0]+a1*b1+c1*d1);
	double C=p0[0]*p0[0]+b1*b1+d1*d1-r*r;
	double D=B*B-4*A*C;

	if(D>0) //��� ����� ������������
	{
		output1[0]=(-B+sqrt(D))/(2*A);
		output1[1]=(output1[0]-p1[0])*(p2[1]-p1[1])/(p2[0]-p1[0])+p1[1];
		output1[2]=(output1[0]-p1[0])*(p2[2]-p1[2])/(p2[0]-p1[0])+p1[2];
		output2[0]=(-B-sqrt(D))/(2*A);
		output2[1]=(output2[0]-p1[0])*(p2[1]-p1[1])/(p2[0]-p1[0])+p1[1];
		output2[2]=(output2[0]-p1[0])*(p2[2]-p1[2])/(p2[0]-p1[0])+p1[2];
		return 2;
	}
	else if (D==0) //���� ����� ������������
	{
		output1[0]=-B/(2*A);
		output1[1]=(output1[0]-p1[0])*(p2[1]-p1[1])/(p2[0]-p1[0])+p1[1];
		output1[2]=(output1[0]-p1[0])*(p2[2]-p1[2])/(p2[0]-p1[0])+p1[2];
		return 1;
	}
	else //��� ����� ������������
	{
		return 0;
	}
}
 
template <class Container>
//��������� ��������� ������ ������� ������
void SplitString(const std::string& str, Container& cont, const std::string& separators =" []{},")
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

int wmain(int argc, wchar_t *argv[])
{
	if (argc==2)
	{
	double p0[3], r; //���������� ������ � ������ �����
	double p1[3], p2[3]; //���������� �����, ������������� ������
	int NoP; //���������� ����� ��������
	double swap; //��������������� ����������
	int offset;

	wstring infilePath=argv[1]; //���� � �������� �����
	ifstream infile(infilePath); //���� ������� ������
	
	if(!infile){
			setlocale(LC_ALL,"");
			wcout<<L"���� �� ������."<<endl;
			return 1;
	}

	string strInput; //������ ������� ������	
	getline(infile, strInput);
	std::vector<std::string>words;//��������� �������� �������� ������ ������
	SplitString(strInput,words);

	for(unsigned int i=0;i<words.size();++i)
	{
		if (words[i]=="center:"){
			p0[0]=stod(words[i+1]);
			p0[1]=stod(words[i+2]);
			p0[2]=stod(words[i+3]);
		}
		if (words[i]=="radius:"){
			r=stod(words[i+1]);
		}
		if (words[i]=="line:"){
			p1[0]=stod(words[i+1]);
			p1[1]=stod(words[i+2]);
			p1[2]=stod(words[i+3]);
			p2[0]=stod(words[i+4]);
			p2[1]=stod(words[i+5]);
			p2[2]=stod(words[i+6]);
		}
	}

	/*
	� ������������� ����� ���������� ��������������� ������������ ��������
	��� ��������� ������� �� ���� � �������������� �������.
	*/
	if((p2[0]-p1[0])!=0)
	{
		offset=0;
	}
	else if((p2[1]-p1[1])!=0)
	{
		swap=p0[0]; p0[0]=p0[1]; p0[1]=p0[2]; p0[2]=swap;
		swap=p1[0]; p1[0]=p1[1]; p1[1]=p1[2]; p1[2]=swap;
		swap=p2[0]; p2[0]=p2[1]; p2[1]=p2[2]; p2[2]=swap;
		offset=1;
	}
	else
	{
		swap=p0[0]; p0[0]=p0[2]; p0[2]=p0[1]; p0[1]=swap;
		swap=p1[0]; p1[0]=p1[2]; p1[2]=p1[1]; p1[1]=swap;
		swap=p2[0]; p2[0]=p2[2]; p2[2]=p2[1]; p2[1]=swap;		
		offset=2;
	}
	
	//����������� ���������� ����� �������� � ���������� �� ���������
	NoP=CalculatePoints(p0,r,p1,p2);

	/*
	� ������������� ����� ���������� �������� ��������������� ������������ ��������
	� ����� ����������� � ����������� �� ���������� ����� ��������.
	*/
	switch(NoP)
	{
		case(0):
			setlocale(LC_ALL,"");
			wcout<<L"�������� �� �������"<<endl;
			break;

		case(1):
			switch(offset)
			{
				case(0):
					break;

				case(1):
					swap=output1[0];
					output1[0]=output1[2];
					output1[2]=output1[1];
					output1[1]=swap;
					break;

				case(2):
					swap=output1[0];
					output1[0]=output1[1];
					output1[1]=output1[2];
					output1[2]=swap;
					break;
			}
			wcout<<output1[0]<<endl<<output1[1]<<endl<<output1[2]<<endl;
			break;

		case(2):
			switch(offset)
			{
				case(0):
					break;

				case(1):
					swap=output1[0];
					output1[0]=output1[2];
					output1[2]=output1[1];
					output1[1]=swap;
					swap=output2[0];
					output2[0]=output2[2];
					output2[2]=output2[1];
					output2[1]=swap;
					break;

				case(2):
					swap=output1[0];
					output1[0]=output1[1];
					output1[1]=output1[2];
					output1[2]=swap;
					swap=output2[0];
					output2[0]=output2[1];
					output2[1]=output2[2];
					output2[2]=swap;
					break;
			}
			wcout<<output1[0]<<endl<<output1[1]<<endl<<output1[2]<<endl<<output2[0]<<endl<<output2[1]<<endl<<output2[2]<<endl;
			break;
	}
	return 0;
	}
	else
	{
		//����� usage ��� �������� ���������� ����������
		wcout<<L"Usage:\ntask2 full_file_path\n";
		return 1;
	}
    return 2;
}