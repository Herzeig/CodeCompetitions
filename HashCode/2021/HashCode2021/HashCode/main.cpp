#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <map>
#include <set>

#include <ctime>
#include <cmath>
#include <iterator>

#include <cstdlib>

using namespace std;

//#define CICLI_SEMAFORO_TEMPO_SIM 200

struct Street {
 uint32_t startIntersection;
 uint32_t endIntersection;
 //string name;
 uint32_t timeL;
};

struct Car {
 uint16_t P;
 vector<uint32_t> IntersectionTappa;
 uint32_t tempoF1;
};

struct ligthDuration {
  string streetName;
  uint32_t ligthDuration;
  uint64_t costoViaggi;
  uint32_t numeroUtenze;
};

struct IntersectionLight {
  vector<ligthDuration> lightDurations;
};

vector<Street> streets;
vector<Car> cars;
vector<IntersectionLight> soluzione;
vector<string> nameStreets;
map<string, int> streetTokened;

void OrdinaRisorse()
{

};

uint32_t CalcolaPunteggio()
{
  // Usa vector<Soluzione> soluzioni;
 return 0;
};

void TrovaSoluzione()
{
  clock_t time_req;
  int i, j, tmpNum;

  //time_req = clock();

  /*time_req = clock() - time_req;
    cout << "T" << numPers << "-"<< *t << ", it took " << (float)time_req/CLOCKS_PER_SEC << " seconds. Points: " << ingredientiAttuali.size() << endl;*/
};

