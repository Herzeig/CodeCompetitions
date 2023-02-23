#include <stdio.h>
#include <iostream>
#include <fstream>
#include <list>
#include <algorithm>
#include <iostream>
#include <vector>
#include <locale>
#include <codecvt>
#include <random>

using namespace std;

class point {
public:
    point( int a = 0, int b = 0 ) { x = a; y = b; }
    bool operator ==( const point& o ) { return o.x == x && o.y == y; }
    point operator +( const point& o ) { return point( o.x + x, o.y + y ); }
    int x, y;
};
 
class map {
public:
    map(){};
    map(vector<vector<char> > mappa, int col, int row) {
        w = col;
        h = row;
        for( int r = 0; r < h; r++ )
            for( int s = 0; s < w; s++ )
                m[s][r] = mappa[r][s];
    }
    int operator() ( int x, int y ) { return m[x][y]; }
    char m[1000][1000];
    int w, h;
};
 
class node {
public:
    bool operator == (const node& o ) { return pos == o.pos; }
    bool operator == (const point& o ) { return pos == o; }
    bool operator < (const node& o ) { return dist + cost < o.dist + o.cost; }
    point pos, parent;
    int dist, cost;
};
 
class aStar {
public:
    aStar() {
        neighbours[0] = point( 0, -1 ); neighbours[1] = point(  0, 1 );
        neighbours[2] = point( -1,  0 ); neighbours[3] = point(  1,  0 );
    }
 
    int calcDist( point& p ){
        // need a better heuristic
        int x = end.x - p.x, y = end.y - p.y;
        return( x * x + y * y );
    }
 
    bool isValid( point& p ) {
        return ( p.x >-1 && p.y > -1 && p.x < m.w && p.y < m.h );
    }
 
    bool existPoint( point& p, int cost ) {
        std::list<node>::iterator i;
        i = std::find( closed.begin(), closed.end(), p );
        if( i != closed.end() ) {
            if( ( *i ).cost + ( *i ).dist < cost ) return true;
            else { closed.erase( i ); return false; }
        }
        i = std::find( open.begin(), open.end(), p );
        if( i != open.end() ) {
            if( ( *i ).cost + ( *i ).dist < cost ) return true;
            else { open.erase( i ); return false; }
        }
        return false;
    }
 
    bool fillOpen( node& n ) {
        int stepCost, nc, dist;
        point neighbour;
 
        for( int x = 0; x < 4; x++ ) {
            // one can make diagonals have different cost
            stepCost = x < 4 ? 1 : 1;
            neighbour = n.pos + neighbours[x];
            if( neighbour == end ) return true;
 
            if( isValid( neighbour ) && m( neighbour.x, neighbour.y ) != 1 ) {
                nc = stepCost + n.cost;
                dist = calcDist( neighbour );
                if( !existPoint( neighbour, nc + dist ) ) {
                    node m;
                    m.cost = nc; m.dist = dist;
                    m.pos = neighbour; 
                    m.parent = n.pos;
                    open.push_back( m );
                }
            }
        }
        return false;
    }
 
    bool search( point& s, point& e, map& mp ) {
        int timeout = 0;
        node n; end = e; start = s; m = mp;
        n.cost = 0; n.pos = s; n.parent = 0; n.dist = calcDist( s ); 
        open.push_back( n );
        while( !open.empty() && (timeout < 1500)) {
            timeout++;
            //open.sort();
            node n = open.front();
            open.pop_front();
            closed.push_back( n );
            if( fillOpen( n ) ) return true;
        }
        return false;
    }
 
    int path( std::list<point>& path ) {
        path.push_front( end );
        int cost = 1 + closed.back().cost; 
        path.push_front( closed.back().pos );
        point parent = closed.back().parent;
 
        for( std::list<node>::reverse_iterator i = closed.rbegin(); i != closed.rend(); i++ ) {
            if( ( *i ).pos == parent && !( ( *i ).pos == start ) ) {
                path.push_front( ( *i ).pos );
                parent = ( *i ).parent;
            }
        }
        path.push_front( start );
        return cost;
    }
 
    map m; point end, start;
    point neighbours[4];
    std::list<node> open;
    std::list<node> closed;
    string percorso;
};

//string nomeFile = "1_victoria_lake";
//string nomeFile = "2_himalayas";
string nomeFile = "3_budapest";
//string nomeFile = "4_manhattan";
//string nomeFile = "5_oceania";

struct customer{
    unsigned int x;
    unsigned int y;
    unsigned int value;
};

vector<customer> customers;

struct replyOffice{
    unsigned int x;
    unsigned int y;
    vector<string> paths;
};

int terrainValue(char cella){
    int retVal = 0xFFFF;
    switch(cella){
            case '#':
                retVal = -0xFFFF;
            break;
            case '~':
                retVal = -800;
            break;
            case '*':
                retVal = -200;
            break;
            case '+':
                retVal = -150;
            break;
            case 'X':
                retVal = -120;
            break;
            case '_':
                retVal = -100;
            break;
            case 'H':
                retVal = -70;
            break;
            case 'T':
                retVal = -50;
            break;
    }
    return retVal;
}

bool compareByValue(const customer &a, const customer &b)
{
    return a.value > b.value;
}

