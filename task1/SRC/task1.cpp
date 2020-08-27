#include <string>
#include <locale>
#include <iostream>
using namespace std;

//������� ������� �������� �� ���������� �� � ������������
wstring itoBase(unsigned int nb, wstring Base)
{
	unsigned int BaseB = Base.length();
	wstring result;

	while(nb)
    {
        result.push_back(Base[nb%BaseB]);
        nb /= BaseB;
    }

    for(int i = 0, j = result.length()-1; i < j; ++i, --j)
    {
        wchar_t swap = result[i];
        result[i] = result[j];
        result[j] = swap;
    }

	return(result);
}

//������������� ������� �������� �� ������������ �� � ������������
wstring itoBase(wstring nb, wstring BaseSrc, wstring BaseDst)
{
	unsigned int BaseA = BaseSrc.length();
	unsigned int BaseB = BaseDst.length();
	wstring result;
	unsigned int Number = 0;

	//������ ������������ ������� �� ��������� �� � ����������
    for (unsigned int i = 0; i < nb.length(); ++i)
    {
        Number = Number*BaseA + BaseSrc.find(nb[i]);
    }
	
	//����� �������������� ������� �� ���������� �� � ��������
	while(Number)
    {
        result.push_back(BaseDst[Number%BaseB]);
        Number /= BaseB;
    }

    for(int i = 0, j = result.length()-1; i < j; ++i, --j)
    {
        wchar_t swap = result[i];
        result[i] = result[j];
        result[j] = swap;
    }
	return(result);
}

int wmain(int argc, wchar_t *argv[])
{
	setlocale(LC_ALL,"");

	if (argc==3)
	{
		//����� ������� ������� (�� ���������� �� � ������������)
		unsigned int Number=stoi(argv[1]);
		wstring BaseDst=argv[2];
		wstring result = itoBase(Number,BaseDst);
		wcout << result << endl;
		return 0;
	}
	else if (argc==4)
	{
		//����� ������������� ������� (�� ������������ �� � ������������)
		wstring Number=argv[1], BaseSrc=argv[2], BaseDst=argv[3];
		wstring result = itoBase(Number,BaseSrc,BaseDst);
		wcout << result << endl;
		return 0;
	}
	else
	{
		//����� usage ��� �������� ���������� ����������
		wcout<<L"Usage:\ntask1 decimal_number string_base\n���\ntask1 string_number string_base_src string_base_dst\n";
		return 1;
	}
    return 2;
}