int main(int argc, char* argv[])
{
  clock_t time_req;
  fstream fs, fsTmp;
  string filenameIn, filenameOut, stringa, tmpStr, filenameTmp;
  uint32_t maxScore = 0;
  uint32_t D, I, S, V, F;
  uint32_t i, j, k, costo, indice;
  uint32_t tempoCicloSemaforo, sommaUtenze, tmpCalc, sizeSemafori, sizeSoluzione;

  Street tmpStreet;
  Car tmpCar;
  ligthDuration tmpLight;
  vector<string> fileName;

  /*fileName.push_back("a");
  fileName.push_back("b");
  fileName.push_back("c');
  fileName.push_back("d");
  fileName.push_back(e");
  fileName.push_back('f');*/

  //Lettura File In
  //for(int kk; kk<fileName.size(); kk++){
    time_req = clock();
    filenameIn = filenameOut = argv[1];
    filenameIn = filenameIn + ".txt";
    filenameOut = filenameOut + ".out";
    filenameTmp = filenameOut + ".tmp";

    time_req = clock();
    fs.open (filenameIn.c_str(), fstream::in);
    fs >> D >> I >> S >> V >> F;
    //tempoCicloSemaforo = D / CICLI_SEMAFORO_TEMPO_SIM + 1;
    tempoCicloSemaforo = D / atoi(argv[2]) + 1;
    //tempoCicloSemaforo = 1;
    soluzione.resize(I);
    for(i=0; i<S; i++){
      fs >> tmpStreet.startIntersection >> tmpStreet.endIntersection >> tmpStr >> tmpStreet.timeL;
      nameStreets.push_back(tmpStr);

      streetTokened.insert(pair<string, int>(tmpStr, i));

      streets.push_back(tmpStreet);
      tmpLight.streetName = tmpStr;

      //tmpLight.ligthDuration = tmpStreet.timeL;
      tmpLight.ligthDuration = 1;
      soluzione[tmpStreet.endIntersection].lightDurations.push_back(tmpLight);
    }
    for(i=0; i<V; i++){
      fs >> tmpCar.P;
      costo = 0;
      for(j=0; j<tmpCar.P; j++){
        fs >> tmpStr;
        //tmpCar.streets.push_back(tmpStr);
        /*vector<string>::iterator itr = find(nameStreets.begin(), nameStreets.end(), tmpStr);
        indice = itr - nameStreets.begin();*/
        indice = streetTokened[tmpStr];
        tmpCar.IntersectionTappa.push_back(streets[indice].endIntersection);
        if(j>0){
          costo += streets[indice].timeL;
          for(k=0; k<soluzione[streets[indice].endIntersection].lightDurations.size(); k++){
            if(soluzione[streets[indice].endIntersection].lightDurations[k].streetName == tmpStr){
              soluzione[streets[indice].endIntersection].lightDurations[k].numeroUtenze++;
              //soluzione[streets[indice].endIntersection].lightDurations[streets[indice].startIntersection].numeroUtenze++;
              break;
            }
          }
        }
      }
      tmpCar.tempoF1 = costo;
      //cout << costo << endl;
      cars.push_back(tmpCar);
    }
    fs.close();

    // Algo
    TrovaSoluzione();
    time_req = clock() - time_req;
                  cout << "TrovaSoluzione, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

    // Scrittura File Out
    fs.open (filenameTmp.c_str(), fstream::out);
    sizeSoluzione = soluzione.size();
    cout << sizeSoluzione << endl;
    //fs << soluzione.size() << endl;
    for(i=0; i<soluzione.size(); i++){
      sizeSemafori = soluzione[i].lightDurations.size();
      /*sommaUtenze = 1;
      for(j=0; j<soluzione[i].lightDurations.size(); j++){
        sommaUtenze += soluzione[i].lightDurations[j].numeroUtenze;
      }*/
      //sommaUtenze = D;
      sommaUtenze = 0;
      for(j=0; j<soluzione[i].lightDurations.size(); j++){
        if(soluzione[i].lightDurations[j].numeroUtenze != 0){
          /*if(soluzione[i].lightDurations[j].numeroUtenze < sommaUtenze){
            sommaUtenze = soluzione[i].lightDurations[j].numeroUtenze;
          }*/
          sommaUtenze += soluzione[i].lightDurations[j].numeroUtenze;
        } else {
          sizeSemafori--;
        }
      }
      if(sizeSemafori >= 1){
        fs << i << endl;
        fs << sizeSemafori << endl;
        for(j=0; j<soluzione[i].lightDurations.size(); j++){
          if(soluzione[i].lightDurations[j].numeroUtenze!=0){
            if(soluzione[i].lightDurations.size() == 1){
                soluzione[i].lightDurations[j].ligthDuration = D;
            } else {
              /*tmpCalc = tempoCicloSemaforo * soluzione[i].lightDurations[j].numeroUtenze;
              if((tmpCalc % sommaUtenze) == 0){
                tmpCalc /= sommaUtenze;
              } else {
                tmpCalc = tmpCalc / sommaUtenze + 1;
              }
              if(tmpCalc > D){
                tmpCalc = D;
              }
              soluzione[i].lightDurations[j].ligthDuration = tmpCalc;*/
              tmpCalc = tempoCicloSemaforo * (soluzione[i].lightDurations[j].numeroUtenze / sommaUtenze + 1);
              if(tmpCalc == 0){
                tmpCalc = 1;
              }
              if(tmpCalc > D){
                tmpCalc = D;
              }
              soluzione[i].lightDurations[j].ligthDuration = tmpCalc;

            }
            //fs << soluzione[i].lightDurations[j].streetName << ' ' << soluzione[i].lightDurations[j].ligthDuration << endl;
            fs << soluzione[i].lightDurations[j].streetName << ' ' << 2 << endl;
          }
        }
      } else {
        sizeSoluzione--;
      }
    }
    //cout << sizeSoluzione << endl;
    fs.close();

      // Scrittura File Out
  fsTmp.open (filenameTmp.c_str(), fstream::in);
  fs.open (filenameOut.c_str(), fstream::out);
  fs << sizeSoluzione << endl;
  fs << fsTmp.rdbuf();
  fs.close();
  fsTmp.close();
  //}
}