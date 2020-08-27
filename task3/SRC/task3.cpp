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
	Алгоритм работает в предположении, что входной лог-файл
	отсортирован в порядке возрастания даты и времени.
*/

template <class Container>
//процедура разбиения строки входных данных
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

//эта функция сравнивает две даты и делает вывод о том, позже ли первая дата, чем вторая
bool date_comp(unsigned int date1[6], unsigned int date2[6])
{
	if (date1[0]!=date2[0]) return (date1[0]>date2[0]); //сравнение по годам
	if (date1[1]!=date2[1]) return (date1[1]>date2[1]); //по месяцам
	if (date1[2]!=date2[2]) return (date1[2]>date2[2]); //по дням
	if (date1[3]!=date2[3]) return (date1[3]>date2[3]); //по часам
	if (date1[4]!=date2[4]) return (date1[4]>date2[4]); //по минутам
	if (date1[5]!=date2[5]) return (date1[5]>date2[5]); //по секундам
	return false; //в случае полного совпадения дат возвращаем ложь
}

int main(int argc, char *argv[]) 
{ 
	if (argc==5)
	{
	unsigned int Volume,MaxVolume,cVolume; //текущий и максимальный объем бочки, а также вспомогательный объем
	unsigned int startDate[6], endDate[6], cDate[6]; //начало и конец искомого временного периода, а также вспомогательная дата
	
	//собираемая статистика
	unsigned int add_tries=0,add_failed_tries=0,add_volume=0,add_failed_volume=0;
	double add_fail_percentage;
	unsigned int scoop_tries=0,scoop_failed_tries=0,scoop_volume=0,scoop_failed_volume=0;
	double scoop_fail_percentage;
	unsigned int startVolume=0,endVolume=0;

	bool addscoop, cstate, reached=false;//вспомогательные флаги

	string strDate1=argv[3], strDate2=argv[4]; //входные строки с датами
	std::vector<std::string>words;//отдельные ключевые элементы входных строк
	
	//обработка входных дат
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
		cout<<"Неверная очередность дат."<<endl;
		return 1;
	}

	string infilePath=argv[1]; //путь к входному файлу
	ifstream infile(infilePath); //файл входных данных
	
	string outfilePath=argv[2]; //путь к выходному файлу
	ofstream outfile; //файл выходных данных

	if(!infile){
			setlocale(LC_ALL,"");
			cout<<"Входной файл не найден."<<endl;
			return 1;
	}

	string strInput; //строка входных данных	
	
	//Считывание максимального и текущего объема
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

		//если дошли до границы заданного периода, начинаем собирать статистику
		if(!reached){
			if(date_comp(cDate,startDate)){
				startVolume=Volume;
				reached=true;
			}
		}

		//если дошли до даты позже заданного периода, прекращаем выполнение
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

		//учет изменения объема воды в бочке ведется в любом случае
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
		
		//статистика собирается, только если дошли до указанного периода
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

	//запись статистики в файл
	outfile.open(outfilePath);
	setlocale(LC_ALL,"");
	outfile<<"Попытки наливания"<<";";
	outfile<<"Доля провалов наливания"<<";";
	outfile<<"Объем налитого"<<";";
	outfile<<"Объем неналитого"<<";";
	outfile<<"Попытки выливания"<<";";
	outfile<<"Доля провалов выливания"<<";";
	outfile<<"Объем вылитого"<<";";
	outfile<<"Объем невылитого"<<";";
	outfile<<"Начальный объем"<<";";
	outfile<<"Конечный объем"<<";";
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
		//вывод usage
		setlocale(LC_ALL,"");
		cout<<"Usage: task3 full_in_file_path full_out_file_path date1 date2"<<endl;
		cout<<"Формат даты: yyyy:mm:dd-hh:mm:ss"<<endl;
		cout<<"Вторая дата должна быть позже первой"<<endl;
		return 1;
	}
	return 2;
}