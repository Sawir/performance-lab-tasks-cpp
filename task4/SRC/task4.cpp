#include <cmath> 
#include <string>
#include <locale>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
using namespace std;

/*
	Задача сводится к разбиению второй строки на упорядоченную последовательность подстрок 
	по разделителю '*' и последующему поиску этой последовательности в первой строке. Если
	упорядоченная последовательность подстрок полностью найдена среди символов первой строки, 
	можно сделать однозначный вывод об идентичности строк. В противном случае можно сделать
	однозначный вывод об их неидентичности.
	
	Предварительно совершается попытка сделать однозначный вывод на основе частных случаев;
	при невозможности однозначного вывода выполняется алгоритм для общего случая.

	В случае, когда во второй строке отсутствуют разделители, достаточно сравнить
	строки стандартным методом, чтобы сделать однозначный вывод об их идентичности
	либо однозначный вывод об их неидентичности.

	В граничных случаях, когда вторая строка начинается не с разделителя и/или
	заканчивается не на разделителе, достаточно провести сравнение начал и/или концов
	двух строк, чтобы сделать однозначный вывод о неидентичности строк (но этого недостаточно 
	для однозначного вывода об идентичности строк). Если в этих случаях начала и/или концы
	совпадают, то они исключаются из дальнейшего алгоритма поиска подстрок.
		
	Тривиальны случаи, когда одна или обе строки пусты.
*/

template <class Container>
//процедура разбиения строки входных данных
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
		string str1=argv[1], str2=argv[2]; //входные строки

		bool anystart=true; //вспомогательный флаг: вторая строка начинается с разделителя
		bool anyend=true; //вспомогательный флаг: вторая строка оканчивается разделителем
		bool resolved=false; //вспомогательный флаг для основного цикла поиска подстрок
		
		if (str2[0]!='*') anystart=false;
		if (str2[str2.length()-1]!='*') anyend=false;
		
		int strstart=0, strend=str1.length()-1;//номера первого и последнего элементов проверяемой части первой строки

		std::vector<std::string>words;//отдельные подстроки второй строки
		SplitString(str2,words);

		//вырожденные случаи: одна или обе входные строки пустые
		
		//пустая строка str1 может быть идентична лишь пустой строке либо строке, состоящей из одних разделителей 
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

		//если первая строка не пустая, а вторая - пустая, строки не идентичны
		if (str2=="")
		{
			cout<<"KO"<<endl;;
			return 0;
		}

		//вырожденный случай: во второй строке нет разделителей
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

		//граничный случай: фиксированное начало второй строки
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

		//граничный случай: фиксированный конец второй строки
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

		//поиск для общего случая
		int substr_number=0; //текущий номер подстроки, для которой осуществляется поиск
		int substr_number_cap=words.size()-1; //номер последней подстроки, для которой должен осуществляться поиск
		
		//первая и последняя подстроки уже могли быть учтены в граничных случаях
		if (!anystart) substr_number++;
		if (!anyend) substr_number_cap--;
		
		//если до основного цикла поиска подстрок все подстроки уже найдены, делаем однозначный вывод об идентичности строк
		if(substr_number>substr_number_cap)
		{
			cout<<"OK"<<endl;
			return 0;
		}

		//основной цикл поиска
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

		//найдены ли все подстроки в общем случае?
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