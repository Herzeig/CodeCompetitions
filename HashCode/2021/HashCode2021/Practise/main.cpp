#include <iostream>

#include <fstream>

#include <vector>
#include <string>
#include <map>
#include <set>

#include <ctime>
#include <cmath>

using namespace std;

#define DUE 0
#define TRE 1
#define QUATTRO 2

#define DEPTH 4

struct Pizza {
  int id;
  int numIngredienti;
  vector<int> ingredienti;
};

struct Delivery {
  int numPersone;
  vector<int> pizze;
};

vector<Pizza> pizze;

int deltaIngredienti(void){
  int retVal;

  retVal = 0;

  return retVal;
};

void ordina()
{
  int i, j;
  int n = pizze.size();
  Pizza tmp;

  for (j = 0; j < n; j++){
    for (i=n-1; i>=j; i--){
      if (pizze[i].numIngredienti<pizze[i+1].numIngredienti){
        tmp = pizze[i];
        pizze[i] = pizze[i+1];
        pizze[i+1] = tmp;
      }
    }
  }
}

int deltaPizza(set<int> listaIngredienti, Pizza piz)
{
  int retVal, i;

  retVal = 0;
  for(i=0; i<piz.numIngredienti; i++){
    if(listaIngredienti.find(piz.ingredienti[i]) == listaIngredienti.end()){
      retVal++;
    }
  }

  return retVal;
};

void FindNextPizza(Delivery* soluzione, set<int>* listaIngredienti)
{
  int i;
  int maxDelta = 0;
  int delta;
  int countTry = 0;
  vector<Pizza>::iterator it, maxIt;

  maxIt = pizze.end();
  for(it = pizze.begin(), i=0; it != pizze.end(); it++,i++){
    delta = deltaPizza(*listaIngredienti, pizze[i]);
    /*if(delta == pizze[i].numIngredienti)
    {
      // Take Pizza
      maxIt = it;
      break;
    } else*/
    if(delta > maxDelta){
        maxDelta = delta;
        maxIt = it;
    }
    /*if((i<pizze.size()-1) && (maxDelta >= pizze[i+1].numIngredienti)){
      break;
    }*/
    if(++countTry>DEPTH){
      break;
    }
  }

  if(maxIt != pizze.end()){
    //cout << (*maxIt).id << endl;
    soluzione->pizze.push_back((*maxIt).id);
    for(i=0; i<(*maxIt).numIngredienti; i++){
      listaIngredienti->insert((*maxIt).ingredienti[i]);
    }
    pizze.erase(maxIt);
  } else {
    soluzione->pizze.push_back(pizze[0].id);
    for(i=0; i<pizze[0].numIngredienti; i++){
      listaIngredienti->insert(pizze[0].ingredienti[i]);
    }
    pizze.erase(pizze.begin());
  }
};

void algo(int* t, int T, int numPers, string filename, int* numSol)
{
  clock_t time_req;
  fstream fs;

  int i, j, tmpNum;
  Pizza tmp;
  Delivery sol;
  bool exitCond;
  //vector<int> ingredientiAttuali;
  set<int> ingredientiAttuali;

  fs.open (filename.c_str(), fstream::out | fstream::app);

  exitCond = false;
  while((*t < T) && (exitCond == false)){
    //time_req = clock();
    sol.pizze.clear();
    sol.numPersone = numPers;
    if(pizze.size()<numPers){
      exitCond = true;
    } else {
      ingredientiAttuali.clear();
      //cout << "Pizze size: " << pizze.size() << endl;
      //cout << "Prima Pizza: " << pizze[0].id << endl;
      sol.pizze.push_back(pizze[0].id);
      //cout << "Ingredienti attuali: ";
      for(i=0; i<pizze[0].numIngredienti; i++){
        //cout << pizze[0].ingredienti[i] << ' ';
        ingredientiAttuali.insert(pizze[0].ingredienti[i]);
      }
      //cout << endl;
      pizze.erase(pizze.begin());

      for(j=1; j<numPers; j++){
        //cout << "Pizze size: " << pizze.size() << endl;
        FindNextPizza(&sol, &ingredientiAttuali);
        //cout << j+1 << " pizza: " << sol.pizze[j] << endl;
        //cout << "Ingredienti attuali size : " << ingredientiAttuali.size() << endl;
      }

      //cout << "Pizze size final: " << pizze.size() << endl;

      fs << sol.numPersone;
      for(j=0; j<sol.numPersone; j++){
        fs << ' ' << sol.pizze[j];
      }
      fs << endl;

      *numSol = *numSol + 1;

      /*time_req = clock() - time_req;
                    cout << "T" << numPers << "-"<< *t << ", it took " << (float)time_req/CLOCKS_PER_SEC << " seconds. Points: " << ingredientiAttuali.size() << endl;*/
      cout << "T" << numPers << "-"<< *t << endl;
      *t = *t + 1;
    }
  }

  fs.close();
};