int isCustomer(int x, int y){
    int retVal = -1;

    vector<customer>::iterator it=customers.begin();
    for(;it!=customers.end();it++)
    {
        if(((*it).x == x) && ((*it).y == y))
        {
            retVal = (*it).value;
            break;
        }
    }

    return retVal;
}

int main()
{
    fstream inputFile;
    fstream outputFile;
    int customerValue;
    int NColum, MRow, CustomersNum, ReplyOffices;
    customer cstTmp;
    char cella;
    vector<replyOffice> offices;

    inputFile.open(nomeFile + ".txt", ios::in);
    if(inputFile.is_open()){
        inputFile >> NColum >> MRow >> CustomersNum >> ReplyOffices;

        for(int i = 0; i < CustomersNum; i++){
            inputFile >> cstTmp.x >> cstTmp.y >> cstTmp.value;
            customers.push_back(cstTmp);
        }

        vector<vector<int> > mappa;
        vector<vector<char> > mappaReacheable;
        vector<int> rowTmp (1, NColum);
        vector<char> rowReach (1, NColum);
        for(int y = 0; y < MRow; y++){
            rowTmp.clear();
            rowReach.clear();
            for(int x = 0; x < NColum; x++){
                inputFile >> cella;
                customerValue = isCustomer(x, y);
                if(customerValue > 0){
                    rowTmp.push_back(customerValue);
                    rowReach.push_back(1);
                    //cout << x << ' ' << y << ' ' << customerValue << endl;
                } else {
                    rowTmp.push_back(terrainValue(cella));
                    if(cella == '#'){
                        rowReach.push_back(1);
                    } else {
                        rowReach.push_back(0);
                    }
                }
            }
            mappa.push_back(rowTmp);
            mappaReacheable.push_back(rowReach);
        }
        inputFile.close();

        //sort(customers.begin(), customers.end(), compareByValue);

        aStar as;
        map m(mappaReacheable, NColum, MRow);


        //point s(customers[0].x, customers[0].x), e(customers[1].x, customers[1].y);
        point s, e;
        string percorsoCasso;
        replyOffice offTmp;

        int office = 0;
        int pathPos;

        int assigned = 0;

        for( std::vector<customer>::iterator i = customers.begin(); (i != customers.end()) && (office <= ReplyOffices); i+=2 ) {
            s.x = ( *i ).x;
            s.y = ( *i ).y;
            e.x = ( *(i+1) ).x;
            e.y = ( *(i+1) ).y;

            if( as.search( s, e, m ) ) {
                assigned += 2;
                std::list<point> path;
                int c = as.path( path );  
                int diffx, diffy;
                point start = path.front();
                path.pop_front();
                percorsoCasso = "";
                pathPos = 0;
                office++;
                offTmp.paths.clear();
                std::list<point>::iterator i;
                for(i = path.begin(); i != path.end(); i++ ) {
                    diffx = ( *i ).x - start.x;
                    diffy = ( *i ).y - start.y;
                    if(diffx == 1){
                        percorsoCasso += 'L';
                    }
                    if(diffx == -1){
                        percorsoCasso += 'R';
                    }
                    if(diffy == 1){
                        percorsoCasso += 'U';
                    }
                    if(diffy == -1){
                        percorsoCasso += 'D';
                    }
                    start = ( *i );
                    pathPos++;
                    if(pathPos >= path.size() / 2)
                    {
                        offTmp.x = ( *i ).x;
                        offTmp.y = ( *i ).y;
                        break;
                    }
                }  
                offTmp.paths.push_back(percorsoCasso);
                percorsoCasso = "";
                for(; i != path.end(); i++ ) {
                    diffx = ( *i ).x - start.x;
                    diffy = ( *i ).y - start.y;
                    if(diffx == 1){
                        percorsoCasso += 'R';
                    }
                    if(diffx == -1){
                        percorsoCasso += 'L';
                    }
                    if(diffy == 1){
                        percorsoCasso += 'D';
                    }
                    if(diffy == -1){
                        percorsoCasso += 'U';
                    }
                    start = ( *i );
                } 
                offTmp.paths.push_back(percorsoCasso);
                offices.push_back(offTmp);
                //customers.erase(i);
                //customers.erase(i+1);
            }        
        }  

        /*for( std::vector<replyOffice>::iterator k = offices.begin(); (k != offices.end()) && (office <= ReplyOffices); k++ ) {
            
        }*/
  

        // write solution
        outputFile.open(nomeFile + ".out", ios::out | ios::binary);

        unsigned char smarker[3];
        smarker[0] = 0xEF;
        smarker[1] = 0xBB;
        smarker[2] = 0xBF;

        outputFile << smarker;
        outputFile.close();

        outputFile.open(nomeFile + ".out", ios::out | ios::app);

        for( std::vector<replyOffice>::iterator i = offices.begin(); i != offices.end(); i++ ) {
            for( std::vector<string>::iterator j = (*i).paths.begin(); j != (*i).paths.end(); j++ ){ 
                outputFile << (*i).x << ' ' << (*i).y << ' ' << (*j) << endl;
                std::cout << (*i).x << ' ' << (*i).y << ' ' << (*j) << endl;
            }
        }

        outputFile.close();
    }
    return 0;
}