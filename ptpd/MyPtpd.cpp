#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>
#include <unistd.h>
//#include <stdlib.h>

using namespace std;

bool pingEnviroment (int argc, char const *argv[]);
bool ping (int argc, char const *argv[]);
bool restartNetworking ();

int main(int argc, char const *argv[])
{
    cout << "[INICIALIZADOR DE PTPD]        (by Rick)" << endl;

    //Ping
    char resp = 0;
    if(!pingEnviroment(argc, argv))
    {
        cout << "\n[FIM] Entao, boa sorte numa proxima. Byebye!   ;)" << endl;
        return 1;
    }
    
    cout << "\n\t(continuando...)\n" << endl;

    //Line up hw and sys clocks before
    cout << "[TIME] Gostaria de forcar o hwclock a se alinhar com o sysclock? (s/n) \n"
            << "\tEssa é uma etapa importante em embarcados que nao possuem bateria interna (COMO A BEAGLEBONE)\n"
            << "\tSe voce não sabe, melhor responder 'N'\n\tResposta: ";
    cin >> resp;
    while (tolower(resp) != 'n')
    {
        if (tolower(resp) == 's')
        {   
            system("sudo hwclock --systohc");
            break;
        }
        cout << "\tEscreva s ou n \n\tResposta: ";
        cin >> resp;
    }

    cout << "\n[TIME] Horario antes de iniciar o ptpd:" << endl;
    time_t actualTime = time(NULL);  
    cout << "\t" << ctime(&actualTime) << endl; 
    cout << "\t(finalmente vamos ao ptpd entao...)\n" << endl;

    //Catch config file
    string configPath; 
    cout << "[PTPD] Digite o endereço completo do aquivo de configurações do ptpd: \n--> ";
    cin >> configPath;

    ifstream configFile;
    configFile.open(configPath);
    if (configFile.fail())
    {
        cout << "[PTPD] Erro ao abrir o arquivo de configuracao" << endl;
        configFile.close();
        return 1;
    }

    string line, logFile = "", statisticsFile = "";
    size_t position1, position2;
    while (configFile)
    {
        getline(configFile, line);
        if( (position1 = line.find("log_file=")) != string::npos)
        {
            for (int i = position1 + strlen("log_file="); i < line.size(); i++)
            {
                logFile += line[i];
            }
        }
        if( (position2 = line.find("statistics_file=")) != string::npos)
        {
            for (int i = position2 + strlen("statistics_file="); i < line.size(); i++)
            {
                statisticsFile += line[i];
            }
        } 
    }
    cout << endl;
    if (logFile != "") 
    {
        cout << "\tlog_file = " << logFile << endl;
    }
    if (statisticsFile != "") 
    {
        cout << "\tstatistics_file = " << statisticsFile << endl << endl;
    }
    configFile.close();

    //Start ptpd
    stringstream sscmd;
    int answer; 
    cout << "[PTPD] Pode começar o ptpd? (s/n) \n\tResposta: ";
    cin >> resp;
    while (tolower(resp) != 'n')
    {
        if (tolower(resp) == 's')
        {   
            cout << "[PTPD] Comecando...!" << endl;
            //(reusing "sscmd" and "answer")
            sscmd << "sudo ptpd -c " << configPath;
            answer = system(sscmd.str().data());

            cout << "\n[TIME] Horario apos iniciar o ptpd:" << endl;
            for (int i = 0; 1; )
            {
                actualTime = time(NULL);  
                cout << "\t" << ctime(&actualTime);
                sleep(4);
            }       
            
            return 0;
        }
        cout << "\tEscreva s ou n \n\tResposta: ";
        cin >> resp;
    }
    cout << "\n[FIM] Beleza entao, ate mais!   ;)" << endl;
    return 0;
}

bool pingEnviroment (int argc, char const *argv[])
{    
    char resp = 0;
    cout << "\n[PING] Voce quer comecar com um Ping? (s/n) \n\tResposta: ";
    cin >> resp;
    while (tolower(resp) != 'n')
    {
        if (tolower(resp) == 's')
        {  
            if (!ping(argc, argv))
            {
                if (!restartNetworking())
                {
                    return false;
                }
                if (!ping(argc, argv))
                {
                    return false;
                }
            }
            break;
        } // if 's'
        cout << "\tEscreva s ou n \n\tResposta: ";
        cin >> resp;
    }
    return true;
}

bool ping (int argc, char const *argv[]) 
{   
    stringstream sscmd;
    int answer;
    string ip;

    //Catch IP
    if (argc == 1)
    {
        cout << "[PING] Digite o endereco de IP: ";
        cin >> ip;
    } else
    {
        ip = argv[1];
    }
    
    //Ping
    cout << endl;
    sscmd << "ping -c 5 " << ip;
    answer = system(sscmd.str().data());

    //Analise the answer
    if (answer != 0)
    {
        cout << "\n[PING] Ops, não foi possivel achar '" << ip << "'!   :(" << endl;
        return false;
    } else
    {
        cout << "\n[PING] '" << ip << "' foi achado!   :)\n";
        return true;
    }
}

bool restartNetworking ()
{
    char resp = 0;
    cout << "[PING] Quer reiniciar o \"nertworking service\" e tentar novamente? (s/n) \n"
            << "\t(Isso costuma funcionar na bbgreen)\n"
            << "\tResposta: ";
    cin >> resp;
    while (tolower(resp) != 'n')
    {
        if (tolower(resp) == 's')
        {   
            system("sudo service networking restart");
            return true;
        }
        cout << "\tEscreva s ou n \n\tResposta: ";
        cin >> resp;
    }
    return false;
}