int main(int argc, char* argv[])
{
  clock_t time_req;
  fstream fs, fsTmp;
  string filenameIn, filenameOut, filenameTmp, stringa;

  int M, T[3], i, j;
  int t[3];
  string ingred;
  int numSoluzioni = 0;
  map<string, int> convertiIngredienti;
  map<string, int>::iterator foundIngred;
  int indiceIngrediente;

  Pizza tmp;

  t[DUE] = t[TRE] = t[QUATTRO] = 0;
  indiceIngrediente = 0;

  //cout << copysign(4.2, 7.9) << endl;

  //Lettura File In
  time_req = clock();
  filenameIn = filenameOut = filenameTmp = argv[1];
  filenameIn = filenameIn + ".in";
  filenameOut = filenameOut + ".out";
  filenameTmp = filenameTmp + ".tmp";
  fs.open (filenameIn.c_str(), fstream::in);
  fs >> M >> T[DUE] >> T[TRE] >> T[QUATTRO];
  for(i=0; i<M; i++){
    tmp.ingredienti.clear();
    tmp.id = i;
    fs >> tmp.numIngredienti;
    for(j=0; j<tmp.numIngredienti; j++){
      fs >> ingred;
      foundIngred = convertiIngredienti.find(ingred);
      if(foundIngred == convertiIngredienti.end()){
        tmp.ingredienti.push_back(indiceIngrediente);
        convertiIngredienti.insert(make_pair(ingred, indiceIngrediente++));
      } else {
        tmp.ingredienti.push_back((*foundIngred).second);
      }
    }
    pizze.push_back(tmp);
  }
  fs.close();
  time_req = clock() - time_req;
                cout << "Lettura File In, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

  // Algo
  // Ordinare le pizze in numero ingredienti decrescente
  time_req = clock();
  ordina();
  time_req = clock() - time_req;
  cout << "Ordina, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

  time_req = clock();
  algo(&(t[QUATTRO]), T[QUATTRO], 4, filenameTmp, &numSoluzioni);
  time_req = clock() - time_req;
                cout << "Algo QUATTRO, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

  time_req = clock();
  algo(&(t[TRE]), T[TRE], 3, filenameTmp, &numSoluzioni);
  time_req = clock() - time_req;
                cout << "Algo TRE, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;

  time_req = clock();
  algo(&(t[DUE]), T[DUE], 2, filenameTmp, &numSoluzioni);
  time_req = clock() - time_req;
                cout << "Algo DUE, it took " << (float)time_req/CLOCKS_PER_SEC << " seconds" << endl;


  // Scrittura File Out
  fsTmp.open (filenameTmp.c_str(), fstream::in);
  fs.open (filenameOut.c_str(), fstream::out | fstream::app);
  fs << numSoluzioni << endl;
  fs << fsTmp.rdbuf();
  fs.close();
  fsTmp.close();
}