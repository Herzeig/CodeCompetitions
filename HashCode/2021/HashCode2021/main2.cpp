#include <iostream>
#include <fstream>

#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>

#include <ctime>
#include <cmath>
#include <iterator>

#include <cstdlib>

using namespace std;

#define SOGLIA_SPEED_W 95

struct Building {
  uint32_t id;
  uint32_t x;
  uint32_t y;
  uint32_t latencyW;
  uint32_t speedW;

  bool operator<(const Building & rhs) const
  {
    bool retVal;

    if(latencyW > rhs.latencyW){
      return true;
    } else if(latencyW == rhs.latencyW){
      if(speedW > rhs.speedW){
          return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  };
};

struct Antenna {
  uint32_t id;
  uint32_t range;
  uint32_t speed;

  bool operator<(const Antenna & rhs) const
  {
    bool retVal;

    if(range > rhs.range){
      return true;
    } else if(range == rhs.range){
      if(speed > rhs.speed){
          return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }
  /*bool operator<(const Antenna & rhs) const
  {
    bool retVal;

    if(range > rhs.range){
      return true;
    } else if(range == rhs.range){
      if(speed > rhs.speed){
          return true;
      }
    } else {
      return false;
    }
  }*/
};

struct Soluzione{
  uint32_t id;
  uint32_t x;
  uint32_t y;
};

vector<Building> buildings;
vector<Antenna> antennas;
vector<Soluzione> soluzioni;
vector< vector<bool> > copertura;
//map<string, int> streetTokened;
//Iterator<map<string, int>, bool> responseMap;

void OrdinaBuildingsSpeedWDecrescente()
{
  clock_t time_req;
  time_req = clock();
  sort(buildings.begin(), buildings.end());
  time_req = clock() - time_req;
  cout << "OrdinaBuildingsSpeedWDecrescente, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds." << endl;
};

void OrdinaAntennasSpeedDecrescente()
{
  clock_t time_req;
  time_req = clock();
  sort(antennas.begin(), antennas.end());
  time_req = clock() - time_req;
  cout << "OrdinaAntennasSpeedDecrescente, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds." << endl;
};

bool Coperto(Building b, Soluzione a)
{
  bool retVal;
  uint32_t dist;

  retVal = false;
  dist = abs((int)b.x - (int)a.x) + abs((int)b.y - (int)a.y);
  if(dist <= antennas[a.id].range){
    retVal = true;
  } else {
    retVal = false;
  }

  return retVal;
};

void TrovaSoluzione2()
{
  clock_t time_req;
  int i, j, k, tmpNum;

  Soluzione tmpSol;
  for(i=0; i<antennas.size(); i++){
      tmpSol.id = antennas[i].id;
      tmpSol.x = buildings[i].x;
      tmpSol.y = buildings[i].y;
      soluzioni.push_back(tmpSol);
      //cout << tmpSol.id << ' ' << tmpSol.x << ' ' << tmpSol.y << endl;
  }
}

void TrovaSoluzione()
{
  clock_t time_req;
  int i, j, k, tmpNum;

  Soluzione tmpSol;

  //OrdinaBuildingsSpeedWDecrescente();
  OrdinaAntennasSpeedDecrescente();
  random_shuffle(buildings.begin(), buildings.end());
  random_shuffle(buildings.begin(), buildings.end());
  //random_shuffle(antennas.begin(), antennas.end());

  j=0;
  for(i=0; i<antennas.size(); i++){
      if(j >= buildings.size()){
        break;
      }
      /*for(k=0; k<soluzioni.size(); k++){
        if(Coperto(buildings[j], soluzioni[k])){
          j++;
          k=0;
        }
      }*/
      tmpSol.id = antennas[i].id;
      tmpSol.x = buildings[j].x;
      tmpSol.y = buildings[j].y;
      soluzioni.push_back(tmpSol);
      j++;
      //cout << tmpSol.id << ' ' << tmpSol.x << ' ' << tmpSol.y << endl;
  }
  cout << soluzioni.size() << endl;
  cout << j << endl;
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
  uint32_t W, H, N, M, R;
  uint32_t i, id;

  Antenna tmpAntenna;
  Building tmpBuilding;

  /*fileName.push_back("a");
  fileName.push_back("b");
  fileName.push_back("c');
  fileName.push_back("d");
  fileName.push_back(e");
  fileName.push_back('f');*/
  //time_req = clock();
  filenameIn = filenameOut = argv[1];
  filenameIn = filenameIn + ".in";
  filenameOut = filenameOut + ".out";

  fs.open (filenameIn.c_str(), fstream::in);
  fs >> W >> H;

  copertura.resize(W , vector<bool>(H, false ));

  //cout << W << ' ' << H << endl;
  fs >> N >> M >> R;
  //cout << N << ' ' << M << ' ' << R  << endl;
  cout << "Antenne: " << M  << endl;
  id = 0;
  for(i=0; i<N; i++){
    fs >> tmpBuilding.x >> tmpBuilding.y >> tmpBuilding.latencyW >> tmpBuilding.speedW;
    tmpBuilding.id = id++;
    //cout << tmpBuilding.id << ' ' << tmpBuilding.x << ' ' << tmpBuilding.y << ' ' << tmpBuilding.latencyW << ' ' <<tmpBuilding.speedW << endl;
    // Seleziona building sopra soglia
    //if(tmpBuilding.latencyW>=atoi(argv[2])){
      buildings.push_back(tmpBuilding);
    //}
  }
  cout << "Buildings: " << buildings.size() << endl;
  id = 0;
  for(i=0; i<M; i++){
    fs >> tmpAntenna.range >> tmpAntenna.speed;
    tmpAntenna.id = id++;
    antennas.push_back(tmpAntenna);
  }
  //cout << "Buildings: " << id << endl;
  fs.close();

  time_req = clock();
  TrovaSoluzione();
  time_req = clock() - time_req;
  cout << "TrovaSoluzione, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

  //fsTmp.open (filenameTmp.c_str(), fstream::in);
  fs.open (filenameOut.c_str(), fstream::out);
  fs << soluzioni.size() << endl;
  for(i=0; i<soluzioni.size(); i++){
    fs << soluzioni[i].id << "  " << soluzioni[i].x << "  " << soluzioni[i].y << endl;
  }
  fs.close();
  //fsTmp.close();
}