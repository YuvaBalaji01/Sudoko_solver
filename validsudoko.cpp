#include<iostream>
using namespace std;

bool isvalid(int x,int y,int d,int matrix[][9]){

    for(int i=0;i<9;i++){
        if(matrix[i][y]==d || matrix[x][i]==d) return false;
    }

    int row=x-x%3;
    int col=y-y%3;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            if(matrix[row+i][col+j]==d) return false;
        }
    }

    return true;
}

pair<int,int> validxy(int x,int y,int matrix[][9]){

    for(int i=x;i<9;i++){
        for(int j=(i==x?y:0);j<9;j++){
            if(matrix[i][j]==0) return {i,j};
        }
    }

    return {-1,-1};
}

bool validsudoko(int x,int y,int matrix[][9]){

    if(x==-1){
        return true;
    }

    for(int i=1;i<=9;i++){
        if(isvalid(x,y,i,matrix)){
            matrix[x][y]=i;
            pair<int,int> next = validxy(x,y,matrix);
            if(next.first==-1) return true;
            if(validsudoko(next.first,next.second,matrix)) return true;
            matrix[x][y]=0;
        }
    }

    return false;
}

bool sudoko(int matrix[9][9]){
    pair<int,int> next = validxy(0,0,matrix);

    if(next.first==-1) return true;

    return validsudoko(next.first,next.second,matrix);
}

int main(){

    int matrix[9][9];
    for(int i=0;i<9;i++){
        for(int j=0;j<9;j++){
            cin >> matrix[i][j];
        }
    }

    if(sudoko(matrix))
    cout << "Their exisist a solution"<< endl;
    else cout << "No solution exsist" << endl;

    cout << "Press 1 if you want the solution" << endl;
    int v;
    cin >> v;

    if(v==1){
     for(int i=0;i<9;i++){
        cout << "|";
        for(int j=0;j<9;j++){
            cout << matrix[i][j] <<" " ;
            if(j==2 || j==5 || j==8) cout << "|";
        }
        cout  << endl;
        if(i==2 || i==5 || i==8) {cout << "-----------------------" ;
        cout << endl;}

        
    }
    }

}
